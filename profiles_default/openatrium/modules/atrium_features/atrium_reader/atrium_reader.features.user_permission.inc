<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function atrium_reader_user_default_permissions() {
  $permissions = array();

  // Exported permission: create feed_reader content
  $permissions['create feed_reader content'] = array(
    'name' => 'create feed_reader content',
    'roles' => array(
      '0' => 'authenticated user',
      '1' => 'manager',
    ),
  );

  // Exported permission: delete own feed_reader content
  $permissions['delete own feed_reader content'] = array(
    'name' => 'delete own feed_reader content',
    'roles' => array(
      '0' => 'authenticated user',
      '1' => 'manager',
    ),
  );

  // Exported permission: edit own feed_reader content
  $permissions['edit own feed_reader content'] = array(
    'name' => 'edit own feed_reader content',
    'roles' => array(
      '0' => 'authenticated user',
      '1' => 'manager',
    ),
  );

  return $permissions;
}
