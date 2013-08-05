<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function mindmaps_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'mindmap_context';
  $context->description = 'Mindmaps';
  $context->tag = 'Mindmap';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'graphmind' => 'graphmind',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'views' => array(
      'values' => array(
        'mindmaps' => 'mindmaps',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'mindmaps',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Mindmap');
  t('Mindmaps');

  $export['mindmap_context'] = $context;
  return $export;
}
