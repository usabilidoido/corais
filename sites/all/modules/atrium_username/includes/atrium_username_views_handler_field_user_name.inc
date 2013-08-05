<?php

/**
 * Field handler to provide a renderer that theme user name in Open Atrium views
 * using atrium_username instead of username field.
 *
 * @ingroup views_field_handlers
 */
class atrium_username_views_handler_field_user_name extends views_handler_field_user_name {

  function render_link($data, $values) {

    // get uid field name
    $field_uid = isset($this->aliases['uid']) ? $this->aliases['uid'] : 'uid';

    if (!empty($this->options['link_to_user']) || !empty($this->options['overwrite_anonymous'])) {
      $account = new stdClass();
      $account->uid = $values->{$field_uid};
      if (!empty($this->options['overwrite_anonymous']) && !$account->uid) {
        // This is an anonymous user, and we're overriting the text.
        return check_plain($this->options['anonymous_text']);
      }
      elseif (!empty($this->options['link_to_user'])) {
        $account->name = $values->{$this->field_alias};
        return theme('username', $account);
      }
    }
    // Otherwise, there's no special handling, so try to return atrium_username.
    if (isset($values->{$field_uid})) {
      $atrium_username = atrium_username_get_name($values->{$field_uid});
      return $atrium_username ? $atrium_username : $data;
    }
    else {
       return $data;
    }
  }

}
