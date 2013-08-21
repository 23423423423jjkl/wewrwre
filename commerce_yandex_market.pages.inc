<?php

/**
 * @file
 * Page callbacks for Commerce Yandex Market.
 */

/**
 * AJAX callback for tokens autocomplete.
 */
function commerce_yandex_market_tokens_ajax_autocomplete($display_type, $string) {

  if (empty($display_type)) {
    return;
  }

  // Load class for autocomplete ui.
  $matches = commerceYandexMarketAutocomplete::processAutocomplete($string);

  drupal_json_output($matches);
}

/**
 * Output a file for export to Yandex Markete.
 */
function commerce_yandex_market_output_export() {
  $file = COMMERCE_YANDEX_MARKET_DIRECTORY . '/' . COMMERCE_YANDEX_MARKET_FILE_NAME;
  if (!file_exists($file) || !is_readable($file)) {
    return MENU_NOT_FOUND;
  }

  $mtime = filemtime($file);
  $last_modified = gmdate(DATE_RFC1123, $mtime);
  $etag = '"' . md5($last_modified) . '"';

  // See if the client has provided the required HTTP headers.
  $if_modified_since = isset($_SERVER['HTTP_IF_MODIFIED_SINCE']) ? stripslashes($_SERVER['HTTP_IF_MODIFIED_SINCE']) : FALSE;
  $if_none_match = isset($_SERVER['HTTP_IF_NONE_MATCH']) ? stripslashes($_SERVER['HTTP_IF_NONE_MATCH']) : FALSE;
  if ($if_modified_since && $if_none_match && $if_none_match == $etag && $if_modified_since == $last_modified) {
    header('HTTP/1.1 304 Not Modified');
    // All 304 responses must send an etag if the 200 response for the same object contained an etag
    header('Etag: ' . $etag);
    exit;
  }

  $http_headers = array(
    'Content-type' => 'text/xml; charset=utf-8',
    'Last-modified' => $last_modified,
    'Etag' => $etag,
    'Expires' => gmdate(DATE_RFC1123, $mtime),
    'Cache-Control' => 'must-revalidate',
    'X-Robots-Tag' => 'noindex, follow',
  );

  // Transfer the file as output.
  file_transfer($file, $http_headers);
}
