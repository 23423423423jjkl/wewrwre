<?php

/**
 * @file
 * Administrative callbacks for Commerce Yandex Market module.
 */

/**
 * Page callback for module settings page.
 */
function commerce_yandex_market_product_list($form, &$form_state) {

  // Load product displays.
  $result = commerce_yandex_market_load_all();

  $form['#tree'] = TRUE;

  foreach ($result as $display) {
    $form['displays'][$display->display_type]['display_type']['#markup'] = $display->display_type;

    // Create operations for product.
    $operations = array();

    if (!$display->status) {
      $operations[] = array(
        'title' => t('Enable'),
        'href' => 'admin/commerce/yandex-market/enable/' . $display->display_type,
      );
    }

    $operations[] = array(
      'title' => t('Edit'),
      'href' => 'admin/commerce/yandex-market/edit/' . $display->display_type,
    );
    $operations[] = array(
      'title' => t('Delete'),
      'href' => 'admin/commerce/yandex-market/delete/' . $display->display_type,
    );
    $operations[] = array(
      'title' => t('Export'),
      'href' => 'admin/commerce/yandex-market/export/' . $display->display_type,
    );

    if ($display->status) {
      $operations[] = array(
        'title' => t('Disable'),
        'href' => 'admin/commerce/yandex-market/disable/' . $display->display_type,
      );
    }

    $form['displays'][$display->display_type]['disabled'] = array(
      '#type' => 'value',
      '#value' => $display->status,
    );

    $form['displays'][$display->display_type]['actions'] = array(
      '#theme' => 'links__ctools_dropbutton',
      '#links' => $operations,
      '#attributes' => array('class' => array('links', 'inline'))
    );
  }

  $form['#attached']['css'][] = drupal_get_path('module', 'commerce_yandex_market') . '/css/commerce-yandex-market.css';

  return $form;
}

/**
 * Commerce Yandex Market settings form.
 */
function commerce_yandex_market_settings($form, $form_state) {

  $form['commerce_yandex_market_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yandex Market settings'),
  );

  $site_name = filter_xss(variable_get('site_name', 'Drupal'), array());
  $form['commerce_yandex_market_settings']['commerce_yandex_market_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The short name of the store'),
    '#default_value' => variable_get('commerce_yandex_market_name', $site_name),
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => TRUE,
  );

  $form['commerce_yandex_market_settings']['commerce_yandex_market_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company'),
    '#description' => t('Full name of the company that owns the store.'),
    '#default_value' => variable_get('commerce_yandex_market_company'),
    '#required' => TRUE,
  );

  $form['commerce_yandex_market_settings']['commerce_yandex_market_agency'] = array(
    '#type' => 'textfield',
    '#title' => t('Agency'),
    '#description' => t('Name of the agency, which provides technical support to the store.'),
    '#default_value' => variable_get('commerce_yandex_market_agency'),
    '#required' => TRUE,
  );

  $form['commerce_yandex_market_settings']['commerce_yandex_market_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#description' => t('Contact the developers carrying out technical support.'),
    '#default_value' => variable_get('commerce_yandex_market_mail'),
    '#element_validate' => array('commerce_yandex_market_validate_mail'),
    '#required' => TRUE,
  );

  $options = array();
  foreach (taxonomy_get_vocabularies() as $vocabulary) {
    $options[$vocabulary->vid] = $vocabulary->name;
  }

  $form['commerce_yandex_market_settings']['commerce_yandex_market_categories'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Yandex Market categories'),
    '#description' => t('Vocabularies used to export store categories'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_yandex_market_categories', array()),
  );

  $form['commerce_yandex_market_settings']['commerce_yandex_market_local_delivery_cost'] = array(
    '#type' => 'textfield',
    '#title' => t('Local delivery cost'),
    '#description' => t('Total shipping cost for your region.'),
    '#default_value' => variable_get('commerce_yandex_market_local_delivery_cost'),
    '#element_validate' => array('commerce_yandex_market_validate_cost'),
  );

  $form['commerce_yandex_market_settings']['commerce_yandex_market_safe_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Run update xml file every'),
    '#default_value' => variable_get('commerce_yandex_market_safe_threshold', DRUPAL_CRON_DEFAULT_THRESHOLD),
    '#options' => array(0 => t('Never')) + drupal_map_assoc(array(3600, 10800, 21600, 43200, 86400, 604800), 'format_interval'),
  );

  return system_settings_form($form);
}

/**
 * Validate callback for "email" field.
 */
function commerce_yandex_market_validate_mail($form, &$form_state) {
  if (!valid_email_address($form_state['values']['commerce_yandex_market_mail'])) {
    form_set_error('commerce_yandex_market_mail', t('You must enter a valid e-mail address.'));
  }
}

/**
 * Validate callback for "local_delivery_cost" field.
 */
function commerce_yandex_market_validate_cost($form, &$form_state) {
  $local_delivery_cost = $form_state['values']['commerce_yandex_market_local_delivery_cost'];
  if (!empty($local_delivery_cost) && !is_numeric($local_delivery_cost)) {
    form_set_error('commerce_yandex_market_local_delivery_cost', t('You must enter a valid local delivery cost.'));
  }
}

/**
 * Form for editing product display.
 */
function commerce_yandex_market_edit_page($display) {

  // Save form in cache.
  commerce_yandex_market_object_cache_set($display->display_type, $display);

  $form_state = array(
    'storage' => array(
      'display_type' => $display->display_type,
      'step' => 2,
    ),
  );

  $form = drupal_build_form('commerce_yandex_market_edit_form', $form_state);
  $page['content']['edit_form'] = $form;

  $page['content']['#attached']['library'][] = array('system', 'drupal.ajax');

  return $page;
}

/**
 * Ajax callback for product display EDIT links.
 */
function commerce_yandex_market_edit_form_ajax_links($display, $step) {

  $form_state = array(
    'storage' => array(
      'display_type' => $display->display_type,
      'step' => $step,
    ),
  );

  $form = drupal_build_form('commerce_yandex_market_edit_form', $form_state);
  $commands = array();
  $commands[] = ajax_command_replace('#commerce-yandex-market-form-wrapper', render($form));
  $commands[] = ajax_command_invoke('#commerce-yandex-market-menu li a', 'removeClass', array('active'));
  $commands[] = ajax_command_invoke('#commerce-yandex-market-menu li a:eq(' . --$step . ')', 'addClass', array('active'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Disable product display.
 */
function commerce_yandex_market_disable_display_page($display) {
  $display->status = COMMERCE_YANDEX_MARKET_DISABLED;
  commerce_yandex_market_update($display);

  drupal_set_message(t('Display %name was disabled', array('%name' => $display->display_type)));

  drupal_goto('admin/commerce/yandex-market');
}

/**
 * Enable product display.
 */
function commerce_yandex_market_enable_display_page($display) {
  $display->status = COMMERCE_YANDEX_MARKET_ENABLED;
  commerce_yandex_market_update($display);

  drupal_set_message(t('Display %name was enabled', array('%name' => $display->display_type)));

  drupal_goto('admin/commerce/yandex-market');
}

/**
 * Title callback for export product display page.
 */
function commerce_yandex_market_title_callback($display) {
  return t('Export a product display "!display_type"', array('!display_type' => $display->display_type));
}

/**
 * Export product display.
 */
function commerce_yandex_market_export_form($form, $form_state, $display) {
  ctools_include('export');

  $code = ctools_export_crud_export('commerce_yandex_market', $display);
  $lines = substr_count($code, "\n");

  $form['export'] = array(
    '#title' => t('Export data'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into import area.'),
  );

  return $form;
}

/**
 * Import product display.
 */
function commerce_yandex_market_import_form($form, $form_state) {

  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste code here to import a product display'),
  );

  $form['actions'] = array(
    '#type' => 'action',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Submit callback for commerce_yandex_market_import_form.
 */
function commerce_yandex_market_import_form_submit($form, &$form_state) {
  $code = $form_state['values']['import'];
  ctools_include('export');

  $display_raw = ctools_export_crud_import('commerce_yandex_market', $code);
  if (is_string($display_raw)) {
    form_error($form['import'], t('Unable to get an import from the code. Errors reported: @errors', array('@errors' => $display_raw)));
    $form_state['rebuild'] = TRUE;
    return;
  }

  // Because we get raw object from Ctools we should prepare it before saving.
  $display = commerce_yandex_market_load_prepare((array) $display_raw);
  $display_original = commerce_yandex_market_load_by_name($display->display_type);

  // If there is a product display object with the same product type,
  // delete the original so that this one writes properly.
  if (!empty($display_original)) {
    commerce_yandex_market_delete($display->display_type);
  }

  if (commerce_yandex_market_save($display, $display->status)) {
    if (!empty($display_original)) {
      drupal_set_message(filter_xss(t('Product display "!display_type" was successfully updated.', array('!display_type' => $display->display_type))));
    }
    else {
      drupal_set_message(filter_xss(t('Product display "!display_type" was successfully imported.', array('!display_type' => $display->display_type))));
    }
  }
  else {
    form_error($form['import'], t('Could not import a product display.'));
  }

  $form_state['redirect'] = 'admin/commerce/yandex-market';
}
