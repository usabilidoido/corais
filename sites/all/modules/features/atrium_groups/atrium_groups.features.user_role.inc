<?php

/**
 * Implementation of hook_user_default_roles().
 */
function atrium_groups_user_default_roles() {
  $roles = array();

  // Exported role: participant
  $roles['participant'] = array(
    'name' => 'participant',
  );

  return $roles;
}
