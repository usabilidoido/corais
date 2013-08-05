<?php
// $Id: domain_locale.admin.inc,v 1.2.2.2 2010/01/14 17:59:24 nonsie Exp $

/**
 * @file
 * Provides admin functionality for domain specific language settings
 */

/**
 * The domain language page callback router.
 *
 * @param $domain
 *  The $domain object created by domain_lookup().
 */
function domain_locale_page($domain) {
  if (isset($domain['domain_id'])) {
    // Ensure we are on the proper domain.
    domain_goto($domain);

    drupal_set_title(t('!site : Domain language settings', array('!site' => $domain['sitename'])));
    $output = theme_domain_locale_reset($domain);
    return $output . drupal_get_form('domain_locale_languages_overview_form', $domain['domain_id']);
  }
  else {
    return t('Invalid domain request.');
  }
}

/**
 * A modified version of locale_languages_overview_form to manage domain language settings.
 *
 * @param string $type
 * @param int $domain_id
 */
function domain_locale_languages_overview_form($type = 'all', $domain_id) {
  $languages = language_list('language', TRUE);
  $domain_languages = domain_locale_lookup($domain_id);
  $default_domain_language = domain_conf_variable_get($domain_id, 'language_default');
  $default_install_language = language_default('language');
  $domain_default = isset($default_domain_language) ? $default_domain_language : $default_install_language->language;
  $options = array();
  $form['domain_id'] = array(
    '#type' => 'value',
    '#value' => $domain_id,
  );
  $form['weight'] = array('#tree' => TRUE);
  foreach ($languages as $langcode => $language) {
    // Only show enabled languages
    if ($language->enabled == 1) {
      $options[$langcode] = '';

      if (isset($domain_languages[$langcode]) && $language->enabled == 1) {
        $enabled[] = $langcode;
      }
      $form['weight'][$langcode] = array(
        '#type' => 'weight',
        '#default_value' => $domain_languages[$langcode]['weight'],
      );
      $form['name'][$langcode] = array('#value' => check_plain($language->name));
      $form['native'][$langcode] = array('#value' => check_plain($language->native));
    }
  }
  $form['enabled'] = array('#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $enabled,
  );
  $form['site_default'] = array('#type' => 'radios',
    '#options' => $options,
    '#default_value' => $domain_default,
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  $form['#theme'] = 'domain_locale_languages_overview_form';
  return $form;
}

/**
 * Theme the language overview form for domain specific settings.
 *
 * @ingroup themeable
 */
function theme_domain_locale_languages_overview_form($form) {
  $language_default = $form['site_default']['#default_value'];
  foreach ($form['name'] as $key => $element) {
    // Do not take form control structures.
    if (is_array($element) && element_child($key)) {
      // Disable checkbox for the default language, because it cannot be disabled
      if ($key == $language_default) {
        $form['enabled'][$key]['#attributes']['disabled'] = 'disabled';
      }
      $rows[] = array(
        array('data' => drupal_render($form['enabled'][$key]), 'align' => 'center'),
        check_plain($key),
        '<strong>'. drupal_render($form['name'][$key]) .'</strong>',
        drupal_render($form['native'][$key]),
        drupal_render($form['site_default'][$key]),
        drupal_render($form['weight'][$key])
      );
    }
  }
  $header = array(array('data' => t('Enabled')), array('data' => t('Code')), array('data' => t('English name')), array('data' => t('Native name')), array('data' => t('Default')), array('data' => t('Weight')));
  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

/**
 * FormsAPI submit handler for domain language settings
 */
function domain_locale_languages_overview_form_submit($form, &$form_state) {
  $domain_id = $form_state['values']['domain_id'];
  $check = domain_locale_lookup($domain_id);
  if (is_array($check) && count($check) >=1) {
    domain_locale_delete_domain($domain_id);
  }
  $languages = language_list();
  $default_domain_language = domain_conf_variable_get($domain_id, 'language_default');
  $default_install_language = language_default('language');
  $domain_default = isset($default_domain_language) ? $default_domain_language : $default_install_language->language;
  $default = $form_state['values']['site_default'];
  //Construct the array for inserts and updates. Looping over existing languages list.
  $params = array();
  foreach ($languages as $langcode => $language) {
    //Workaround for disabled checkbox and not enabled languages
    if (($default == $langcode || is_string($form_state['values']['enabled'][$langcode]) || $langcode == $domain_default) && $language->enabled == 1) {
      $params[$langcode]['language'] = $langcode;
      $params[$langcode]['weight'] = $form_state['values']['weight'][$langcode];
    }
  }
  domain_locale_insert_domain($domain_id, $params);
  //Update domain_conf language_default variable
  domain_conf_variable_set($domain_id, 'language_default', $default);

  // Changing the language settings impacts the interface.
  cache_clear_all('*', 'cache_page', TRUE);
  drupal_set_message(t('Updated domain specific language settings.'));
  // Return to the correct page.
  $form_state['redirect'] = 'admin/build/domain/language/'. $domain_id;
}


/**
 * Resets locale settings by setting them to default domain settings in {domain_locale}.
 *
 * @param $domain
 * The $domain object created by domain_lookup().
 * @return
 * A confirmation form.
 */
function domain_locale_reset($domain) {
  if ($domain == -1) {
    return t('An invalid request has been made.');
  }
  return drupal_get_form('domain_locale_reset_form', $domain);
}

/**
 * FormsAPI for resetting domain languages.
 *
 * @param $domain
 * The $domain object for the selected domain.
 * @return
 * Themed HTML form.
 */
function domain_locale_reset_form($form_state, $domain) {
  $extra['domain_id'] = array('#type' => 'value', '#value' => $domain['domain_id']);
  $form = confirm_form($extra, t('Are you sure you wish to reset languages for %name?', array('%name' => $domain['sitename'])), 'admin/build/domain/language/'. $domain['domain_id'], t('Submitting this form will apply default domain languages for this domain.'));
  return $form;
}

/**
 * FormsAPI for domain_locale_reset_form.
 */
function domain_locale_reset_form_submit($form, &$form_state) {
  domain_locale_delete_domain($form_state['values']['domain_id']);
  domain_locale_restore_default($form_state['values']['domain_id']);
  drupal_set_message(t('Domain language settings have been reset to default domain settings.'));
  $form_state['redirect'] = 'admin/build/domain/language/'. $form_state['values']['domain_id'];
}

/**
 * Theme a message at the top of domain locale pages.
 *
 * @param $domain
 * The $domain object for the selected domain.
 * @return
 * Themed HTML messages.
 */
function theme_domain_locale_reset($domain) {
  $output = '';
  $output .= '<p>'. t('These settings will replace your default site language settings when %name is the active domain.', array('%name' => $domain['sitename'])) .'</p>';
  $data = db_fetch_array(db_query("SELECT language FROM {domain_locale} WHERE domain_id = %d", $domain['domain_id']));

  if (!empty($data)) {
    $output .= '<p>'. t('You may <a href="!url">erase these settings</a> to restore the default behavior.', array('!url' => url('admin/build/domain/language-reset/'. $domain['domain_id']))) .'</p>';
  }
  return $output;
}
