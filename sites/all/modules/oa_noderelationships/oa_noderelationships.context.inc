<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function oa_noderelationships_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'oa_noderelationships';
  $context->description = '';
  $context->tag = 'oa_noderelationships';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'noderelationships/*' => 'noderelationships/*',
      ),
    ),
  );
  $context->reactions = array(
    'context_reaction_active_theme' => array(
      'theme' => 'garland',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('oa_noderelationships');

  $export['oa_noderelationships'] = $context;
  return $export;
}
