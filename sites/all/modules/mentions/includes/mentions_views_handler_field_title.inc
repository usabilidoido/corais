<?php
// $Id: mentions_views_handler_field_title.inc,v 1.2 2009/07/23 05:20:03 deciphered Exp $
/**
 * @file
 * Field handler to present a link to mention.
 */
class mentions_views_handler_field_title extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';

    // Invoke mentions_hook_views_handler_field_title_fields().
    foreach (module_list() as $module) {
      if (function_exists($function = 'mentions_'. $module .'_views_handler_field_title_fields')) {
        $function($this);
      }
    }
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Invoke mentions_hook_views_handler_field_title().
    if (function_exists($function = 'mentions_'. $values->{$this->aliases['type']} .'_views_handler_field_title')) {
      return check_plain($function($this, $values));
    }

    return FALSE;
  }
}
