<?php

/**
 * @file course_enrollment_resource.inc
 * Expose course enrollment as as service resource.
 */
function _course_enrollment_resource() {
  return array(
    'course_enrollment' => array(
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_enrollment_resource'),
        'access callback file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_report_resource'),
        'callback' => '_course_enrollment_retrieve',
        'access callback' => '_course_access_services',
        'access arguments' => array('course_enrollment', 'retrieve'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'ccaid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The course credit ID to get.',
          ),
        ),
      ),
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_enrollment_resource'),
        'access callback file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_report_resource'),
        'callback' => '_course_enrollment_index',
        'access callback' => '_course_access_services',
        'access arguments' => array('course_enrollment', 'index'),
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
            'default value' => variable_get('services_course_enrollment_index_page_size', 20),
            'source' => array('param' => 'pagesize'),
          ),
        ),
      ),
      'create' => array(
        'file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_enrollment_resource'),
        'callback' => '_course_enrollment_create',
        'args' => array(
          array(
            'name' => 'enrollment',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The enrollment to create',
            'type' => 'array',
          ),
        ),
        'access callback' => '_course_access_services',
        'access callback file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_report_resource'),
        'access arguments' => array('course_enrollment', 'create'),
        'access arguments append' => TRUE,
      ),
      'delete' => array(
        'file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_enrollment_resource'),
        'callback' => '_course_enrollment_delete',
        'args' => array(
          array(
            'name' => 'eid',
            'optional' => FALSE,
            'type' => 'int',
            'source' => array('path' => 0),
          ),
        ),
        'access callback' => '_course_access_services',
        'access callback file' => array('type' => 'inc', 'module' => 'course', 'name' => 'services/course_report_resource'),
        'access arguments' => array('course_enrollment', 'delete'),
        'access arguments append' => TRUE,
      ),
    ),
  );
}

/**
 * Delete an enrollment by eid.
 */
function _course_enrollment_delete($eid) {
  if ($enrollment = course_enrolment_load($eid)) {
    $node = node_load($enrollment->nid);
    $user = user_load($enrollment->uid);
    course_unenrol($node, $user);
    return TRUE;
  }
  else {
    services_error('Could not find the enrollment.');
  }

}

/**
 * Get one enrollment record.
 */
function _course_enrollment_retrieve($id) {
  return course_enrolment_load($id);
}

/**
 * Create an enrollment.
 */
function _course_enrollment_create($enrollment) {
  $node = node_load($enrollment['nid']);
  $user = user_load($enrollment['uid']);
  if ($node->nid && $user->uid) {
    if (course_enrolment_check($node->nid, $user->uid)) {
      services_error('Enrollment already exists.');
    }
    if ($enroll = course_enrol($node, $user)) {
      return $enroll;
    }
    else {
      services_error('Enrollment failed.');
    }
  }
  else {
    services_error('Missing parameters nid and uid.');
  }
}

/**
 * Get many course enrollments.
 */
function _course_enrollment_index($page, $fields, $parameters, $page_size) {
  $out = array();

  $result = services_resource_build_index_query('course_enrolment', 'created', $page, $fields, $parameters, 'ce', 'eid', $page_size, 'course_enrollment');
  while ($row = db_fetch_object($result)) {
    $out[] = $row;
  }

  return services_resource_build_index_list($out, 'course_enrollment', 'eid');
}
