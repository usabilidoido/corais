<?php
// $Id: xmlsitemap.admin.inc,v 1.1.4.3 2009/09/05 00:44:53 davereid Exp $


/**
 * @file
 * XML sitemap settings UI.
 */

/**
 * @addtogroup xmlsitemap
 * @{
 */

/*****************************************************************************
 * Menu callbacks / form builders, submit/validate functions.
 ****************************************************************************/

/**
 * Form builder; return the sitemap settings form.
 */
function xmlsitemap_settings() {
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#weight' => -1,
  );
  $form['general']['xmlsitemap_all_links_to_default_language'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add all the links to the default language sitemap'),
    '#default_value' => variable_get('xmlsitemap_all_links_to_default_language', 0),
    '#description' => t('This option will be used only when the language negotiation uses a path prefix mechanism.'),
  );
  $form['general']['xmlsitemap_cron_limit'] = array(
    '#type' => 'select',
    '#title' => t('Cron limit'),
    '#description' => t('The number of links that are updated in each pass of a <a href="@cron">cron maintenance task</a>.', array('@cron' => url('admin/reports/status'))),
    '#default_value' => variable_get('xmlsitemap_cron_limit', 100),
    '#options' => xmlsitemap_cron_options(),
  );
  $form['general']['xmlsitemap_cache_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache directory'),
    '#value' => variable_get('xmlsitemap_cache_directory', file_directory_path() .'/xmlsitemap'),
    '#size' => 60,
    '#description' => t('The directory where the cache files are created; change it only if you are having problems with the default setting.'),
    '#disabled' => TRUE,
  );
  $form['general']['xmlsitemap_use_stylesheet'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use stylesheet'),
    '#default_value' => variable_get('xmlsitemap_use_stylesheet', FALSE),
    '#description' => t('Specify a xml stylesheet for the sitemap?'),
  );
  $form['frontpage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front page'),
    '#collapsible' => TRUE,
  );
  $form['frontpage']['xmlsitemap_front_page_changefreq'] = array(
    '#type' => 'select',
    '#title' => t('Front page change frequency'),
    '#description' => t('The change frequency associated with the front page.'),
    '#default_value' => variable_get('xmlsitemap_front_page_changefreq', 3600),
    '#options' => array(
      '3600' => t('Hourly'),
      '86400' => t('Daily'),
      '604800' => t('Weekly'),
      '2419200' => t('Monthly'),
      '29030400' => t('Yearly'),
    ),
  );
  $form['frontpage']['xmlsitemap_front_page_priority'] = array(
    '#type' => 'select',
    '#title' => t('Front page priority'),
    '#description' => t('The absolute priority for the front page.'),
    '#default_value' => variable_get('xmlsitemap_front_page_priority', 1),
    '#options' => xmlsitemap_priority_options(),
  );
  $form = system_settings_form($form);
  $form['buttons']['#weight'] = 10;
  $form['#submit'][] = 'xmlsitemap_settings_submit';
  $form['#validate'][] = 'xmlsitemap_settings_validate';
  return $form;
}

/**
 * Validate the sitemap settings form.
 */
function xmlsitemap_settings_validate($form, &$form_state) {
  $directory = $form_state['values']['xmlsitemap_cache_directory'];
  file_check_directory($directory, FILE_CREATE_DIRECTORY, 'xmlsitemap_cache_directory');
  $form_state['values']['xmlsitemap_cache_directory'] = $directory;
}

/**
 * Submit the settings form.
 */
function xmlsitemap_settings_submit($form, &$form_state) {
  xmlsitemap_flag_sitemap();
}

/**
 * Form builder; return the tools form.
 */
function xmlsitemap_tools() {
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Operations'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $options = array();
  foreach (module_invoke_all('xmlsitemap_operations') as $operation => $info) {
    $options[$operation] = $info['label'];
  }
  reset($options);
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => key($options),
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('xmlsitemap_tools_submit'),
  );
  return $form;
}

/**
 * Submit the tools form.
 */
function xmlsitemap_tools_submit($form, &$form_state) {
  $operations = module_invoke_all('xmlsitemap_operations');
  $operation  = $operations[$form_state['values']['operation']];
  $function   = $operation['callback'];
  if (isset($operation['callback arguments'])) {
    call_user_func_array($function, $operation['callback arguments']);
  }
  else {
    call_user_func($function, NULL);
  }
  $form_state['redirect'] = 'admin/settings/xmlsitemap/tools';
}

/**
 * @} End of "addtogroup xmlsitemap".
 */
