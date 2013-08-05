<?php

/**
 * Organic groups integration for Spaces.
 */
class space_og extends space_type_purl {
  var $group = NULL;

  /**
   * Override of title().
   */
  function title() {
    return $this->group->title;
  }

  /**
   * Override of activate().
   */
  function activate() {
    if (parent::activate()) {
      og_set_group_context($this->group);
      og_set_language($this->group);

      // Handle theme switching for OG
      if (!empty($this->group->og_theme)) {
        global $custom_theme;
        $custom_theme = $this->group->og_theme;
      }

      return TRUE;
    }
    return FALSE;
  }

  /**
   * Override of load().
   */
  function load() {
    // The 0 id means a new group is being saved. Instantiate a space
    // so preset values can become active.
    if ($this->id === 0) {
      return TRUE;
    }
    else if ($this->group = node_load($this->id)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Override of access_space().
   */
  function access_space($account = NULL) {
    global $user;
    $account = isset($account) ? $account : $user;

    return parent::access_space($account) && node_access('view', $this->group, $account);
  }

  /**
   * Override access_admin().
   */
  function access_admin($account = NULL) {
    global $user;
    $account = isset($account) ? $account : $user;

    if ($this->group && (og_is_group_admin($this->group, $account) || $this->group->uid === $account->uid)) {
      return TRUE;
    }
    else if (user_access('administer spaces', $account) || user_access('administer organic groups', $account)) {
      return TRUE;
    }
    return parent::access_admin($account);
  }

  /**
   * Override of access_feature().
   */
  function access_feature($op = 'view', $feature, $account = NULL) {
    $access = parent::access_feature($op, $feature, $account);

    global $user;
    $account = isset($account) ? $account : $user;

    // Only allow access if: user can administer OG, group is public,
    // or user is a member.
    $access = $access && (user_access('administer organic groups') || !$this->group->og_private || og_is_group_member($this->id, TRUE, $account->uid));

    // Additional check for group membership if authoring content in this group.
    if ($op === 'create') {
      return $access && og_is_group_member($this->id, TRUE, $account->uid);
    }
    return $access;
  }

  /**
   * Override of access_user().
   */
  function access_user($op = 'view', $account = NULL) {
    global $user;
    $account = isset($account) ? $account : $user;

    // Test whether both user and account belong to current space.
    $test_user = og_is_group_member($this->id);
    $test_account = og_is_group_member($this->id, FALSE, $account->uid);
    if (($test_user && $test_account) || user_access('view users outside groups')) {
      return parent::access_user($op, $account);
    }
    return FALSE;
  }

  /**
    * Implementation of space->router().
    */
  function router($op, $object = NULL, $is_active = TRUE) {
    global $user;
    switch ($op) {
      case 'init':
        return;
      case 'node':
        $node = $object;
        $form = !isset($node->nid) || isset($node->date) ? TRUE : FALSE;

        // Group node
        if (og_is_group_type($node->type)) {
          // Don't make new groups from within a group space.
          if (!isset($node->nid) && $this->active) {
            $this->deactivate();
            return;
          }
          // If editing or viewing an existing node, make sure we're in the correct space
          else if (isset($node->nid) && (!$this->active || ($this->active && $this->id != $node->nid)) && $space = spaces_load('og', $node->nid)) {
            $space->activate();
            return;
          }
        }
        // Group enabled node, route to correct group context.
        else if (!og_is_omitted_type($node->type) && !empty($node->og_groups)) {
          // If the node belongs to the current active group space, or we're in an allowable other space type, pass thru
          if ($this->active && in_array($this->id, $node->og_groups)) {
            return;
          }
          // Otherwise route
          reset($node->og_groups);
          spaces_load('og', current($node->og_groups))->activate();
          return;
        }
        break;
    }
  }

  // Spaces OG views filter
  function views_filter(&$query, $base_table = '', $relationship = '') {
    switch ($base_table) {
      case 'node':
        $table = $query->ensure_table('og_ancestry', $relationship);
        $query->add_where(0, "$table.group_nid  = ***CURRENT_GID***");
        break;
      case 'users':
        $table = $query->ensure_table('og_uid', $relationship);
        $query->add_where(0, "$table.nid = ***CURRENT_GID***");
        break;
    }
  }

  /**
   * Custom function group_node_admin_access().
   * Check that the current menu node object is also a group node.
   */
  function group_node_admin_access($node) {
    if ($node && og_is_group_type($node->type)) {
      return $this->admin_access();
    }
    return FALSE;
  }

  /**
   * Override of excluded_paths().
   * Adds og, og/* paths.
   */
  protected function excluded_paths() {
    $excluded = parent::excluded_paths();
    $excluded[] = 'og';
    $excluded[] = 'og/my';
    $excluded[] = 'og/all';
    $excluded[] = 'user';
    $excluded[] = 'user/login';
    $excluded[] = 'user/password';
    $excluded[] = 'user/register';
    $excluded[] = 'logout';
    return $excluded;
  }
}
