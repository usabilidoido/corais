<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function grayside_oa_feeds_user_default_permissions() {
  $permissions = array();

  // Exported permission: access tokenauth
  $permissions['access tokenauth'] = array(
    'name' => 'access tokenauth',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: administer tokenauth
  $permissions['administer tokenauth'] = array(
    'name' => 'administer tokenauth',
    'roles' => array(
      '0' => 'administrator',
    ),
  );

  return $permissions;
}
