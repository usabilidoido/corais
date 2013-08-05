<?php
// $Id: reldate_handler_field_ncs_last_updated.inc,v 1.1.2.2 2010/08/11 14:51:17 yhahn Exp $

class reldate_handler_field_ncs_last_updated extends views_handler_field_ncs_last_updated {
  /**
   * Override of option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['reldate'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Override of options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['reldate'] = array(
      '#title' => t('Custom formats'),
      '#type' => 'select',
      '#options' => array(
        FALSE => '< '. t('Custom format') .' >',
        'reldate' => t('Relative date'),
      ),
      '#default_value' => $this->options['reldate'],
      '#description' => t('Use a custom format for this field. This will override any other date format settings.'),
    );
    if (module_exists('date_api')) {
      $time = time();
      $custom_formats = array_diff_key(date_get_formats(), array_fill_keys(array('short', 'medium', 'long', 'custom'), TRUE));
      if (!empty($custom_formats)) {
        foreach ($custom_formats as $format => $format_options) {
          $form['reldate']['#options'][$format] = format_date($time, 'custom', variable_get("date_format_{$format}", key($format_options)));
        }
      }
    }
  }

  /**
   * Override of render().
   */
  function render($values) {
    if (!empty($this->options['reldate'])) {
      $value = $values->{$this->field_alias};
      if ($this->options['reldate'] === 'reldate') {
        return $value ? reldate_format_date($value) : theme('views_nodate');
      }
      else {
        $format = variable_get("date_format_{$this->options['reldate']}", FALSE);
        return $value && $format ? format_date($value, 'custom', $format) : theme('views_nodate');
      }
    }
    return parent::render($values);
  }
}
