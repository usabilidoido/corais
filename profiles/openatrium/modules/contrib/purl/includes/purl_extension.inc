<?php
// $Id: purl_extension.inc,v 1.1.2.5 2010/01/05 02:48:37 yhahn Exp $

/**
 * File extension style. Like ".csv"
 */
class purl_extension implements purl_processor {

  public function admin_form(&$form, $id) { }

  public function detect($q) {
    $last = explode('.', array_pop(explode('/', $q)));
    if (count($last) > 1) {
      return array_pop($last);
    }
    return '';
  }

  public function method() {
    return 'extension';
  }

  public function description() {
    return t('Enter a extension for this context, such as "csv".');
  }

  public function parse($valid_values, $q) {
    $parsed = array();
    if (isset($valid_values[$q])) {
      $parsed[$q] = $valid_values[$q];
    }
    return purl_path_elements($this, $parsed);
  }

  /**
   * Rewrite the query string. Note that this is being passed through
   * the custom_url_rewrite_inbound() stack and may *not* directly
   * affect $_GET['q']. See purl_init() for how $_GET['q'] is affected
   * by processors.
   */
  public function adjust(&$value, $item, &$q) {
    $q = $this->remove($q, $item);
    $value = $this->remove($value, $item);
  }

  /**
   * Remove our extension from the tail end of the path.
   *
   * @param $q
   *   The current path.
   * @param $element
   *   a purl_path_element object
   * @return path string with the extension removed.
   */
  public function remove($q, $element) {
    $args = explode('.', $q);
    if (count($args > 1)) {
      $extension = array_pop($args);
      if ($element->value == $extension) {
        return implode('.', $args);
      }
    }
    return $q;
  }

  /**
   * Because of the expected usage of the files extensions we don't provide
   * a rewrite.
   */
  public function rewrite(&$path, &$options, $element) {}
}
