<?php

/**
 * @file
 * Provides a views filter handler class for the sticky_notes priority field.
 */

 /**
  * Simple filter class to select field for priority
  *
  * @author berliner
  */

class views_handler_filter_sticky_notes_access extends views_handler_filter {
  
  function admin_summary() { }
  function operator_form() { }
  function can_expose() {
    return FALSE;
  }
  
  /**
   * Add this filter to the query.
   *
   * Due to the nature of fapi, the value and the operator have an unintended
   * level of indirection. You will find them in $this->operator
   * and $this->value respectively.
   */
  function query() {
    global $user;
    if (!user_access('administer sticky notes') && $user->uid != 1) {
      $this->ensure_my_table();
    
      // join with the roles table to check access for this user
      $roles = sticky_notes_get_users_permitted_roles();
      $role_conditions = array();
      $args = array();
      if (isset($roles[DRUPAL_ANONYMOUS_RID])) {
        $role_conditions[] = ' (node.uid = 0) ';
        unset($roles[DRUPAL_ANONYMOUS_RID]);
      }
    
      if (isset($roles[DRUPAL_AUTHENTICATED_RID])) {
        $role_conditions[] = ' (node.uid != 0) ';
        unset($roles[DRUPAL_AUTHENTICATED_RID]);
      }
    
      if (count($roles)) {
        $rids = array_keys($roles);
        $role_conditions[] = " users_roles.rid IN (" . db_placeholders($rids) . ") ";
        $args = array_merge($args, $rids);
      }
    
      $conditions = " (" . implode(' OR ', $role_conditions) . ") ";
      $this->query->queue_table('users_roles');
      $this->query->add_where($this->options['group'], vsprintf($conditions, $args));
    }
  }

}

