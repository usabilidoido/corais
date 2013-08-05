<?php

/**
 * User integration for Spaces. 
 */
class space_user extends space_type {
  /**
   * Override of load().
   * Load the user account object for this space and bail if it can't be loaded.
   */
  function load() {
    parent::load();
    $account = user_load(array('uid' => $this->id));
    if ($account) {
      $this->user = $account;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Override of deactivate().
   * We don't have too many good options here except to leave the user/x path.
   * If the site frontpage has also been set to a user/x path, we just need
   * to fail miserably.
   */
  function deactivate() {
    if (strpos(variable_get('site_frontpage', 'node'), 'user') !== 0) {
      drupal_goto('<front>');
    }
    return;
  }

  /**
   * Override of title().
   */
  function title() {
    return $this->user->name;
  }

  /**
   * Override of access_admin().
   */
  function access_admin($account = NULL) {
    global $user;
    $account = isset($account) ? $account : $user;
    $access = $account->uid == $this->id && user_access('configure own user space');
    return  $access || parent::access_admin($account);
  }

  /**
   * Override of access_feature().
   */
  function access_feature($op = 'view', $feature, $account = NULL) {
    $site_features = spaces_features('site');
    $user_features = spaces_features('user');
    // If feature can be used in "site space" but not user space, ensure we
    // still allow access since user spaces are only active "locally" under
    // user/x.
    //
    // Features that can be used in both user and site spaces should implement
    // their own access callbacks (rather than using spaces_access_feature())
    // to manage access control rules when in a user space.
    if (isset($site_features[$feature]) && !isset($user_features[$feature])) {
      $enabled = $this->controllers->variable->get('spaces_features', 'original');
      return user_access('access content') && !empty($enabled[$feature]);
    }
    return parent::access_feature($op, $feature, $account);
  }
}
