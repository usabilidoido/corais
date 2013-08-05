<?php

/**
 * Special join handler to allow join on two colums.
 *
 * Do not sanitize the extra data coming from our course.views.inc. See
 * http://drupal.org/node/560492
 */
class views_join_course extends views_join {

  function db_safe($input, $info) {
    if (!is_array($input)) {
      return $input;
    }
    else {
      return parent::db_safe($input, $info);
    }
  }

}
