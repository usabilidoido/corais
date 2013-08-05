<?php

/**
 * Filter by course type
 */
class views_handler_filter_course_type extends views_handler_filter_in_operator {

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Course type');
      $types = array('enduring' => 'Enduring', 'live' => 'Live');
      foreach ($types as $type => $name) {
        $options[$type] = t($name);
      }
      $this->value_options = $options;
    }
  }

}
