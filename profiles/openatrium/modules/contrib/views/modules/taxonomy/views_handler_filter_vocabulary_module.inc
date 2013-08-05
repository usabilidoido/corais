<?php
// $Id$

/**
 * Filter by vocabulary module.
 */
class views_handler_filter_vocabulary_module extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $voc) {
      $this->value_options[$voc->module] = $voc->module;
    }
  }
}
