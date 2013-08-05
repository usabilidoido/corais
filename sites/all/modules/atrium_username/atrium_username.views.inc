<?php

/**
 * Implementation of hook_views_data_alter().
 */
function atrium_username_views_data_alter(&$data) {

  // override casetracker_case > assign_to
  $data['casetracker_case']['assign_to']['field']['handler'] = 'atrium_username_casetracker_views_handler_field_user_name';

  // oveeride users > name
  $data['users']['name']['field']['handler'] = 'atrium_username_views_handler_field_user_name';

}

/**
 * Implementation of hook_views_handlers().
 */
function atrium_username_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'atrium_username') .'/includes',
    ),
    'handlers' => array(
      'atrium_username_casetracker_views_handler_field_user_name' => array(
        'parent' => 'views_handler_field',
      ),
      'atrium_username_views_handler_field_user_name' => array(
        'parent' => 'views_handler_field_user_name',
      ),
   ),
 );
}

