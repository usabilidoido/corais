<?php

/**
 * @file
 * Hooks provided by Course credit module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to calculate the credit expiration.
 *
 * @param string $type
 *   Credit type from {course_credit_awarded}.type.
 *
 * @return integer
 *   A course credit expiration UNIX timestamp.
 *
 * @see course_credit_award_save()
 */
function hook_course_credit_calculate_expiration($type) {
  // Example: set the credit expiration for a type to 1 week from today.
  if ($type == 'some_type') {
    $expiration = strtotime("+1 week");
  }

  return $expiration;
}

/**
 * Notify modules course credit was awarded.
 *
 * @param object $record
 *   The course credit awarded record, including 'ccaid' Course credit awarded
 *   ID from drupal_write_record().
 *
 * @see course_credit_award_save()
 */
function hook_course_credit_awarded($record) {
  // Example: send users an email when they are awarded credit.
  $params['account'] = $account = user_load($record['uid']);
  $params['record'] = $record;
  $params['subject'] = t("You've got new credit");
  drupal_mail('my_module', 'credit_awarded', $account->mail, user_preferred_language($account), $params);
}

/**
 * Notify modules that course credit application form has been submitted.
 *
 * @param array $form_post
 * @param object $course_node
 * @param object $app_node
 * @param object $account
 *
 * @see course_credit_app_form_submit()
 *
 * @todo Remove this hook. Modules should add their own submit handler through
 * hook_form_alter().
 */
function hook_credit_application_submit($form_post, $course_node, $app_node, $account) {
  // Example: save credit application in a custom table.
  $record['nid'] = $course_node->nid;
  $record['uid'] = $account->uid;
  $record['data'] = serialize($form_post);
  drupal_write_record('my_custom_table', $record);
}

/**
 * Notify modules credit is about to be claimed.
 *
 * @param object $node
 *   The course node.
 */
function hook_course_credit_check_completion($node) {
  // @todo add example, explaining how this hook could be useful.
}

/**
 * Default course credit Features hook, required by hook_features_api().
 *
 * @return array
 *   A course credit type array, matching {course_credit_type} schema fields.
 *
 * @see course_credit_type_features_revert()
 */
function hook_course_credit_default_types() {};

/**
 * Alter the eligible types.
 *
 * @param array $etypes
 *   An array of eligible user credit type objects.
 * @param type $node
 *   The course node.
 * @param type $user
 *   The user taking the course.
 */
function hook_course_credit_user_credit_types_alter(&$etypes, $node, $user) {
  // @todo add example.
}

/**
 * Allow other modules to alter the themed credit application form.
 *
 * @param object $object
 *   By reference.
 *
 * @see theme_course_credit_app_form()
 */
function hook_credit_app_form_alter(&$object) {
  // @todo add example. Display the form as a themed table.
}

/**
 * @}
 */