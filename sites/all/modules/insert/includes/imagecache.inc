<?php

/**
 * @file
 * Insert support for ImageCache module.
 */

/**
 * Implementation of hook_insert_styles().
 */
function imagecache_insert_styles() {
  $presets = imagecache_presets();
  $insert_styles = array();
  foreach ($presets as $preset) {
    $insert_styles['imagecache_' . $preset['presetname']] = array(
      'label' => t($preset['presetname']),
    );
  }
  return $insert_styles;
}

/**
 * Implementation of hook_insert_content().
 */
function imagecache_insert_content($item, $style, $widget) {
  $preset_name = preg_replace('/^imagecache_/', '', $style['name']);
  return theme(array('imagecache_insert_image__' . str_replace('-', '_', $preset_name), 'imagecache_insert_image'), $item, $widget, $preset_name);
}

/**
 * Theme the content that will be inserted for ImageCache presets.
 */
function template_preprocess_imagecache_insert_image(&$vars) {
  $absolute = isset($vars['widget']['insert_absolute']) ? $vars['widget']['insert_absolute'] : NULL;
  $absolute = (bool) (isset($absolute) ? $absolute : variable_get('insert_absolute_paths', FALSE));
  $vars['url'] = imagecache_create_url($vars['preset_name'], $vars['item']['filepath'], FALSE, $absolute);
  $vars['class'] = !empty($vars['widget']['insert_class']) ? $vars['widget']['insert_class'] : '';
}
