<?php

/**
 * Implementation of hook_feeds_importer_default().
 */
function atrium_reader_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'feed_reader';
  $feeds_importer->config = array(
    'name' => 'Reader feed',
    'description' => 'Feed configuration for reader feeds.',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsDataProcessor',
      'config' => array(
        'update_existing' => 1,
        'expire' => '7257600',
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'description',
            'target' => 'description',
            'unique' => 0,
          ),
          2 => array(
            'source' => 'author_name',
            'target' => 'author',
            'unique' => 0,
          ),
          3 => array(
            'source' => 'guid',
            'target' => 'options_raw_guid',
            'unique' => 1,
          ),
          4 => array(
            'source' => 'url',
            'target' => 'options_original_url',
            'unique' => 1,
          ),
          5 => array(
            'source' => 'timestamp',
            'target' => 'timestamp',
            'unique' => 0,
          ),
        ),
      ),
    ),
    'content_type' => 'feed_reader',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
  );
  $export['feed_reader'] = $feeds_importer;

  return $export;
}
