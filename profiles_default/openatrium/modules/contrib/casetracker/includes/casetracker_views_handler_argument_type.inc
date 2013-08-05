<?php
// $Id: casetracker_views_handler_argument_type.inc,v 1.1.2.2 2009/03/09 15:58:22 jmiccolis Exp $

/**
 * Argument handler for case type.
 *
 * @ingroup views_field_handlers
 */
class casetracker_views_handler_argument_type extends views_handler_argument_numeric {
  function summary_name($data) {
    if (!isset($this->case_type_options)) {
      $this->case_type_options = casetracker_realm_load('type');
    }
    return $this->case_type_options[$data->{$this->name_alias}];
  }
}
