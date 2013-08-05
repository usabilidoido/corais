<?php
// $Id: sheetnode_handler_field_date.inc,v 1.1.2.2 2010/12/28 19:36:34 kratib Exp $

class sheetnode_handler_field_date extends views_handler_field {
  function render($values) {
    require_once(drupal_get_path('module', 'sheetnode') . '/socialcalc.inc');
    $value = $values->{$this->field_alias};
    return socialcalc_import_date($value);
  }
}

