<?php
/**
 * @file
 *   Custom text provider for Taskbar.
 */

function taskbar_provider_custom_definition() {
  return array(
    'name' => 'custom',
    'title' => 'Custom',
    'ajax' => FALSE,
    'realtime' => FALSE,
    'callback' => 'taskbar_provider_custom',
    'settings form' => 'taskbar_provider_custom_settings',
    'settings form submit' => 'taskbar_provider_custom_settings_submit',
    'file' => 'taskbar_provider.custom.inc',
    'path' => drupal_get_path('module', 'taskbar') . '/providers',
  );
}

function taskbar_provider_custom($item) {
  // Evaluate tokens in the user-defined text, if token module is installed
  if (module_exists('token')) {
    $content = token_replace($item->settings['data']);
  }
  else {
    $content = $item->settings['data'];
  }

  return array('content' => $content);
}

/**
 * Settings form for the custom provider.
 */
function taskbar_provider_custom_settings(&$form, &$form_state, $item) {
  $form['provider_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block settings'),
    '#collapsible' => TRUE,
  );
  
  if (empty($item->settings['data'])) {
    $item->settings['data'] = '';
  }
  $form['provider_settings']['custom'] = array(
   '#type' => 'textarea',
   '#title' => t('Custom text'),
   '#default_value' => $item->settings['data'],
  );

  $form['provider_settings']['token_help'] = array(
    '#title' => t('Replacement patterns'), 
    '#type' => 'fieldset', 
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE, 
  );
  $form['provider_settings']['token_help']['help'] = array(
    '#value' => theme('token_help'),
  );
}

function taskbar_provider_custom_settings_submit($form, &$form_state, &$item) {
  $item->settings['data'] = $form_state['values']['custom'];
}