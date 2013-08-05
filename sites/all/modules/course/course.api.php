<?php

/**
 * @file
 * Hooks provided by Course module.
 *
 * @todo until we finish cleaning up all files and submodules, grep through
 * everything for 'drupal_alter', 'module_invoke_all', 'module_invoke', and
 * 'module_implements'.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to define handlers that extend Course functionality.
 *
 * @return array
 *   A associative array of handler declarations, keyed by type:
 *   - object: An associative array of course object types, keyed by type:
 *     - name: A string to reference this type on administrative forms.
 *     - description: A string to display more information to administrators.
 *     - class: (optional) A class name which will override the default
 *       CourseObject class.
 *   - context: An asociative array of context handlers, keyed by type:
 *     - callback: A function name that will set course context for special
 *       cases not already covered by Course module.
 *     - file: (optional) A string to locate the callback file. This should be
 *       specified if not located in the implementing module's .module file.
 *     - file path: (optional) The path to the directory containing the file
 *       specified in 'file'. Defaults to the implementing module path.
 *   - outline: An asociative array of outline handlers, keyed by type:
 *     - name: A string to reference this type on administrative forms.
 *     - description: A string to display more information to administrators.
 *     - callback: A function name that will return themed course outline HTML.
 *     - file: (optional) A string to locate the callback file. This should be
 *       specified if not located in the implementing module's .module file.
 *     - file path: (optional) The path to the directory containing the file
 *       specified in 'file'. Defaults to the implementing module path.
 *   - settings: An associative array of configurations, which will be available
 *     as secondary tabs from the Course sitewide settings form:
 *     - name: A string to reference this type on administrative forms.
 *     - description: A string to display more information to administrators.
 *     - callback: A function name that will return a Drupal form API array.
 *     - file: (optional) A string to locate the callback file. This should be
 *       specified if not located in the implementing module's .module file.
 *     - file path: (optional) The path to the directory containing the file
 *       specified in 'file'. Defaults to the implementing module path.
 *     - package: (optional) The key of the settings package this form should be
 *       grouped with.
 */
function hook_course_handlers() {
  // Example: a custom module definition.
  return array(
    'object' => array(
      'custom' => array(
        'name' => t('Custom'),
        'class' => 'CustomCourseObject',
        'description' => t('A custom course object.'),
      ),
    ),
    'outline' => array(
      'custom' => array(
        'name' => t('Custom'),
        'description' => t('Custom outline display.'),
        'callback' => 'custom_outline',
      ),
    ),
    'context' => array(
      'custom' => array(
        'callback' => 'custom_course_context',
      ),
    ),
    'settings' => array(
      'custom' => array(
        'name' => t('Custom'),
        'description' => t('Course custom configurations.'),
        'callback' => 'custom_course_settings',
      ),
      'followup' => array(
        'name' => t('Follow up'),
        'description' => t('Course custom followup configurations.'),
        'callback' => 'custom_course_followup_settings',
        'file' => 'includes/another_module.followup.inc',
        'file path' => drupal_get_path('module', 'another_module'),
        'package' => 'custom',
      ),
    ),
  );
}

/**
 * Allow modules to alter each other's list of handlers.
 *
 * @param array $handlers
 *   By reference. The return value from each module that implements
 *   hook_course_handlers().
 * @param type $module
 *
 * @see course_get_handlers()
 */
function hook_course_handlers_alter(&$handlers, $module) {
  // Example: alter the class of a course object handler.
  $is_quiz_type = isset($handlers['object']) && isset($handlers['object']['quiz']);
  if ($module == 'course_quiz' && $is_quiz_type) {
    // Change which class should be used.
    $handlers['object']['quiz']['class'] = 'CustomQuizCourseObject';
  }
}

/**
 * Allow modules to add links to the course completion landing page, such as
 * post-course actions.
 *
 * @param array $links
 *   By reference. Currently an array of three elements:
 *   - 0: $path param for l().
 *   - 1: $text param for l().
 *   - 2: A description, suitable for theme_form_element().
 * @param object $course_node
 *   The course node object.
 * @param object $account
 *   The user who just took the course.
 *
 * @see course_outline_show_complete_links()
 */
function hook_course_outline_completion_links(&$links, $course_node, $account) {
  // Example: add a link.
  $links[] = array(t('Go home!'), '<front>', t('If you got this far, you
    deserve a link back home'));
}

/**
 * Allow modules to alter remaining incomplete links on the course completion
 * landing page.
 *
 * @param array $links
 *   Same as $links param for hook_course_outline_completion_links().
 * @param object $course_node
 *   The course node object.
 * @param object $account
 *   The user who just took the course.
 *
 * @see course_outline_show_complete_links()
 */
function hook_course_outline_incomplete_links(&$links, $course_node, $account) {
  // Example: change the default link.
  $links['course'] = array(t("Let's try that again"), "node/$course_node->nid/takecourse", t('Looks like you missed something.'));
}

/**
 * Allow modules to restrict menu access to the take course tab.
 *
 * @param object $node
 *   The course node.
 * @param object $user
 *   The user to check access.
 *
 * @return boolean
 *   Any hook returning FALSE will restrict access to the take course tab.
 */
function hook_course_has_takecourse($node, $user) {
  // @todo add example.
}

/**
 * Allow modules to restrict menu access to the course setting tab.
 *
 * @param object $node
 *   The course node.
 * @param object $user
 *   The user to check access.
 *
 * @return boolean
 *   Any hook returning FALSE will restrict access to the course settings tab.
 *
 * @see course_settings_menu_access()
 *
 * @todo why would we want this hook? Modules can already use hook_menu_alter().
 */
function hook_course_has_settings($node, $user) {
  // @todo add example.
}

/**
 * Allow modules to determine if this course should be restricted.
 *
 * If any module implementing this hook returns FALSE or an array containing
 * 'success' => FALSE, the course will be restricted.
 *
 * @param object $node
 *   The course node.
 * @param object $user
 *   The user who may or may not take the course.
 *
 * @return boolean|array
 *   Either FALSE, or an array containing:
 *   - success: Boolean. Indicates whether or not the user has permission to
 *     take this course.
 *   - message: String. If success is FALSE, a message to display to the user.
 *
 * @see course_take_course_access()
 */
function hook_can_take_course($node, $user) {
  // @see course_can_take_course() and hook_can_take_course() for examples of
  // how to use this hook.
}

/**
 * Allow modules to provide the course button.
 *
 * @param object $node
 *   The course node.
 *
 * @see course_take_course_button_html()
 */
function hook_course_button($node) {
  // Example: change the button text, and add a custom class.
  $link = l(t('Take Me'), "node/{$node->nid}/takecourse");
  return '<div class="action-link my-custom-class">' . $link . '</div>';
}

/**
 * Notify modules about a course enrollment.
 *
 * @param object $node
 *   The course node.
 * @param object $user
 *   The enrolling user.
 * @param string $from
 *   The type of enrollment, if applicable. {course_enrolment}.enrollmenttype.
 * @param string $code
 *   The access code used to enroll. {course_enrolment}.code.
 * @param integer $status
 *   The enrolment status. {course_enrolment}.status.
 *
 * @see course_enrol()
 */
function hook_course_enrol($node, $user, $from, $code, $status) {
  // @todo add example.
}

/**
 * Notify other modules after course unenrollment.
 *
 * @param object $node
 *   The course node.
 * @param object $user.
 *   The unenrolled user.
 *
 * @see course_unenrol()
 */
function hook_course_unenrol($node, $user) {
  // @todo add example.
}

/**
 * Allow modules to set self-enrollment access for a user.
 *
 * Modules implementating This hook should return the status, and optionally a
 * failure message if success is FALSE.
 *
 * @param object $node
 *   The course node.
 * @param object $user
 *   The user who may or may not take the course.
 *
 * @return array
 *   An associative array of access arrays, each containing an array of:
 *   - success: Boolean. Indicates whether or not the user has permission to
 *     self-enroll in this course.
 *   - message: String. If success is FALSE, a message to display to the user.
 *
 * @see course_enrol_access()
 */
function hook_course_can_enrol($node, $user) {
  // Example: do not allow users to take courses on Wednesdays.
  if (date('L') == 'wednesday') {
    $hooks[] = array(
      'success' => FALSE,
      'message' => t('Courses are closed on Wednesdays.'),
    );
  }
  // Example: however allow users to bypass enrollment restriction on Christmas.
  elseif ((date('m') == 12) && (date('d') == 25)) {
    $hooks[] = array('success' => TRUE);
  }

  return $hooks;
}

/**
 * Allow modules to alter course reports before saving.
 *
 * @param stdClass $entry
 *   By reference. The object parameter from course_report_save().
 * @param stdClass $account
 *   The fully loaded report user.
 * @param stdClass $old
 *   The currently saved version of the user's report for a course.
 *
 * @see course_report_save()
 */
function hook_course_report_alter(&$entry, $account, $old) {
  // @see course_relationships_course_report_alter()
}

/**
 * Notify modules that a course report has been saved.
 *
 * @param stdClass $entry
 *   By reference. The object parameter from course_report_save(), now including
 *   the 'crid': course report record ID from drupal_write_record().
 * @param stdClass $account
 *   The fully loaded report user.
 * @param stdClass $old
 *   The former saved version of the user's report for a course.
 *
 * @see course_report_save()
 */
function hook_course_report_saved($entry, $account, $old) {
  // Example: Do something completely unnecesary.
  if ($entry->crid == 1000000) {
    drupal_set_message(t('Congratulations %name, you have just saved the one millionth course report!', array('%name' => theme('username', $account))));
  }
}

/**
 * @}
 */
