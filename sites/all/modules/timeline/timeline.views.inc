<?php
/**
 * @file
 * Views plugins definition for timeline.
 */

/**
 * Implementation of hook_views_style_plugins(). Adds view types to views UI interface.
 */
function timeline_views_plugins() {
  return array(
    'style' => array(
      'timeline' => array(
        'title' => t('Timeline'),
        'help' => t('Displays content on a SIMILE Timeline.'),
        'handler' => 'views_views_plugin_style_timeline',
        'theme' => 'views_view_timeline',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'uses row plugin' => TRUE,
        'even empty' => TRUE,
      ),
    ),
  );
}
