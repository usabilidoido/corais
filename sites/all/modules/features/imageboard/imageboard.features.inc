<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function imageboard_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function imageboard_imagecache_default_presets() {
  $items = array(
    'imageboard-thumb' => array(
      'presetname' => 'imageboard-thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '160px',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'imageboard_preview' => array(
      'presetname' => 'imageboard_preview',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '600px',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function imageboard_node_info() {
  $items = array(
    'images' => array(
      'name' => t('Post de imagens'),
      'module' => 'features',
      'description' => t('Série de imagens.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Texto'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function imageboard_views_api() {
  return array(
    'api' => '2',
  );
}
