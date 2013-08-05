<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function atrium_poll_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'atrium_poll';
  $context->description = '';
  $context->tag = 'Poll';
  $context->conditions = array(
    'menu' => array(
      'values' => array(
        'poll' => 'poll',
      ),
    ),
    'node' => array(
      'values' => array(
        'poll' => 'poll',
      ),
      'options' => array(
        'node_form' => 1,
      ),
    ),
    'views' => array(
      'values' => array(
        'atrium_poll' => 'atrium_poll',
        'atrium_poll:page_1' => 'atrium_poll:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
      'layout' => 'wide',
    ),
    'menu' => 'poll',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Poll');
  $export['atrium_poll'] = $context;

  return $export;
}
