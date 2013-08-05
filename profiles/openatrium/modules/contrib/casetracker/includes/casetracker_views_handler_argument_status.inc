<?php
// $Id: casetracker_views_handler_argument_status.inc,v 1.1.2.2 2009/03/09 15:58:22 jmiccolis Exp $

/**
 * Argument handler for case status.
 *
 * @ingroup views_field_handlers
 */
class casetracker_views_handler_argument_status extends views_handler_argument_numeric {
  function summary_name($data) {
    if (!isset($this->case_status_options)) {
      $this->case_status_options = casetracker_realm_load('status');
    }
    return $this->case_status_options[$data->{$this->name_alias}];
  }
}
