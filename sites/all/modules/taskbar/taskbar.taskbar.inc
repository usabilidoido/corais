<?php
/**
 * @file
 *   Taskbar hook-implementations.
 */

/**
 * Implements hook_taskbar_style().
 */
function taskbar_taskbar_style() {
  $path = drupal_get_path('module', 'taskbar') . '/styles';
  return array(
    array(
      'name' => 'taskbar_base',
      'description' => 'Taskbar base style',
      'path' => $path . '/base',
    ),
    array(
      'name' => 'taskbar_top',
      'description' => 'Taskbar base style (on the top)',
      'base' => 'taskbar_base',
      'path' => $path . '/top',
    ),
    array(
      'name' => 'taskbar_bottom',
      'description' => 'Taskbar base style (at the bottom)',
      'base' => 'taskbar_base',
      'path' => $path . '/bottom',
    ),
    array(
      'name' => 'taskbar_default',
      'description' => 'Taskbar default style',
      'base' => 'taskbar_bottom',
      'path' => $path . '/default',
    ),
  );
}

/**
 * Implements hook_taskbar_provider().
 */
function taskbar_taskbar_provider() {
  $providers = array();
  $files = file_scan_directory(drupal_get_path('module', 'taskbar') . '/providers', '\.inc$');
  foreach ($files as $file) {
    require_once('./' . $file->filename);
    $function = preg_replace('/\.([a-z]+)/', '_\1_definition', $file->name);
    $provider = $function();
    if ($provider) {
      $providers[] = $provider;
    }
  }
  return $providers;
}

/**
 * Implements hook_taskbar_default_items().
 */
function taskbar_taskbar_default_items() {
  if (!variable_get('taskbar_provide_sample_items', 1)) {
    return array();
  }

  $items['basic'] = array(
    'name' => 'basic',
    'title' => 'Basic Links',
    'provider' => 'link',
    'region' => 'left',
    'status' => 1,
    'weight' => 0,
    'settings' => array(
      'links' => array(
        'home' => array(
          'title' => t('Frontpage'),
          'url' => '<front>',
          'options' => array(),
        ),
      ),
    ),
  );

  return $items;
}

