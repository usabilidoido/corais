<?php

/**
 * Create a relationship from the signup log to course enrolment.
 */
class course_views_handler_relationship_ce_sl extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $join = new views_join_course();
    $join->definition = array(
      'table' => 'course_enrolment',
      'field' => 'uid',
      'left_table' => !empty($this->relationship) ? $this->relationship : 'signup_log',
      'left_field' => 'uid',
      'extra' => array(
        array(
          'field' => 'nid',
          'value' => (!empty($this->relationship) ? $this->relationship : 'signup_log') . '.nid',
          'numeric' => TRUE,
        ),
      ),
    );

    $join->construct();
    $this->ensure_my_table();

    $alias = $join->definition['table'] . '_' . $join->definition['left_table'];
    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }

}
