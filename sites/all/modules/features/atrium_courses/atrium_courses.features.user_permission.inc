<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function atrium_courses_user_default_permissions() {
  $permissions = array();

  // Exported permission: access all certificates
  $permissions['access all certificates'] = array(
    'name' => 'access all certificates',
    'roles' => array(),
  );

  // Exported permission: access all course reports
  $permissions['access all course reports'] = array(
    'name' => 'access all course reports',
    'roles' => array(),
  );

  // Exported permission: access course
  $permissions['access course'] = array(
    'name' => 'access course',
    'roles' => array(),
  );

  // Exported permission: access course reports
  $permissions['access course reports'] = array(
    'name' => 'access course reports',
    'roles' => array(),
  );

  // Exported permission: administer certificates
  $permissions['administer certificates'] = array(
    'name' => 'administer certificates',
    'roles' => array(),
  );

  // Exported permission: administer course
  $permissions['administer course'] = array(
    'name' => 'administer course',
    'roles' => array(),
  );

  // Exported permission: create certificate content
  $permissions['create certificate content'] = array(
    'name' => 'create certificate content',
    'roles' => array(),
  );

  // Exported permission: delete any certificate content
  $permissions['delete any certificate content'] = array(
    'name' => 'delete any certificate content',
    'roles' => array(),
  );

  // Exported permission: delete own certificate content
  $permissions['delete own certificate content'] = array(
    'name' => 'delete own certificate content',
    'roles' => array(),
  );

  // Exported permission: edit any certificate content
  $permissions['edit any certificate content'] = array(
    'name' => 'edit any certificate content',
    'roles' => array(),
  );

  // Exported permission: edit courses
  $permissions['edit courses'] = array(
    'name' => 'edit courses',
    'roles' => array(),
  );

  // Exported permission: edit own certificate content
  $permissions['edit own certificate content'] = array(
    'name' => 'edit own certificate content',
    'roles' => array(),
  );

  // Exported permission: view all user certificates
  $permissions['view all user certificates'] = array(
    'name' => 'view all user certificates',
    'roles' => array(),
  );

  return $permissions;
}
