<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function atrium_courses_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'courses';
  $context->description = '';
  $context->tag = 'Courses';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'certificate' => 'certificate',
        'course' => 'course',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        '*/takecourse' => '*/takecourse',
        '*/course-outline' => '*/course-outline',
        '*/signups' => '*/signups',
        '*/course-reports/*' => '*/course-reports/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'course-outline' => array(
          'module' => 'course',
          'delta' => 'outline',
          'region' => 'right',
          'weight' => 0,
        ),
        'course-navigation' => array(
          'module' => 'course',
          'delta' => 'navigation',
          'region' => 'right',
          'weight' => 1,
        ),
      ),
    ),
    'menu' => 'courses',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Courses');
  $export['courses'] = $context;

  return $export;
}
