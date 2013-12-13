<?php

/**
 * @file
 * Contains the yml feed display plugin.
 */

/**
 * The plugin that handles a yml feed.
 *
 * For the most part, feeds are page displays but with some subtle differences.
 *
 * @ingroup views_display_plugins
 */
class commerce_yandex_market_plugin_display_yml_feed extends views_plugin_display_feed {

  /**
   * Displays can require a certain type of style plugin. By default, they will
   * be 'yml_feed'.
   */
  function get_style_type() {
    return 'yml_feed';
  }

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['displays'] = array('default' => array());

    // Overrides for standard stuff:
    $options['style_plugin']['default'] = 'yml';
    $options['style_options']['default']  = array(
      'name' => array('default' => filter_xss(variable_get('site_name', 'Drupal'), array())),
      'company' => array('default' => ''),
      'agency' => array('default' => ''),
      'email' => array('default' => ''),
      'categories' => array('default' => array()),
    );
    $options['sitename_title']['default'] = FALSE;
    $options['row_plugin']['default'] = 'simple_model';
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;

    return $options;
  }
}
