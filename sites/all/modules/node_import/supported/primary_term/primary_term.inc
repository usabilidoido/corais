<?php

/**
 * @file
 * Support file for the primary term module.
 *
 * From http://drupal.org/node/565430 :
 * #1
 * mvc - September 1, 2009 - 18:41
 * I forgot to note that exporting primary terms correctly into CSV in the first
 * place requires this patch to the primary term module: #160382: Views field
 * should show only one term
 */

/**
 * Display a taxonomy term during preview.
 */
function node_import_display_primary_term($tid) {
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
function primary_term_node_import_fields($type) {
  $fields = array();

  // Import taxonomy terms for nodes.
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $vids = variable_get('pt_vocabs_'. $node_type, array());

    foreach ($vids as $vid) {
      $vocabulary = taxonomy_vocabulary_load($vid);
      $fields['primary_term:'. $vid] = array(
        'title' => t('Primary Term'),
        'module' => 'primary_term',
        'input_format' => 'taxonomy_term',
        'vocabulary' => $vocabulary,
        'has_multiple' => FALSE,
        'has_hierarchy' => FALSE,
      );
    }
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function primary_term_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  // Import taxonomy terms for nodes.
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $vids = variable_get('pt_vocabs_'. $node_type, array());
    foreach ($vids as $vid) {
      $vocabulary = taxonomy_vocabulary_load($vid);
      if ($vocabulary->tags) {
        $form['primary_term:'. $vocabulary->vid] = array(
          '#type' => 'textfield',
          '#title' => $vocabulary->name,
          '#default_value' => isset($defaults['primary_term:'. $vocabulary->vid]) ? $defaults['primary_term:'. $vocabulary->vid] : '',
          '#autocomplete_path' => 'taxonomy/autocomplete/'. $vocabulary->vid,
        );
      }
      else {
        $form['primary_term:'. $vocabulary->vid] = taxonomy_form($vocabulary->vid, isset($defaults['primary_term:'. $vocabulary->vid])? $defaults['primary_term:'. $vocabulary->vid] : 0);
      }
    }
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function primary_term_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $taxonomy = isset($values['primary_term']) ? $values['primary_term'] : array();
    foreach ($fields as $fieldname => $fieldinfo) {
      if (strpos($fieldname, 'primary_term:') === 0) {
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
    /*
    the primary term module uses node->primary_term and node->primaryterm for
    different purposes, mostly for historical reasons, as changing this would
    break compatibility with every contributed module that uses this field
    (cf. comments in primary_term.module)
    */
    $values['primary_term'] = $taxonomy;
    $tid = array_shift($taxonomy);
    $values['primaryterm'] = $tid;
  }
}