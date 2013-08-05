<?php

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _atrium_answers_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'spaces-feature-answers';
  $context->description = 'Questions and Answers, with User Reputations.';
  $context->tag = 'features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'question' => 'question',
      ),
      'options' => array(
        'node_form' => 1,
      ),
    ),
    'views' => array(
      'values' => array(
        'atrium_answers' => 'atrium_answers',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-atrium_answers-block_1' => array(
          'module' => 'views',
          'delta' => 'atrium_answers-block_1',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
      'layout' => 'default',
    ),
    'menu' => 'answers',
  );
  $context->condition_mode = 0;

  $translatables['spaces-feature-answers'] = array(
    t('Questions and Answers, with User Reputations.'),
    t('features'),
  );

  $export['spaces-feature-answers'] = $context;
  return $export;
}

/**
 * Helper to implementation of hook_ctools_plugin_api().
 */
function _atrium_answers_ctools_plugin_api() {
  $args = func_get_args();
  $module = array_shift($args);
  $api = array_shift($args);
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  else if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Helper to implementation of hook_menu_default_items().
 */
function _atrium_answers_menu_default_items() {
  $items = array();

  $items[] = array(
    'title' => 'Answers',
    'path' => 'answers',
    'weight' => '0',
  );
  // Translatables
  array(
    t('Answers'),
  );


  return $items;
}

/**
 * Helper to implementation of hook_user_default_permissions().
 */
function _atrium_answers_user_default_permissions() {
  $permissions = array();

  // Exported permission: access vote up/down statistics
  $permissions[] = array(
    'name' => 'access vote up/down statistics',
    'roles' => array(
      '0' => 'admin',
      '1' => 'manager',
    ),
  );

  // Exported permission: create question content
  $permissions[] = array(
    'name' => 'create question content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: delete any question content
  $permissions[] = array(
    'name' => 'delete any question content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'manager',
    ),
  );

  // Exported permission: delete own question content
  $permissions[] = array(
    'name' => 'delete own question content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit any question content
  $permissions[] = array(
    'name' => 'edit any question content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'manager',
    ),
  );

  // Exported permission: edit own question content
  $permissions[] = array(
    'name' => 'edit own question content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: reset vote up/down votes
  $permissions[] = array(
    'name' => 'reset vote up/down votes',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: see vote up/down node stats
  $permissions[] = array(
    'name' => 'see vote up/down node stats',
    'roles' => array(
      '0' => 'admin',
      '1' => 'manager',
    ),
  );

  // Exported permission: use vote up/down
  $permissions[] = array(
    'name' => 'use vote up/down',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: use vote up/down on comments
  $permissions[] = array(
    'name' => 'use vote up/down on comments',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: use vote up/down on nodes
  $permissions[] = array(
    'name' => 'use vote up/down on nodes',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view own userpoints
  $permissions[] = array(
    'name' => 'view own userpoints',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view userpoints
  $permissions[] = array(
    'name' => 'view userpoints',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  return $permissions;
}

/**
 * Helper to implementation of hook_strongarm().
 */
function _atrium_answers_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'atrium_update_type_question';
  $strongarm->value = 1;

  $export['atrium_update_type_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_question';
  $strongarm->value = 0;

  $export['comment_anonymous_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_question';
  $strongarm->value = '3';

  $export['comment_controls_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_question';
  $strongarm->value = '2';

  $export['comment_default_mode_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_question';
  $strongarm->value = '2';

  $export['comment_default_order_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_question';
  $strongarm->value = '300';

  $export['comment_default_per_page_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_question';
  $strongarm->value = '1';

  $export['comment_form_location_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_question';
  $strongarm->value = '0';

  $export['comment_preview_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_question';
  $strongarm->value = 2;

  $export['comment_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_question';
  $strongarm->value = '0';

  $export['comment_subject_field_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_upload_question';
  $strongarm->value = '1';

  $export['comment_upload_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_question';
  $strongarm->value = array(
    '0' => 'status',
    '1' => 'revision',
  );

  $export['node_options_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_content_type_question';
  $strongarm->value = array(
    '0' => 'thread',
    '1' => 'nodetype',
    '2' => 'author',
    '3' => 'grouptype',
  );

  $export['notifications_content_type_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_content_type_usage_question';
  $strongarm->value = 'group_post_standard';

  $export['og_content_type_usage_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'show_diff_inline_question';
  $strongarm->value = 0;

  $export['show_diff_inline_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'show_preview_changes_question';
  $strongarm->value = 0;

  $export['show_preview_changes_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_question';
  $strongarm->value = '1';

  $export['upload_question'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_category_default_tid';
  $strongarm->value = '0';

  $export['userpoints_category_default_tid'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_category_profile_display_tid';
  $strongarm->value = 'all';

  $export['userpoints_category_profile_display_tid'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_display_message';
  $strongarm->value = '1';

  $export['userpoints_display_message'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_expireafter_date';
  $strongarm->value = '';

  $export['userpoints_expireafter_date'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_expireon_date';
  $strongarm->value = array(
    'month' => '1',
    'day' => '1',
    'year' => '1980',
  );

  $export['userpoints_expireon_date'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_expiry_description';
  $strongarm->value = '';

  $export['userpoints_expiry_description'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_points_down';
  $strongarm->value = 2;

  $export['userpoints_karma_points_down'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_points_notify';
  $strongarm->value = 1;

  $export['userpoints_karma_points_notify'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_points_up';
  $strongarm->value = '10';

  $export['userpoints_karma_points_up'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_points_voter';
  $strongarm->value = '0';

  $export['userpoints_karma_points_voter'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_points_voter_change';
  $strongarm->value = 0;

  $export['userpoints_karma_points_voter_change'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_points_voter_hide';
  $strongarm->value = 0;

  $export['userpoints_karma_points_voter_hide'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_points_voter_own';
  $strongarm->value = 0;

  $export['userpoints_karma_points_voter_own'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_points_voter_tid';
  $strongarm->value = '0';

  $export['userpoints_karma_points_voter_tid'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_karma_tag';
  $strongarm->value = 'userpoints_karma';

  $export['userpoints_karma_tag'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_points_moderation';
  $strongarm->value = '0';

  $export['userpoints_points_moderation'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_report_displayzero';
  $strongarm->value = '1';

  $export['userpoints_report_displayzero'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_report_limit';
  $strongarm->value = '100';

  $export['userpoints_report_limit'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_report_usercount';
  $strongarm->value = '100';

  $export['userpoints_report_usercount'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_trans_lcpoint';
  $strongarm->value = 'point';

  $export['userpoints_trans_lcpoint'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_trans_lcpoints';
  $strongarm->value = 'points';

  $export['userpoints_trans_lcpoints'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_trans_ucpoint';
  $strongarm->value = 'Point';

  $export['userpoints_trans_ucpoint'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_trans_ucpoints';
  $strongarm->value = 'Points';

  $export['userpoints_trans_ucpoints'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_trans_uncat';
  $strongarm->value = 'Uncategorized';

  $export['userpoints_trans_uncat'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'userpoints_transaction_timestamp';
  $strongarm->value = ' 1';

  $export['userpoints_transaction_timestamp'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'votingapi_anonymous_window';
  $strongarm->value = 86400;

  $export['votingapi_anonymous_window'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'votingapi_calculation_schedule';
  $strongarm->value = 'immediate';

  $export['votingapi_calculation_schedule'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_comment_node_types';
  $strongarm->value = array(
    'question' => 'question',
  );

  $export['vud_comment_node_types'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_comment_reset';
  $strongarm->value = '0';

  $export['vud_comment_reset'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_comment_votes';
  $strongarm->value = '0';

  $export['vud_comment_votes'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_comment_widget';
  $strongarm->value = 'updown';

  $export['vud_comment_widget'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_node_reset';
  $strongarm->value = '0';

  $export['vud_node_reset'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_node_types';
  $strongarm->value = array(
    'question' => 'question',
  );

  $export['vud_node_types'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_node_votes';
  $strongarm->value = '0';

  $export['vud_node_votes'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_node_widget';
  $strongarm->value = 'updown';

  $export['vud_node_widget'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_node_widget_show';
  $strongarm->value = '2';

  $export['vud_node_widget_show'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'vud_tag';
  $strongarm->value = 'userpoints_karma';

  $export['vud_tag'] = $strongarm;
  return $export;
}
