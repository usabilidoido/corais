<?php
// $Id: taxonomy_hide.inc,v 1.1.2.4 2008/10/27 23:33:00 brmassa Exp $

/**
 * @file
 */

/**
 * Helper function to taxonomy_hide_nodeapi() when its time
 * to display the node
 */
function _taxonomy_hide_nodeapi(&$node, $op, $arg = 0, $arg2 = 0) {
  // This module only cares with nodes that have taxonomy terms
  // assigned to them.
  if (!empty($node->taxonomy)) {

    // Get all hidden vocabularies; keys of $hidden are the vocabulary ids.
    // Apply either the node type filter or the global filter, not both
    // First, apply filter for current node type, if it exists, else apply global filter
    // In this way, the setting for the node type overrides the global setting
    // If a filter for this type is not set, the global filter will be used.
    if ($hidden = array_filter(variable_get('taxonomy_hide_vocabularies_'. $node->type, array())) or
        $hidden = array_filter(variable_get('taxonomy_hide_vocabularies', array()))) {

      // Hide terms by removing them from the taxonomy field
      foreach ($node->taxonomy as $key => $value) {
        if (isset($hidden[$value->vid])) {
          unset($node->taxonomy[$key]);
        }
      }
    }

    // Sort terms by sorting the taxonomy field
    if (!empty($node->taxonomy) and variable_get('taxonomy_hide_group_by_vocabulary', 0)) {
      usort($node->taxonomy, "_taxonomy_hide_sort");
    }
  }
}

/**
 * Comparison function for the usort of vocabulary terms.
 */
function _taxonomy_hide_sort($a, $b) {
  // Cache the extra vocabulary information (we need the vocabulary weight)
  static $vocs = array();
  if (!empty($vocs[$a->vid])) {
    $vocs[$a->vid] = taxonomy_vocabulary_load($a->vid);
  }
  if (!empty($vocs[$b->vid])) {
    $vocs[$b->vid] = taxonomy_vocabulary_load($b->vid);
  }

  // Compare first by vocabulary weight, next by vocabulary id, next by term
  // weight, next by term name, and finally by term id. This is the same order
  // as used by taxonomy_node_get_terms, except that we group by vocabulary too.
  if ($vocs[$a->vid]->weight < $vocs[$b->vid]->weight) {
    return -1;
  }
  elseif ($vocs[$a->vid]->weight > $vocs[$b->vid]->weight) {
    return 1;
  }
  elseif ($a->vid < $b->vid) {
    return -1;
  }
  elseif ($a->vid > $b->vid) {
    return 1;
  }
  elseif ($a->weight < $b->weight) {
    return -1;
  }
  elseif ($a->weight > $b->weight) {
    return 1;
  }
  elseif ($string = strcasecmp($a->name, $b->name)) {
    return $string;
  }
  elseif ($a->tid < $b->tid) {
    return -1;
  }
  elseif ($a->tid > $b->tid) {
    return 1;
  }
  else {
    return 0;
  }
}
