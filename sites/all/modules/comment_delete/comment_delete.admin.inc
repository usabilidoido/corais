<?php
// $Id: comment_delete.admin.inc,v 1.1.2.1 2010/04/13 00:53:33 robphillips Exp $ 

/**
 * Settings for comment deletion.
 */
function comment_delete_settings_form() {
  $form['comment_delete_replies'] = array(
    '#type' => 'select',
    '#title' => t('Default action for replies'),
    '#description' => t('Specify how replies should be handled by default when deleting a comment.'),
    '#options' => array(
      0 => t('Delete all replies of the comment'),
      1 => t('Move all replies up one level'),
      2 => t('Delete only the comment, or it\'s text and the subject if it has replies'),
    ),
    '#required' => TRUE,
    '#default_value' => variable_get('comment_delete_replies', 0),
  );
  $form['comment_delete_clock'] = array(
    '#type' => 'textfield',
    '#title' => t('Deletion clock'),
    '#description' => t('Enter a time in seconds that a user can delete a comment after it\'s creation.'),
    '#size' => 10,
    '#default_value' => variable_get('comment_delete_clock', 0),
  );
  return system_settings_form($form);
}

/**
 * Validates comment deletion settings.
 */
function comment_delete_settings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['comment_delete_clock']) || $form_state['values']['comment_delete_clock'] < 0) {
    form_set_error('comment_delete_clock', t('Deletion clock must be a numerical value greater than or equal to zero (0).'));
  }
}