<?php
// $Id: casetracker_views_handler_field_type_name.inc,v 1.2.2.2 2010/04/15 19:19:13 jmiccolis Exp $

/**
 * Field handler to show Selective state.
 *
 * @ingroup views_field_handlers
 */
class casetracker_views_handler_field_type_name extends views_handler_field {

  function render($values) {
    if ($values->{$this->field_alias}) {
      return check_plain(casetracker_case_state_load($values->{$this->field_alias}, 'type'));
    }
    return '';
  }
}
