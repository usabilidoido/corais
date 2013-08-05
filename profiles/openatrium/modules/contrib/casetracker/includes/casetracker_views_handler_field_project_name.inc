<?php
// $Id: casetracker_views_handler_field_project_name.inc,v 1.1.2.1 2010/04/15 19:19:13 jmiccolis Exp $

/**
 * Field handler to show Selective state.
 *
 * @ingroup views_field_handlers
 */
class casetracker_views_handler_field_project_name extends views_handler_field {

  function render($values) {
    $node = db_fetch_object(db_query(db_rewrite_sql("SELECT n.nid, n.title FROM {node} n WHERE n.nid = %d"), $values->{$this->field_alias}));
    if ($node->nid) { 
      return l($node->title, "node/$node->nid"); 
    } // returns a linked case tracker project title.
  }
}
