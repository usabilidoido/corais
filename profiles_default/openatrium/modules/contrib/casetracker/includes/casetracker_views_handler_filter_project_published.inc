<?php
// $Id: casetracker_views_handler_filter_project_published.inc,v 1.2 2009/02/10 22:33:33 jmiccolis Exp $

class casetracker_views_handler_filter_project_published extends views_handler_filter {
  function admin_summary() { }
  function operator_form() { }

  function query() {
    $table = $this->ensure_my_table();
    //print_r($table);
    //$this->query->add_where($this->options['group'], "$table.status <> 0 OR $table.uid = ***CURRENT_USER*** or ***ADMINISTER_NODES*** = 1");
    //$this->query->add_where($this->options['group'], "$table.status <> 0");
  }
}

