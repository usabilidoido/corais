<?php
// $Id: views_flag_refresh.api.php,v 1.6 2010/09/02 20:57:04 cpliakas Exp $

/**
 * @file
 * Hooks defined by the Views Flag Refresh module.
 */

/**
 * Defines refresh widgets that are displayed when a view is being refreshed.
 *
 * @return
 *   An array of widget definitions.
 */
function hook_views_flag_refresh_widgets() {
  $widgets = array();

  // Array keys are the machine readable name of the widget.
  $widgets['throbber'] = array(

    // The human-readable name of the widget.
    'title' => t('Throbber image'),

    // The name of the method in the ViewsFlagRefresh.theme javascript class
    // that is called after the flag action has been completed and before the
    // Views AJAX request is executed.
    // (OPTIONAL)
    'theme hook' => 'throbber',

    // The name of the method in the ViewsFlagRefresh.theme javascript class
    // that is called after the data has been returned by the Views AJAX
    // request. This is normally implemented to clean up any elements added to
    // the the DOM by the method defined in the "theme hook" key above.
    // (OPTIONAL)
    'theme hook post' => 'throbberPost',

    // A brief description of what the widget does.
    // (OPTIONAL)
    'description' => t('Display a throbber image in place of the view content while it is being refreshed.'),

    // Specifies a javascript file that must be loaded to render the widget.
    // Usually the file contains the methods defined in the "theme hook" and
    // "theme hook post" keys above.
    // (OPTIONAL)
    'js file' => drupal_get_path('module', 'views_flag_refresh') .'/views_flag_refresh.js',

    // Specifies a CSS file that must be loaded to render the widget properly.
    // (OPTIONAL)
    'css file' => drupal_get_path('module', 'views_flag_refresh') .'/views_flag_refresh.css',
  );

  return $widgets;
}

/**
 * Allows modules to alter widget definitions.
 *
 * @param &$widgets
 *   An array of widget definitions.
 */
function hook_views_flag_refresh_widgets_alter(&$widgets) {
  // NOTE: This code is for demonstration purposes only. We are probably better
  // off creating another widget.
  $widgets['throbber']['theme hook'] = 'betterThrobber';
  $widgets['throbber']['js file'] = drupal_get_path('module', 'mymodule') .'/mymodule.js';
}
