<?php

class views_handler_field_ow_want extends views_handler_field_boolean {
  function init(&$view, $options) {
    parent::init($view, $options);

    $default_formats = array(
      'only' => array(0 => t('Offers'), 1 => t('Wants'),),
    );
    $output_formats = isset($this->definition['output formats']) ? $this->definition['output formats'] : array();
    $this->formats = array_merge($default_formats, $output_formats);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['type'] = array('default' => 'only');
    unset($options['not']);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['not'], $form['relationship'], $form['alter'], $form['empty'], $form['empty_zero'], $form['hide_empty']);

  }

  function render($values) {
    //return $values->offers_wants_want;
    return $this->formats['only'][$values->offers_wants_want];
  }
}
