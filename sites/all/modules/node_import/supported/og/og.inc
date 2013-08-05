<?php

/**
 * @file
 * Support file for the og (Organic Groups) module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function og_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    if (og_is_group_type($node_type)) {
      $fields['og_description'] = array(
        'title' => t('Description'),
        'group' => t('Organic group'),
        'map_required' => TRUE,
      );

      $fields['og_selective'] = array(
        'title' => t('Membership requests'),
        'group' => t('Organic group'),
        'allowed_values' => og_selective_map(),
        'default_value' => OG_OPEN,
      );

      $fields['og_register'] = array(
        'title' => t('Registration form'),
        'group' => t('Organic group'),
        'input_format' => 'boolean',
      );
      $visibility = variable_get('og_visibility_registration', OG_REGISTRATION_CHOOSE_FALSE);
      if (user_access('administer nodes')) {
        $visibility = in_array($visibility, array(OG_REGISTRATION_NEVER, OG_REGISTRATION_CHOOSE_FALSE)) ? OG_REGISTRATION_CHOOSE_FALSE  : OG_REGISTRATION_CHOOSE_TRUE;
      }
      $default = FALSE;
      switch ($visibility) {
        case OG_REGISTRATION_ALWAYS:
          $default = TRUE;
          // fall through
        case OG_REGISTRATION_NEVER:
          $fields['og_register']['is_mappable'] = FALSE;
          break;

        case OG_REGISTRATION_CHOOSE_TRUE:
          $default = TRUE;
          // fall through
        case OG_REGISTRATION_CHOOSE_FALSE:
          break;
      }
      $fields['og_register']['default_value'] = $default;

      $fields['og_directory'] = array(
        'title' => t('List in groups directory'),
        'group' => t('Organic group'),
        'input_format' => 'boolean',
      );
      $visibility = variable_get('og_visibility_directory', OG_DIRECTORY_CHOOSE_TRUE);
      if (user_access('administer nodes')) {
        $visibility = in_array($visibility, array(OG_DIRECTORY_NEVER, OG_DIRECTORY_CHOOSE_FALSE)) ? OG_DIRECTORY_CHOOSE_FALSE  : OG_DIRECTORY_CHOOSE_TRUE;
      }
      $default = FALSE;
      switch ($visibility) {
        case OG_DIRECTORY_ALWAYS:
          $default = TRUE;
          // fall through
        case OG_DIRECTORY_NEVER:
          $fields['og_directory']['is_mappable'] = FALSE;
          break;

        case OG_DIRECTORY_CHOOSE_TRUE:
          $default = TRUE;
          // fall through
        case OG_DIRECTORY_CHOOSE_FALSE:
          break;
      }
      $fields['og_directory']['default_value'] = $default;

      if (module_exists('locale') && $languages = locale_language_list()) {
        $fields['og_language'] = array(
          'title' => t('Group language'),
          'group' => t('Organic group'),
        );
      }
    }

    if (og_is_group_post_type($node_type)) {
      $fields['og_groups'] = array(
        'title' => t('Audience'),
        'group' => t('Organic group'),
        'input_format' => 'nodereference',
        'has_multiple' => TRUE,
        'is_checkboxes' => TRUE,
        'allowed_values' => _node_import_og_groups(),
      );
    }
  }

  return $fields;
}

/**
 * Returns a list of available og_groups for current user.
 */
function _node_import_og_groups() {
  global $user;
  $og_groups = array();

  if (user_access('administer nodes')) {
    $og_groups = og_all_groups_options();
  }
  else {
    foreach (og_get_subscriptions($user->uid) as $key => $value) {
      $og_groups[$key] = $value['title'];
    }
  }

  return $og_groups;
}

/**
 * Implementation of hook_node_import_fields_alter().
 */
function og_node_import_fields_alter(&$fields, $type) {
  if (($node_type = node_import_type_is_node($type)) !== FALSE && og_is_group_type($node_type)) {
    if (isset($fields['body']) && $fields['body']['title'] == t('Body')) {
      $fields['body']['title'] = t('Mission statement');
    }
  }
}

/**
 * Implementation of hook_node_import_defaults().
 */
function og_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    if (og_is_group_type($node_type)) {
      $form['og_selective'] = array(
        '#type' => 'radios',
        '#title' => t('Membership requests'),
        '#options' => og_selective_map(),
        '#default_value' => isset($defaults['og_selective']) ? $defaults['og_selective'] : OG_OPEN,
      );

      $form['og_register'] = array(
        '#type' => 'radios',
        '#title' => t('Registration form'),
        '#options' => array(0 => t('No'), 1 => t('Yes')),
      );
      $visibility = variable_get('og_visibility_registration', OG_REGISTRATION_CHOOSE_FALSE);
      if (user_access('administer nodes')) {
        $visibility = in_array($visibility, array(OG_REGISTRATION_NEVER, OG_REGISTRATION_CHOOSE_FALSE)) ? OG_REGISTRATION_CHOOSE_FALSE  : OG_REGISTRATION_CHOOSE_TRUE;
      }
      $default = 0;
      switch ($visibility) {
        case OG_REGISTRATION_ALWAYS:
          $default = 1;
          // fall through
        case OG_REGISTRATION_NEVER:
          $form['og_register']['#access'] = FALSE;
          break;

        case OG_REGISTRATION_CHOOSE_TRUE:
          $default = 1;
          // fall through
        case OG_REGISTRATION_CHOOSE_FALSE:
          break;
      }
      $form['og_register']['#default_value'] = isset($defaults['og_register']) ? $defaults['og_register'] : $default;

      $form['og_directory'] = array(
        '#type' => 'radios',
        '#title' => t('List in groups directory'),
        '#options' => array(0 => t('No'), 1 => t('Yes')),
      );
      $visibility = variable_get('og_visibility_directory', OG_DIRECTORY_CHOOSE_TRUE);
      if (user_access('administer nodes')) {
        $visibility = in_array($visibility, array(OG_DIRECTORY_NEVER, OG_DIRECTORY_CHOOSE_FALSE)) ? OG_DIRECTORY_CHOOSE_FALSE  : OG_DIRECTORY_CHOOSE_TRUE;
      }
      $default = 0;
      switch ($visibility) {
        case OG_DIRECTORY_ALWAYS:
          $default = 1;
          // fall through
        case OG_DIRECTORY_NEVER:
          $form['og_directory']['#access'] = FALSE;
          break;

        case OG_DIRECTORY_CHOOSE_TRUE:
          $default = 1;
          // fall through
        case OG_DIRECTORY_CHOOSE_FALSE:
          break;
      }
      $form['og_directory']['#default_value'] = isset($defaults['og_directory']) ? $defaults['og_directory'] : $default;
    }

    if (og_is_group_post_type($node_type)) {
      $groups = _node_import_og_groups();
      if (!empty($groups)) {
        $form['og_groups'] = array(
          '#title' => t('Audience'),
          '#type' => count($groups, COUNT_RECURSIVE) > 20 ? 'select' : 'checkboxes',
          '#options' => $groups,
          '#multiple' => TRUE,
          '#default_value' => isset($defaults['og_groups']) ? $defaults['og_groups'] : array(),
        );
      }
    }
  }

  return $form;
}

