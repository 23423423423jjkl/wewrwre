<?php

/**
 * @file
 * Provides the view style plugin.
 *
 * This is the file for defining views handlers and data.
 */

/**
 * Implements of hook_views_plugins().
 */
function commerce_yandex_market_views_plugins() {
  $plugins = array(
    'module' => 'commerce_yandex_market',
    'display' => array(
      'yml_feed' => array(
        'title' => t('YML Feed'),
        'help' => t('Display the view as a yml feed.'),
        'handler' => 'commerce_yandex_market_plugin_display_yml_feed',
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('YML Feed'),
      ),
    ),
    'style' => array(
      'yml' => array(
        'title' => t('Yandex.Market'),
        'help' => t('Generates a xml feed from a view for export data to Yandex.Market.'),
        'handler' => 'commerce_yandex_market_plugin_style_yml',
        'theme' => 'commerce_yandex_market_view_yml',
        'theme path' => drupal_get_path('module', 'commerce_yandex_market') . '/theme',
        'theme file' => 'commerce_yandex_market.theme.inc',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
        'even empty' => TRUE,
        'type' => 'yml_feed',
      ),
    ),
    'row' => array(
      'simple_model' => array(
        'title' => t('Simple.Model'),
        'help' => t('Display fields as Simple.Model.'),
        'handler' => 'commerce_yandex_market_plugin_row_simple_model',
        'theme' => 'commerce_yandex_market_view_row_simple_model',
        'theme path' => drupal_get_path('module', 'commerce_yandex_market') . '/theme',
        'theme file' => 'commerce_yandex_market.theme.inc',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'yml_feed',
      ),
    ),
  );

  return $plugins;
}
