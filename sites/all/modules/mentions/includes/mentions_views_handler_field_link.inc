<?php
// $Id: mentions_views_handler_field_link.inc,v 1.2 2009/07/23 05:20:03 deciphered Exp $
/**
 * @file
 * Field handler to present a link to mention.
 */
class mentions_views_handler_field_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['mid'] = 'mid';

    // Invoke mentions_hook_views_handler_field_link_fields().
    foreach (module_list() as $module) {
      if (function_exists($function = 'mentions_'. $module .'_views_handler_field_link_fields')) {
        $function($this);
      }
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $link = array('options' => array());

    // Invoke mentions_hook_views_handler_field_link().
    if (function_exists($function = 'mentions_'. $values->{$this->aliases['type']} .'_views_handler_field_link')) {
      $function($this, $values, $link);

      $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
      return l($text, $link['url'], $link['options']);
    }

    return FALSE;
  }
}
