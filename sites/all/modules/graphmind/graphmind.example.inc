<?php
// $Id: graphmind.example.inc,v 1.1.2.2 2011/02/08 16:27:37 itarato Exp $


/**
 * Examples for the Graphmind hooks.
 */
 
 
/**
 * Implementation of hook_graphmind_flashvars_alter().
 * Adds variables to the flash object.
 *
 * @param array &$flashvars
 */
function graphmind_example_graphmind_flashvars_alter(&$flashvars) {
  $flashvars['foo'] = 'bar';
}


/**
 * Implementation of hook_graphmind_features().
 * 
 * @return array
 */
function graphmind_example_graphmind_features() {
  return array(
    'foo' => t('Foo'),
    'bar' => t('Bar'), // ...
  );
}


/**
 * Implementation of hook_graphmind_plugin_info().
 *
 * @return array
 */
function graphmind_example_graphmind_plugin_info() {
  return array(
    'graphmind_example' => array(
      // Module implement this plugin
      'module' => 'graphmind_example',
      // Name of the plugin in flash (src/plugin/PLUGIN.as)
      'plugin' => 'GraphmindExample',
      'name' => t('Graphmind example'),
      'description' => t('...'),
    ),
  );
}