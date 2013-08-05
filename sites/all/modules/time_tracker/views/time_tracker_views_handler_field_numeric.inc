<?php

class time_tracker_views_handler_field_numeric extends views_handler_field_numeric {
  function option_definition() {
    $options = parent::option_definition();

    $options['output_format'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['output_format'] = array(
      '#type' => 'select',
      '#title' => t('Output Format'),
      '#default_value' => $this->options['output_format'],
      '#description' => t('The format to output this number as in the view. If "Time" or "Time String" is chosen, number formatting will be ignored (e.g. rounding, format plural, prefix/suffix etc.'),
      '#options' => array(
        'hours' => 'Hours',
        'minutes' => 'Minutes',
        'seconds' => 'Seconds',
        'clock' => 'Time (e.g. hh:ss)',
        'time' => 'Time String (e.g. x hours, x mins)',
      ),
    );

  }

  function render($values) {
    $saved_values = unserialize(serialize($values));
    $value = $values->{$this->field_alias};
    if ($this->options['output_format'] == 'clock') {
      return format_hours_to_hours_and_minutes($value, TRUE);
    }
    elseif ($this->options['output_format'] == 'time') {
      return format_hours_to_hours_and_minutes($value);
    }
    else {
      // Convert number to proper unit -- Database value is in hours
      if ($this->options['output_format'] == 'hours') {
        $saved_values->{$this->field_alias} = $value;
      }
      if ($this->options['output_format'] == 'minutes') {
        $saved_values->{$this->field_alias} = round($value * 60);
      }
      elseif ($this->options['output_format'] == 'seconds') {
        $saved_values->{$this->field_alias} = round($value * 60 * 60);
      }
      return parent::render($saved_values);
    }
  }
}
