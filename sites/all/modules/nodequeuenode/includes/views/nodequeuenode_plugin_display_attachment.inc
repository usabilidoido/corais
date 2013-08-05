<?php
// $Id: nodequeuenode_plugin_display_attachment.inc,v 1.1 2009/05/20 13:44:21 daniboy Exp $

/**
 * @file nodequeuenode_plugin_display_attachment.inc
 * Provides a display plugin for Views to display Meta nodes.
 */


class nodequeuenode_plugin_display_attachment extends views_plugin_display_attachment {
  
  /**
   * Set default options for this display.
   * @see views/includes/views_object#options()
   */
  function options(&$display) {
    parent::options($display);
    // Default to a unformatted "list" with a single node.
    $display['style_plugin'] = 'default';
    $display['items_per_page'] = 1;
    $display['row_plugin'] = 'node';
		
    // Disable relationships, fields and sorts for this display
    $display['relationships'] = array();
    $display['fields'] = array();
    $display['sorts'] = array();
    
    // Default the arguments to a "Subqueue ID (meta)" argument.
    $display['arguments'] = array(
      'sqid' => array(
        'default_action' => 'not found', // Hide on empty
        'table' => 'nodequeuenode_relation',
        'field' => 'sqid',
      ),
    );
    
    // Default the filters to a "Published or Admin" filter.
    $display['filters'] = array(
      'status_extra' => array(
        'table' => 'node',
        'field' => 'status_extra',
      ),
    );
    
    // Prevent defaulting to the same options as the "defaults" display
    $display['defaults']['style_plugin'] =
    $display['defaults']['style_options'] =
    $display['defaults']['items_per_page'] =
    $display['defaults']['row_plugin'] =
    $display['defaults']['row_options'] =
    $display['defaults']['relationships'] =
    $display['defaults']['fields'] =
    $display['defaults']['sorts'] =
    $display['defaults']['arguments'] =
    $display['defaults']['filters'] =
      FALSE;
    
  }
  
}

