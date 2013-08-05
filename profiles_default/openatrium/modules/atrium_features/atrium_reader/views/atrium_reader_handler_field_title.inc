<?php
// $Id$

class atrium_reader_handler_field_title extends views_handler_field {
  /**
   * Override of render(). Use filter_xss() rather than check_plain().
   */
  function render($values) {
    $value = $values->{$this->field_alias};
    return filter_xss($value);
  }
}
