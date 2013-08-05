<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function atrium_book_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:knowledge
  $menu_links['features:knowledge'] = array(
    'menu_name' => 'features',
    'link_path' => 'knowledge',
    'router_path' => 'knowledge',
    'link_title' => 'Knowledge Tree',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Knowledge Tree');


  return $menu_links;
}
