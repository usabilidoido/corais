<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function mindmaps_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer services
  $permissions['administer services'] = array(
    'name' => 'administer services',
    'roles' => array(),
  );

  // Exported permission: create graphmind content
  $permissions['create graphmind content'] = array(
    'name' => 'create graphmind content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: delete any graphmind content
  $permissions['delete any graphmind content'] = array(
    'name' => 'delete any graphmind content',
    'roles' => array(),
  );

  // Exported permission: delete own graphmind content
  $permissions['delete own graphmind content'] = array(
    'name' => 'delete own graphmind content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit any graphmind content
  $permissions['edit any graphmind content'] = array(
    'name' => 'edit any graphmind content',
    'roles' => array(),
  );

  // Exported permission: edit own graphmind content
  $permissions['edit own graphmind content'] = array(
    'name' => 'edit own graphmind content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: get any user data
  $permissions['get any user data'] = array(
    'name' => 'get any user data',
    'roles' => array(),
  );

  // Exported permission: get available services
  $permissions['get available services'] = array(
    'name' => 'get available services',
    'roles' => array(),
  );

  // Exported permission: get own user data
  $permissions['get own user data'] = array(
    'name' => 'get own user data',
    'roles' => array(),
  );

  // Exported permission: load node data
  $permissions['load node data'] = array(
    'name' => 'load node data',
    'roles' => array(),
  );

  return $permissions;
}
