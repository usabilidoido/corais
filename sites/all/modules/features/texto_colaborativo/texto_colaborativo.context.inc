<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function texto_colaborativo_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'texto_colaborativo';
  $context->description = '';
  $context->tag = 'Texto Colaborativo';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'text' => 'text',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'views' => array(
      'values' => array(
        'text:page_1' => 'text:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
      'layout' => 'wide',
    ),
    'menu' => 'texts',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Texto Colaborativo');
  $export['texto_colaborativo'] = $context;

  return $export;
}
