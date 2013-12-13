<?php

/**
 * @file
 * Contains an implementation of simple.model items based on fields on a row plugin.
 */

/**
 * Renders an simple.model item based on fields.
 */
class commerce_yandex_market_plugin_row_simple_model extends views_plugin_row {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['offer_attributes'] = array(
      'contains' => array(
        'id' => array('default' => ''),
        'available' => array('default' => ''),
      ),
    );

    $options['offer_elements'] = array(
      'contains' => array(
        'url' => array('default' => ''),
        'price' => array('default' => ''),
        'currency_id' => array('default' => ''),
        'picture' => array('default' => ''),
        'store' => array('default' => ''),
      ),
    );

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $initial_labels = array('' => t('- None -'));
    $view_fields_labels = $this->display->handler->get_field_labels();
    $view_fields_labels = array_merge($initial_labels, $view_fields_labels);

    // Offer's attributes.
    $form['offer_attributes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Offer\'s attributes'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['offer_attributes']['id'] = array(
      '#type' => 'select',
      '#title' => t('Offer ID'),
      '#description' => t('The field that is going to be used as offer id attribute.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_attributes']['id'],
      '#required' => TRUE,
    );
    $form['offer_attributes']['available'] = array(
      '#type' => 'select',
      '#title' => t('Available'),
      '#description' => t('The field that is going to be used as offer available attribute.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_attributes']['available'],
      '#required' => TRUE,
    );

    // Offer's elements.
    $form['offer_elements'] = array(
      '#type' => 'fieldset',
      '#title' => t('Offer\'s elements'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['offer_elements']['url'] = array(
      '#type' => 'select',
      '#title' => t('URL'),
      '#description' => t('The field that is going to be used as offer url element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['url'],
      '#required' => TRUE,
    );
    $form['offer_elements']['price'] = array(
      '#type' => 'select',
      '#title' => t('Price'),
      '#description' => t('The field that is going to be used as offer price element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['price'],
      '#required' => TRUE,
    );
    $form['offer_elements']['currency_id'] = array(
      '#type' => 'select',
      '#title' => t('Currency ID'),
      '#description' => t('The field that is going to be used as offer currency id element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['currency_id'],
      '#required' => TRUE,
    );
    $form['offer_elements']['category_id'] = array(
      '#type' => 'select',
      '#title' => t('Category ID'),
      '#description' => t('The field that is going to be used as offer category id element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['category_id'],
      '#required' => TRUE,
    );
    $form['offer_elements']['picture'] = array(
      '#type' => 'select',
      '#title' => t('Picture'),
      '#description' => t('The field that is going to be used as offer picture element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['picture'],
      '#required' => FALSE,
    );
    $form['offer_elements']['store'] = array(
      '#type' => 'select',
      '#title' => t('Store'),
      '#description' => t('The field that is going to be used as offer store element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['store'],
      '#required' => FALSE,
    );
    $form['offer_elements']['pickup'] = array(
      '#type' => 'select',
      '#title' => t('Pickup'),
      '#description' => t('The field that is going to be used as offer pickup element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['pickup'],
      '#required' => FALSE,
    );
    $form['offer_elements']['delivery'] = array(
      '#type' => 'select',
      '#title' => t('Delivery'),
      '#description' => t('The field that is going to be used as offer delivery element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['delivery'],
      '#required' => FALSE,
    );
    $form['offer_elements']['local_delivery_cost'] = array(
      '#type' => 'select',
      '#title' => t('Local delivery cost'),
      '#description' => t('The field that is going to be used as offer local delivery cost element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['local_delivery_cost'],
      '#required' => FALSE,
    );
    $form['offer_elements']['name'] = array(
      '#type' => 'select',
      '#title' => t('Name'),
      '#description' => t('The field that is going to be used as offer name element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['name'],
      '#required' => TRUE,
    );
    $form['offer_elements']['vendor'] = array(
      '#type' => 'select',
      '#title' => t('Vendor'),
      '#description' => t('The field that is going to be used as offer vendor element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['vendor'],
      '#required' => FALSE,
    );
    $form['offer_elements']['vendor_code'] = array(
      '#type' => 'select',
      '#title' => t('Vendor code'),
      '#description' => t('The field that is going to be used as offer vendor code element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['vendor_code'],
      '#required' => FALSE,
    );
    $form['offer_elements']['description'] = array(
      '#type' => 'select',
      '#title' => t('Description'),
      '#description' => t('The field that is going to be used as offer description element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['description'],
      '#required' => FALSE,
    );
    $form['offer_elements']['sales_notes'] = array(
      '#type' => 'select',
      '#title' => t('Sales notes'),
      '#description' => t('The field that is going to be used as offer sales notes element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['sales_notes'],
      '#required' => FALSE,
    );
    $form['offer_elements']['country_of_origin'] = array(
      '#type' => 'select',
      '#title' => t('Country of origin'),
      '#description' => t('The field that is going to be used as offer country of origin element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['country_of_origin'],
      '#required' => FALSE,
    );
    $form['offer_elements']['adult'] = array(
      '#type' => 'select',
      '#title' => t('Adult'),
      '#description' => t('The field that is going to be used as offer adult element.'),
      '#options' => $view_fields_labels,
      '#default_value' => $this->options['offer_elements']['adult'],
      '#required' => FALSE,
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

    $required_options = array();
    $required_options['offer_attributes'] = array('id', 'available');
    $required_options['offer_elements'] = array('url', 'price', 'currency_id', 'category_id', 'name');

    foreach ($required_options as $key => $value) {
      foreach ($value as $required_option) {
        if (empty($this->options[$key][$required_option])) {
          $errors[] = t('Row style plugin requires specifying which views fields to use for XML item.');
          break;
        }
      }
    }

    return $errors;
  }

  function render($row) {
    static $row_index;
    if (!isset($row_index)) {
      $row_index = 0;
    }

    $available = $this->get_field_value($row_index, $this->options['offer_attributes']['available']);

    // Create the offer item object.
    $item = new stdClass();
    $item->offer[$row_index] = array(
      'key' => 'offer',
      'attributes' => array(
        'id' => $this->get_field($row_index, $this->options['offer_attributes']['id']),
        'available' => $available ? 'true' : 'false',
      ),
    );

    // Collect url data.
    $item->offer[$row_index]['value']['url'] = url($this->get_field($row_index, $this->options['offer_elements']['url']), array('absolute' => TRUE));

    // Collect price and currency data.
    if ($currency_data = $this->get_field_value($row_index, $this->options['offer_elements']['currency_id'])) {
      $currency_code = $currency_data[0]['currency_code'];

      if ($price_data = $this->get_field_value($row_index, $this->options['offer_elements']['price'])) {
        $amount = $price_data[0]['amount'];
        $price = number_format(commerce_currency_amount_to_decimal($amount, $currency_code), 2, '.', '');
        $item->offer[$row_index]['value']['price'] = $price;
      }

      $item->offer[$row_index]['value']['currencyId'] = $currency_code;
    }

    // Collect category data.
    if ($category_id = $this->get_field($row_index, $this->options['offer_elements']['category_id'])) {
      $item->offer[$row_index]['value']['categoryId'] = $category_id;
    }

    // Collect pictures data.
    if ($pictures = $this->get_field_value($row_index, $this->options['offer_elements']['picture'])) {
      $i = 0;
      foreach ($pictures as $picture) {
        $item->offer[$row_index]['value'][] = array(
          'key' => 'picture',
          'value' => file_create_url($picture['uri']),
        );

        // Max 10 picture elements.
        if (++$i == 10) {
          break;
        }
      }
    }

    // Collect store data.
    if ($store = $this->get_field_value($row_index, $this->options['offer_elements']['store'])) {
      $item->offer[$row_index]['value']['store'] = $store[0]['value'] ? 'true' : 'false';
    }

    // Collect pickup data.
    if ($pickup = $this->get_field_value($row_index, $this->options['offer_elements']['pickup'])) {
      $item->offer[$row_index]['value']['pickup'] = $pickup[0]['value'] ? 'true' : 'false';
    }

    // Collect delivery data.
    if ($delivery = $this->get_field_value($row_index, $this->options['offer_elements']['delivery'])) {
      $item->offer[$row_index]['value']['delivery'] = $delivery[0]['value'] ? 'true' : 'false';
    }

    // Collect local_delivery_cost data.
    if ($local_delivery_cost = $this->get_field($row_index, $this->options['offer_elements']['local_delivery_cost'])) {
      $item->offer[$row_index]['value']['local_delivery_cost'] = $local_delivery_cost;
    }

    // Collect name data.
    if ($name = $this->get_field($row_index, $this->options['offer_elements']['name'])) {
      $item->offer[$row_index]['value']['name'] = $name;
    }

    // Collect vendor data.
    if ($vendor = $this->get_field($row_index, $this->options['offer_elements']['vendor'])) {
      $item->offer[$row_index]['value']['vendor'] = $vendor;
    }

    // Collect vendor code data.
    if ($vendor_code = $this->get_field($row_index, $this->options['offer_elements']['vendor_code'])) {
      $item->offer[$row_index]['value']['vendorCode'] = $vendor_code;
    }

    // Collect description data.
    if ($description = $this->get_field($row_index, $this->options['offer_elements']['description'])) {
      $item->offer[$row_index]['value']['description'] = $description;
    }

    // Collect sales notes data.
    if ($sales_notes = $this->get_field($row_index, $this->options['offer_elements']['sales_notes'])) {
      $item->offer[$row_index]['value']['sales_notes'] = $sales_notes;
    }

    // Collect country of origin data.
    if ($country_of_origin = $this->get_field($row_index, $this->options['offer_elements']['country_of_origin'])) {
      $item->offer[$row_index]['value']['country_of_origin'] = $country_of_origin;
    }

    // Collect adult data.
    if ($adult = $this->get_field_value($row_index, $this->options['offer_elements']['adult'])) {
      $item->offer[$row_index]['value']['adult'] = $adult[0]['value'] ? 'true' : 'false';
    }

    $row_index++;

    return theme($this->theme_functions(), array(
      'view' => $this->view,
      'options' => $this->options,
      'row' => $item,
      'field_alias' => isset($this->field_alias) ? $this->field_alias : '',
    ));
  }

  /**
   * Retrieves a views field value from the style plugin.
   *
   * @param $index
   *   The index count of the row as expected by views_plugin_style::get_field().
   * @param $field_id
   *   The ID assigned to the required field in the display.
   *
   * @return string
   *   The retrieved a views field value from the style plugin.
   */
  function get_field($index, $field_id) {
    if (empty($this->view->style_plugin) || !is_object($this->view->style_plugin) || empty($field_id)) {
      return '';
    }
    return $this->view->style_plugin->get_field($index, $field_id);
  }

  /**
   * Get the raw field value.
   *
   * @param $index
   *   The index count of the row.
   * @param $field_id
   *   The id of the field.
   *
   * @return mixed
   *   The raw field value.
   */
  function get_field_value($index, $field_id) {
    if (empty($this->view->style_plugin) || !is_object($this->view->style_plugin) || empty($field_id)) {
      return '';
    }
    return $this->view->style_plugin->get_field_value($index, $field_id);
  }
}
