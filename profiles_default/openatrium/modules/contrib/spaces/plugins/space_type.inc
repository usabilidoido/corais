<?php

/**
 * Common functionality for space types that implement access, routing,
 * delegated Views filtering and feature management concepts.
 * Examples: spaces_og, spaces_user, spaces_taxonomy.
 */
class space_type extends space {
  /**
   * Get the title for this space.
   *
   * @return
   *   The string title for this space.
   */
  function title() {
    return '';
  }

  /**
   * Get the possible feature setting values for this space.
   *
   * @return
   *   A keyed array of options.
   */
  function feature_options() {
    return array(0 => t('Disabled'), 1 => t('Enabled'));
  }

  /**
   * Access & routing =================================================
   */

  /**
   * Route the user as necessary.
   *
   * @param $op
   *   The hook or Drupal implementation point where the router is being given
   *   a chance to intervene.
   * @param $object
   *   Optional: the menu object related to the current page request where
   *   routing may occur. For example, $object is the node object if routing
   *   occurs at node/5.
   */
  function router($op, $object = NULL) {
    switch ($op) {
      case 'init':
      case 'user':
      case 'node':
      default:
        return TRUE;
    }
  }

  /**
   * Grant a user access to anything in this space. This method can be used to
   * deny access to any page where this space is active.
   *
   * @param $account
   *   Optional: user account object to check against. If omitted, the global
   *   $user object (current user) will be used.
   *
   * @return
   *   TRUE if access should be granted. FALSE if not.
   */
  function access_space($account = NULL) {
    return TRUE;
  }

  /**
   * Grant a user administrative access to this space.
   *
   * @param $account
   *   Optional: user account object to check against. If omitted, the global
   *   $user object (current user) will be used.
   *
   * @return
   *   TRUE if access should be granted. FALSE if not.
   */
  function access_admin($account = NULL) {
    global $user;
    $account = isset($account) ? $account : $user;
    return user_access('administer site configuration', $account) || user_access('administer spaces', $account);
  }

  /**
   * Grant a user access to the specified feature in this space.
   *
   * @param $op
   *   May be 'view' or 'create'.
   * @param $feature
   *   The feature in question to check access for.
   * @param $account
   *   Optional: user account object to check against. If omitted, the global
   *   $user object (current user) will be used.
   *
   * @return
   *   TRUE if access should be granted. FALSE if not.
   */
  function access_feature($op = 'view', $feature, $account = NULL) {
    $usable = spaces_features($this->type);
    $features = $this->controllers->variable->get('spaces_features');
    return user_access('access content', $account) && isset($usable[$feature]) && !empty($features[$feature]);
  }

  /**
   * Grant a user access to the given account in this space.
   *
   * @param $op
   *   May be 'view' or 'edit'.
   *
   * @return
   *   TRUE if access should be granted. FALSE if not.
   */
  function access_user($op = 'view', $account = NULL) {
    return TRUE;
  }

  /**
   * Views filter callback.
   *
   * @param $query
   *   A views query object, passed by reference.
   * @param $base_table
   *   The base table for this View, e.g. "node", "user"
   * @param $relationship
   *   The relationship being used by the views spaces filter.
   */
  function views_filter(&$query, $base_table = '', $relationship = '') {
    return;
  }
}
