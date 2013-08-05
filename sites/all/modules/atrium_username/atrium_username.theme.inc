<?php
/**
 * Simulate a preprocess function for theme("username")
 * delegating the actual theming to the original theme function/
 */
function atrium_username_preprocess_username($object) {
  $theme_username = function_exists($_SESSION['theme_registry_username_function']) ? $_SESSION['theme_registry_username_function'] : 'theme_username';
  $user =  user_load($object->uid);
  if ($user->uid) {
    $atrium_username = atrium_username_get_name($user->uid);
    $user->name = $atrium_username ? $atrium_username : $user->name;
  }

  return $theme_username($user);
}

/**
 * Override theme_casetracker_comment_changes.
 * Unfortunately we have to rewrite everything as the lines
 * to be changed are inside a switch/case with no other options.
 * Only the case 'assign_to' is modified.
 *
 * Displays the changes a comment has made to the case fields.
 *
 * @param $case_data
 *   An array of both 'old' and 'new' objects that contains
 *   the before and after values this comment has changed.
 */
function atrium_username_theme_casetracker_comment_changes($old, $new) {
  $rows = array();

  $fields = array(
    'pid' => t('Project'),
    'title' => t('Title'),
    'case_status_id' => t('Status'),
    'assign_to' => t('Assigned'),
    'case_priority_id' => t('Priority'),
    'case_type_id' => t('Type'),
  );
  foreach ($fields as $field => $label) {
    if ($new->{$field} != $old->{$field}) {
      switch ($field) {
        case 'pid':
          $old_title = db_result(db_query("SELECT title FROM {node} WHERE nid = %d", $old->pid));
          $new_title = db_result(db_query("SELECT title FROM {node} WHERE nid = %d", $new->pid));
          $old->{$field} = l($old_title, "node/{$old->pid}");
          $new->{$field} = l($new_title, "node/{$new->pid}");
          break;
        case 'case_status_id':
          $old->{$field} = check_plain(casetracker_case_state_load($old->{$field}, 'status'));
          $new->{$field} = check_plain(casetracker_case_state_load($new->{$field}, 'status'));
          break;
        case 'assign_to':
          $old->{$field} = atrium_username_get_name($old->{$field}) ? atrium_username_get_name($old->{$field}) : check_plain(casetracker_get_name($old->{$field}));
          $new->{$field} = atrium_username_get_name($new->{$field}) ? atrium_username_get_name($new->{$field}) : check_plain(casetracker_get_name($new->{$field}));
          break;
        case 'case_priority_id':
          $old->{$field} = check_plain(casetracker_case_state_load($old->{$field}, 'priority'));
          $new->{$field} = check_plain(casetracker_case_state_load($new->{$field}, 'priority'));
          break;
        case 'case_type_id':
          $old->{$field} = check_plain(casetracker_case_state_load($old->{$field}, 'type'));
          $new->{$field} = check_plain(casetracker_case_state_load($new->{$field}, 'type'));
          break;
      }
      $rows[] = array(t('@label: !old &raquo; !new', array('@label' => $label, '!old' => $old->{$field}, '!new' => $new->{$field})));
    }
  }

  if (!empty($rows)) {
    return theme('table', NULL, $rows, array('class' => 'case_changes'));
  }
}

/**
 * Implementation of hook_preprocess_block().
 * - atrium-account: override block title using atrium_username
 *                   check _atrium_block_account()
 *                   in /profiles/openatrium/modules/atrium_features/atrium/atrium.blocks.inc
 */
function atrium_username_preprocess_block(&$variables) {
  // atrium-account
  if ($variables['block']->bid == "atrium-account") {
    global $user;
    if ($user->uid) {
      $atrium_username = atrium_username_get_name($user->uid);
      $user_name = $atrium_username ? $atrium_username : check_plain($user->name);
      $variables['block']->subject = theme('user_picture', $user) . $user_name;
    }
  }
}

