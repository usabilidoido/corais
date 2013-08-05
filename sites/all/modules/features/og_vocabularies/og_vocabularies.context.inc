<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function og_vocabularies_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'og_vocab';
  $context->description = '';
  $context->tag = 'OG vocabularies';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'og_vocab-0' => array(
          'module' => 'og_vocab',
          'delta' => 0,
          'region' => 'space_tools',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('OG vocabularies');
  $export['og_vocab'] = $context;

  return $export;
}
