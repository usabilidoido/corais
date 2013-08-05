<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function features_task_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'task_calendar';
  $context->description = '';
  $context->tag = 'Tasks';
  $context->conditions = array(
    'menu' => array(
      'values' => array(
        'tasks' => 'tasks',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
      'layout' => 'wide',
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Tasks');
  $export['task_calendar'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'taskview';
  $context->description = '';
  $context->tag = 'Tasks';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'milestone' => 'milestone',
        'task' => 'task',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-oa_custom_task2-block_3' => array(
          'module' => 'views',
          'delta' => 'oa_custom_task2-block_3',
          'region' => 'right',
          'weight' => 0,
        ),
        'views-oa_custom_task2-block_2' => array(
          'module' => 'views',
          'delta' => 'oa_custom_task2-block_2',
          'region' => 'right',
          'weight' => 1,
        ),
        'block-8' => array(
          'module' => 'block',
          'delta' => 8,
          'region' => 'right',
          'weight' => 2,
        ),
      ),
      'layout' => 'split',
    ),
    'menu' => 'tasks',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Tasks');
  $export['taskview'] = $context;

  return $export;
}
