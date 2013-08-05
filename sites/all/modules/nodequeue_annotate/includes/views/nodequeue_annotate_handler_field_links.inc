<?php

/**
 * @file nodequeue_annotate_handler_field_links.inc
 * Field handler for Nodequeue Annotate links
 */

/**
 * Views field handler for the nodequeue annotate links
 */
class nodequeue_annotate_handler_field_links extends views_handler_field_node_link {
  
  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {
    // Load the node
    $nid = $values->{$this->aliases['nid']};
    $node = node_load($nid);
    
    // Get the nodequeue links and alter them
    $links = nodequeue_link('node', $node);
    nodequeue_annotate_link_alter($links, $node);
    
    // Theme the result
    return theme('nodequeue_annotate_views_links', $links);
  }
  
}
