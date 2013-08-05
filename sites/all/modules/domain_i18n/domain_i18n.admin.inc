<?php
// $Id: domain_i18n.admin.inc,v 1.3 2009/06/08 23:17:05 nonsie Exp $

/**
 * @file
 * @defgroup domain_i18n
 * Domain Internationalization: manage domain specific
 * multilingual variables
 */

/**
 * Provide an interface to view multilingual variables
 *
 * Provide an interface to view multilingual variables available for
 * Domain Access defined domains.
 */
function domain_i18n_settings_page() {
  drupal_set_title(t('Available multilingual variables for Domain Access domains'));
  return $output . drupal_get_form('domain_i18n_variables_overview_form');
}

function domain_i18n_variables_overview_form() {
  global $conf;
  $domains = domain_domains();
  $domain_i18n_variables = $conf['domain_i18n_variables'];
  // Multidimensional array of domains and their defined variables per language
  $domain_i18n_settings = _domain_i18n_variables_settings();
  $options = array();
  $enabled = array();
  if (count($domain_i18n_variables) > 0) {
    foreach ($domain_i18n_variables as $key => $variable) {
      $options[$variable] = $variable;
      if (is_array($domain_i18n_settings) && array_key_exists($variable, $domain_i18n_settings)) {
        $variable_domains = $domain_i18n_settings[$variable];
        if (is_array($variable_domains) && count($variable_domains) > 0) {
          foreach ($variable_domains as $id => $single_domain) {
            $enabled[$single_domain][$variable] = check_plain($variable);
          }
        }
      }
    }
    foreach ($domains as $domain) {
      $enabled_vars = isset($enabled[$domain['domain_id']]) ? $enabled[$domain['domain_id']] : array();
      $form['domains'][$domain['domain_id']] = array('#type' => 'checkboxes',
        '#options' => $options,
        '#default_value' => $enabled_vars,
        '#title' => '',
      );
      $form['domain_names'][$domain['domain_id']] = array('#value' => $domain['sitename']);
    }
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
    $form['#theme'] = 'domain_i18n_variables_overview_form';
    return $form;
  }
}

/**
 * Theme the variables overview form for domain specific settings.
 *
 * @ingroup themeable
 */
function theme_domain_i18n_variables_overview_form($form) {
  $output = '';
  if (is_array($form['domains']) && count($form['domains'] > 0)) {
    foreach ($form['domains'] as $key => $element) {
      // Do not take form control structures.
      if (is_array($element) && element_child($key)) {
        $admin_link = l(t('edit domain variables'), 'admin/build/domain/i18n/variables/'. $key, array('attributes' => array('class' => 'domain_i18n_admin_link')));
        $rows[] = array(
          array('data' => drupal_render($form['domain_names'][$key]), 'align' => 'left'),
          array('data' => drupal_render($form['domains'][$key]), 'align' => 'left'),
          array('data' => $admin_link, 'align' => 'center'),
        );
      }
    }

    $header = array(array('data' => t('Domain name')), array('data' => t('Multilingual variables'), 'colspan' => 2));
    unset($form['domain_names']);
    $output = theme('table', $header, $rows);
    $output .= drupal_render($form);
  }
  else {
    drupal_set_message(t('There are currently no multilingual variables set up for domains. Please refer to README.txt for instructions.'), 'warning');
  }
  return $output;
}

/**
 * FormsAPI submit handler for domain sepcific variable settings
 */
function domain_i18n_variables_overview_form_submit($form, &$form_state) {
  $enabled_domains = domain_domains();
  $domain_i18n_settings = array();
  foreach ($form_state['values'] as $key => $values) {
    if (is_int($key) && array_key_exists($key, $enabled_domains) && is_array($values)) {
      $domain_i18n_settings[$key] = array();
      foreach ($values as $variable => $enabled) {
        if (!empty($enabled)) {
          array_push($domain_i18n_settings[$key], $variable);
        }
        else {
          // Variable is disabled, clean up its various values
          domain_i18n_variable_delete($key, $variable);
        }
      }
    }
  }
  variable_set('domain_i18n_settings', $domain_i18n_settings);
  drupal_set_message(t('Updated multilingual variable settings for Domain Access domains.'));
}

/**
 * Multilingual variable administration on per domain basis
 */
function domain_i18n_variables_page($domain) {
  drupal_set_title(t('Available multilingual variables for %sitename', array('%sitename' => $domain['sitename'])));
  return $output . drupal_get_form('domain_i18n_domain_variables_form', $domain);
}

function domain_i18n_domain_variables_form($form_state, $domain) {
  // We need to be on the domain to get correct varaibles
  $domain_id = $domain['domain_id'];
  domain_set_domain($domain_id);
  $domain_languages = domain_i18n_supported_languages($domain_id);
  $domain_variable_settings = domain_i18n_variable_settings($domain_id);
  $domain_i18n_variables = domain_i18n_domain_variable_get($domain_id);
  // Special handling for domain_conf variables
  $domain_conf_variables = module_exists('domain_conf') ? TRUE : FALSE;
  if (count($domain_variable_settings) > 0) {
    foreach ($domain_variable_settings as $variable) {
      $default_variable_value = variable_get($variable, '');
      $variable_format = _domain_i18n_variable_type($default_variable_value);
      if ($variable_format > 1) {
        $form['variables'][$variable] = array(
          '#type' => 'item',
          '#title' => $variable,
          '#tree' => TRUE,
          '#description' => t('This variable is of complex data type and cannot be saved as an array or string.'),
        );
      }
      else {
        if ($variable_format == 1) {
          $default_variable_value = serialize($default_variable_value);
        }
        $form['variables'][$variable] = array(
          '#type' => 'fieldset',
          '#title' => $variable,
          '#tree' => TRUE,
          '#description' => t('This variable will be saved as <strong>%type</strong>', array('%type' => $variable_format == 0 ? t('a string') : t('an array'))),
        );
        $form['variables'][$variable]['default'] = array(
          '#type' => 'fieldset',
          '#title' => t('Default values'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#weight' => 100,
        );
        $form['variables'][$variable]['default']['site'] = array(
          '#type' => 'textarea',
          '#title' => t('Site default value for %variable', array('%variable' => $variable)),
          '#description' => 'Default value can contain HTML so it is presented in the textarea for easier editing. This value will not be saved on form submt.',
          '#default_value' => $default_variable_value,
          '#rows' => 5,
        );
        if ($domain_conf_variables) {
          if ($domain_conf_value = domain_conf_variable_get($domain_id, $variable)) {
            $form['variables'][$variable]['default']['domain_conf'] = array(
              '#type' => 'textarea',
              '#rows' => 5,
              '#title' => t('Domain Conf value for %variable', array('%variable' => $variable)),
              '#value' => (is_string($domain_conf_value) || is_numeric($domain_conf_value)) ? $domain_conf_value : serialize($domain_conf_value),
              '#description' => 'Domain Conf value can contain HTML so it is presented in the textarea for easier editing. This value will not be saved on form submt.',
            );
          }
        }
        foreach ($domain_languages as $langcode => $language) {
          $domain_default_value = '';
          if (is_array($domain_i18n_variables[$domain_id][$variable]) && array_key_exists($langcode, $domain_i18n_variables[$domain_id][$variable])) {
            $domain_default_value = unserialize($domain_i18n_variables[$domain_id][$variable][$langcode]);
          }
          $form['variables'][$variable][$langcode] = array(
            '#type' => 'textarea',
            '#title' => $variable .' ('. $langcode .')',
            '#default_value' => (is_string($domain_default_value) || is_numeric($domain_default_value)) ? $domain_default_value : serialize($domain_default_value),
            '#rows' => 5,
          );
          $form['variables'][$variable]['format'] = array(
            '#type' => 'hidden',
            '#value' => $variable_format,
          );
        }
      }
    }
    $form['domain_id'] = array(
      '#type' => 'hidden',
      '#value' => $domain_id,
    );
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  }
  else {
    $output = t('Please enable multilingual variables for this domain.');
    drupal_set_message($output);
  }
  return $form;
}

/**
 * Submit function for domain_i18n_domain_variables_form
 */
function domain_i18n_domain_variables_form_submit($form, &$form_state) {
  $domain_id = $form_state['values']['domain_id'];
  $domain_variable_settings = domain_i18n_variable_settings($domain_id);
  $domain_i18n_variables = domain_i18n_domain_variable_get($domain_id);
  $settings_count = count($domain_variable_settings);
  foreach ($form_state['values'] as $key => $values) {
    if (in_array($key, $domain_variable_settings) && is_array($values) && $key != 'format') {
      $reset = FALSE;
      // Site default and domain_conf settings cannot be saved using this form!
      unset($values['default']);
      foreach ($values as $langcode => $value) {
        if ($langcode != 'format') {
        $domain_variable_values = array();
        // If user did not submit a value for this language specific variable reset it from domain specific variables
        if (!empty($value)) {
          // $value can be a serialized array in some cases
          $value = $form_state['values'][$key]['format'] == 1 ? $value : serialize($value);
          $domain_variable_values[] = array(
            'name' => $key,
            'domain' => $domain_id,
            'language' => $langcode,
            'value' => $value,
          );
        }
        else {
          $reset = TRUE;
        }
        // Variable value was previously defined or has been deleted
        if (isset($domain_i18n_variables[$domain_id][$key][$langcode]) || $reset) {
          domain_i18n_variable_delete($domain_id, $key, $langcode);
        }
        domain_i18n_variable_insert($domain_variable_values);
      }
      }
    }
  }
  drupal_set_message(t('Updated domain specific multilingual variables.'));
}

/**
 * Determine variable data type
 */
function _domain_i18n_variable_type($variable) {
  $type = 0; // string or numeric
  if (!is_string($variable) && !is_numeric($variable)) {
    if (is_array($variable)) {
      $type = 1;
      foreach ($variable as $key => $value) {
        if (is_array($value)) {
          $type = 2;
          break;
        }
      }
    }
    else {
      $type = 2;
    }
  }
  return $type;
}