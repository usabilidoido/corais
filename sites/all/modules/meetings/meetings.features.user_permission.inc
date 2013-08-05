<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function meetings_user_default_permissions() {
  $permissions = array();

  // Exported permission: edit all meeting fields
  $permissions['edit all meeting fields'] = array(
    'name' => 'edit all meeting fields',
    'roles' => array(),
  );

  // Exported permission: edit any meeting content
  $permissions['edit any meeting content'] = array(
    'name' => 'edit any meeting content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit meeting agenda
  $permissions['edit meeting agenda'] = array(
    'name' => 'edit meeting agenda',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit meeting attachments
  $permissions['edit meeting attachments'] = array(
    'name' => 'edit meeting attachments',
    'roles' => array(),
  );

  // Exported permission: edit meeting date
  $permissions['edit meeting date'] = array(
    'name' => 'edit meeting date',
    'roles' => array(),
  );

  // Exported permission: edit meeting location
  $permissions['edit meeting location'] = array(
    'name' => 'edit meeting location',
    'roles' => array(),
  );

  // Exported permission: edit meeting notes
  $permissions['edit meeting notes'] = array(
    'name' => 'edit meeting notes',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit meeting purpose
  $permissions['edit meeting purpose'] = array(
    'name' => 'edit meeting purpose',
    'roles' => array(),
  );

  // Exported permission: edit own meeting content
  $permissions['edit own meeting content'] = array(
    'name' => 'edit own meeting content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: subscribe other users
  $permissions['subscribe other users'] = array(
    'name' => 'subscribe other users',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: subscribe to content
  $permissions['subscribe to content'] = array(
    'name' => 'subscribe to content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: take attendance for meetings
  $permissions['take attendance for meetings'] = array(
    'name' => 'take attendance for meetings',
    'roles' => array(),
  );

  // Exported permission: view attendance for meetings
  $permissions['view attendance for meetings'] = array(
    'name' => 'view attendance for meetings',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  return $permissions;
}
