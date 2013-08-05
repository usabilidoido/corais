<?php
// $Id: views_handler_filter_mc_role_current_user.inc,v 1.1.2.2 2010/12/22 20:21:51 matslats Exp $

/**
 * @file
 * Filter handler to users sharing a role with the current user
 */
class views_handler_filter_role_current_user extends views_handler_filter {

  function query() {
    $rids = array_keys($GLOBALS['user']->roles);
    unset($rids[DRUPAL_AUTHENTICATED_RID]);
    
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field IN (". implode(',', $roles) .")");
  }
}