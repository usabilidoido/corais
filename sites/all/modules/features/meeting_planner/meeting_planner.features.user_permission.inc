<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function meeting_planner_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer big blue button
  $permissions['administer big blue button'] = array(
    'name' => 'administer big blue button',
    'roles' => array(),
  );

  // Exported permission: attend meetings
  $permissions['attend meetings'] = array(
    'name' => 'attend meetings',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: create meeting_planner content
  $permissions['create meeting_planner content'] = array(
    'name' => 'create meeting_planner content',
    'roles' => array(),
  );

  // Exported permission: edit own meeting_planner content
  $permissions['edit own meeting_planner content'] = array(
    'name' => 'edit own meeting_planner content',
    'roles' => array(),
  );

  // Exported permission: moderate meetings
  $permissions['moderate meetings'] = array(
    'name' => 'moderate meetings',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: moderate own meetings
  $permissions['moderate own meetings'] = array(
    'name' => 'moderate own meetings',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: pick dates
  $permissions['pick dates'] = array(
    'name' => 'pick dates',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
      '4' => 'translator',
    ),
  );

  return $permissions;
}
