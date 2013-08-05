<?php

class sheetnode_handler_field_cell extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['reference'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['reference'] = array(
      '#type' => 'textfield',
      '#title' => t('Cell reference'),
      '#default_value' => $this->options['reference'],
      '#description' => t('Use a cell coordinate such as A1 to refer to a specific cell.'),
    );
  }

  function render($values) {
    require_once(drupal_get_path('module', 'sheetnode') .'/socialcalc.inc');
    $sheet = $values->{$this->field_alias};
    if (empty($sheet)) return NULL;
    $sc = socialcalc_parse_values($sheet);
    return isset($sc['cells'][$this->options['reference']]) ? $sc['cells'][$this->options['reference']]['datavalue'] : NULL;
  }
}

