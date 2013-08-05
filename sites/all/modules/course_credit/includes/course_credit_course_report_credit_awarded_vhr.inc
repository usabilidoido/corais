<?php

/**
 * Relate the course report to awarded credit.
 *
 * Join report.uid to awarded.uid, and report.nid to awarded.nid
 */
class course_credit_course_report_credit_awarded_vhr extends views_handler_relationship {
  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $join = new views_join_course_credit();
    $join->definition = array(
      'table' => 'course_credit_awarded',
      'field' => 'uid',
      'left_table' => !empty($this->relationship) ? $this->relationship : 'course_report',
      'left_field' => 'uid',
      'extra' => array(
        array(
          'field' => 'nid', // course_credit_awarded.nid
          'numeric' => TRUE,
          'value' => (!empty($this->relationship) ? $this->relationship : 'course_report') . '.nid',
        ),
      ),
    );

    $join->construct();
    $this->ensure_my_table();

    $alias = $join->definition['table'] .'_'. $join->definition['left_table'];
    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }
}
