<?php
// $Id$
/**
 * Implementation of hook_views_plugins().
 */
function litenode_views_plugins() {
  return array(
    'style' => array(
      'litenode' => array(
        'title' => t('Litenode'),
        'help' => t('Lighter, faster version of the node style.'),
        'handler' => 'litenode_plugin_style_litenode',
        'path' => drupal_get_path('module', 'litenode') .'/views',
        'theme' => 'litenode_view',
        'type' => 'normal',
        'uses row plugin' => FALSE,
        'uses options' => TRUE,
      ),
    ),
  );
}

/**
 * Implementation of hook_views_pre_build().
 */
function litenode_views_pre_build(&$view) {
  if ($view->display_handler->get_option('style_plugin') == 'litenode') {
    if (!empty($view->display[$view->current_display]->display_options['fields'])) {
      $fields = $view->display[$view->current_display]->display_options['fields']; 
    }
    else {
      $fields = $view->display['default']->display_options['fields']; 
    }

    $options = $view->display_handler->get_option('style_options');
    foreach (litenode_views_fields() as $module => $module_fields) {
      if (!empty($options['modules'][$module])) {
        $fields = array_merge($fields, $module_fields);
      }
    }
    $view->display_handler->override_option('fields', $fields);
  }
}

/**
 * Implementation of hook_views_pre_execute().
 */
function litenode_views_pre_execute(&$view) {
  if ($view->display_handler->get_option('style_plugin') == 'litenode' && empty($view->query->fields['nid']['distinct'])) {
    // Remove any unnecessary joins from the count query.
    // This is only possible if the View in question lacks
    // a DISTINCT on the base field.
    // @TODO test this with relationships present
    $count_query = drupal_clone($view->query);
    $where = $count_query->condition_sql();
    // If table is not present in the where condition, we should drop the join
    foreach ($count_query->tables[$count_query->base_table] as $table => $info) {
      if ($table != $count_query->base_table && strpos($where, $table) === FALSE) {
        unset($count_query->tables[$count_query->base_table][$table]);
        unset($count_query->table_queue[$table]);
      }
    }
    $view->build_info['count_query'] = $count_query->query(TRUE);
  }
}
