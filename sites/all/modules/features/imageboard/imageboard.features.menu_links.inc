<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function imageboard_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:imageboard
  $menu_links['features:imageboard'] = array(
    'menu_name' => 'features',
    'link_path' => 'imageboard',
    'router_path' => 'imageboard',
    'link_title' => 'Galeria de Imagens',
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
  t('Galeria de Imagens');


  return $menu_links;
}
