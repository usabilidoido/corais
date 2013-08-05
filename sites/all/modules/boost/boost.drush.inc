<?php

/**
 * @file
 * Drush commands for Boost.
 */

/**
 * Implementation of hook_drush_help().
 */
function boost_drush_help($section) {
  switch ($section) {
    case 'drush:boost-cache-clear-all':
      return dt('Clears all Boost cached data.');
    case 'drush:boost-cache-clear-expired':
      return dt('Clears Boost expired data.');
    case 'drush:boost-reset':
      return dt("Clears Boost's database and file cache.");
  }
}

/**
 * Implementation of hook_drush_command().
 */
function boost_drush_command() {
  $items['boost-cache-clear-all'] = array(
    'callback' => 'boost_drush_cache_clear_all',
    'description' => dt('Clears all Boost cached data.'),
  );
  $items['boost-cache-clear-expired'] = array(
    'callback' => 'boost_drush_cache_clear_expired',
    'description' => dt('Clears Boost expired data.'),
  );
  $items['boost-reset'] = array(
    'callback' => 'boost_drush_cache_reset',
    'description' => dt("Clears Boost's database and file cache."),
  );
  return $items;
}

/**
 * Clears all Boost cached data.
 */
function boost_drush_cache_clear_all() {
  module_load_include('inc', 'boost', 'boost.admin');
  $ignore = variable_get('boost_ignore_flush', 0);
  $GLOBALS['conf']['boost_ignore_flush'] = 0;
  if (boost_cache_clear_all()) {
    boost_clear_cache_parallel(BOOST_PERM_FILE_PATH);
    boost_clear_cache_parallel(BOOST_PERM_GZIP_FILE_PATH);
    print drush_log(dt('Boost: Static page cache cleared.'), 'ok');
  }
  else {
    print drush_log(dt('Boost: Static page cache not cleared.'), 'error');
  }
  $GLOBALS['conf']['boost_ignore_flush'] = $ignore;
}

/**
 * Clears Boost expired data.
 */
function boost_drush_cache_clear_expired() {
  $ignore = variable_get('boost_ignore_flush', 0);
  $GLOBALS['conf']['boost_ignore_flush'] = 0;
  if (boost_cache_expire_all()) {
    print drush_log(dt('Boost: Expired stale files from static page cache.'), 'ok');
  }
  else {
    print drush_log(dt('Boost: Expired stale files from static page cache NOT cleared'), 'error');
  }
  $GLOBALS['conf']['boost_ignore_flush'] = $ignore;
}

/**
 * Clears Boost's database and file cache.
 */
function boost_drush_cache_reset() {
  $ignore = variable_get('boost_ignore_flush', 0);
  $GLOBALS['conf']['boost_ignore_flush'] = 0;
  if (boost_cache_clear_all()) {
    db_query("TRUNCATE {boost_cache}");
    db_query("TRUNCATE {boost_cache_relationships}");
    db_query("TRUNCATE {boost_crawler}");
    _boost_rmdir_rf(BOOST_ROOT_CACHE_DIR, TRUE, TRUE, TRUE);
    print drush_log(dt('Boost: Static page cache & 3 database tables cleared.'), 'ok');
  }
  else {
    print drush_log(dt('Boost: Static page cache & database tables NOT cleared'), 'error');
  }
  $GLOBALS['conf']['boost_ignore_flush'] = $ignore;
}
