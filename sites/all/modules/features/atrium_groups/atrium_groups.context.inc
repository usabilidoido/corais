<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function atrium_groups_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'groups_listing';
  $context->description = '';
  $context->tag = 'Groups';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'groups_listing' => 'groups_listing',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'og',
    'block' => array(
      'blocks' => array(),
      'layout' => 'wide',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Groups');
  $export['groups_listing'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'profile_groups';
  $context->description = '';
  $context->tag = 'Profile';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'all',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-groups_listing-block_1' => array(
          'module' => 'views',
          'delta' => 'groups_listing-block_1',
          'region' => 'left',
          'weight' => 0,
        ),
        'views-my_flagged-block_2' => array(
          'module' => 'views',
          'delta' => 'my_flagged-block_2',
          'region' => 'right',
          'weight' => 0,
        ),
        'views-my_flagged-block_1' => array(
          'module' => 'views',
          'delta' => 'my_flagged-block_1',
          'region' => 'right',
          'weight' => 1,
        ),
        'views-my_flagged-block_3' => array(
          'module' => 'views',
          'delta' => 'my_flagged-block_3',
          'region' => 'right',
          'weight' => 2,
        ),
      ),
      'layout' => 'columns',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Profile');
  $export['profile_groups'] = $context;

  return $export;
}
