<?php
// $Id: mentions_views_handler_field_user_name.inc,v 1.1 2009/07/17 05:57:52 deciphered Exp $
/**
 * @file
 * Field handler to provide simple renderer that allows using a themed user link.
 */
class mentions_views_handler_field_user_name extends views_handler_field_user_name {
  function render_link($data, $values) {
    if (!empty($this->options['link_to_user']) || !empty($this->options['overwrite_anonymous'])) {
      $account = user_load($data);
      if (!empty($this->options['overwrite_anonymous']) && !$account->uid) {
        // This is an anonymous user, and we're overriting the text.
        return check_plain($this->options['anonymous_text']);
      }
      elseif (!empty($this->options['link_to_user'])) {
        return theme('username', $account);
      }
    }
    // Otherwise, there's no special handling, so return the data directly.
    return $data;
  }
}
