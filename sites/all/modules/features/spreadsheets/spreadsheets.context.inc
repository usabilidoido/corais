<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function spreadsheets_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'sheetnode';
  $context->description = '';
  $context->tag = 'Sheetnode';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'sheetnode' => 'sheetnode',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
      'layout' => 'wide',
    ),
    'css_injector' => array(
      7 => 7,
      1 => 0,
      2 => 0,
      3 => 0,
      4 => 0,
      5 => 0,
      6 => 0,
    ),
    'menu' => 'spreadsheets',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Sheetnode');
  $export['sheetnode'] = $context;

  return $export;
}
