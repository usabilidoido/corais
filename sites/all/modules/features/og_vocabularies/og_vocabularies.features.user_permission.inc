<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function og_vocabularies_user_default_permissions() {
  $permissions = array();

  // Exported permission: access og_vocab service
  $permissions['access og_vocab service'] = array(
    'name' => 'access og_vocab service',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: add own group vocabulary
  $permissions['add own group vocabulary'] = array(
    'name' => 'add own group vocabulary',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: administer own group vocabulary
  $permissions['administer own group vocabulary'] = array(
    'name' => 'administer own group vocabulary',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: edit own group term
  $permissions['edit own group term'] = array(
    'name' => 'edit own group term',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: edit own group vocabulary
  $permissions['edit own group vocabulary'] = array(
    'name' => 'edit own group vocabulary',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  return $permissions;
}
