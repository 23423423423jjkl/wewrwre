<?php

/**
 * @file
 * Contains the yml style plugin.
 */

/**
 * Default style plugin to render a XML feed.
 *
 * @ingroup views_style_plugins
 */
class commerce_yandex_market_plugin_style_yml extends views_plugin_style {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['name'] = array('default' => filter_xss(variable_get('site_name', 'Drupal'), array()));
    $options['company'] = array('default' => '');
    $options['agency'] = array('default' => '');
    $options['email'] = array('default' => '');
    $options['currencies'] = array('default' => 'RUR|1');
    $options['category'] = array('default' => '');
    $options['local_delivery_cost'] = array('default' => '');

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#default_value' => $this->options['name'],
      '#description' => t('The short name of the store.'),
      '#maxlength' => 20,
      '#required' => TRUE,
    );
    $form['company'] = array(
      '#type' => 'textfield',
      '#title' => t('Company'),
      '#default_value' => $this->options['company'],
      '#description' => t('Full name of the company that owns the store.'),
      '#required' => TRUE,
    );
    $form['agency'] = array(
      '#type' => 'textfield',
      '#title' => t('Agency'),
      '#default_value' => $this->options['agency'],
      '#description' => t('Name of the agency, which provides technical support to the store.'),
      '#required' => TRUE,
    );
    $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#default_value' => $this->options['email'],
      '#description' => t('Contact the developers carrying out technical support.'),
      '#required' => TRUE,
      '#element_validate' => array('commerce_yandex_market_validate_mail'),
    );
    $form['currencies'] = array(
      '#type' => 'textarea',
      '#title' => t('Currencies'),
      '#default_value' => $this->options['currencies'],
      '#description' => t('List of currencies used in the shop. One pair - one\'s element. Enter one value per line,
        in the format id|rate. "id" element is code of currency, "rate" is currency rate. "rate" also can be CBRF, NBU, NBK or CB.'),
      '#required' => TRUE,
      '#element_validate' => array('commerce_yandex_market_validate_currencies'),
    );

    $categories = array('' => t('- None -'));
    foreach (taxonomy_get_vocabularies() as $vocabulary) {
      $categories[$vocabulary->vid] = $vocabulary->name;
    }

    $form['category'] = array(
      '#type' => 'select',
      '#title' => t('Categories'),
      '#options' => $categories,
      '#default_value' => $this->options['category'],
      '#description' => t('Vocabularies used for export store categories'),
    );
    $form['local_delivery_cost'] = array(
      '#type' => 'textfield',
      '#title' => t('Local delivery cost'),
      '#default_value' => $this->options['local_delivery_cost'],
      '#description' => t('The cost of delivery of the goods in local region.'),
      '#element_validate' => array('element_validate_number'),
    );
  }

  /**
   * Validate that the plugin is correct and can be saved.
   *
   * @return array
   *   An array of error strings to tell the user what is wrong with this
   *   plugin.
   */
  function validate() {
    $errors = parent::validate();

    $required_options = array('name', 'company', 'agency', 'email', 'currencies');
    foreach ($required_options as $required_option) {

      if (is_array($this->options[$required_option]) && empty($this->options[$required_option]['default'])
        || empty($this->options[$required_option])) {

        $errors[] = t('Row style plugin requires specifying which views fields to use for xml item.');
        break;
      }
    }

    return $errors;
  }

  /**
   * Get settings.
   *
   * @param string $setting_name
   *   The setting name that you want to get the value.
   *
   * @return string
   *   The string containing the setting value.
   */
  function get_settings($setting_name) {
    return $this->options[$setting_name];
  }

  function render() {
    if (empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return '';
    }
    $rows = '';

    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;
      $rows .= $this->row_plugin->render($row);
    }

    $output = theme($this->theme_functions(), array(
      'view' => $this->view,
      'options' => $this->options,
      'rows' => $rows,
    ));

    unset($this->view->row_index);
    return $output;
  }
}
