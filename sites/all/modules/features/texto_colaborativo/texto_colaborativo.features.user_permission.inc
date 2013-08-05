<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function texto_colaborativo_user_default_permissions() {
  $permissions = array();

  // Exported permission: convert from text
  $permissions['convert from text'] = array(
    'name' => 'convert from text',
    'roles' => array(),
  );

  // Exported permission: create text content
  $permissions['create text content'] = array(
    'name' => 'create text content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: delete any text content
  $permissions['delete any text content'] = array(
    'name' => 'delete any text content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: delete own text content
  $permissions['delete own text content'] = array(
    'name' => 'delete own text content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: edit any text content
  $permissions['edit any text content'] = array(
    'name' => 'edit any text content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: edit own text content
  $permissions['edit own text content'] = array(
    'name' => 'edit own text content',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  return $permissions;
}
