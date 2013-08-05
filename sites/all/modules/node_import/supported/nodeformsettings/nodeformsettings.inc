<?php
/**
 * @file
 * Support file for Node (and Comment) Settings form.
 *
 * Node Form Settings can be configure to remove the
 * "Preview" button and rename the "Save" button,
 * which causes Node import to fail (not able to
 * submit "Save" or "Preview").
 *
 * We solve this by clearing the settings variable
 * but only during an import of course.
 *
 * @see http://drupal.org/node/1130860
 */

/**
 * Implements hook_node_import_values().
 */
function nodeformsettings_node_import_values($type, $defaults, $options, $fields, $preview) {
  global $conf;
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    // See nodeformsettings_get_settings($type) in nodeformsettings.module
    $conf['nodeformsettings_' . $node_type] = '';
  }
}

