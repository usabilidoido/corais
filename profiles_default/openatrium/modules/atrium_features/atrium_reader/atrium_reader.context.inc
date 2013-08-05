<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function atrium_reader_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'atrium_reader';
  $context->description = 'Featurewide context for Atrium Reader.';
  $context->tag = 'atrium_reader';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'feed_reader' => 'feed_reader',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'reader' => 'reader',
        'reader/*' => 'reader/*',
      ),
    ),
    'views' => array(
      'values' => array(
        'feed_reader_listing' => 'feed_reader_listing',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'reader',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Featurewide context for Atrium Reader.');
  t('atrium_reader');
  $export['atrium_reader'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'atrium_reader_listing';
  $context->description = 'Single feed item listing page within the reader.';
  $context->tag = 'atrium_reader';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'feed_reader' => 'feed_reader',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'views' => array(
      'values' => array(
        'feed_reader_listing:page_1' => 'feed_reader_listing:page_1',
        'feed_reader_listing:page_2' => 'feed_reader_listing:page_2',
        'feed_reader_listing:page_3' => 'feed_reader_listing:page_3',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-feed_reader_feeds-block_1' => array(
          'module' => 'views',
          'delta' => 'feed_reader_feeds-block_1',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Single feed item listing page within the reader.');
  t('atrium_reader');
  $export['atrium_reader_listing'] = $context;

  return $export;
}
