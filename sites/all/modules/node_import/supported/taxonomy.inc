<?php

/**
 * @file
 * Support file for the core taxonomy module.
 */

/**
 * Implementation of hook_node_import_types().
 */
function taxonomy_node_import_types() {
  $types = array();

  $vocabularies = taxonomy_get_vocabularies();

  // Import taxonomy vocabularies.
  $types['vocabulary'] = array(
    'title' => t('Vocabularies'),
    'can_create' => user_access('administer taxonomy'),
    'create' => 'node_import_create_taxonomy',
  );

  // Import taxonomy terms.
  foreach ((array)$vocabularies as $vid => $vocabulary) {
    $types['term:'. $vid] = array(
      'title' => t('Terms of %name vocabulary', array('%name' => $vocabulary->name)),
      'vocabulary' => $vocabulary,
      'can_create' => user_access('administer taxonomy'),
      'create' => 'node_import_create_taxonomy',
    );
  }

  return $types;
}

/**
 * Create a new vocabulary or term by submitting the corresponding
 * form.
 */
function node_import_create_taxonomy($type, $values, $preview) {
  $output = $preview ? '' : FALSE;

  if ($preview) {
    if ($type == 'vocabulary') {
      $node_types = array();
      foreach ($values['nodes'] as $node_type => $enabled) {
        if ($enabled) {
          $node_types[] = check_plain(node_get_types('name', $node_type));
        }
      }

      $output .= '<div class="preview">';
      $output .= '<h2>'. check_plain($values['name']) .'</h2>';
      $output .= '<div>'. filter_xss_admin($values['description']) .'</div>';
      $output .= '<dl>';
      $output .= '<dt>'. t('Help text') .'</dt>';
      $output .= '<dd>'. check_plain($values['help']) .'</dd>';
      $output .= '<dt>'. t('Content types') .'</dt>';
      $output .= '<dd>'. implode('</dd><dd>', $node_types) .'</dd>';
      $output .= '<dt>'. t('Settings') .'</dt>';

      $settings = array();
      if ($values['tags']) $settings[] = t('Tags');
      if ($values['multiple']) $settings[] = t('Multiple select');
      if ($values['required']) $settings[] = t('Required');
      if ($values['hierarchy'] == 1) $settings[] = t('Single hierarchy');
      if ($values['hierarchy'] == 2) $settings[] = t('Multiple hierarchy');

      $output .= '<dd>'. implode('</dd><dd>', $settings) .'</dd>';
      $output .= '<dt>'. t('Weight') .'</dt>';
      $output .= '<dd>'. check_plain($values['weight']) .'</dd>';
      $output .= '</dl>';
      $output .= '</div>';
    }
    else {
      $output .= '<div class="preview">';
      $output .= '<h2>'. check_plain($values['name']) .'</h2>';
      $output .= '<div>'. filter_xss_admin($values['description']) .'</div>';

      if (!empty($values['parent'])) {
        $output .= '<p><strong>'. t('Parent:') .'</strong> ';
        $output .= node_import_display_taxonomy_term($values['parent']) .'</p>';
      }

      if (!empty($values['relations']) || !empty($values['synonyms'])) {
        $output .= '<dl>';
        if (!empty($values['relations'])) {
          $output .= '<dt>'. t('Related terms:') .'</dt> ';
          $output .= '<dd>'. implode('</dd><dd>', array_map('node_import_display_taxonomy_term', (array)$values['relations'])) .'</dd>';
        }

        if (!empty($values['synonyms'])) {
          $output .= '<dt>'. t('Synonyms:') .'</dt> ';
          $output .= '<dd>'. implode('</dd><dd>', array_map('check_plain', (array)$values['synonyms'])) .'</dd>';
        }
        $output .= '</dl>';
      }

      $output .= '</div>';

      // If a term is created and a later term refers to it (as relation
      // or parent), we get an error in preview. In order to avoid this
      // we manually add the (will-be-created) term to the checking
      // function.
      $types = node_import_types();
      $vocabulary = $types[$type]['vocabulary'];
      $field = array('vocabulary' => $vocabulary);

      node_import_check_taxonomy_term($values['name'], $field, array(), $preview, 'add');
    }
  }
  else {
    module_load_include('inc', 'taxonomy', 'taxonomy.admin');

    $values['op'] = t('Save');
    $form_state = array(
      'values' => $values,
    );

    if ($type == 'vocabulary') {
      node_import_drupal_execute('taxonomy_form_vocabulary', $form_state);
      $output = $form_state['vid'];
    }
    else {
      $types = node_import_types();
      $vocabulary = $types[$type]['vocabulary'];

      node_import_drupal_execute('taxonomy_form_term', $form_state, $vocabulary);
      $output = $form_state['tid'];

      // As a term can refer to other terms (parent, relation), we
      // can not continue if we have created a term.
      global $node_import_can_continue;
      $node_import_can_continue = FALSE;
    }
  }

  return $output;
}

/**
 * Display a taxonomy term during preview.
 */
function node_import_display_taxonomy_term($tid) {
  static $output;

  if (!isset($output)) {
    $output = array();
  }

  if (!isset($output[$tid])) {
    if ($tid === 0) {
      $output[$tid] = theme('placeholder', t('The term will be created during import'));
    }
    else if (($term = taxonomy_get_term($tid))) {
      $output[$tid] = l($term->name, 'taxonomy/term/'. $tid);
    }
    else {
      $output[$tid] = theme('placeholder', $tid);
    }
  }

  return $output[$tid];
}

/**
 * Implementation of hook_node_import_fields().
 */
function taxonomy_node_import_fields($type) {
  $fields = array();

  // Import taxonomy vocabularies.
  if ($type == 'vocabulary') {
    $fields['name'] = array(
      'title' => t('Vocabulary name'),
      'group' => t('Identification'),
      'module' => 'taxonomy',
      'map_required' => TRUE,
      'default_value' => '',
    );
    $fields['description'] = array(
      'title' => t('Description'),
      'group' => t('Identification'),
      'module' => 'taxonomy',
      'default_value' => '',
    );
    $fields['help'] = array(
      'title' => t('Help text'),
      'group' => t('Identification'),
      'module' => 'taxonomy',
      'default_value' => '',
    );

    $fields['nodes'] = array(
      'title' => t('Content types'),
      'group' => t('Content types'),
      'module' => 'taxonomy',
      'has_multiple' => TRUE,
      'is_checkboxes' => TRUE,
      'allowed_values' => node_get_types('names'),
    );

    $fields['tags'] = array(
      'title' => t('Tags'),
      'group' => t('Settings'),
      'module' => 'taxonomy',
      'input_format' => 'boolean',
    );
    $fields['multiple'] = array(
      'title' => t('Multiple select'),
      'group' => t('Settings'),
      'module' => 'taxonomy',
      'input_format' => 'boolean',
    );
    $fields['required'] = array(
      'title' => t('Required'),
      'group' => t('Settings'),
      'module' => 'taxonomy',
      'input_format' => 'boolean',
    );

    $fields['weight'] = array(
      'title' => t('Weight'),
      'group' => t('Settings'),
      'module' => 'taxonomy',
      'input_format' => 'weight',
    );

    $fields['hierarchy'] = array(
      'title' => t('Hierarchy'),
      'group' => t('Advanced settings'),
      'module' => 'taxonomy',
      'allowed_values' => array(
        '0' => t('Disabled'),
        '1' => t('Single'),
        '2' => t('Multiple'),
      ),
    );
  }

  // Import taxonomy terms.
  else if (strpos($type, 'term:') === 0) {
    $types = node_import_types();
    $vocab = $types[$type]['vocabulary'];

    $fields['vid'] = array(
      'is_mappable' => FALSE,
      'default_value' => $vocab->vid,
    );

    $fields['name'] = array(
      'title' => t('Term name'),
      'group' => t('Identification'),
      'module' => 'taxonomy',
      'map_required' => TRUE,
      'default_value' => '',
    );
    $fields['description'] = array(
      'title' => t('Description'),
      'group' => t('Identification'),
      'module' => 'taxonomy',
      'default_value' => '',
    );

    $fields['parent'] = array(
      'title' => t('Parents'),
      'group' => t('Advanced options'),
      'module' => 'taxonomy',
      'has_multiple' => FALSE, // Although all vocabs in Drupal6.x allow for multiple parents, we only allow one.
      'has_hierarchy' => TRUE,
      'input_format' => 'taxonomy_term',
      'vocabulary' => $vocab,
    );
    $fields['relations'] = array(
      'title' => t('Related terms'),
      'group' => t('Advanced options'),
      'module' => 'taxonomy',
      'has_multiple' => TRUE,
      'has_hierarchy' => TRUE,
      'input_format' => 'taxonomy_term',
      'vocabulary' => $vocab,
    );
    $fields['synonyms'] = array(
      'title' => t('Synonyms'),
      'group' => t('Advanced options'),
      'module' => 'taxonomy',
      'has_multiple' => TRUE,
      'multiple_separator' => ',',
    );

    $fields['weight'] = array(
      'title' => t('Weight'),
      'group' => t('Advanced options'),
      'module' => 'taxonomy',
      'input_format' => 'weight',
    );
  }

  // Import taxonomy terms for nodes.
  else if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    // Copy from taxonomy_form_alter().
    $c = db_query(db_rewrite_sql("SELECT v.* FROM {vocabulary} v INNER JOIN {vocabulary_node_types} n ON v.vid = n.vid WHERE n.type = '%s' ORDER BY v.weight, v.name", 'v', 'vid'), $node_type);

    while ($vocabulary = db_fetch_object($c)) {
      $fields['taxonomy:'. $vocabulary->vid] = array(
        'title' => $vocabulary->name,
        'group' => t('Vocabularies'),
        'module' => 'taxonomy',
        'input_format' => 'taxonomy_term',
        'vocabulary' => $vocabulary,
        'has_multiple' => $vocabulary->multiple || $vocabulary->tags,
        'has_hierarchy' => $vocabulary->hierarchy > 0,
        'multiple_separator' => $vocabulary->tags ? ',' : '||',
      );
    }
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_fields_alter().
 */
function taxonomy_node_import_fields_alter(&$fields, $type) {
  foreach ($fields as $fieldname => $fieldinfo) {
    if ($fieldinfo['input_format'] == 'taxonomy_term') {
      $vocab = $fieldinfo['vocabulary'];
      if (!$vocab->tags) {
        $fields[$fieldname]['preprocess'][] = 'node_import_check_taxonomy_term';
      }
      $fields[$fieldname]['tips'][] = t('Taxonomy term (by tid, name or synonym).');
    }
  }
}

/**
 * Check if the value is a valid taxonomy term (by tid or name).
 *
 * Uses: $field['vocabulary'].
 *
 * @ingroup node_import_preprocess
 */
function node_import_check_taxonomy_term(&$value, $field, $options, $preview, $op = 'lookup') {
  static $tids;

  $vocab = $field['vocabulary'];

  if ($vocab->tags) {
    // No need to check when tags vocabulary.
    return TRUE;
  }

  // We cache the terms already looked up.
  if (!isset($tids)) {
    $tids = array();
  }
  if (!isset($tids[$vocab->vid])) {
    $tids[$vocab->vid] = array();
  }
  $store_value = is_array($value) ? implode("\n", array_map('drupal_strtolower', $value)) : drupal_strtolower($value);

  // Special case for previews: signify that the term will be created.
  if ($op == 'add') {
    $tids[$vocab->vid][$store_value] = 0;
    return;
  }

  if (isset($tids[$vocab->vid][$store_value])) {
    // We have looked up this value already.
    $value = $tids[$vocab->vid][$store_value];
  }
  else if (!is_array($value)) {
    // One term specified, look up the tid or give error.
    if (($tid = db_result(db_query("SELECT tid FROM {term_data} WHERE vid = %d AND (tid = %d OR LOWER(name) = '%s')", $vocab->vid, (is_numeric($store_value) && intval($store_value) > 0) ? $store_value : -1, $store_value))) ||
        ($tid = db_result(db_query("SELECT td.tid FROM {term_synonym} AS ts, {term_data} AS td WHERE td.tid = ts.tid AND td.vid = %d AND LOWER(ts.name) = '%s'", $vocab->vid, $store_value)))) {
      $value = $tid;
    }
    else {
      $value = '';
    }
  }
  else if (count($value) == 1) {
    // The user has specified the term as a hierarchy trail, but he
    // only specified one parent. Lookup that value.
    $value = array_shift($value);
    node_import_check_taxonomy_term($value, $field, $options, $preview);
  }
  else {
    // The user has specified the term as a hierarchy trail of terms.
    // Example: array(grandparent, parent, child). We need to find
    // the tid of the child.
    //
    // Pop child.
    // Find a parent_tid for array(grandparent, parent).
    // Find a tid for child that has a parent with parent_tid.
    $child = array_pop($value);
    $child = drupal_strtolower($child);
    node_import_check_taxonomy_term($value, $field, $options, $preview);

    if ($value === 0 || $value === '') {
      // We can't look for a proper child as we did not find a proper
      // parent (or the parent will be created). So stop.
    }
    else if (($tid = db_result(db_query("SELECT td.tid FROM {term_data} td, {term_hierarchy} th WHERE td.vid = %d AND th.parent = %d AND td.tid = th.tid AND (td.tid = %d OR LOWER(td.name) = '%s')", $vocab->vid, $value, (is_numeric($child) && intval($child) > 0) ? $child : -1, $child))) ||
             ($tid = db_result(db_query("SELECT td.tid FROM {term_data} td, {term_hierarchy} th, {term_synonym} ts WHERE td.vid = %d AND th.parent = %d AND td.tid = th.tid AND td.tid = ts.tid AND LOWER(ts.name) = '%s'", $vocab->vid, $value, $child)))) {
      $value = $tid;
    }
    else {
      $value = '';
    }
  }

  $tids[$vocab->vid][$store_value] = $value;

  // Report error.
  if ($value === 0) {
    drupal_set_message(t('The term %value will be added to %vocabulary during import and will be used for %name.', array('%value' => $value, '%name' => $field['title'], '%vocabulary' => $vocab->name)));
  }
  else if ($value === '') {
    node_import_input_error(t('Input error: %value is not allowed for %name (not a term or synonym in %vocabulary).', array('%value' => $value, '%name' => $field['title'], '%vocabulary' => $vocab->name)));
    return FALSE;
  }

  return TRUE;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function taxonomy_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  // Import taxonomy vocabularies.
  if ($type == 'vocabulary') {
    $form['nodes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Content types'),
      '#default_value' => isset($defaults['nodes']) ? $defaults['nodes'] : array(),
      '#options' => array_map('check_plain', node_get_types('names')),
    );

    $options = array(
      'tags' => t('Tags'),
      'multiple' => t('Multiple select'),
      'required' => t('Required'),
    );
    foreach ($options as $key => $title) {
      $form[$key] = array(
        '#title' => $title,
        '#type' => 'radios',
        '#options' => array('0' => t('No'), '1' => t('Yes')),
        '#default_value' => isset($defaults[$key]) ? $defaults[$key] : '0',
      );
    }

    $form['weight'] = array(
      '#title' => t('Weight'),
      '#type' => 'weight',
      '#default_value' => isset($defaults['weight']) ? $defaults['weight'] : 0,
    );

    $form['hierarchy'] = array(
      '#title' => t('Hierarchy'),
      '#type' => 'radios',
      '#options' => array('0' => t('Disabled'), '1' => t('Single'), '2' => t('Multiple')),
      '#default_value' => isset($defaults['hierarchy']) ? $defaults['hierarchy'] : '0',
    );
  }

  // Import taxonomy terms.
  else if (strpos($type, 'term:') === 0) {
    $types = node_import_types();
    $vocab = $types[$type]['vocabulary'];

    $form['vocabulary'] = array(
      '#type' => 'item',
      '#title' => t('Vocabulary'),
      '#value' => check_plain($vocab->name),
    );

    $options = array();
    foreach (taxonomy_get_tree($vocab->vid) as $term) {
      $choice = new stdClass();
      $choice->option = array($term->tid => str_repeat('-', $term->depth). $term->name);
      $options[] = $choice;
    }
    if ($vocab->hierarchy > 0) {
      $form['parent'] = array(
        '#type' => 'select',
        '#title' => t('Parents'),
        '#multiple' => $vocab->hierarchy == 2,
        '#options' => array_merge(array('' => '<'. t('root') .'>'), $options),
        '#default_value' => isset($defaults['parent']) ? $defaults['parent'] : '',
      );
    }
    $form['relations'] = array(
      '#type' => 'select',
      '#title' => t('Related terms'),
      '#multiple' => TRUE,
      '#options' => array_merge(array('' => '<'. t('none') .'>'), $options),
      '#default_value' => isset($defaults['relations']) ? $defaults['relations'] : '',
    );
    $form['synonyms'] = array(
      '#type' => 'textarea',
      '#title' => t('Synonyms'),
      '#default_value' => isset($defaults['synonyms']) ? $defaults['synonyms'] : '',
    );
    $form['weight'] = array(
      '#title' => t('Weight'),
      '#type' => 'textfield',
      '#size' => 6,
      '#default_value' => isset($defaults['weight']) ? $defaults['weight'] : 0,
    );
  }

  // Import taxonomy terms for nodes.
  else if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    // Copy from taxonomy_form_alter().
    $c = db_query(db_rewrite_sql("SELECT v.* FROM {vocabulary} v INNER JOIN {vocabulary_node_types} n ON v.vid = n.vid WHERE n.type = '%s' ORDER BY v.weight, v.name", 'v', 'vid'), $node_type);

    while (($vocabulary = db_fetch_object($c))) {
      if ($vocabulary->tags) {
        $form['taxonomy:'. $vocabulary->vid] = array(
          '#type' => 'textfield',
          '#title' => $vocabulary->name,
          '#default_value' => isset($defaults['taxonomy:'. $vocabulary->vid]) ? $defaults['taxonomy:'. $vocabulary->vid] : '',
          '#autocomplete_path' => 'taxonomy/autocomplete/'. $vocabulary->vid,
        );
      }
      else {
        $form['taxonomy:'. $vocabulary->vid] = taxonomy_form($vocabulary->vid, isset($defaults['taxonomy:'. $vocabulary->vid])? $defaults['taxonomy:'. $vocabulary->vid] : 0);
      }
    }
  }

  return $form;
}

/**
 * Implementation of hook_node_import_options().
 */
function taxonomy_node_import_options($type, $options, $fields, $map) {
  $form = array();
  return $form; //TODO: reenable this.

  // Import taxonomy terms for nodes.
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    // Copy from taxonomy_form_alter().
    $c = db_query(db_rewrite_sql("SELECT v.* FROM {vocabulary} v INNER JOIN {vocabulary_node_types} n ON v.vid = n.vid WHERE n.type = '%s' ORDER BY v.weight, v.name", 'v', 'vid'), $node_type);

    $create_options = array(
      'create' => t('create it') .',',
      'remove' => t('remove it from the selection') .',',
      'error' => t('treat it as an import error') .'.',
    );
    if (user_access('administer taxonomy')) {
      $create_default = 'create';
    }
    else {
      unset($create_options['create']);
      $create_default = 'error';
    }

    while (($vocabulary = db_fetch_object($c))) {
      $field = 'taxonomy:'. $vocabulary->vid;
      if (isset($map[$field]) && !empty($map[$field])) {
        if (!$vocabulary->tags) {
          $form[$field] = array(
            '#title' => $vocabulary->name,

            'non_existing_terms' => array(
              '#title' => t('When a term does not exist'),
              '#type' => 'radios',
              '#options' => $create_options,
              '#default_value' => isset($options[$field]['non_existing_terms']) ? $options[$field]['non_existing_terms'] : $create_default,
            ),
          );
        }
      }
    }
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function taxonomy_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  if (strpos($type, 'term:') === 0) {
    if (!$preview) {
      $values['synonyms'] = implode("\n", (array)$values['synonyms']);
      $values['parent'] = array($values['parent']);
    }
  }
  else if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $taxonomy = isset($values['taxonomy']) ? $values['taxonomy'] : array();
    foreach ($fields as $fieldname => $fieldinfo) {
      if (strpos($fieldname, 'taxonomy:') === 0) {
        $vocab = $fieldinfo['vocabulary'];
        if ($vocab->tags) {
          $taxonomy['tags'] = isset($taxonomy['tags']) ? $taxonomy['tags'] : array();
          $taxonomy['tags'][$vocab->vid] = implode(',', (array)$values[$fieldname]);
        }
        else {
          $taxonomy[$vocab->vid] = $values[$fieldname];
        }
        unset($values[$fieldname]);
      }
    }
    $values['taxonomy'] = $taxonomy;
  }
}

