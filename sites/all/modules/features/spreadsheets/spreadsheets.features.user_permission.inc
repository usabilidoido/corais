<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function spreadsheets_user_default_permissions() {
  $permissions = array();

  // Exported permission: create sheetnode
  $permissions['create sheetnode'] = array(
    'name' => 'create sheetnode',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: delete any sheetnode
  $permissions['delete any sheetnode'] = array(
    'name' => 'delete any sheetnode',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: delete own sheetnode
  $permissions['delete own sheetnode'] = array(
    'name' => 'delete own sheetnode',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: edit any sheetnode
  $permissions['edit any sheetnode'] = array(
    'name' => 'edit any sheetnode',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: edit own sheetnode
  $permissions['edit own sheetnode'] = array(
    'name' => 'edit own sheetnode',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  return $permissions;
}
