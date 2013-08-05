<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function features_task_user_default_permissions() {
  $permissions = array();

  // Exported permission: Allow Reordering
  $permissions['Allow Reordering'] = array(
    'name' => 'Allow Reordering',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: add time tracker entries
  $permissions['add time tracker entries'] = array(
    'name' => 'add time tracker entries',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: administer time entries
  $permissions['administer time entries'] = array(
    'name' => 'administer time entries',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: administer time estimate
  $permissions['administer time estimate'] = array(
    'name' => 'administer time estimate',
    'roles' => array(
      '0' => 'administrator',
    ),
  );

  // Exported permission: administer time tracker
  $permissions['administer time tracker'] = array(
    'name' => 'administer time tracker',
    'roles' => array(
      '0' => 'administrator',
    ),
  );

  // Exported permission: administer time tracker timer
  $permissions['administer time tracker timer'] = array(
    'name' => 'administer time tracker timer',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: create milestone content
  $permissions['create milestone content'] = array(
    'name' => 'create milestone content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: create task content
  $permissions['create task content'] = array(
    'name' => 'create task content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: delete any milestone content
  $permissions['delete any milestone content'] = array(
    'name' => 'delete any milestone content',
    'roles' => array(
      '0' => 'administrator',
    ),
  );

  // Exported permission: delete any task content
  $permissions['delete any task content'] = array(
    'name' => 'delete any task content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: delete own milestone content
  $permissions['delete own milestone content'] = array(
    'name' => 'delete own milestone content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: delete own task content
  $permissions['delete own task content'] = array(
    'name' => 'delete own task content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: delete time tracker entries
  $permissions['delete time tracker entries'] = array(
    'name' => 'delete time tracker entries',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: edit any milestone content
  $permissions['edit any milestone content'] = array(
    'name' => 'edit any milestone content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
      '2' => 'participant',
    ),
  );

  // Exported permission: edit any task content
  $permissions['edit any task content'] = array(
    'name' => 'edit any task content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
      '2' => 'participant',
    ),
  );

  // Exported permission: edit own milestone content
  $permissions['edit own milestone content'] = array(
    'name' => 'edit own milestone content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: edit own task content
  $permissions['edit own task content'] = array(
    'name' => 'edit own task content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: edit time tracker entries
  $permissions['edit time tracker entries'] = array(
    'name' => 'edit time tracker entries',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: manually enter deductions
  $permissions['manually enter deductions'] = array(
    'name' => 'manually enter deductions',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: manually enter time
  $permissions['manually enter time'] = array(
    'name' => 'manually enter time',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: view all time tracker entries
  $permissions['view all time tracker entries'] = array(
    'name' => 'view all time tracker entries',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'manager',
    ),
  );

  // Exported permission: view all timers
  $permissions['view all timers'] = array(
    'name' => 'view all timers',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'manager',
      '4' => 'participant',
    ),
  );

  // Exported permission: view own time tracker entries
  $permissions['view own time tracker entries'] = array(
    'name' => 'view own time tracker entries',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: view time estimate entries
  $permissions['view time estimate entries'] = array(
    'name' => 'view time estimate entries',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'manager',
      '4' => 'participant',
    ),
  );

  return $permissions;
}
