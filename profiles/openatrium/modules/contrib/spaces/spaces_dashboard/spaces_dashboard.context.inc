<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function spaces_dashboard_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'spaces_dashboard';
  $context->description = 'Dashboard';
  $context->tag = 'Dashboard';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'dashboard' => 'dashboard',
        'dashboard/*' => 'dashboard/*',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'dashboard',
  );
  $context->condition_mode = 0;

  $translatables['spaces_dashboard'] = array(
    t('Dashboard'),
  );

  $export['spaces_dashboard'] = $context;
  return $export;
}
