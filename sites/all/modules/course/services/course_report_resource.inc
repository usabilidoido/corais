<?php

/**
 * @file course_report_resource.inc
 * Expose the course report as as service resource.
 */

function _course_report_resource() {
  return array(
    'course_report' => array(
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_report_resource'),
        'access callback file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_report_resource'),
        'callback' => '_course_report_retrieve',
        'access callback' => '_course_access_services',
        'access arguments' => array('course_report', 'retrieve'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'crid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The course report ID to get.',
          ),
        ),
      ),
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_report_resource'),
        'access callback file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_report_resource'),
        'callback' => '_course_report_index',
        'access callback' => '_course_access_services',
        'access arguments' => array('course_report', 'index'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'page',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'The zero-based index of the page to get, defaults to 0.',
            'default value' => 0,
            'source' => array('param' => 'page'),
          ),
          array(
            'name' => 'fields',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The fields to get.',
            'default value' => '*',
            'source' => array('param' => 'fields'),
          ),
          array(
            'name' => 'parameters',
            'optional' => TRUE,
            'type' => 'array',
            'description' => 'Parameters array',
            'default value' => array(),
            'source' => array('param' => 'parameters'),
          ),
          array(
            'name' => 'pagesize',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'Number of records to get per page.',
            'default value' => variable_get('services_course_index_page_size', 20),
            'source' => array('param' => 'pagesize'),
          ),
        ),
      ),
    ),
  );
}

/**
 * Access callback for course services.
 */
function _course_access_services($resource, $op) {
  $access = user_access('access all course reports');
  if ($access) {
    return $access;
  }
  else {
    return services_error("No access to $resource.$op");
  }
}

/**
 * Get one course report record.
 */
function _course_report_retrieve($id) {
  $course_report = course_report_load($id);
  return $course_report;
}

/**
 * Get many course report records.
 */
function _course_report_index($page, $fields, $parameters, $page_size) {
  $out = array();

  $result = services_resource_build_index_query('course_report', 'date_completed', $page, $fields, $parameters, 'cr', 'id', $page_size, 'course_report');
  while ($row = db_fetch_object($result)) {
    $out[] = $row;
  }

  return services_resource_build_index_list($out, 'course_report', 'id');
}
