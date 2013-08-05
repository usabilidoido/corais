<?php

/**
 * @file Helper file for adminnistration of date_picker_formatter module
 */

/**
 * Admin settings
 */
function date_picker_formatter_admin_settings_form() {
  // @todo move some of those settings within the display field settings for more flexibility
  $form = array();
  $form['date_picker_formatter_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Date format'),
    '#default_value' => variable_get('date_picker_formatter_date_format', 'd/m/Y'),
  );
  $form['date_picker_formatter_time_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Time format'),
    '#default_value' => variable_get('date_picker_formatter_time_format', 'H:i'),
  );
  $form['date_picker_formatter_display_user_picture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display user picture'),
    '#default_value' => variable_get('date_picker_formatter_display_user_picture', 1),
  );
  $form['date_picker_formatter_vote_method'] = array(
    '#type' => 'radios',
    '#title' => t('Vote method'),
    '#options' => array(
      'checkbox' => t('Checkbox'),
      'select' => t('Select'),
      'radios' => t('Radios'),
    ),
    '#default_value' => variable_get('date_picker_formatter_vote_method', 'checkbox'),
  );
  $form['date_picker_formatter_vote_label'] = array(
    '#type' => 'textarea',
    '#title' => t('Vote labels'),
    '#default_value' => variable_get('date_picker_formatter_vote_label', "0|No\n100|Yes"),
    '#description' => t('Use a value|Label formatting (one per line) where value is a numerical value between 0 and 100 and the Label a human readable string (translation will be done using i18n module).') .'<br/>'. t('This setting only does not apply with the checkbox "Vote method".'),
  );
  return system_settings_form($form);
}