<?php
/**
 * @file
 * uuid_node hooks on behalf of the taxonomy module.
 */

/**
 * Implementation of hook_uuid_node_features_export_render().
 */
function taxonomy_uuid_node_features_export_alter(&$export, &$pipe, $node) {
  if (!empty($node->taxonomy)) {
    foreach ($node->taxonomy as $term) {
      $voc_uuid = uuid_get_uuid('vocabulary', 'vid', $term->vid);
      if (empty($voc_uuid)) {
        $voc_uuid = uuid_set_uuid('vocabulary', 'vid', $term->vid);
      }
      $pipe['uuid_vocabulary'][$voc_uuid] = $voc_uuid;

      $term_uuid = uuid_get_uuid('term_data', 'tid', $term->tid);
      if (empty($term_uuid)) {
        $term_uuid = uuid_set_uuid('term_data', 'tid', $term->tid);
      }
      $pipe['uuid_term'][$term_uuid] = $term_uuid;
    }
  }
}

/**
 * Implementation of hook_uuid_node_features_export_render_alter().
 */
function taxonomy_uuid_node_features_export_render_alter(&$export, $node, $module) {
  if (!empty($node->taxonomy)) {
    $export->uuid_term = array();
    foreach ($node->taxonomy as $term) {
      $export->uuid_term[] = uuid_get_uuid('term_data', 'tid', $term->tid);
    }
  }
}

/**
 * Implementation of hook_uuid_node_features_rebuild_alter().
 * Build a taxonomy array suitable for node_save() from the uuid_term array.
 */
function taxonomy_uuid_node_features_rebuild_alter(&$node, $module) {
  if (!empty($node->uuid_term)) {
    $node->taxonomy = array();
    foreach ($node->uuid_term as $uuid) {
      $tid = uuid_get_serial_id('term_data', 'tid', $uuid);
      if (empty($tid)) {
        // If no tid was found, then the term doesn't exist, and most likely
        // the uuid_term rebuild needs to run first.
        // TODO: figure out how to weight feature components.
        uuid_term_features_rebuild($module);

        // Now try again.
        $tid = uuid_get_serial_id('term_data', 'tid', $uuid);
        if (empty($tid)) {
          watchdog('uuid_features', 'The term specified by %uuid could not be found.', array('%uuid' => $uuid));
          continue;
        }
      }

      $node->taxonomy[] = $tid;
    }
  }
}