<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function meeting_planner_user_default_permissions() {
  $permissions = array();

  // Exported permission: create meeting_planner content
  $permissions['create meeting_planner content'] = array(
    'name' => 'create meeting_planner content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit own meeting_planner content
  $permissions['edit own meeting_planner content'] = array(
    'name' => 'edit own meeting_planner content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  return $permissions;
}
