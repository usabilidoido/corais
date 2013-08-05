<?php
// $Id: ogur_handler_field_og_users_roles.inc,v 1.1.2.1 2009/01/30 07:32:44 somebodysysop Exp $
/**
 * Field handler to provide a list of roles.
 */
class ogur_handler_field_og_users_roles extends views_handler_field_prerender_list {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'users', 'field' => 'uid');
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['uid'];
  }

  function pre_render($values) {
    $uids = array();
    $this->items = array();
    $node = og_get_group_context();

    foreach ($values as $result) {
      $uids[] = $result->{$this->aliases['uid']};
    }

    if ($uids) {
      $result = db_query("SELECT u.uid, u.rid, r.name FROM {role} r INNER JOIN {og_users_roles} u ON u.rid = r.rid WHERE u.uid IN (" . implode(', ', $uids) . ") AND u.gid = $node->nid ORDER BY r.name");
      while ($role = db_fetch_object($result)) {
//        $this->items[$role->uid][$role->rid] = check_plain($role->name);
          $this->items[$role->uid][$role->rid] = l(t($role->name), "og/users/$node->nid/roles", array('query' => drupal_get_destination()));
      }
    }
  }
}
