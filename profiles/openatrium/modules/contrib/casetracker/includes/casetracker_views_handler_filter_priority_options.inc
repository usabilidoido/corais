<?php
// $Id: casetracker_views_handler_filter_priority_options.inc,v 1.1.2.1 2009/03/09 15:58:22 jmiccolis Exp $

/**
 * //
 */
class casetracker_views_handler_filter_priority_options extends views_handler_filter_many_to_one {
  
  function get_value_options() {
    $this->value_options = casetracker_realm_load('priority');
  }
}