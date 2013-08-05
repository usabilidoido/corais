<?php

/**
 * @file
 * Interface between finder_views.module and views.module.
 */

/**
 * Implementation of hook_views_plugins().
 *
 * @see hook_views_plugins()
 */
function finder_views_views_plugins() {
  $plugins = array(
    'module' => 'finder_views', // This just tells our themes are elsewhere.
    'display' => array(
      'finder_views' => array(
        'path' => drupal_get_path('module', 'finder_views') .'/includes',
        'title' => 'Finder',
        'help' => 'Destination-agnostic display. Mostly useful for programmatic views.',
        'handler' => 'finder_views_plugin_display_finder',
        'no ui' => TRUE, // Programmatic use only.
        'uses hook menu' => FALSE,
        'use ajax' => FALSE,
        'use pager' => TRUE,
        'accept attachments' => FALSE,
      ),
    ),
    'style' => array(
      'finder_views_php_array_finder' => array(
        'path' => drupal_get_path('module', 'finder_views') .'/includes',
        'title' => 'Results array (with title)',
        'help' => 'Returns the view as a PHP array.',
        'handler' => 'finder_views_plugin_style_php_array_finder',
        'no ui' => TRUE, // Programmatic use only.
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );
  return $plugins;
}

/**
 * Implementation of hook_views_data().
 */
function finder_views_views_data() {
  $data = array();
  if (module_exists("taxonomy")) {
    $vocabs = taxonomy_get_vocabularies();
    foreach ($vocabs as $vid => $vocab) {
      $data['term_data_'. $vid]['table']['group']  = t('Taxonomy');
      $data['term_data_'. $vid]['table']['join'] = array(
        'node' => array(
          'left_table' => 'term_node',
          'left_field' => 'tid',
          'field' => 'tid',
          'table' => 'term_data',
          'extra' => array(
            'vid' => array(
              'field' => 'vid',
              'value' => $vid,
              'numeric' => TRUE,
            ),
          ),
        ),
        'node_revision' => array(
          'left_table' => 'term_node',
          'left_field' => 'tid',
          'field' => 'tid',
          'table' => 'term_data',
          'extra' => array(
            'vid' => array(
              'field' => 'vid',
              'value' => $vid,
              'numeric' => TRUE,
            ),
          ),
        ),
      );
      // Term name field
      $data['term_data_'. $vid]['name'] = array(
        'title' => t('Terms for !vocab', array('!vocab' => $vocab->name)),
        'help' => t('Taxonomy terms for !vocab. Note that using this can cause duplicate nodes to appear in views; you must add filters to reduce the result set.', array('!vocab' => $vocab->name)),
        'field' => array(
          'handler' => 'views_handler_field_taxonomy',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
          'help' => t('Taxonomy term name.'),
        ),
      );
    }
  }
  return $data;
}