<?php

/**
 * @file
 * Provide views filter handler for workflow.module.
 */

/**
 * Filter by state.
 */
class workflow_views_handler_filter_sid extends views_handler_filter_in_operator {
  var $value_form_type = 'select';

  function has_extra_options() {
    return $this->options['limit'];
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['limit'] = array('default' => TRUE);
    $options['wid'] = array('default' => 0);
    $options['include_creation'] = TRUE;
    return $options;
  }

  function extra_options_form(&$form, &$form_state) {
    if ($this->options['limit']) {
      // We only do this when the form is displayed so this query doesn't run
      // unnecessarily just when the object is constructed.
      if ($this->options['wid'] == 0) {
        $this->options['wid'] = db_result(db_query('SELECT MIN(wid) FROM {workflows}'));
      }
      $form['wid'] = array(
        '#prefix' => '<div class="views-left-40">',
        '#suffix' => '</div>',
        '#type' => 'radios',
        '#title' => t('Workflows'),
        '#options' => workflow_get_all(),
        '#description' => t('Select which workflow the states are given from.'),
        '#default_value' => $this->options['wid'],
      );
    }
    $form['include_creation'] = array(
      '#type' => 'checkbox',
      '#title' => 'Include (creation) state?',
      '#default_value' => $this->options['include_creation'],
    );
  }

  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $this->value_title = t('Workflow state');
    if ($this->options['limit']) {
      $this->value_options = workflow_get_states($this->options['wid']);
      if (!$this->options['include_creation']) {
        foreach ($this->value_options as $key => $state) {
          if ($state == '(creation)') {
            unset($this->value_options[$key]);
          }
        }
      }
      return;
    }

    $workflows = workflow_get_all();
    if (count($workflows) > 1) {
      $states = array('' => t('No state'));
      foreach ($workflows as $wid => $wname) {
        $states[$wname] = workflow_get_states($wid);
      }
    }
    else {
      $states = workflow_get_states();
    }
    if (!$this->options['include_creation']) {
      foreach ($states as $key => $state) {
        if ($state == '(creation)') {
          unset($states[$key]);
        }
      }
    }
    $this->value_options = $states;
  }

  function query() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();
    $placeholder = !empty($this->definition['numeric']) ? '%d' : "'%s'";

    if (count($this->value) == 1) {
      $this->operator = ($this->operator == 'in') ? '= ' : '!= ';
      $in = !empty($this->definition['numeric']) ? '%d' : "'%s'";
    }
    else {
      $replace = array_fill(0, sizeof($this->value), $placeholder);
      $in = ' (' . implode(", ", $replace) . ')';
    }
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . $this->operator . $in, $this->value);
  }
}
