<?php
/**
 * @file
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 */

/**
 * Implementation of hook_ckeditor_link_TYPE_autocomplete().
 */
function ckeditor_link_ckeditor_link_i18ntaxonomy_autocomplete($string, $limit) {
  // Currently, this function only supports MySQL.
  // TODO: Add support for pgsql.
  if (!in_array($GLOBALS['db_type'], array('mysql', 'mysqli'))) {
    return array();
  }

  $matches = array();

  $vocabularies = array_keys(array_filter(variable_get('ckeditor_link_autocomplete_vocabularies', array())));
  if (count($vocabularies)) {
    $sql = "SELECT t.tid, CONVERT(lt.translation USING utf8) name, lt.language FROM {term_data} t
      INNER JOIN {locales_source} ls ON ls.location = CONCAT('term:', t.tid, ':name')
      INNER JOIN {locales_target} lt ON lt.lid = ls.lid
      WHERE CONVERT(lt.translation USING utf8) LIKE '%%%s%%'";
    $args = array($string);
    if (!in_array('- any -', $vocabularies)) {
      $sql .= ' AND t.vid IN ('. db_placeholders($vocabularies) .')';
      $args = array_merge($args, $vocabularies);
    }
    $sql .= ' ORDER BY name';
    $result = db_query_range(db_rewrite_sql($sql, 't', 'tid'), $args, 0, $limit);
    while ($term = db_fetch_object($result)) {
      $path = ckeditor_link_path_prefix_language('taxonomy/term/'. $term->tid, $term->language);
      $matches[$path] = $term->name;
    }
  }

  return $matches;
}

/**
 * Implementation of hook_ckeditor_link_TYPE_revert().
 */
function ckeditor_link_ckeditor_link_i18ntaxonomy_revert($path, &$langcode) {
  if (!preg_match('`^taxonomy/term/(\d+)$`', $path, $matches)) {
    return;
  }

  $tid = $matches[1];
  $result = db_query(db_rewrite_sql('SELECT t.tid, t.vid, t.name, t.language FROM {term_data} t WHERE t.tid = %d', 't', 'tid'), $tid);
  if ($term = db_fetch_object($result)) {
    if (empty($term->language)) {
      return html_entity_decode(i18ntaxonomy_translate_term_name($term, '', $langcode), ENT_QUOTES);
    }
    else {
      $langcode = '';
      return $term->name;
    }
  }

  return FALSE;
}
