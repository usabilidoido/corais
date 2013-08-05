<?php

/**
 * @file
 * Hooks provided by Course restrict credit module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to restrict credit for a course.
 *
 * @param object $node
 *   The course node.
 * @param object $user
 *   The user who may take the course.
 *
 * @return array
 *   A course restriction array, containing:
 *    - takecourse: boolean, whether or not the user may take the course.
 *    - message: translated string to display to the user.
 */
function hook_course_restrict($node, $user) {
  // Example: only allow authenticated users to receive credit.
  if (!$user->uid) {
    $messages = array(t('Only logged in users may claim credit.'));
    $destination = drupal_get_destination();
    $args = array(
      '@login' => url('user/login', $destination),
      '@register' => url('user/register', $destination)
    );
    $messages[] = variable_get('user_register', 1)
      ? t('<a href="@login">Login</a> or <a href="@register">register</a> to receive credit', $args)
      : t('<a href="@login">Login</a> to receive credit', $args);

    return array(
      'takecourse' => FALSE,
      'message' => implode(' ', $messages),
    );
  }
}

/**
 * @}
 */
