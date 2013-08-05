<?php
// $Id: casetracker_views_handler_argument_priority.inc,v 1.1.2.2 2009/03/09 15:58:22 jmiccolis Exp $

/**
 * Argument handler for case priority.
 *
 * @ingroup views_field_handlers
 */
class casetracker_views_handler_argument_priority extends views_handler_argument_numeric {
  function summary_name($data) {
    if (!isset($this->case_priority_options)) {
      $this->case_priority_options = casetracker_realm_load('priority');
    }
    return $this->case_priority_options[$data->{$this->name_alias}];
  }
}
