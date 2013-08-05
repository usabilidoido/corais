<?php
// $Id: casetracker_views_handler_field_user_name.inc,v 1.1.2.1 2010/04/15 19:19:13 jmiccolis Exp $

/**
 * Field handler to show Selective state.
 *
 * @ingroup views_field_handlers
 */
class casetracker_views_handler_field_user_name extends views_handler_field {

  function render($values) {
    return casetracker_get_name($values->{$this->field_alias});
  }
}
