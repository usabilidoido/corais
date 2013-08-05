<?php
// $Id: facebook_status.inc,v 1.1 2009/07/23 05:20:03 deciphered Exp $
/**
 * @file
 * Contains Facebook-style Statuses module integration for the Mentions module.
 */

/**
 * Implementation of hook_facebook_status_save().
 */
function mentions_facebook_status_save($account, $status, $sid) {
  _mentions_update('facebook_status', mentions_find_users($status), $sid, $account->uid);
}

/**
 * Implementation of mentions_hook_views_data_alter().
 */
function mentions_facebook_status_views_data_alter(&$data) {
  $data['facebook_status']['table']['join']['mentions'] = array(
    'left_table' => 'mentions',
    'left_field' => 'mid',
    'field' => 'sid',
  );
}

/**
 * Implementation of hook_facebook_status_delete().
 */
function mentions_facebook_status_delete($sid) {
  _mentions_delete('facebook_status', $sid);
}

/**
 * Implementation of mentions_hook_views_handler_field_content_fields().
 */
function mentions_facebook_status_views_handler_field_content_fields($view) {
  $view->additional_fields['status'] = array('table' => 'facebook_status', 'field' => 'status');
}

/**
 * Implementation of mentions_hook_views_handler_field_content().
 */
function mentions_facebook_status_views_handler_field_content($view, $values) {
  return $values->{$view->aliases['status']};
}

/**
 * Implementation of mentions_hook_views_handler_field_title().
 */
function mentions_facebook_status_views_handler_field_title($view, $values) {
  return 'Status Update';
}
