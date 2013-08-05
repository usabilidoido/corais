<?php
/**
 * @file
 *   Admin forms for the Taskbar Activity module.
 */

/**
 * Settings form.
 */

function taskbar_activity_admin(&$form_state) {
  $form['taskbar_activity_cron_cleanup'] = array(
    '#type' => 'select',
    '#title' => t('Delete taskbar alerts from database after'),
    '#description' => t('taskbar will delete alerts from the database that are older than this value when cron is run. Disabled by default.'),
    '#required' => TRUE,
    '#default_value' => variable_get('taskbar_activity_cron_cleanup', 0),
    '#options' => array(
      0 => t('Disabled'),
      604800 => t('1 week'),
      1209600 => t('2 weeks'),
      2592000 => t('1 month'),
      7862400 => t('3 months'),
      15811200 => t('6 months'),
      31536000 => t('1 year'),
    ),
  );
  $form['taskbar_activity_cron_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Rows to delete per cron run'),
    '#description' => t('Delete this number of taskbar alerts from the database during cron runs. Only used if option above is not set to Disabled.'),
    '#required' => TRUE,
    '#default_value' => variable_get('taskbar_activity_cron_limit', 1000),
  );
  return system_settings_form($form);
}

/**
 * Validate function for the admin settings form.
 */
function taskbar_activity_admin_validate($form, &$form_state) {
  $l = $form_state['values']['taskbar_activity_cron_limit'];
  if (!is_int($l) && $l < 0) {
    form_set_error('taskbar_cron_limit', t('The rows to delete per cron run must be a positive integer.'));
  }
}

/**
 * Settings form for the activity provider.
 */
function taskbar_activity_settings(&$form, &$form_state, $item) {
  $form['provider_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Activity settings'),
    '#collapsible' => TRUE,
  );
  
  if (empty($item->settings['source'])) {
    $item->settings['source'] = 'tacore';
  }
  if (empty($item->settings['max items'])) {
    $item->settings['max items'] = 5;
  }

  $sources = drupal_map_assoc(array_keys(taskbar_activity_get_sources()));
  $form['provider_settings']['source'] = array(
    '#type' => 'select',
    '#title' => t('Source'),
    '#description' => t('Select the activity source you would like to use for this item.'),
    '#required' => TRUE,
    '#default_value' => $item->settings['source'],
    '#options' => $sources,
  );
  $form['provider_settings']['max_items'] = array(
    '#type' => 'select',
    '#title' => t('Max recent messages'),
    '#description' => t('Select the number of recent messages to display.'),
    '#required' => TRUE,
    '#default_value' => $item->settings['max items'],
    '#options' => drupal_map_assoc(range(1,10)),
  );
}

function taskbar_activity_settings_submit($form, &$form_state, &$item) {
  $item->settings['source'] = $form_state['values']['source'];
  $item->settings['max items'] = (int) $form_state['values']['max_items'];
}

