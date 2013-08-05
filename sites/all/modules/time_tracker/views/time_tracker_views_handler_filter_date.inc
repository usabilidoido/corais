<?php

/**
 * Filter to handle time tracker dates.
 * Accepts Relative time values like "bimonthly"
 */
class time_tracker_views_handler_filter_date extends views_handler_filter_numeric {

  /**
   * Convert the relative timestamp values if they exist so that the view is
   * initialized properly with the correct filtering
   */
  function init(&$view, $options) {
    // if (module_exists('date_popup') && $options['value']['type'] == 'date_popup') {
    //   $options['value']['value']= date('Y-m-d H:i:s', $this->process_relative_time_frame($options['value']['value'], 'min'));
    //   $options['value']['min'] = date('Y-m-d H:i:s', $this->process_relative_time_frame($options['value']['min'], 'min'));
    //   $options['value']['max'] = date('Y-m-d H:i:s', $this->process_relative_time_frame($options['value']['max'], 'max'));
    // }
    parent::init($view, $options);
  }

  function option_definition() {
    $options = parent::option_definition();

    // value is already set up properly, we're just adding our new field to it.
    $options['value']['type']['default'] = 'date';

    return $options;
  }

  /**
   * Add a type selector to the value format
   */
  function value_form(&$form, &$form_state) {
    if (empty($form_state['exposed'])) {
      $form['value']['type'] = array(
        '#type' => 'radios',
        '#title' => t('Value type'),
        '#options' => array(
          'date' => t('A date in any machine readable format. CCYY-MM-DD HH:MM:SS is preferred.'),
          'offset' => t('An offset from the current time such as "+1 day" or "-2 hours -30 minutes"'),
          'relative' => t('A date in any machine readable format OR an expression representing a date relative to the current date. See Accepted Relative Values below for a list of supported expressions'),
        ),
        '#default_value' => !empty($this->value['type']) ? $this->value['type'] : 'relative',
      );
      if (module_exists('date_popup')) {
        $form['value']['type']['#options']['date_popup'] = t('Use a date popup field for date entry. Relative values can be used for the default value of the field, but won\'t be accepted on the exposed form.');
      }
      $form['value']['accepted_relative'] = array(
        '#title' => t('Accepted Relative Values'),
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => 20,
        '#attributes' => array('class' => 'accepted-relative-values-fieldset'),
      );
      $form['value']['accepted_relative']['info'] = array(
        '#type' => 'markup',
        '#value' => "All expressions are case insensitive. <br/>" .
                    "'<strong>First Of This Month</strong>' = First Day of this Month<br/>" .
                    "'<strong>Last Of This Month</strong>' = Last Day of this Month<br/>" .
                    "'<strong>(<i>n</i>)th/(<i>n</i>)st Of This Month</strong>' =  <i>n</i> being a numbered day, this expression will represent that numbered day of this month. e.g. 21st Of This Month or 14th Of This Month<br/>" .
                    "'<strong>Bimonthly</strong>' = Put in both Min and Max to dynamically generate a date range based on the current date. e.g. if it is the 3rd of the month, it will return a range of the 1st to the 15th. If it is after the 15th, it will return the 16th to the end of the month.",
        '#attributes' => array('class' => 'accepted-relative-values'),
      );
    }
    parent::value_form($form, $form_state);
  }

  /**
   * Override the exposed form to use a date_popup if enabled
   */
  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    // Get the filter identifier. This is defined in views in the filter settings
    // so it could vary from the usual 'timestamp', 'start', and 'end'
    $filter_identifier = $this->options['expose']['identifier'];
    // Check if the date popup module is enabled
    if (module_exists('date_popup') && $this->options['value']['type'] == 'date_popup') {
      if (variable_get('time_entry_method', 'duration') == 'duration' || $filter_identifier == 'timestamp') {
        $format = variable_get('timestamp_date_format', 'F d, Y');
      }
      else {
        $format = variable_get('time_interval_date_format', 'h:i A - M d, Y');
      }
      if (isset($form[$filter_identifier]['#type'])) {
        $timestamp = $this->check_exposed_input_timestamp($form_state['input']['timestamp']);
        $form[$filter_identifier]['#type'] = 'date_popup';
        $form[$filter_identifier]['#date_format'] = $format;
        // Process the value for relative timeframes
        // I don't know why but the datepopup widget won't take our format
        // for the default value so we have to use one it understands
        $form[$filter_identifier]['#default_value'] = date(
          'Y-m-d H:i:s',
          $this->process_relative_time_frame($timestamp, 'min')
        );
        // For some reason the date popup #default_value won't be set when you
        // first visit the page, before exposed input, unless we set the #value
        $form[$filter_identifier]['#value'] = $form[$filter_identifier]['#default_value'];
      }
      elseif (isset($form['timestamp']['min']) && isset($form['timestamp']['max'])) {

        $min = $this->check_exposed_input_timestamp($form_state['input']['timestamp']['min']);

        $form[$filter_identifier]['min']['#type'] = 'date_popup';
        $form[$filter_identifier]['min']['#date_format'] = $format;
        // Check if the default value is relative
        $form[$filter_identifier]['min']['#default_value'] = date(
          'Y-m-d H:i:s',
          $this->process_relative_time_frame($min, 'min')
        );
        // For some reason the date popup #default_value won't be set when you
        // first visit the page, before exposed input, unless we set the #value
        $form[$filter_identifier]['min']['#value'] = $form[$filter_identifier]['min']['#default_value'];

        // Process the "max" value
        $max = $this->check_exposed_input_timestamp($form_state['input']['timestamp']['max']);

        $form[$filter_identifier]['max']['#type'] = 'date_popup';
        $form[$filter_identifier]['max']['#date_format'] = $format;
        // Check if the default value is relative
        $form[$filter_identifier]['max']['#default_value'] = date(
          'Y-m-d H:i:s',
          $this->process_relative_time_frame($max, 'max')
        );
        // For some reason the date popup #default_value won't be set when you
        // first visit the page, before exposed input, unless we set the #value
        $form[$filter_identifier]['max']['#value'] = $form[$filter_identifier]['max']['#default_value'];
      }
    }
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);

    if (!empty($form_state['values']['options']['expose']['optional'])) {
      // Who cares what the value is if it's exposed and optional.
      return;
    }

    $this->validate_valid_time($form['value'], $form_state['values']['options']['operator'], $form_state['values']['options']['value']);
  }

  function exposed_validate(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    if (!empty($this->options['expose']['optional'])) {
      // Who cares what the value is if it's exposed and optional.
      return;
    }

    $value = &$form_state['values'][$this->options['expose']['identifier']];
    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator'])) {
      $operator = $form_state['values'][$this->options['expose']['operator']];
    }
    else {
      $operator = $this->operator;
    }

    $this->validate_valid_time($this->options['expose']['identifier'], $operator, $value);

  }

  /**
   * Validate that the time values convert to something usable.
   */
  function validate_valid_time(&$form, $operator, $value) {
    $operators = $this->operators();

    if ($operators[$operator]['values'] == 1 && $value['value']) {
      $convert = strtotime($value['value']);
      if ($convert == -1 || $convert === FALSE) {
        form_error($form['value'], t('Invalid date format.'));
      }
    }
    elseif ($operators[$operator]['values'] == 2) {
      if ($value['type'] == 'relative' || $value['type'] == 'date_popup') {
        $value['min'] = date('m/d/Y', $this->process_relative_time_frame($this->value['min'], 'min'));
        $value['max'] = date('m/d/Y', $this->process_relative_time_frame($this->value['max'], 'max'));
      }
      $min = strtotime($value['min']);
      if ($min == -1 || $min === FALSE) {
        form_error($form['min'], t('Invalid date format.'));
      }
      $max = strtotime($value['max']);
      if ($max == -1 || $max === FALSE) {
        form_error($form['max'], t('Invalid date format.'));
      }
    }
  }

  function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // Store this because it will get overwritten.
    $type = $this->value['type'];
    $rc = parent::accept_exposed_input($input);

    // Don't filter if value(s) are empty.
    $operators = $this->operators();
    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator'])) {
      $operator = $input[$this->options['expose']['operator']];
    }
    else {
      $operator = $this->operator;
    }

    if ($operators[$operator]['values'] == 1) {
      if ($this->value['value'] == '') {
        return FALSE;
      }
    }
    else {
      if ($this->value['min'] == '' || $this->value['max'] == '') {
        return FALSE;
      }
    }
    //dpm($operators);
    // restore what got overwritten by the parent.
    $this->value['type'] = $type;
    return $rc;
  }

  function op_between($field) {

    if ($this->operator == 'between') {
      $a = intval(strtotime($this->value['min'], 0));
      $b = intval(strtotime($this->value['max'], 0));
    }
    else {
      $a = intval(strtotime($this->value['max'], 0));
      $b = intval(strtotime($this->value['min'], 0));
    }

    if ($this->value['type'] == 'offset') {
      $a = '***CURRENT_TIME***' . sprintf('%+d', $a); // keep sign
      $b = '***CURRENT_TIME***' . sprintf('%+d', $b); // keep sign
    }

    if ($this->value['type'] == 'relative') {
      $a = $this->process_relative_time_frame($this->value['min'], 'min');
      $b = $this->process_relative_time_frame($this->value['max'], 'max');
    }

    //dpm($a);
    // %s is safe here because strtotime scrubbed the input and we might
    // have a string if using offset.
    $this->query->add_where($this->options['group'], "$field >= %s", $a);
    $this->query->add_where($this->options['group'], "$field <= %s", $b);
  }

  function op_simple($field) {
    $value = intval(strtotime($this->value['value'], 0));
    if (!empty($this->value['type']) && $this->value['type'] == 'offset') {
      $value = '***CURRENT_TIME***' . sprintf('%+d', $value); // keep sign
    }
    $this->query->add_where($this->options['group'], "$field $this->operator %s", $value);
  }

  /**
   * Checks the value to see if a relative time value has been passed in
   * If it has, we convert it to an actual timestamp
   */
  function process_relative_time_frame($value, $limit = 'min') {
    $timestamp = 0;
    $value = drupal_strtolower($value);
    switch ($value) {
      case 'first of this month':
        $timestamp = strtotime(date('m') . '/01/' . date('Y'));
        break;
      case 'last of this month':
        $timestamp = strtotime('-1 hour', strtotime('+1 month', strtotime(date('m') . '/01/' . date('Y'))));;
        break;
      case 'bimonthly':
        if ($limit == 'min') {
          if (date('j') < 16) {
            $timestamp = strtotime(date('m') . '/01/' . date('Y'));
          }
          else {
            $timestamp = strtotime(date('m') . '/16/' . date('Y'));
          }
        }
        else {
          if (date('j') < 16) {
            $timestamp = strtotime(date('m') . '/15/' . date('Y'));
          }
          else {
            $timestamp = strtotime('-1 hour', strtotime('+1 month', strtotime(date('m') . '/01/' . date('Y'))));
          }
        }
        break;
      default:
        $convert = strtotime($value);
        if ($convert == -1 || $convert === FALSE) {
          $matches = array();
          preg_match('/[0-9]+/', $value, $matches);
          if (isset($matches[0]) && is_numeric($matches[0])) {
            if (preg_match('/this month/', $value)) {
              $timestamp = strtotime(date('m') .'/'. $matches[0] .'/'. date('Y'));
            }
          }
        }
        else {
          $timestamp = $convert;
        }

    }
    return $timestamp;
  }

  /**
   * Default value for the field will be in form_state, but it will be an
   * array if it's exposed input and a value if it's the default value
   */
  function check_exposed_input_timestamp($value) {
    if (is_array($value)) {
      return $value['date'];
    }
    else {
      return $value;
    }
  }
}
