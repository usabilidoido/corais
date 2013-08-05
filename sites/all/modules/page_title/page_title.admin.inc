<?php

/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function page_title_admin_settings() {
  // Set the theme callback for the patterns section
  $form['patterns'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Title Patterns'),
    '#collapsible' => FALSE,
    '#theme' => 'page_title_admin_settings'
  );

  $all_settings = page_title_get_settings();

  foreach ($all_settings as $key => $settings) {
    $form['patterns']['pattern'][$key] = array(
      '#title' => t($settings['label'], $settings['label arguments']),
      '#default_value' => variable_get($key, $settings['default']),
      '#required' => $settings['required'],
      '#description' => t($settings['description'], $settings['description arguments']),
      '#weight' => $settings['weight'],
      '#token_types' => $settings['scopes'],
      '#element_validate' => array('token_element_validate'),
      '#type' => 'textfield',
      '#size' => 30,
      '#maxlength' => 255,
    );

    $form['patterns']['scope'][$key] = array(
      '#type' => 'markup',
      '#value' => implode('<br />', array_map('_page_title_scope_t', $settings['scopes'])),
    );

    if ($settings['show field']) {
      $form['patterns']['showfield'][$key .'_showfield'] = array(
        '#type' => 'checkbox',
        '#default_value' => variable_get($key .'_showfield', 0),
      );
    }
  }

  // Add the token tree at the end of the configuration page.
  $form['token_help'] = array(
    '#type' => 'markup',
    '#value' => theme('token_tree', 'all', TRUE),
  );

  // Return the form with the system buttons & action added to it
  return system_settings_form($form);
}


function _page_title_scope_t($item) {
  return t(ucfirst($item));
}
