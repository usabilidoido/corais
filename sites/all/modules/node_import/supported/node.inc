<?php
/**
 * @file
 * Support file for the core node module.
 */

/**
 * Implementation of hook_node_import_types().
 */
function node_node_import_types() {
  $types = array();

  foreach (node_get_types('names') as $type => $name) {
    $types['node:'. $type] = array(
      'title' => t('%name content type', array('%name' => $name)),
      'can_create' => node_access('create', $type),
      'create' => 'node_import_create_node',
    );
  }

  return $types;
}

/**
 * Check if the node_import $type is a node content type.
 *
 * @param $type
 *   String. Node_import $type.
 *
 * @return
 *   FALSE if $type is not a node content type. Otherwise
 *   the node content type.
 */
function node_import_type_is_node($type) {
  if (strpos($type, 'node:') === 0) {
    return substr($type, strlen('node:'));
  }
  return FALSE;
}

/**
 * Create a new node by submitting $type_node_form.
 */
function node_import_create_node($type, &$values, $preview) {
  $output = $preview ? '' : FALSE;

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    module_load_include('inc', 'node', 'node.pages');

    $form_id = $node_type .'_node_form';

    $values['op'] = $preview ? t('Preview') : t('Save');
    $form_state = array(
      'values' => $values,
    );

    $node = (object)array(
      'type' => $node_type,
      'name' => isset($values['name']) ? $values['name'] : '',
    );
    node_import_drupal_execute($form_id, $form_state, $node);

    if ($preview) {
      $output = isset($form_state['node_preview']) ? $form_state['node_preview'] : '';
    }
    else {
      $output = $values['nid'] = $form_state['nid'];
    }
  }

  return $output;
}

/**
 * Implementation of hook_node_import_fields().
 */
function node_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    global $user;

    $type_info = node_get_types('type', $node_type);
    $administer_nodes = user_access('administer nodes');
    $node_options = variable_get('node_options_'. $node_type, array('status', 'promote'));

    $fields['type'] = array(
      'title' => t('Content type'),
      'module' => 'node',
      'is_mappable' => FALSE,
      'default_value' => $node_type,
    );

    if ($type_info->has_title) {
      $fields['title'] = array(
        'title' => $type_info->title_label,
        'module' => 'node',
        'weight' => -5,
        'map_required' => TRUE,
        'default_value' => '',
      );
    }

    if ($type_info->has_body) {
      $fields['body'] = array(
        'title' => $type_info->body_label,
        'group' => $type_info->body_label,
        'module' => 'node',
        'map_required' => ($type_info->min_word_count > 0),
        'default_value' => '',
      );

      $fields['format'] = array(
        'title' => t('Input format'),
        'group' => $type_info->body_label,
        'module' => 'node',
        'allowed_values' => node_import_extract_property(filter_formats(), 'name'),
        'default_value' => filter_resolve_format(FILTER_FORMAT_DEFAULT),
      );
    }

    $fields['uid'] = array(
      'title' => t('Authored by'),
      'group' => t('Authoring information'),
      'module' => 'node',
      'weight' => 20,
      'is_mappable' => $administer_nodes,
      'input_format' => 'user_reference',
      'default_value' => $user->uid,
    );
    $fields['created'] = array(
      'title' => t('Authored on'),
      'group' => t('Authoring information'),
      'module' => 'node',
      'weight' => 20,
      'is_mappable' => $administer_nodes,
      'input_format' => 'date',
      'output_format' => DATE_UNIX,
    );

    $fields['log'] = array(
      'title' => t('Log message'),
      'group' => t('Revision information'),
      'module' => 'node',
      'weight' => 20,
      'is_mappable' => FALSE,
      'default_value' => '',
    );
    $fields['revision'] = array(
      'title' => t('Create new revision'),
      'group' => t('Revision information'),
      'module' => 'node',
      'weight' => 20,
      'is_mappable' => FALSE,
      'default_value' => in_array('revision', $node_options) ? '1' : '0',
    );

    $workflow = array(
      'status' => t('Published'),
      'promote' => t('Promoted to front page'),
      'sticky' => t('Sticky at top of lists'),
    );
    foreach ($workflow as $key => $title) {
      $fields[$key] = array(
        'title' => $title,
        'group' => t('Publishing options'),
        'module' => 'node',
        'weight' => 25,
        'is_mappable' => $administer_nodes,
        'input_format' => 'boolean',
        'default_value' => in_array($key, $node_options) ? '1' : '0',
      );
    }
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function node_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    global $user;

    $type_info = node_get_types('type', $node_type);

    if ($type_info->has_body) {
      $form['format'] = array(
        '#title' => t('Input format'),
        '#type' => 'radios',
        '#options' => node_import_extract_property(filter_formats(), 'name'),
        '#default_value' => isset($defaults['format']) ? $defaults['format'] : filter_resolve_format(FILTER_FORMAT_DEFAULT),
      );
    }

    if (user_access('administer nodes')) {
      $form['uid'] = array(
        '#title' => t('Authored by'),
        '#type' => 'textfield',
        '#maxlength' => 60,
        '#autocomplete_path' => 'user/autocomplete',
        '#default_value' => isset($defaults['uid']) ? $defaults['uid'] : $user->name,
      );
      $form['created'] = array(
        '#title' => t('Authored on'),
        '#type' => 'textfield',
        '#maxlength' => 25,
      );

      $form['log'] = array(
        '#title' => t('Log message'),
        '#type' => 'textarea',
        '#rows' => 2,
        '#default_value' => isset($defaults['log']) ? $defaults['log'] : t('Imported with node_import.'),
      );

      $workflow = array(
        'status' => t('Published'),
        'promote' => t('Promoted to front page'),
        'sticky' => t('Sticky at top of lists'),
      );
      $node_options = variable_get('node_options_'. $node_type, array('status', 'promote'));
      foreach ($workflow as $key => $title) {
        $form[$key] = array(
          '#title' => $title,
          '#type' => 'radios',
          '#options' => array(0 => t('No'), 1 => t('Yes')),
          '#default_value' => isset($defaults[$key]) ? $defaults[$key] : (in_array($key, $node_options) ? 1 : 0),
        );
      }
    }
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values().
 */
function node_node_import_values($type, $defaults, $options, $fields, $preview) {
  $values = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $values = array(
      'created' => time(),
    );
  }

  return $values;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function node_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    if (isset($values['uid']) && drupal_strlen($values['uid']) > 0) {
      if (($name = node_import_get_object('user:name', $values['uid'])) ||
          ($name = db_result(db_query("SELECT name FROM {users} WHERE uid = %d", $values['uid'])))) {
        $values['name'] = $name;
        node_import_set_object('user:name', $values['uid'], $name);
      }
    }
    else {
      $values['name'] = '';
    }

    if (isset($values['created']) && drupal_strlen($values['created']) > 0) {
      $values['date'] = format_date($values['created'], 'custom', 'Y-m-d H:i:s O');
    }
  }
}

