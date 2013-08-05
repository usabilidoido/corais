<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function meeting_planner_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'conferencia';
  $context->description = '';
  $context->tag = 'BigBlueButton';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '*/meeting/*' => '*/meeting/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
      'layout' => 'wide',
    ),
    'menu' => 'meetings',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('BigBlueButton');
  $export['conferencia'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'meetings';
  $context->description = '';
  $context->tag = 'BigBlueButton';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'meeting_planner' => 'meeting_planner',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'meetings',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('BigBlueButton');
  $export['meetings'] = $context;

  return $export;
}
