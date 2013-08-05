<?php
/**
 * @file
 * Support file for core locale module.
 */

/**
 * Implements hook_node_import_fields().
 */
function locale_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $fields['language'] = array(
      'title' => t('Language'),
      'weight' => 2,

      'is_mappable' => variable_get('language_content_type_' . $node_type, 0),

      'input_format' => 'language',
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_fields_alter().
 */
function locale_node_import_fields_alter(&$fields, $type) {
  foreach ($fields as $fieldname => $fieldinfo) {
    if ($fieldinfo['input_format'] == 'language') {
      $fields[$fieldname]['preprocess'][] = 'node_import_check_language';
      $fields[$fieldname]['tips'][] = t('Language (by code (en/nl), name (English/Dutch) or native name (English/Nederlands).');
    }
  }
}

/**
 * Check if the supplied value for the language field is a valid language.
 */
function node_import_check_language(&$value, $field, $options, $preview) {
  $lvalue = drupal_strtolower($value);

  foreach (language_list() as $code => $data) {
    // allow use of language code
    if ($lvalue == '' || $lvalue == drupal_strtolower($code)) {
      $value = $code;
      return TRUE;
    }

    // allow use of english or native language name
    if ($lvalue == drupal_strtolower($data->name) || $lvalue == drupal_strtolower($data->native)) {
      $value = $code;
      return TRUE;
    }

    // allow use of language name translated into current language
    if ($lvalue == drupal_strtolower(t($data->name))) {
      $value = $code;
      return TRUE;
    }
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a language).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function locale_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  $options = array('' => t('Language neutral'));
  foreach (language_list() as $code => $data) {
    $options[$code] = t($data->name);
  }

  foreach ($fields as $fieldname => $fieldinfo) {
    if ($fieldinfo['input_format'] == 'language') {
      $form[$fieldname] = array(
        '#title' => isset($fieldinfo['title']) ? $fieldinfo['title'] : t('Language'),
        '#type' => 'select',
        '#default_value' => isset($defaults[$fieldname]) ? $defaults[$fieldname] : '',
        '#options' => $options,
      );
    }
  }

  return $form;
}

