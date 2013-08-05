<?php

class sheetnode_handler_field_date extends views_handler_field {
  function render($values) {
    require_once(drupal_get_path('module', 'sheetnode') . '/socialcalc.inc');
    $value = $values->{$this->field_alias};
    return socialcalc_import_date($value);
  }
}

