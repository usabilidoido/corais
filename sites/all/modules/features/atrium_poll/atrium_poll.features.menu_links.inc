<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function atrium_poll_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:poll
  $menu_links['features:poll'] = array(
    'menu_name' => 'features',
    'link_path' => 'poll',
    'router_path' => 'poll',
    'link_title' => 'Polls',
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
  t('Polls');


  return $menu_links;
}
