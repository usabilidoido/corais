<?php


/**
 * Get the full taxonomy forest.
 *
 * @return array
 */
function graphmind_taxonomy_manager_get_all() {
  $vocabularies = taxonomy_get_vocabularies();
  foreach ((array)$vocabularies as $key => $vocabulary) {
    $vocabularies[$key]->terms = taxonomy_get_tree($vocabulary->vid);
  }

  return $vocabularies;
}


/**
 * Callback for graphmindTaxonomyManager.moveTerm RCP.
 *
 * @param integer $source_tid
 * @param integer $target_vid
 * @param integer $parent_tid
 * @param string $order - Format: 43|2|5|24...
 * @param string $child_nodes - Format: 43|2|5|24...
 */
function graphmind_taxonomy_manager_move_term($source_tid, $target_vid, $parent_tid, $order, $child_nodes) {
  // Missing base term
  if ((int)$source_tid == 0) return 0;
  // Missing vocabulary
  if ((int)$target_vid == 0) return 0;

  // parent == null means it is a first level term, it has to be 0 than
  $parent_tid = (int)$parent_tid;

  // New vocabulary VID
  $newVocabularyNodes = explode('|', $child_nodes);
  if (!empty($newVocabularyNodes)) {
    db_query('UPDATE {term_data} SET vid = %d WHERE tid IN ('.db_placeholders($newVocabularyNodes, 'int').');', array_merge(array($target_vid), $newVocabularyNodes));
  }

  // New WEIGHT
  $orderedNodes = explode('|', $order);
  $weight = 0;
  foreach ((array)$orderedNodes as $tid) {
    db_query('UPDATE {term_data} SET weight = %d WHERE tid = %d;', $weight++, $tid);
  }

  // Set proper state in term hierarchy
  $result = db_fetch_object(db_query('SELECT COUNT(*) AS count FROM {term_hierarchy} WHERE tid = %d;', $source_tid));
  if ($result->count >= 1) {
    db_query('UPDATE {term_hierarchy} SET parent = %d WHERE tid = %d;', $parent_tid, $source_tid);
  } else {
    db_query('INSERT INTO {term_hierarchy} (tid, parent) VALUES (%d, %d);', $source_tid, $parent_tid);
  }

  return 1;
}


/**
 * Callback for the graphmindTaxonomyManager.deleteTerm RCP call.
 *
 * @param integer $tid
 * @return 0|1
 */
function graphmind_taxonomy_manager_delete_term($tid) {
  taxonomy_del_term($tid);
  
  return 1;
}


/**
 * Add a subtree to a term or vocabulary
 *
 * @param integer $tid
 * @param integer $vid
 * @param array $subTree
 */
function graphmind_taxonomy_manager_add_subtree($tid, $vid, $subtree) {
  _graphmind_taxonomy_manager_build_term_subtree($subtree, $vid, $tid);

  return $subtree;
}


/**
 * Create a taxonomy subtree from a given object (via services).
 *
 * @param string $name
 * @param integer $vid
 * @param integer $parent_tid
 * @param array $childs
 */
function _graphmind_taxonomy_manager_build_term_subtree(&$subtree, $vid, $parent_tid) {
  $term_data = new stdClass();
  $term_data->name = strip_tags($subtree['name']);
  $term_data->vid  = $vid;
  drupal_write_record('term_data', $term_data);
  $subtree['tid'] = $term_data->tid;
  $subtree['vid'] = $term_data->vid;

  $term_hierarchy = new stdClass();
  $term_hierarchy->tid = $term_data->tid;
  $term_hierarchy->parent = $parent_tid;
  drupal_write_record('term_hierarchy', $term_hierarchy);

  foreach ((array)$subtree['terms'] as $key => $child) {
    _graphmind_taxonomy_manager_build_term_subtree($subtree['terms'][$key], $vid, $term_data->tid);
  }
}


/**
 * Change a term's name.
 *
 * @param integer $tid
 * @param string $name
 * @return integer
 */
function graphmind_taxonomy_manager_rename_term($tid, $name) {
  if ((int)$tid == 0) return 0;

  db_query('UPDATE {term_data} SET name = \'%s\' WHERE tid = %d;', $name, $tid);

  return 1;
}