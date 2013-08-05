<?php
// $Id: imagepicker.group.inc,v 1.1.2.14 2010/10/09 22:37:53 hutch Exp $
// $Name: DRUPAL-6--2-10 $

/**
 * @file
 * contains the functions for group management
 */

function imagepicker_groups() {
  drupal_add_js(IMAGEPICKER_PATH .'/imagepicker.js');

  // from iframe
  if (! arg(2)) {
    $content = imagepicker_groups_list();
    $content .= drupal_get_form('imagepicker_groups_form');
  }
  elseif ( arg(2) == 'edit') {
    $record = imagepicker_get_user_group(arg(3));
    $content .= drupal_get_form('imagepicker_groups_form', $record);
  }
  elseif (arg(2) == 'delete') {
    $content .= drupal_get_form('imagepicker_group_delete_form', arg(3));
  }
  elseif ( is_numeric(arg(2)) && arg(2) > 0 ) {
    imagepicker_set_user_group_state(1, arg(2));
    drupal_goto('imagepicker/browse');
  }
  return theme('imagepicker_iframe', $content);
}

/**
 * groups
 * from imagepicker groups in my account
 */
function imagepicker_user_groups($mode="", $gid=0) {
  $content = '';
  // $mode is edit, delete or empty
  if (empty($mode)) {
    $content .= imagepicker_groups_list('account');
    $content .= drupal_get_form('imagepicker_groups_form');
  }
  elseif ($mode == 'edit') {
    $record = imagepicker_get_user_group($gid);
    $content .= drupal_get_form('imagepicker_groups_form', $record);
  }
  elseif ($mode == 'delete') {
    $content .= drupal_get_form('imagepicker_group_delete_form', $gid);
  }
  return $content;
}

function imagepicker_groups_form(&$form_state, $record=0, $account=FALSE) {
  $form['groupsave'] = array(
    '#type' => 'fieldset',
    '#title' => ( $record ?  t('Edit group') : t('Add group')),
    '#description' => t('Give your group a brief name and optionally put any additional information in the group description box'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['groupsave']['group_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Group name'),
    '#size' => 20,
    '#default_value' => (isset($record->group_name) ? $record->group_name : ''),
    '#description' => t(''),
    '#required' => TRUE
  );
  $form['groupsave']['group_description'] = array(
    '#type' => 'textfield',
    '#title' => t('group description'),
    '#size' => 50,
    '#maxlength' => 50,
    '#default_value' => (isset($record->group_description) ? $record->group_description : ''),
    '#description' => t('Maximum 50 characters'),
    '#required' => FALSE
  );
  if (user_access('use public imagepicker') && user_access('create public groups') && variable_get('imagepicker_public_enabled', 1)) {
    $form['groupsave']['group_public'] = array(
      '#type' => 'checkbox',
      '#title' => t('Public'),
      '#return_value' => 1,
      '#default_value' => (isset($record->public) ? $record->public : ''),
      '#description' => t('Make this group publicly available'),
    );
    // Create role restrictions
    if (variable_get('imagepicker_publicroles_enabled', 1)) {
      $form['groupsave']['group_public_roles'] = array(
        '#type' => 'fieldset',
        '#title' => ( isset($record->gid) ?  t('Edit group roles') : t('Add public group roles')),
        '#description' => t('Add roles for public access to your group. (Don\'t check anything to make it available to all roles.)'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      // Output a list of available roles as checkboxes
      $form['groupsave']['group_public_roles']['roles']['#tree'] = TRUE;
      if (isset($record->avail_roles)) {
        if ($record->avail_roles != 'all') $availroles = explode(':', $record->avail_roles);
      }
      foreach (user_roles(TRUE) as $rid => $role) {
        if (imagepicker_role_has_permission($rid)) {
          if (isset($availroles) && is_array($availroles) && in_array($role, array_values($availroles))) $checked = TRUE;
          else $checked = FALSE;
          $form['groupsave']['group_public_roles']['roles'][$rid] = array(
            '#type' => 'checkbox',
            '#title' => $role,
            '#return_value' => $role,
            '#default_value' => ($checked ? 1 : ''),
          );
        }
      }
    }
  }

  if ($account) {
    $form['groupsave']['account'] = array(
      '#type' => 'value',
      '#value' => $account->uid,
    );
  }
  if (isset($record->gid)) {
    $form['groupsave']['gid'] = array(
      '#type' => 'value',
      '#value' => $record->gid,
    );
  }
  $form['groupsave']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save group'),
  );
  return $form;
}

function imagepicker_groups_form_submit($form, &$form_state) {
  global $user;
  $record['group_name'] = $form_state['values']['group_name'];
  $record['group_description'] = $form_state['values']['group_description'];
  $record['public'] = $form_state['values']['group_public'];

  // Check role restrictions submitted
  if ( isset($form_state['values']['roles']) && $form_state['values']['roles']) {
    $found = 0;
    foreach ($form_state['values']['roles'] as $rid => $role) {
      if (!$role) {
        unset($form_state['values']['roles'][$rid]);
      }
      else {
        $found++;
      }
    }
    if ($found) {
      $record['avail_roles'] = implode(':', $form_state['values']['roles']);
    }
    else {
      $record['avail_roles'] = 'all';
    }
  }

  if ( isset($form_state['values']['account'])) {
    $record['uid'] = $form_state['values']['account'];
  }
  else {
    $record['uid'] = $user->uid;
  }
  if (isset($form_state['values']['gid'])) {
    $record['gid'] = $form_state['values']['gid'];
    imagepicker_update_user_group($record);
  }
  else {
    imagepicker_insert_user_group($record);
  }
}

/**
 * imagepicker_user_groups functions
 */
function imagepicker_insert_user_group($record) {
  if (db_query(
    "INSERT INTO {imagepicker_user_groups} (uid, group_name, group_description, public, avail_roles) VALUES (%d, '%s', '%s', %d, '%s')",
    array($record['uid'], $record['group_name'], $record['group_description'], $record['public'], $record['avail_roles']))) {
    drupal_set_message(t('Group was successfully inserted'));
  }
  else {
    drupal_set_message(t('Error while trying to insert your group.'));
  }
}

function imagepicker_update_user_group($record) {
  if (db_query(
    "UPDATE {imagepicker_user_groups} SET group_name='%s', group_description='%s', public=%d, avail_roles='%s' WHERE gid = %d",
    array($record['group_name'], $record['group_description'], $record['public'], $record['avail_roles'], $record['gid']))) {
    drupal_set_message(t('Group was successfully updated'));
  }
  else {
    drupal_set_message(t('Error while trying to update your group.'));
  }
}

function imagepicker_delete_user_group($gid) {
  if (db_query("DELETE FROM {imagepicker_user_groups} WHERE gid = %d", array($gid))
    && db_query("DELETE FROM {imagepicker_group_images} WHERE gid=%d", array($gid)) ) {
    drupal_set_message(t('Group was successfully deleted'));
  }
  else {
    drupal_set_message(t('Error while trying to delete group.'));
  }
}

function imagepicker_get_user_groups_by_user($uid, $obj=TRUE) {
  $result = db_query("SELECT * FROM {imagepicker_user_groups} WHERE uid = %d", array($uid));
  if ($obj) {
    return db_fetch_object($result);
  }
  return db_fetch_array($result);
}

function imagepicker_group_edit($gid) {
  $record = imagepicker_get_user_group($gid);
  $content .= drupal_get_form('imagepicker_groups_form', $record);
  return $content;
}

/**
 * group delete form
 */
function imagepicker_group_delete_form(&$form_state, $gid) {
  $record = imagepicker_get_user_group($gid);
  if ($record) {
    $form['groupname'] = array(
      '#value' => '<p>'. $record->group_name .'</p>',
    );
    $form['mode'] = array(
      '#type' => 'value',
      '#value' => 'reallydelete',
    );
    $form['gid'] = array(
      '#type' => 'value',
      '#value' => $gid,
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Really Delete record'),
    );
    return $form;
  }
  else {

  }
}

/**
 * Submit group delete form
 */
function imagepicker_group_delete_form_submit($form, &$form_state) {
  imagepicker_delete_user_group($form_state['values']['gid']);
}


function imagepicker_role_has_permission($rid) {
  $result = db_query("SELECT count(pid) AS ct FROM {permission} WHERE rid = %d AND perm LIKE '%%use public imagepicker%%'", array($rid));
  $row = db_fetch_array($result);
  return $row['ct'];
}

// build a table
function imagepicker_groups_list($src=FALSE, $account=FALSE, $label="") {
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }
  if ($src) {
    if ($src == 'admin') {
      $editpath = IMAGEPICKER_ADMIN_PATH .'/groups/user/'. $user->uid .'/edit/';
      $deletepath = IMAGEPICKER_ADMIN_PATH .'/groups/user/'. $user->uid .'/delete/';
      $browsepath = IMAGEPICKER_ADMIN_PATH .'/groups/user/'. $user->uid .'/browse/';
    }
    else {
      $editpath = 'user/'. $user->uid .'/imagepicker/groups/edit/';
      $deletepath = 'user/'. $user->uid .'/imagepicker/groups/delete/';
      $browsepath = 'user/'. $user->uid .'/imagepicker/groups/browse/';
    }
  }
  else {
    $editpath = "imagepicker/groups/edit/";
    $deletepath = "imagepicker/groups/delete/";
    $browsepath = 'imagepicker/groups/';
  }

  $content = "";
  $how_many = variable_get('imagepicker_rows_per_page', 25);
  $use_icons = variable_get('imagepicker_use_icons', 1);

  if (user_access('use public imagepicker') && variable_get('imagepicker_public_enabled', 1)) {
    $header = array(
      array('data' => t('Group name'), 'field' => 'g.group_name'),
      array('data' => t('Description'), 'field' => 'g.group_description'),
      array('data' => t('No. images'), 'field' => 'ct'),
      t('State'),
      array('data' => t('Public'), 'field' => 'g.public'),
      array('data' => t('Actions'), 'colspan' => 2),
    );
    $cols = 7;
  }
  else {
    $header = array(
      array('data' => t('Group name'), 'field' => 'g.group_name'),
      array('data' => t('Description'), 'field' => 'g.group_description'),
      array('data' => t('No. images'), 'field' => 'ct'),
      t('State'),
      array('data' => t('Actions'), 'colspan' => 2),
    );
    $cols = 6;
  }

  $countsql = "SELECT COUNT(DISTINCT(g.gid)) FROM {imagepicker_user_groups} g JOIN {imagepicker_group_images} u USING(gid) WHERE  g.uid=%d";
  $sql = "SELECT g.gid, g.group_name, g.group_description, g.public, COUNT(i.img_id) AS ct
    FROM {imagepicker_user_groups} g LEFT JOIN {imagepicker_group_images} i USING(gid)
    WHERE  g.uid=%d
    GROUP BY g.gid". tablesort_sql($header);

  $result = pager_query($sql, $how_many, 0, $countsql, array($user->uid));
  $rows = array();
  $totcount = 0;
  $rowcount = 0;
  $enabledlist = imagepicker_get_enabled_group($account);
  while ($row = db_fetch_array($result)) {
    if (user_access('use public imagepicker') && variable_get('imagepicker_public_enabled', 1)) {
      $row_data = array(
        check_plain($row['group_name']),
        check_plain($row['group_description']),
        ($row['ct'] && $browsepath ?  l($row['ct'], $browsepath . $row['gid']) : $row['ct']),
        (($enabledlist && in_array($row['gid'], $enabledlist)) ? t('selected') : ''),
        ($row['public'] ? t('Yes') : t('No')),
        ($use_icons ? _imagepicker_get_icon('edit', $editpath . $row['gid'], array('title' => t('Edit'))) : l(t('Edit'), $editpath . $row['gid'])),
        ($use_icons ? _imagepicker_get_icon('delete', $deletepath . $row['gid'], array('title' => t('Delete'))) : l(t('Delete'), $deletepath . $row['gid'])),
      );
    }
    else {
      $row_data = array(
        check_plain($row['group_name']),
        check_plain($row['group_description']),
        ($row['ct'] && $browsepath ?  l($row['ct'], $browsepath . $row['gid']) : $row['ct']),
        (($enabledlist && in_array($row['gid'], $enabledlist)) ? t('selected') : ''),
        ($use_icons ? _imagepicker_get_icon('edit', $editpath . $row['gid'], array('title' => t('Edit'))) : l(t('Edit'), $editpath . $row['gid'])),
        ($use_icons ? _imagepicker_get_icon('delete', $deletepath . $row['gid'], array('title' => t('Delete'))) : l(t('Delete'), $deletepath . $row['gid'])),
      );
    }
    $rows[] = $row_data;
    $rowcount++;
  }
  return theme('imagepicker_list', $header, $rows, $how_many, t('No groups found.'), '<div class="imgp_groups_list">', '</div>', $label, $cols);

}

// not in use I think
function imagepicker_get_group_images_count($gid) {
  $result = db_query("SELECT count(gid) as gidct FROM {imagepicker_group_images} WHERE gid = %d", array($gid));
  $row = db_fetch_array($result);
  return $row['gidct'];
}

