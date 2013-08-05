<?php

/**
 * @file
 * Support file for the core profile module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function profile_node_import_fields($type) {
  $fields = array();

  if ($type == 'user') {
    $results = db_query("SELECT * FROM {profile_fields} ORDER BY category, weight, title");

    while (($field = db_fetch_object($results))) {
      $fields[$field->name] = array(
        'title' => check_plain($field->title),
        'group' => check_plain($field->category),
        'map_required' => $field->required,
      );

      switch ($field->type) {
        case 'selection':
          $options = $field->required ? array() : array('--');
          $lines = split("[,\n\r]", $field->options);
          foreach ($lines as $line) {
            if (($line = trim($line))) {
              $options[$line] = $line;
            }
          }
          $fields[$field->name]['allowed_values'] = $options;
          break;

        case 'date':
          $fields[$field->name]['input_format'] = 'date';
          break;

        case 'checkbox':
        case 'textfield':
        case 'url':
        case 'textarea':
        case 'list':
          break;

        default:
          $fields[$field->name]['title'] = t('Unsupported: ') . $fields[$field->name]['title'];
          break;
      }
    }
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function profile_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if ($type == 'user') {
    foreach ((array)profile_categories() as $category) {
      $element = profile_form_profile(array(), NULL, $category['name']);
      foreach (element_children($element[$category['name']]) as $child) {
        $form[$child] = $element[$category['name']][$child];
        if (isset($defaults[$child])) {
          $form[$child]['#default_value'] = $defaults[$child];
        }
        $form[$child]['#required'] = FALSE;
      }
    }
  }

  return $form;
}

