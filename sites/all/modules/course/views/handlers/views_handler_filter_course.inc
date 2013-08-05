<?php

/**
 * A filter that allows the user to show or hide all courses.
 */
class views_handler_filter_course extends views_handler_filter_boolean_operator {

  function query() {
    $this->ensure_my_table();
    $types = course_get_types();
    $in = $this->options['value'] ? 'in' : 'not in';
    $placeholders = db_placeholders($types, 'varchar');
    $where = "$this->table_alias.$this->real_field $in ($placeholders)";
    $this->query->add_where($this->options['group'], $where, $types);
  }

  function get_value_options() {
    $this->value_options = array(1 => t('Yes'), 0 => t('No'));
  }

}
