<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function atrium_credit_user_default_permissions() {
  $permissions = array();

  // Exported permission: configure currencies
  $permissions['configure currencies'] = array(
    'name' => 'configure currencies',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: create 3rdparty exchanges
  $permissions['create 3rdparty exchanges'] = array(
    'name' => 'create 3rdparty exchanges',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: declare currency
  $permissions['declare currency'] = array(
    'name' => 'declare currency',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: edit all exchanges
  $permissions['edit all exchanges'] = array(
    'name' => 'edit all exchanges',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: edit own finished exchanges
  $permissions['edit own finished exchanges'] = array(
    'name' => 'edit own finished exchanges',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'manager',
    ),
  );

  // Exported permission: exchange
  $permissions['exchange'] = array(
    'name' => 'exchange',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'authenticated user',
      '2' => 'manager',
      '3' => 'participant',
    ),
  );

  // Exported permission: view all balances
  $permissions['view all balances'] = array(
    'name' => 'view all balances',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'manager',
      '4' => 'participant',
    ),
  );

  // Exported permission: view all exchanges
  $permissions['view all exchanges'] = array(
    'name' => 'view all exchanges',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'manager',
      '4' => 'participant',
    ),
  );

  // Exported permission: view exchange stats
  $permissions['view exchange stats'] = array(
    'name' => 'view exchange stats',
    'roles' => array(
      '0' => 'administrator',
      '1' => 'anonymous user',
      '2' => 'authenticated user',
      '3' => 'manager',
    ),
  );

  return $permissions;
}
