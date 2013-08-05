<?php
/**
 * @file
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 */

/**
 * Implementation of hook_ckeditor_link_TYPE_autocomplete().
 */
function ckeditor_link_ckeditor_link_node_autocomplete($string, $limit) {
  $matches = array();

  $node_types = array_keys(array_filter(variable_get('ckeditor_link_autocomplete_node_types', array('- any -' => '- any -'))));
  if (count($node_types)) {
    $sql = "SELECT n.nid, n.title FROM {node} n WHERE n.title LIKE '%%%s%%'";
    $args = array($string);
    if (!in_array('- any -', $node_types)) {
      $sql .= ' AND n.type IN('. db_placeholders($node_types, 'text') .')';
      $args = array_merge($args, $node_types);
    }
    $sql .= ' ORDER BY n.title, n.type';
    $result = db_query_range(db_rewrite_sql($sql), $args, 0, $limit);
    while ($node = db_fetch_object($result)) {
      $matches['node/'. $node->nid] = $node->title;
    }
  }

  return $matches;
}

/**
 * Implementation of hook_ckeditor_link_TYPE_revert().
 */
function ckeditor_link_ckeditor_link_node_revert($path, &$langcode) {
  if (!preg_match('`^node/(\d+)$`', $path, $matches)) {
    return;
  }

  $nid = $matches[1];
  $result = db_query(db_rewrite_sql('SELECT n.title, n.language FROM {node} n WHERE n.nid = %d'), $nid);
  if ($node = db_fetch_object($result)) {
    if (!empty($node->language)) {
      $langcode = '';
    }
    return $node->title;
  }

  return FALSE;
}

/**
 * Implementation of hook_ckeditor_link_TYPE_url().
 */
function ckeditor_link_ckeditor_link_node_url($path, $langcode) {
  if (!preg_match('`^node/(\d+)$`', $path, $matches)) {
    return;
  }

  $nid = $matches[1];

  $languages = ckeditor_link_get_languages();
  if ($languages && ($language = db_result(db_query('SELECT language FROM {node} WHERE nid = %d', $nid))) && isset($languages[$language])) {
    $langcode = $language;
  }

  return ckeditor_link_url("node/$nid", $langcode);
}

/**
 * Implementation of hook_ckeditor_link_TYPE_settings().
 */
function ckeditor_link_ckeditor_link_node_settings() {
  $form['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nodes'),
  );
  $form['node']['ckeditor_link_autocomplete_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#options' => array('- any -' => t('<em>Any content type</em>')) + array_map('check_plain', node_get_types('names')),
    '#default_value' => variable_get('ckeditor_link_autocomplete_node_types', array('- any -' => '- any -')),
    '#description' => t('Select the content types to be available as autocomplete suggestions.'),
  );

  return $form;
}
