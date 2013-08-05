<?php
// $Id: casetracker_views_handler_filter_case.inc,v 1.1.2.2 2010/02/12 14:57:55 jmiccolis Exp $
/**
 * Filter to only show casetracker cases.
 */
class casetracker_views_handler_filter_case extends views_handler_filter {
  function query() {    
    $case_types = array_filter(variable_get('casetracker_case_node_types', array('casetracker_basic_case'))); 

    if (!empty($case_types)) {
      $placeholders = db_placeholders($case_types, 'varchar');
      $table = $this->query->ensure_table('node');
      $this->query->add_where($this->options['group'], "$table.type IN ($placeholders)", $case_types);
    }
    else {
      $this->query->add_where($this->options['group'], "FALSE");
      $msg = t('You have no node types which are acting as cases.');
      if (user_access('administer case tracker')) {
        $msg .= ' ' . l(t('Configure case tracker.'), 'admin/settings/casetracker');
      }
      drupal_set_message($msg, 'warning');
    }
  }
}
