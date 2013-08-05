<?php
// $Id: mentions_views_handler_field_content.inc,v 1.2 2009/07/23 05:20:03 deciphered Exp $
/**
 * @file
 * Field handler to present mention content.
 */
class mentions_views_handler_field_content extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';

    // Invoke mentions_hook_views_handler_field_content_fields().
    foreach (module_list() as $module) {
      if (function_exists($function = 'mentions_'. $module .'_views_handler_field_content_fields')) {
        $function($this);
      }
    }
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Invoke mentions_hook_views_handler_field_content().
    if (function_exists($function = 'mentions_'. $values->{$this->aliases['type']} .'_views_handler_field_content')) {
      $function($this, $values, $link);

      return check_markup($function($this, $values));
    }

    return FALSE;
  }

  function element_type() {
    if (isset($this->definition['element type'])) {
      return $this->definition['element type'];
    }

    return 'div';
  }
}
