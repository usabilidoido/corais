<?php
/**
 * @file
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 */

/**
 * Implementation of hook_ckeditor_link_TYPE_autocomplete().
 */
function ckeditor_link_ckeditor_link_taxonomy_autocomplete($string, $limit) {
  $matches = array();

  $vocabularies = array_keys(array_filter(variable_get('ckeditor_link_autocomplete_vocabularies', array())));
  if (count($vocabularies)) {
    $sql = "SELECT t.tid, t.name FROM {term_data} t WHERE t.name LIKE '%%%s%%'";
    $args = array($string);
    if (!in_array('- any -', $vocabularies)) {
      $sql .= ' AND t.vid IN ('. db_placeholders($vocabularies) .')';
      $args = array_merge($args, $vocabularies);
    }
    $sql .= ' ORDER BY t.name';
    $result = db_query_range(db_rewrite_sql($sql, 't', 'tid'), $args, 0, $limit);
    while ($term = db_fetch_object($result)) {
      $matches['taxonomy/term/'. $term->tid] = $term->name;
    }
  }

  return $matches;
}

/**
 * Implementation of hook_ckeditor_link_TYPE_revert().
 */
function ckeditor_link_ckeditor_link_taxonomy_revert($path, &$langcode) {
  if (function_exists('ckeditor_link_ckeditor_link_i18ntaxonomy_revert')
   || !preg_match('`^taxonomy/term/(\d+)$`', $path, $matches)) {
    return;
  }

  $tid = $matches[1];
  $name = db_result(db_query(db_rewrite_sql('SELECT t.name FROM {term_data} t WHERE t.tid = %d', 't', 'tid'), $tid));
  return ($name) ? $name : FALSE;
}

/**
 * Implementation of hook_ckeditor_link_TYPE_url().
 */
function ckeditor_link_ckeditor_link_taxonomy_url($path, $langcode) {
  if (!preg_match('`^taxonomy/term/(\d+)$`', $path, $matches)) {
    return;
  }

  $tid = $matches[1];

  $languages = ckeditor_link_get_languages();
  if ($languages) {
    $term = taxonomy_get_term($tid);
    if ($term && ($language = @$term->language) && isset($languages[$language])) {
      $langcode = $language;
    }
  }

  return ckeditor_link_url("taxonomy/term/$tid", $langcode);
}

/**
 * Implementation of hook_ckeditor_link_TYPE_settings().
 */
function ckeditor_link_ckeditor_link_taxonomy_settings() {
  $form['taxonomy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy terms'),
  );

  $vocabularies = taxonomy_get_vocabularies();
  $options = array('- any -' => t('<em>Any vocabulary</em>'));
  foreach ($vocabularies as $vid => $vocabulary) {
    $options[$vid] = check_plain($vocabulary->name);
  }
  $form['taxonomy']['ckeditor_link_autocomplete_vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Vocabularies'),
    '#options' => $options,
    '#default_value' => variable_get('ckeditor_link_autocomplete_vocabularies', array()),
    '#description' => t('Select the vocabularies to be available as autocomplete suggestions.'),
  );

  return $form;
}
