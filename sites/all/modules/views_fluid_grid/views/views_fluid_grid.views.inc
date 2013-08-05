<?php
// $Id: views_fluid_grid.views.inc,v 1.1.2.3 2009/10/13 23:49:21 markuspetrux Exp $

/**
 * @file
 * Integration with Views.
 */

/**
 * Implementation of hook_views_plugins().
 */
function views_fluid_grid_views_plugins() {
  $module_path = drupal_get_path('module', 'views_fluid_grid');
  return array(
    'module' => 'views_fluid_grid', // This just tells our themes are elsewhere.
    'style' => array(
      'fluid_grid' => array(
        'title' => t('Fluid grid'),
        'help' => t('Displays items in a fluid grid.'),
        'handler' => 'views_fluid_grid_plugin_style',
        'path' => $module_path .'/views',
        'theme path' => $module_path .'/theme',
        'theme file' => 'theme.inc',
        'theme' => 'views_fluid_grid_plugin_style',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-fluid-grid',
      ),
    ),
  );
}
