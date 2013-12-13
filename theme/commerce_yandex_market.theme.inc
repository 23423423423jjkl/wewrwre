<?php

/**
 * @file
 * Preprocessors and helper functions to make theming easier.
 */

/**
 * Preprocess an YML feed.
 */
function template_preprocess_commerce_yandex_market_view_yml(&$vars) {
  global $base_url;

  $view = &$vars['view'];

  $handler = &$view->style_plugin;

  $vars['doctype'] = '<!DOCTYPE yml_catalog SYSTEM "shops.dtd">';
  $vars['date'] = date('Y-m-d H:i', REQUEST_TIME);
  $vars['url'] = format_xml_elements(array('url' => $base_url));
  $vars['platform'] = format_xml_elements(array('platform' => 'Drupal'));
  $vars['version'] = format_xml_elements(array('version' => VERSION));

  // Provide shop settings.
  $settings = array('name', 'company', 'agency', 'email');
  foreach ($settings as $setting) {
    // The yml "spec" doesn't indicate HTML can be used.
    // We strip all HTML tags, but need to prevent double encoding from properly
    // escaped source data (such as &amp becoming &amp;amp;).
    $element = array(
      $setting => check_plain(decode_entities(strip_tags($handler->get_settings($setting)))),
    );

    $vars[$setting] = format_xml_elements($element);
  }

  // Provide currencies.
  $currencies = array();
  foreach (explode("\n", trim($handler->get_settings('currencies'))) as $line) {
    if (strpos($line, '|') !== FALSE) {
      list($id, $rate) = explode('|', $line);

      if (!empty($id) && !empty($rate)) {
        $currencies[] = array(
          'key' => 'currency',
          'attributes' => array('id' => $id, 'rate' => $rate),
        );
      }
    }
  }

  $vars['currencies'] = !empty($currencies) ? format_xml_elements(array('currencies' => $currencies)) : '';

  // Provide shop categories.
  $categories = array();
  if ($vocabulary = taxonomy_get_tree($handler->get_settings('category'))) {
    foreach ($vocabulary as $term) {
      $categories[$term->tid] = array(
        'key' => 'category',
        'value' => decode_entities(strip_tags(($term->name))),
        'attributes' => array('id' => $term->tid),
      );

      if ($term->parents[0]) {
        $categories[$term->tid]['attributes']['parentId'] = $term->parents[0];
      }
    }
  }
  $vars['categories'] = !empty($categories) ? format_xml_elements(array('categories' => $categories)) : '';

  // Provide local delivery cost.
  $local_delivery_cost = $handler->get_settings('local_delivery_cost');
  $vars['local_delivery_cost'] = !empty($local_delivery_cost) ? format_xml_elements(array('local_delivery_cost' => $local_delivery_cost)) : '';

  // During live preview we don't want to output the header since the contents
  // of the feed are being displayed inside a normal HTML page.
  if (empty($vars['view']->live_preview)) {
    drupal_add_http_header('Content-Type', 'text/xml; charset=utf-8');
  }
}

/**
 * Default theme function for all RSS rows.
 */
function template_preprocess_commerce_yandex_market_view_row_simple_model(&$vars) {
  $item = &$vars['row'];
  $vars['item_elements'] = !empty($item->offer) ? format_xml_elements($item->offer) : '';
}
