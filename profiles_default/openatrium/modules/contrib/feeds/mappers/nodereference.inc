<?php

/**
 * @file
 * Implementation of Feeds API for mapping nodereference.module fields (CCK).
 */

/**
 * Implementation of hook_feeds_node_processor_targets_alter().
 *
 * @see FeedsNodeProcessor::getMappingTargets()
 */
function nodereference_feeds_node_processor_targets_alter(&$targets, $content_type) {
  $info = content_types($content_type);
  if (isset($info['fields']) && is_array($info['fields'])) {
    foreach ($info['fields'] as $field_name => $field) {
      if ($field['type'] == 'nodereference') {
        $field_label = !empty($field['widget']['label']) ? $field['widget']['label'] : $field_name;

        $targets[$field_name . ':title'] = array(
          'name' => t('@field_label (by title)', array('@field_label' => $field_label)),
          'callback' => 'nodereference_feeds_set_target',
          'description' => t('The CCK node reference @field_label of the node, matched by node title.', array('@field_label' => $field_label)),
          'real_target' => $field_name,
        );

        $targets[$field_name . ':nid'] = array(
          'name' => t('@field_label (by nid)', array('@field_label' => $field_label)),
          'callback' => 'nodereference_feeds_set_target',
          'description' => t('The CCK node reference @field_label of the node, matched by node ID.', array('@field_label' => $field_label)),
          'real_target' => $field_name,
        );

        $targets[$field_name . ':url'] = array(
          'name' => t('@field_label (by Feeds URL)', array('@field_label' => $field_label)),
          'callback' => 'nodereference_feeds_set_target',
          'description' => t('The CCK node reference @field_label of the node, matched by Feeds URL.', array('@field_label' => $field_label)),
          'real_target' => $field_name,
        );

        $targets[$field_name . ':guid'] = array(
          'name' => t('@field_label (by Feeds GUID)', array('@field_label' => $field_label)),
          'callback' => 'nodereference_feeds_set_target',
          'description' => t('The CCK node reference @field_label of the node, matched by Feeds GUID.', array('@field_label' => $field_label)),
          'real_target' => $field_name,
        );
      }
    }
  }
}

/**
 * Callback for mapping. Here is where the actual mapping happens.
 *
 * When the callback is invoked, $target contains the name of the field the
 * user has decided to map to and $value contains the value of the feed item
 * element the user has picked as a source.
 *
 * @param stdClass $node
 *   The node to to be created or updated.
 * @param str $target
 *   The name of the field on the node to be mapped.
 * @param str|array $value
 *   The value(s) of the source feed item element to be mapped.
 */
function nodereference_feeds_set_target($node, $target, $value) {
  // Determine whether we are matching against title, nid, URL, or GUID.
  list($target, $match_key) = explode(':', $target, 2);

  // Load field definition.
  $field_info = content_fields($target, $node->type);

  // Allow for multiple-value fields.
  $value = is_array($value) ? $value : array($value);

  // Allow importing to the same target with multiple mappers.
  $field = isset($node->{$target}) ? $node->{$target} : array();

  // Match values against nodes and add to field.
  foreach ($value as $v) {
    $nids = array();
    $v = trim($v);

    switch ($match_key) {
      case 'url':
      case 'guid':
        // Lookup node ID by Feeds unique value.
        $result = db_query("SELECT nid FROM {feeds_node_item} WHERE %s = '%s'", $match_key, $v);
        // Since GUID and URL are only guaranteed to be unique per feed,
        // multiple nids from different feeds may result.
        while ($row = db_fetch_array($result)) {
          $nids[] = $row['nid'];
        }
        // Ensure nids are valid node ids for this field.
        $nids = !empty($nids) ? array_keys(_nodereference_potential_references($field_info, '', NULL, $nids)) : array();
        break;

      case 'title':
        // Validate title.
        if ((is_string($v) && $v !== '') || is_numeric($v)) {
          // Lookup potential exact matches for the value.
          $nids = array_keys(_nodereference_potential_references($field_info, $v, 'equals', array()));
        }
        break;

      case 'nid':
        // Ensure nid is a valid node id for this field.
        $nids = array_keys(_nodereference_potential_references($field_info, '', NULL, array($v)));
        break;
    }

    if (empty($nids)) {
      // Alert if no matches were found.
      drupal_set_message(t("'%value' does not match a valid node %key for the '%field' field.", array('%value' => $v, '%key' => $match_key, '%field' => $target)));
    }
    else {
      // Add the reference (ignoring duplicates).
      foreach ($nids as $nid) {
        $field[] = array('nid' => $nid);
      }
    }

  }

  $node->{$target} = $field;
}
