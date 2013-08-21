<?php

/**
 * @file
 * Commerce Yandex Market Theme functions.
 */

/**
 * Themes commerce_yandex_market autocomplete forms.
 * @ingroup themeable
 */
function theme_commerce_yandex_market_autocomplete($variables) {
  $element = $variables['element'];
  drupal_add_js(drupal_get_path('module', 'commerce_yandex_market') . '/js/commerce-yandex-market.autocomplete.js');
  drupal_add_library('system', 'ui.autocomplete');
  drupal_add_library('system', 'ui.button');

  $element['#attributes']['type'] = 'text';
  element_set_attributes($element, array('id', 'name', 'value', 'size', 'maxlength', 'required'));
  _form_set_class($element, array('form-text', 'commerce-yandex-market-autocomplete'));

  $id = $element['#attributes']['id'];
  $id_button = drupal_html_id($id . '-button');

  $js_vars['commerce_yandex_market_autocomplete'][$id] = array(
    'inputId' => $id,
    'source' => url($element['#autocomplete_path'], array('absolute' => TRUE)),
  );

  drupal_add_js($js_vars, 'setting');

  $output = '<div class="commerce-yandex-market-autocomplete">';
  $output .= '<input' . drupal_attributes($element['#attributes']) . ' />';
  $output .= '</div>';

  return $output;
}

/**
 * Themes commerce_yandex_market table forms.
 * @ingroup themeable
 */
function theme_commerce_yandex_market_add_form($variables) {
  return _commerce_yandex_market_render_table($variables);
}

/**
 * Themes commerce_yandex_market table forms.
 * @ingroup themeable
 */
function theme_commerce_yandex_market_edit_form($variables) {
  return _commerce_yandex_market_render_table($variables);
}

/**
 * @param $variables
 * @return string
 */
function _commerce_yandex_market_render_table($variables) {
  $form = $variables['form'];

  if (isset($form['export_table'])) {
    $rows = array();
    foreach (element_children($form['export_table']) as $value) {
      $row = array();
      $input = $form['export_table'][$value];

      if (isset($input['attr']) && isset($input['attr_title']) && isset($input['attr_value'])) {
        $row[] = render($input['attr']) . render($input['attr_title']);

        $data = isset($input['attr_param_name']) ? render($input['attr_param_name']) : '';
        $data .= isset($input['attr_param_unit']) ? render($input['attr_param_unit']) : '';
        $data .= render($input['attr_value']);
        $row[] = array(
          'data' => $data,
          'colspan' => isset($input['delete']) ? 1 : 2,
        );

        if (isset($input['delete'])) {
          $row[] = render($input['delete']);
        }

        $rows[] = $row;
      }
    }

    unset($form['export_table']);

    $rows[] = array(
      'data' => array(array('data' => render($form['more']), 'colspan' => 3))
    );

    $headers = array(t('Attribute'), t('Value'), t('Delete'));
    $output = theme('table', array(
      'header' => $headers,
      'rows' => $rows,
      'attributes' => array(
        'class' => array('export-tokens')
      ),
    ));

    // Build some other elements after table.
    $buttons = render($form['actions']);

    return drupal_render_children($form) . $output . $buttons;
  }
}

/**
 * Themes commerce_yandex_market_product_list table form.
 * @ingroup themeable
 */
function theme_commerce_yandex_market_product_list($variables) {
  $form = $variables['form'];
  $rows = array();

  if (!empty($form['displays'])) {
    foreach (element_children($form['displays']) as $key) {
      $row = array();
      $row[] = render($form['displays'][$key]['display_type']);
      $row[] = render($form['displays'][$key]['actions']);

      $class = 'disabled';
      if ($form['displays'][$key]['disabled']['#value']) {
        $class = 'enabled';
      }

      $rows[] = array(
        'data' => $row,
        'class' => array($class),
      );
    }
  }

  $header = array(t('Display type'), t('Actions'));
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('There are no created product displays yet.'),
    'attributes' => array('id' => 'commerce-yandex-market-table-list'),
  ));

  return $output . drupal_render_children($form);
}
