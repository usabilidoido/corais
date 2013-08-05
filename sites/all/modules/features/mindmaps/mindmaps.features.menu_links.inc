<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function mindmaps_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:mindmaps
  $menu_links['features:mindmaps'] = array(
    'menu_name' => 'features',
    'link_path' => 'mindmaps',
    'router_path' => 'mindmaps',
    'link_title' => 'Mindmaps',
    'options' => array(
      'attributes' => array(
        'title' => 'Mindmaps',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Mindmaps');


  return $menu_links;
}
