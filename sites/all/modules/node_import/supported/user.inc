<?php

/**
 * @file
 * Support file for the core user module.
 */

/**
 * Implementation of hook_node_import_types().
 */
function user_node_import_types() {
  return array(
    'user' => array(
      'title' => t('Users'),
      'can_create' => user_access('administer users'),
      'create' => 'node_import_create_user',
    )
  );
}

/**
 * Create a new user.
 */
function node_import_create_user($type, $values, $preview) {
  $output = $preview ? '' : FALSE;

  $fields = node_import_fields($type);

  if ($preview) {
    $form = array(); //Not needed - TODO: we should use $form['#validate']
    $form_state = array(
      'values' => $values,
    );
    user_register_validate($form, $form_state);

    $output = '<div class="preview">';
    $output .= '<h3>' . check_plain($values['name']) . "</h3>\n";
    $output .= "<ul>\n";
    $output .= '<li>' . t('Email') . ': ' . check_plain($values['mail']) . "</li>\n";

    foreach ( $fields as $fname => $field ) {
      // ignore the standard fields
      switch ( $fname ) {
        case 'name':
        case 'mail':
        case 'pass':
        case 'status':
        case 'notify':
          continue(2);
      }

      $group = '';
      if (isset($field['group'])) {
        $group = check_plain($field['group']) . ' - ';
      }

      if (isset($values[$fname])) {
        $output .= '<li>' . $group . check_plain($field['title']) . ': ' . check_plain($values[$fname]) . "</li>\n";
      }

    }

    $output .= "</ul>\n";
    $output .= '</div>';
  }
  else {
    $form_id = 'user_register';

    $values['op'] = t('Save');
    $form_state = array(
      'values' => $values,
    );

    node_import_drupal_execute($form_id, $form_state);
    $output = $form_state['user']->uid;

    // Save user profile.
    $user = $form_state['user'];
    $profile = array();

    foreach ($fields as $fname => $field) {
      switch ($fname) {
        case 'name':
        case 'mail':
        case 'pass':
        case 'status':
        case 'notify':
          continue(2);
      }

      if (!isset($profile[$field['group']])) {
        $profile[$field['group']] = array();
      }

      $profile[$field['group']][$fname] = $values[$fname];
    }

    foreach ($profile as $group => $values) {
      user_save($user, $values, $group);
    }

    $output = $uid;
  }

  return $output;
}

/**
 * Implementation of hook_node_import_fields().
 */
function user_node_import_fields($type) {
  $fields = array();

  if ($type === 'user') {
    $fields['name'] = array(
      'title' => t('Username'),
      'module' => 'user',
      'map_required' => TRUE,
    );

    $fields['mail'] = array(
      'title' => t('E-mail address'),
      'module' => 'user',
      'input_format' => 'email',
      'map_required' => TRUE,
    );

    $fields['pass'] = array(
      'title' => t('Password'),
      'module' => 'user',
    );

    $fields['status'] = array(
      'title' => t('Status'),
      'module' => 'user',
      'allowed_values' => array('1' => t('Active'), '0' => t('Blocked')),
    );

    $fields['notify'] = array(
      'title' => t('Notify user of new account'),
      'module' => 'user',
      'is_mappable' => FALSE,
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_options().
 */
function user_node_import_options($type, $options, $fields, $map) {
  $form = array();

  if ($type === 'user') {
    $options = array(
      'never' => t('Treat records without password as an error'),
      'always' => t('Generate password for each record (even if set in the file)'),
      'empty' => t('Generate password for records with empty password'),
    );
    $default = isset($map['pass']) && strlen($map['pass']) > 0 ? 'empty' : 'never';

    $form['generate_pass'] = array(
      '#type' => 'radios',
      '#title' => t('Password'),
      '#options' => $options,
      '#default_value' => isset($options['generate_pass']) ? $options['generate_pass'] : $default,
    );
  }

  return $form;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function user_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if ($type == 'user') {
    $form['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#options' => array('0' => t('Blocked'), '1' => t('Active')),
      '#default_value' => isset($defaults['status']) ? $defaults['status'] : '1',
    );

    $form['notify'] = array(
      '#type' => 'checkbox',
      '#title' => t('Notify user of new account'),
      '#default_value' => isset($defaults['notify']) ? $defaults['notify'] : '0',
    );
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function user_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  if ($type === 'user') {
    if ($options['generate_pass'] === 'always' || ($options['generate_pass'] === 'empty' && strlen($values['pass']) == 0)) {
      $values['pass'] = user_password();
    }

    if (!$preview) {
      $values['pass'] = array(
        'pass1' => $values['pass'],
        'pass2' => $values['pass'],
      );
    }
  }
}

