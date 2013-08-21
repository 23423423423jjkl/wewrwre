<?php

/**
 * @file
 * Contains the class for generate xml file to export.
 */

class XMLCommerceYandexMarketException extends Exception {}

class XMLCommerceYandexMarketGenerationException extends XMLCommerceYandexMarketException {}

/**
 * Extended class for writing XML files.
 */
class XMLCommerceYandexMarketWriter extends XMLWriter {
  protected $uri = NULL;
  protected $base_url = NULL;
  protected $elementCount = 0;
  protected $elementCountFlush = 500;
  protected $rootElement = 'yml_catalog';
  protected $shopElement = 'shop';

  /**
   * Constructor.
   */
  public function __construct() {
    global $base_url;
    $this->base_url = $base_url;
    $this->uri = COMMERCE_YANDEX_MARKET_DIRECTORY . '/' . COMMERCE_YANDEX_MARKET_FILE_NAME;
    $this->openUri($this->uri);
  }

  /**
   * Open xml file by uri.
   */
  public function openUri($uri) {
    $return = parent::openUri(drupal_realpath($uri));
    if (!$return) {
      throw new XMLCommerceYandexMarketGenerationException(t('Could not open file @file for writing.', array('@file' => $uri)));
    }
    return $return;
  }

  /*
   * Start writing xml document.
   */
  public function startDocument($version = '1.0', $encoding = 'UTF-8', $standalone = NULL) {
    $this->setIndent(FALSE);
    parent::startDocument($version, $encoding);
    $this->writeDoctype();
    $this->startElement($this->rootElement, TRUE);
    $this->startElement($this->shopElement);
  }

  /**
   * Add the Doctype to the XML page.
   */
  public function writeDoctype() {
    $this->startDTD('yml_catalog SYSTEM "shops.dtd"');
    $this->endDTD();
    $this->writeRaw(PHP_EOL);
  }

  /**
   * Return an array of attributes for the root element of the XML.
   */
  public function getRootAttributes() {
    $attributes['date'] = date('Y-m-d H:i', REQUEST_TIME);
    return $attributes;
  }

  /*
   * Generate a xml file for export to Yandex Market.
   */
  public function generateXML() {

    // Write site name.
    $site_name = filter_xss(variable_get('site_name', 'Drupal'), array());
    $name = variable_get('commerce_yandex_market_name', $site_name);
    $this->writeElement(array('name' => 'name'), filter_xss($name, array()));

    // Write company name.
    if ($company = variable_get('commerce_yandex_market_company', FALSE)) {
      $this->writeElement(array('name' => 'company'), filter_xss($company, array()));
    }

    // Write site url name.
    $this->writeElement(array('name' => 'url'), $this->base_url);

    // Write core info.
    $this->writeElement(array('name' => 'platform'), 'Drupal');
    $this->writeElement(array('name' => 'version'), VERSION);

    // Write agency.
    if ($agency = variable_get('commerce_yandex_market_agency', FALSE)) {
      $this->writeElement(array('name' => 'agency'), $agency);
    }

    // Write agency e-mail address.
    if ($email = variable_get('commerce_yandex_market_mail', FALSE)) {
      $this->writeElement(array('name' => 'email'), $email);
    }

    // Write online store currency.
    $element_content = array();
    $element_content[] = array(
      'element' => array(
        'name' => 'currency',
        'attributes' => array(
          'id' => commerce_default_currency(),
          'rate' => 1,
        ),
      ),
      'content' => '',
    );
    $this->writeElement(array('name' => 'currencies'), $element_content);

    // Get vocabularies id.
    $vids = _commerce_yandex_market_get_selected_categories();

    // Write online store categories.
    $element_content = array();
    foreach ($vids as $vid) {
      if ($vocabulary = taxonomy_get_tree($vid)) {
        foreach ($vocabulary as $term) {
          $element_content[$term->tid] = array(
            'element' => array(
              'name' => 'category',
              'attributes' => array(
                'id' => $term->tid,
              ),
            ),
            'content' => filter_xss($term->name, array()),
          );

          if ($term->parents[0]) {
            $element_content[$term->tid]['element']['attributes']['parentId'] = $term->parents[0];
          }
        }
      }
    }

    if (!empty($element_content)) {
      $this->writeElement(array('name' => 'categories'), $element_content);
    }

    // Write online store local delivery cost.
    if ($local_delivery_cost = variable_get('commerce_yandex_market_local_delivery_cost', FALSE)) {
      $this->writeElement(array('name' => 'local_delivery_cost'), $local_delivery_cost);
    }

    // Load product displays.
    $displays = commerce_yandex_market_load_all();

    $types = array();
    foreach ($displays as $display) {
      if ($display->status == COMMERCE_YANDEX_MARKET_ENABLED) {
        $types[] = $display->display_type;
      }
      else {

        // Remove disabled displays.
        unset($displays[$display->display_type]);
      }
    }

    if (!empty($types)) {
      // Include ctools library for contexts.
      ctools_include('context');

      $context = array();

      // Add custom ctools context for site.
      $site = new ctools_context('site');
      $site->plugin = 'site';
      $site->keyword = 'site';
      $contexts['site'] = $site;

      $element_content = array();
      $nodes = node_load_multiple(array(), array('type' => $types, 'status' => NODE_PUBLISHED));
      $exports = array();
      foreach ($nodes as $node) {
        $export = new stdClass();
        $contexts['node'] = ctools_context_create('node', $node);
        $available = ctools_context_keyword_substitute($displays[$node->type]->available, array(), $contexts);

        if ($available == t('Active') || $available == 1) {
          $available = 'true';
        }
        elseif ($available == t('Disabled') || $available == 0) {
          $available = 'false';
        }

        $element_content[$node->nid] = array(
          'element' => array(
            'name' => 'offer',
            'attributes' => array(
              'id' => $node->nid,
              // 'type'=> 'vendor.model',
              'available' => $available,
            ),
          ),
        );

        if (!empty($displays[$node->type]->export_values)) {
          foreach ($displays[$node->type]->export_values as $key => $value) {
            if (!empty($displays[$node->type]->export_attributes[$key])) {
              if (!is_array($displays[$node->type]->export_attributes[$key])) {
                $sub_element_name = array(
                  'name' => $displays[$node->type]->export_attributes[$key],
                );
              }
              else {
                $sub_element_name = array(
                  'name' => $displays[$node->type]->export_attributes[$key]['attr'],
                  'attributes' => array(),
                );

                if (!empty($displays[$node->type]->export_attributes[$key]['attr_param_name'])) {
                  $sub_element_name['attributes']['name'] = $displays[$node->type]->export_attributes[$key]['attr_param_name'];
                }

                if (!empty($displays[$node->type]->export_attributes[$key]['attr_param_unit'])) {
                  $sub_element_name['attributes']['unit'] = $displays[$node->type]->export_attributes[$key]['attr_param_unit'];
                }
              }

              $sub_element_content = ctools_context_keyword_substitute($value, array(), $contexts);

              if ($sub_element_name['name'] == 'price') {
                $sub_element_content = str_replace(',', '', $sub_element_content);
              }

              if ($sub_element_content = filter_xss($sub_element_content, array())) {
                $element_content[$node->nid]['content'][] = array(
                  'element' => $sub_element_name,
                  'content' => $sub_element_content,
                );
              }
            }
          }
        }
      }

      $this->writeElement(array('name' => 'offers'), $element_content);
    }
  }

  /**
   * Write root element tag.
   */
  public function startElement($name, $root = FALSE) {
    parent::startElement($name);

    if ($root) {
      foreach ($this->getRootAttributes() as $name => $value) {
        $this->writeAttribute($name, $value);
      }
      $this->writeRaw(PHP_EOL);
    }
  }

  /**
   * Write full element tag including support for nested elements.
   *
   * @param $element
   *   The element name and attributes.
   * @param $content
   *   The element contents or an array of the elements' sub-elements.
   */
  public function writeElement($element, $content = '') {
    if (is_array($content)) {
      parent::startElement($element['name']);
      if (!empty($element['attributes'])) {
        foreach ($element['attributes'] as $attr => $attr_value) {
          $this->writeAttribute($attr, $attr_value);
        }
      }

      foreach ($content as $sub_element) {
        $this->writeElement($sub_element['element'], $sub_element['content']);
      }

      $this->endElement();
      $this->writeRaw(PHP_EOL);
    }
    else {
      parent::startElement($element['name']);
      if (!empty($element['attributes'])) {
        foreach ($element['attributes'] as $attr => $attr_value) {
          $this->writeAttribute($attr, $attr_value);
        }
      }
      $this->writeRaw($content);
      $this->endElement();
      $this->writeRaw(PHP_EOL);
    }

    // After a certain number of elements have been added, flush the buffer
    // to the output file.
    $this->elementCount++;
    if (($this->elementCount % $this->elementCountFlush) == 0) {
      parent::flush(TRUE);
    }
  }

  public function endDocument() {
    return parent::endDocument();
  }
}
