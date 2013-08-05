<?php

/**
 * @file
 * Node implementations of the page title hooks
 */


/**
 * Implementation of hook_page_title_alter().
 */
function node_page_title_alter(&$title) {
  $menu_item = menu_get_item();

  // Test if this is a node page.
  if ( !strncmp($menu_item['path'], 'node/%', 6) &&
       ($node = menu_get_object()) &&
       !empty($node->page_title) &&
       variable_get('page_title_type_'. $node->type .'_showfield', 0)) {
    // If the node has a custom page title and the node type is configured
    // to have a custom page title (ie, it's not a leftover from a previous
    // setting), then use it.
    $title = $node->page_title;
  }
}


/**
 * Implementation of hook_page_title_pattern_alter().
 */
function node_page_title_pattern_alter(&$pattern, &$types) {
  $menu_item = menu_get_item();

  // Test if this is a node page.
  if ( !strncmp($menu_item['path'], 'node/%', 6) &&
       $node = menu_get_object() ) {
    $types['node'] = $node;

    // If the node has any taxonomy, grab the first term for user in tokens.
    // TODO: Handle multiple terms? Only pass specific terms per content type?
    if (!empty($types['node']->taxonomy)) {
      $types['taxonomy'] = current($types['node']->taxonomy);
    }

    $pattern = variable_get('page_title_type_'. $node->type, '');
  }
}


/**
 * Implementation of hook_page_title_settings().
 */
function node_page_title_settings() {
  $settings = array();

  $types = node_get_types();
  foreach ($types as $type) {
    $settings['page_title_type_'. $type->type] = array(
      'label' => 'Content Type - %type',
      'label arguments' => array('%type' => $type->name),
      'scopes' => array('global', 'node', 'taxonomy'),
      'show field' => TRUE,
      'description' => 'This pattern will be used for all %type node-type pages',
      'description arguments' => array('%type' => $type->name),
    );
  }

  return $settings;
}
