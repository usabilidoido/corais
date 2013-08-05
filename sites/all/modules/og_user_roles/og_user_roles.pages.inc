<?php

/**
 * @file
 * Page callbacks for OG user roles.
 */

/**
 * Form builder for administrative settings.
 */
function og_user_roles_admin_settings() {
  // Get list of all og-enabled node types.
  $group_types = og_get_types('group');
  $types = array();
  foreach ($group_types as $type) {
    $types[$type] = node_get_types('name', $type);
  }

  // Get list of roles, not counting authenticated and anonymous user.
  $all_roles = array_map('check_plain', user_roles());
  foreach ($all_roles as $rid => $role) {
    if ($rid > DRUPAL_AUTHENTICATED_RID) {
      $roles[$rid] = $role;
    }
  }
  // If no assignable roles, advise user to add some.
  if (empty($roles)) {
    $form['og_user_roles'] = array(
      '#value' => '<p>' . t('No assignable roles were found. User roles can be created on the <a href="@admin-user-roles-url">Roles</a> configuration page.', array('@admin-user-roles-url' => url('admin/user/roles'))) . '</p>',
    );
    return $form;
  }

  // Create a fieldset for each group type containing role selections.
  $form['og_user_roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Assignable roles'),
  );
  foreach ($types as $type => $name) {
    $form['og_user_roles']["og_user_roles_roles_$type"] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles for %name groups', array('%name' => $name)),
      '#options' => $roles,
      '#default_value' => variable_get("og_user_roles_roles_$type", array()),
    );
  }

  $form['og_user_roles_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default role assignments'),
    '#description' => t('Select a role to automatically assign to users who join a group.'),
    '#collapsible' => TRUE,
  );
  $form['og_user_roles_defaults']['og_user_roles_default_admin_role'] = array(
    '#type' => 'select',
    '#title' => t('Default role for new group administrators'),
    '#options' => array(0 => t('None')) + $roles,
    '#default_value' => variable_get('og_user_roles_default_admin_role', 0),
    '#description' => t('This role can only be unassigned by removing a user from the administrators of a group.'),
  );
  $form['og_user_roles_defaults']['og_user_roles_default_role'] = array(
    '#type' => 'select',
    '#title' => t('Default role for new group members'),
    '#options' => array(0 => t('None')) + $roles,
    '#default_value' => variable_get('og_user_roles_default_role', 0),
    '#description' => t('This setting can be overridden at the group level by group admins with the %permission permission.', array('%permission' => 'override group default role')),
  );

  return system_settings_form($form);
}

/**
 * Menu callback; displays members and role selection
 */
function og_user_roles_page($node) {
  drupal_set_title(t('Member roles for !title', array('!title' => l($node->title, "node/$node->nid"))));

  $roles = og_user_roles_get_group_roles($node->type);

  // Retrieve list of all group users
  $sql = og_list_users_sql(0, 0, 'ou.is_admin DESC, ou.is_active ASC, u.name ASC');
  $result = pager_query($sql, 100, 0, NULL, $node->nid);
  $pager = theme('pager', NULL, 100, 0);

  $output = $pager;
  $output .= drupal_get_form('og_user_roles_page_form', $node, $roles, $result);
  $output .= $pager;

  return $output;
}

/**
 * Form builder for group user roles assignment.
 */
function og_user_roles_page_form($form_state, $node, $roles, $result) {
  $form['#node'] = $node;
  // Used for theming.
  $form['#id'] = 'og-user-roles';
  $form['#assignable_roles'] = $roles;
  $all_roles = array_map('check_plain', user_roles());

  // Unset role titles.
  foreach ($roles as $rid => $name) {
    $roles[$rid] = '';
  }

  $form['user_roles'] = array('#tree' => TRUE);
  while ($account = db_fetch_object($result)) {
    $form['user_roles'][$account->uid]['user'] = array(
      '#value' => theme('username', $account),
    );
    $account_roles = og_user_roles_get_roles_by_group($node->nid, $account->uid);
    $form['user_roles'][$account->uid]['roles'] = array(
      '#type' => 'checkboxes',
      '#default_value' => $account_roles,
      '#options' => $roles,
      '#parents' => array('user_roles', $account->uid),
    );
    // Add default roles that do not appear in the form.
    $default_roles = array();
    foreach ($account_roles as $rid) {
      if (!isset($roles[$rid])) {
        $default_roles[$rid] = $all_roles[$rid];
      }
    }
    $form['user_roles'][$account->uid]['default_roles'] = array(
      '#value' => implode(', ', $default_roles),
    );
  }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Form submit handler for group user roles assignment form.
 */
function og_user_roles_page_form_submit($form, &$form_state) {
  $gid = $form['#node']->nid;

  foreach ($form_state['values']['user_roles'] as $uid => $new_roles) {
    foreach ($form['#assignable_roles'] as $rid => $name) {
      if (empty($new_roles[$rid])) {
        og_user_roles_role_delete($gid, $uid, $rid);
      }
      else {
        og_user_roles_role_add($gid, $uid, $rid);
      }
    }
  }

  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Render the group user roles assignment form like user permissions table.
 */
function theme_og_user_roles_page_form($form) {
  $header = array_merge(array('', t('Default roles')), $form['#assignable_roles']);

  $rows = array();
  foreach (element_children($form['user_roles']) as $item) {
    $row = array();
    $row[] = drupal_render($form['user_roles'][$item]['user']);
    $row[] = drupal_render($form['user_roles'][$item]['default_roles']);
    foreach (element_children($form['user_roles'][$item]['roles']) as $role) {
      $row[] = drupal_render($form['user_roles'][$item]['roles'][$role]);
    }
    $rows[] = $row;
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

