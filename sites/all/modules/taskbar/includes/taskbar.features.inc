<?php
/**
 * @file
 * Features support.
 */

/**
 * Implementation of hook_features_export_options().
 */
function taskbar_features_export_options() {
  $exports = array();
  foreach (taskbar_get_items() as $item) {
    $exports[$item->name] = $item->title;
  }
  return $exports;
}

/**
 * Implementation of hook_features_export().
 */
function taskbar_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $items = taskbar_get_items();

  $export['dependencies']['taskbar'] = 'taskbar';
  foreach ($data as $name) {
    $export['features']['taskbar'][$name] = $name;
  }
  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 */
function taskbar_features_export_render($module_name, $data, $export = NULL) {
  $items = taskbar_get_items();
  $code = array('$items = array();');
  foreach ($data as $name) {
    $code[] = '$items[\'' . $name . '\'] = ' . features_var_export($items[$name]) . ';';
  }

  $code[] = 'return $items;';

  $code = implode($code, "\n\n");
  $code = preg_replace('/^(.+)$/m', '  \1', $code);

  return array('taskbar_default_items' => $code);
}

/**
 * Implementation of hook_features_revert().
 */
function taskbar_features_revert($module = NULL) {
  taskbar_features_rebuild($module);
}

/**
 * Implementations of hook_features_rebuild().
 */
function taskbar_features_rebuild($module = NULL) {
  if (module_hook($module, 'taskbar_default_items')) {
    $items = module_invoke($module, 'taskbar_default_items');

    foreach ($items as $name => $item) {
      taskbar_item_delete($name);
    }

    drupal_flush_all_caches();
  }
}

