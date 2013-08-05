<?php

/**
 * @file
 * Provides a views filter handler class for the sticky_notes priority field.
 */

 /**
  * Simple filter class to select field for priority
  *
  * @author berliner
  */

class views_handler_filter_sticky_notes_priority extends views_handler_filter {

  /**
   * Provide simple equality operator
   */
  function operator_options() {
    return array(
      '=' => t('Is equal to'),
      '!=' => t('Is not equal to'),
    );
  }

  /**
   * Display the filter on the administrative summary
   */
  function admin_summary() {
    if ($this->options['exposed']) {
      $output = t('exposed');
    }
    else {
      $priorities = sticky_notes_get_priority_array();
      $output  = check_plain((string) $this->operator) . ' ';
      $output .= $priorities[$this->value] ? check_plain((string) $priorities[$this->value]) : t('<Any>');
    }
    return $output;
  }

  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Priority'),
      '#size' => 1,
      '#default_value' => 2,
      '#options' => sticky_notes_get_priority_array(),
      '#multiple' => FALSE,
    );

    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];
      if (!isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }
    }

    // I didn't find another way to assure that the "all" filter is the default
    // filter for the exposed form. So I check here, if the filter has been
    // given as an url argument, if not, we set the form elements value
    // to "all". So I hope that all filter queries are done using GET :-(
    if (!isset($_GET[$this->options['expose']['identifier']])) {
      $form['value']['#value'] = 'All';
    }

  }

  /**
   * Add this filter to the query.
   *
   * Due to the nature of fapi, the value and the operator have an unintended
   * level of indirection. You will find them in $this->operator
   * and $this->value respectively.
   */
  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . $this->operator . " '%s'", $this->value);
  }

}

