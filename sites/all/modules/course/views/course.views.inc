<?php

module_load_include('inc', 'course', 'views/handlers/views_join_course');

/**
 * Implements hook_views_data().
 */
function course_views_data() {
  $data = array();

  // Expose course schema to views.
  module_load_install('course');
  $schema = course_schema();

  foreach ($schema as $table => $schemata) {
    $data[$table]['table']['group'] = t($table);
    foreach ($schemata['fields'] as $key => $field) {
      switch ($field['type']) {
        case 'int':
        case 'numeric':
        case 'float':
          $type = "_numeric";
          break;
        default:
          $type = "";
      }
      $data[$table][$key] = array(
        'title' => ucwords($key),
        'help' => isset($field['description']) ? $field['description'] : $key,
        'field' => array(
          'handler' => "views_handler_field$type",
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => "views_handler_sort$type",
        ),
        'filter' => array(
          'handler' => "views_handler_filter$type",
        ),
        'argument' => array(
          'handler' => "views_handler_argument",
        ),
      );
      if (isset($field['scale'])) {
        $data[$table][$key]['field']['float'] = TRUE;
      }
    }
  }

  $data['course_enrolment']['table']['base'] = array(
    'field' => 'eid',
    'title' => t('Course enrollment'),
    'help' => t('Course enrollments.'),
  );

  $data['course_enrolment']['table']['group'] = t('Course enrolments');

  $data['course_enrolment']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['course_report']['course_enrollment'] = array(
    'title' => 'Course enrollment',
    'relationship' => array(
      'base' => 'course_enrolment',
      'handler' => 'course_views_handler_relationship_ce_cr',
      'label' => t('Enrollment'),
      'help' => 'Create a relationship from the course report to the course enrollment data.',
    ),
  );

  $data['signup_log']['course_enrollment'] = array(
    'title' => 'Course enrollment',
    'relationship' => array(
      'base' => 'course_enrolment',
      'handler' => 'course_views_handler_relationship_ce_sl',
      'label' => t('Enrollment'),
      'help' => 'Create a relationship from the signup log to the course enrollment data.',
    ),
  );

  $data['course_enrolment']['nid'] = array(
    'title' => 'Node ID of enrolment',
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click_sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'title' => 'Course node',
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Enrolled node'),
    ),
  );

  $data['course_enrolment']['uid'] = array(
    'title' => 'User ID of enrolment',
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click_sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user',
    ),
    'relationship' => array(
      'title' => 'User',
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Enrolled user'),
    ),
  );

  $data['users']['course_enrollments'] = array(
    'title' => 'Enrollments',
    'relationship' => array(
      'base' => 'course_enrolment',
      'base field' => 'uid',
      'relationship field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => 'Course enrollment',
      'help' => 'Create a relationship from the user to their enrollments',
    ),
  );

  $data['course_enrolment']['timestamp'] = array(
    'title' => t('Start of enrolment'),
    'help' => t('The date the user started the course.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  $data['course_enrolment']['enrol_end'] = array(
    'title' => t('End of enrolment'),
    'help' => t('The date the user will expire in this course.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  $data['course_enrolment']['eid'] = array(
    'title' => t('Enrolment ID'),
    'help' => t('The ID of the enrolment.'),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['course_node']['table']['group'] = t('Course');

  $data['course_node']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['node']['course'] = array(
    'title' => t('Is a course'),
    'help' => t('Whether or not the content type is marked as a course.'),
    'real field' => 'type',
    'filter' => array(
      'handler' => 'views_handler_filter_course',
    ),
  );

  // Add any necessary overrides below.
  $data['course_node']['catalog']['filter']['handler'] = 'views_handler_filter_boolean_operator';
  $data['course_node']['catalog']['filter']['type'] = 'yes-no';
  $data['course_node']['catalog']['filter']['label'] = 'Catalog';
  $data['course_node']['transcript']['filter']['handler'] = 'views_handler_filter_boolean_operator';
  $data['course_node']['transcript']['filter']['type'] = 'yes-no';
  $data['course_node']['transcript']['filter']['label'] = 'Transcript';
  $data['course_node']['type']['filter']['handler'] = 'views_handler_filter_course_type';

  // Open/close dates.
  $data['course_node']['open'] = $data['course_node']['close'] = array(
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );
  $data['course_node']['open']['title'] = 'Release';
  $data['course_node']['open']['help'] = 'Date course opens';
  $data['course_node']['close']['title'] = 'Expiration';
  $data['course_node']['close']['help'] = 'Date course closes';

  // report
  $data['course_report']['table']['group'] = t('Course report');
  $data['course_report']['table']['base'] = array(
    'field' => 'crid',
    'title' => t('Course report'),
    'help' => t('Aggregated table of reportable activities'),
  );

  // Join to node and user, also provide relationships.
  $data['course_report']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['course_report']['nid'] = array(
    'title' => t('Course node'),
    'help' => t('The node of this course report record.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'label' => t('Course node'),
      'help' => t('Create a relationship from the course report to the course node.'),
    ),
  );

  $data['course_report']['uid'] = array(
    'title' => t('Course user'),
    'help' => t('The user of this course report record.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('Course user'),
      'help' => t('Create a relationship from the course report to the course user.'),
    ),
  );

  $data['course_report']['userid'] = array(
    'title' => t('External user ID'),
    'help' => t('External user ID.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['grade_result'] = array(
    'title' => t('Course grade'),
    'help' => t('Course grade received.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['grade_potential'] = array(
    'title' => t('Grade potential'),
    'help' => t('Max course grade.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['date_completed'] = array(
    'title' => t('Course date completed'),
    'help' => t('The date the user completed a course.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  $data['course_report']['courseid'] = array(
    'title' => t('External course ID'),
    'help' => t('External course ID'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['cecredits'] = array(
    'title' => t('Credits'),
    'help' => t('Amount of credits for a course.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['course_report']['rxcredits'] = array(
    'title' => t('rxcredits'),
    'help' => t('Course rxcredits'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['enrollmenttype'] = array(
    'title' => t('Enrollment type'),
    'help' => t('The module responsible for this enrolment.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['certgrade'] = array(
    'title' => t('Certificate grade'),
    'help' => t('Grade needed to attain a certificate.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['course_report']['section_name'] = array(
    'title' => t('Course status'),
    'help' => t("The user's course status."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['course_report']['complete'] = array(
    'title' => t('Complete'),
    'help' => t('Whether or not the user completed the course.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => 'Course completed',
      'type' => 'yes-no',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['course_report']['cecreditsmax'] = array(
    'title' => t('Max credits'),
    'help' => t('Max credits.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['credits_claimed'] = array(
    'title' => t('Credits claimed'),
    'help' => t('Course credits claimed.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['variable_credits'] = array(
    'title' => t('Variable credits'),
    'help' => t('Course variable credits.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['course_report']['api'] = array(
    'title' => t('Report API'),
    'help' => t('A key representing the module or API responsible for the course report record.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['node']['course_report']['relationship'] = array(
    'handler' => 'course_report_node_vhr',
    'label' => t('Course report'),
    'title' => t('Course report'),
    'help' => t('Create a relationship to the course report entry using this node and current user.'),
    'base' => 'course_report',
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function course_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'course') . '/views/handlers',
    ),
    'handlers' => array(
      'views_handler_filter_course_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'views_handler_filter_course' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'course_views_handler_relationship_ce_cr' => array(
        'parent' => 'views_handler_relationship',
      ),
      'course_views_handler_relationship_ce_sl' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}
