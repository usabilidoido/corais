<?php

/**
 * Field handler to show atrium_username instad of plain $user->name.
 *
 * @ingroup views_field_handlers
 */
class atrium_username_casetracker_views_handler_field_user_name extends views_handler_field {

  function render($values) {
    $uid = $values->{$this->field_alias};
    if ($uid==0) {
      return casetracker_get_name($uid);
    }
    else {
      $atrium_username = atrium_username_get_name($uid);
      return $atrium_username ? $atrium_username : casetracker_get_name($uid);
    }
  }

}
