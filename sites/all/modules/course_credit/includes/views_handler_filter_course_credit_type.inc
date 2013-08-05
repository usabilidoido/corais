<?php

class views_handler_filter_course_credit_type extends views_handler_filter_in_operator {
  function get_value_options() {
     if (!isset($this->value_options)) {
      $this->value_title = t('Credit type');
      $types = course_credit_get_credit_types();
      foreach ($types as $type => $info) {
        $options[$type] = t($info->title);
      }
      $this->value_options = $options;
    }
  }
}
