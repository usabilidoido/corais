<?php
// $Id: xmlsitemap.pages.inc,v 1.1.4.5 2009/09/06 16:28:48 davereid Exp $


/**
 * @file
 * XML sitemap page callbacks.
 */

/**
 * @addtogroup xmlsitemap
 * @{
 */

/*****************************************************************************
 * Menu callbacks / form builders, submit/validate functions.
 ****************************************************************************/

/**
 * Menu callback; display the sitemap.
 *
 * @param $chunk
 *  An integer specifying which chunk of the sitemap is being requested.
 *  If not set and there is more than one chunk, display the sitemap index.
 */
function xmlsitemap_output($chunk = NULL) {
  global $user;
  $chunk_size = variable_get('xmlsitemap_chunk_size', XMLSITEMAP_DEFAULT_SITEMAP_LINKS);
  $link_count = xmlsitemap_link_count();
  $chunk_count = xmlsitemap_chunk_count();
  // A sitemap may only have a set number of index links (links to other sitemap
  // files) and a set number of site links in each of the "other" sitemap files.
  // This code adjusts the number of site links specified by the user if the
  // set number of index links has been reached but will not set that number
  // greater than the set number of site links in a file.
  if ($chunk_count > XMLSITEMAP_MAX_SITEMAP_INDEX_LINKS &&
    $chunk_size < XMLSITEMAP_MAX_SITEMAP_LINKS) {
    // Determine the number of chunks we are over the maximum index links.
    $chunk_adjust = $chunk_count - XMLSITEMAP_MAX_SITEMAP_INDEX_LINKS;
    // Determine the number of links to adjust the chunk size.
    $chunk_adjust = $chunk_adjust * $chunk_size;
    if (($chunk_size + $chunk_adjust) <= XMLSITEMAP_MAX_SITEMAP_LINKS) {
      $chunk_size += $chunk_adjust;
    }
    else {
      $chunk_size = XMLSITEMAP_MAX_SITEMAP_LINKS;
    }
    variable_set('xmlsitemap_chunk_size', $chunk_size);
    watchdog('xmlsitemap', 'Chunk size has been updated to @chunk-size.', array('@chunk-size' => $chunk_size));
  }
  elseif($chunk_count > XMLSITEMAP_MAX_SITEMAP_INDEX_LINKS) {
    watchdog('xmlsitemap', 'The maximum number of allowed links has been reached.', NULL, WATCHDOG_ERROR);
  }
  if (isset($chunk) && !is_numeric($chunk)) {
    drupal_not_found();
    exit();
  }
  $id = xmlsitemap_language_id();
  if (variable_get('xmlsitemap_sitemap_needs_update', FALSE)) {
    variable_set('xmlsitemap_update_timestamp', REQUEST_TIME);
    db_query("DELETE FROM {xmlsitemap} WHERE type ='frontpage'");
    $row             = new stdClass();
    $row->module     = 'xmlsitemap';
    $row->type       = 'frontpage';
    $changefreq      = variable_get('xmlsitemap_front_page_changefreq', 3600);
    $row->changed    = REQUEST_TIME - $changefreq;
    $row->changefreq = $changefreq;
    $row->priority   = variable_get('xmlsitemap_front_page_priority', 1);
    drupal_write_record('xmlsitemap', $row);
    module_invoke_all('xmlsitemap_links');
    variable_set('xmlsitemap_sitemap_needs_update', FALSE);
    $result = _xmlsitemap_create_cache_files();
    if (variable_get("xmlsitemap_create_cache_result_$id", -1) !== $result) {
      variable_set("xmlsitemap_create_cache_result_$id", $result);
    }
  }
  elseif (_xmlsitemap_check_cache_files()) {
    $result = _xmlsitemap_create_cache_files();
    if (variable_get("xmlsitemap_create_cache_result_$id", -1) !== $result) {
      variable_set("xmlsitemap_create_cache_result_$id", $result);
    }
  }
  if (!isset($chunk)) {
    if (($chunks = (integer) $link_count / $chunk_size) != variable_get('xmlsitemap_previous_chunks_count', -1)) {
      variable_set('xmlsitemap_previous_chunks_count', $chunks);
      if (!variable_get('menu_rebuild_needed', FALSE)) {
        variable_set('menu_rebuild_needed', TRUE);
      }
    }
  }
  if (isset($result) && !$result) {
    drupal_not_found();
    exit();
  }
  $parent_directory = variable_get('xmlsitemap_cache_directory', file_directory_path() .'/xmlsitemap');
  $headers = array("Content-type: text/xml; charset=utf-8");
  if (isset($chunk)) {
    if ($chunk < $link_count / $chunk_size) {
      file_transfer("$parent_directory/xsm-$id-$chunk.xml", $headers);
    }
  }
  else {
    file_transfer("$parent_directory/xsm-$id.xml", $headers);
  }
}

/*****************************************************************************
 * Private functions.
 ****************************************************************************/

/**
 * Check the cache files.
 *
 * @return
 *   TRUE if the cache files must be updated / created, FALSE otherwise.
 */
function _xmlsitemap_check_cache_files() {
  $chunk_size       = variable_get('xmlsitemap_chunk_size', 1000);
  $link_count       = xmlsitemap_link_count();
  $id               = xmlsitemap_language_id();
  $parent_directory = variable_get('xmlsitemap_cache_directory', file_directory_path() .'/xmlsitemap');
  // If the directory that should contains the cache files doesn't exist, then
  // the cache files must be created.
  if (!file_check_directory($parent_directory, FILE_CREATE_DIRECTORY)) {
    return TRUE;
  }
  $update_timestamp = variable_get('xmlsitemap_update_timestamp', REQUEST_TIME);
  // If the cache files creation has failed last time, the cache files must be
  // created.
  if (variable_get("xmlsitemap_create_cache_result_$id", -1) !== TRUE) {
    return TRUE;
  }
  // If the main cache file doesn't exist, then the cache files must be
  // created.
  if (!file_exists($parent_directory ."/xsm-$id.xml")) {
    return TRUE;
  }
  // If the main cache file has been created before the sitemap content has
  // been updated, then the cache files must be updated.
  if (filemtime($parent_directory ."/xsm-$id.xml") < $update_timestamp) {
    return TRUE;
  }
  // Check also the other cache files.
  if ($link_count > $chunk_size) {
    for ($chunk = 0; $chunk < $link_count / $chunk_size; ++$chunk) {
      if (!file_exists($parent_directory ."/xsm-$id-$chunk.xml")) {
        return TRUE;
      }
      if (filemtime($parent_directory ."/xsm-$id-$chunk.xml") < $update_timestamp) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Create a sitemap chunk cache file.
 *
 * @param $fp
 *  A file resource used to write in.
 * @param $chunk_size
 *  The number of links the chunk must cointain.
 * @param $chunk
 *  The progressive number associated with the sitemap chunk (starting from
 *  0).
 */
function _xmlsitemap_create_cache_chunk($fp, $chunk_size, $chunk = 0) {
  fwrite($fp, '<?xml version="1.0" encoding="UTF-8"?>' . "\n");
  if ($xsl = _xmlsitemap_xsl()) {
    fwrite($fp, '<?xml-stylesheet type="text/xsl" href="' . $xsl . '" ?>' . "\n");
  }
  fwrite($fp, '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9"' . "\n");
  fwrite($fp, '  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"' . "\n");
  fwrite($fp, '  xsi:schemaLocation="http://www.sitemaps.org/schemas/sitemap/0.9' . "\n");
  fwrite($fp, '  http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd">' . "\n");
  $start = $chunk * $chunk_size;
  $links = db_query_range("SELECT xsm.loc, xsm.module, xsm.type, xsm.id, xsm.sid, xsm.changed, xsm.changefreq, xsm.priority". xmlsitemap_sitemap_query() ."ORDER BY xsm.priority DESC, xsm.changed DESC, xsm.changefreq, xsm.loc", $start, $chunk_size);
  while ($link = db_fetch_object($links)) {
    if ($link->type == 'frontpage') {
      $url = url(NULL, array('absolute' => TRUE));
    }
    else {
      $url = url($link->loc, array('absolute' => TRUE));
    }
    $link->url = $url;
    if ($link->module && function_exists($link->module .'_xmlsitemap_link_status')) {
      $function = $link->module .'_xmlsitemap_link_status';
      $link->status = $function($link->type, $link->id, $link->sid);
    }
    else {
      $link->status = 0;
    }
    drupal_alter('xmlsitemap_data', $link);
    if (($link->status & XMLSITEMAP_LINK_DISABLED) != XMLSITEMAP_LINK_DISABLED) {
      $link->output = "<url>\n";
      $link->output .= "\t<loc>". check_url($link->url) ."</loc>\n";
      $link->output .= "\t<lastmod>". gmdate(DATE_W3C, $link->changed) ."</lastmod>\n";
      $link->output .= "\t<changefreq>". xmlsitemap_sitemap_frequency($link->changefreq) ."</changefreq>\n";
      $link->output .= "\t<priority>". number_format($link->priority, 1) ."</priority>\n";
      $link->output .= "</url>\n";
      fwrite($fp, $link->output);
    }
  }
  fwrite($fp, '</urlset>');
}

/**
 * Determine path for a xml stylesheet.
 *
 * @return
 * - The stylesheet path or FALSE.
 */
function _xmlsitemap_xsl() {
  if (variable_get('xmlsitemap_use_stylesheet', FALSE)) {
    $paths = array(
      variable_get('xmlsitemap_cache_directory', file_directory_path() . '/xmlsitemap') . '/gss.xsl',
      drupal_get_path('module', 'xmlsitemap') . '/gss/gss.xsl',
    );
    foreach ($paths as $path) {
      if (file_exists($path)) {
        return base_path() . $path;
      }
    }
  }
  return FALSE;
}

/**
 * Create the cache files containing the sitemap.
 *
 * @return
 *  TRUE if the operation has been successfull, FALSE otherwise.
 */
function _xmlsitemap_create_cache_files() {
  $chunk_size       = variable_get('xmlsitemap_chunk_size', 1000);
  $link_count       = xmlsitemap_link_count();
  $id               = xmlsitemap_language_id();
  $parent_directory = variable_get('xmlsitemap_cache_directory', file_directory_path() .'/xmlsitemap');
  // If the directory doesn't exist, then create it.
  if (!file_check_directory($parent_directory, FILE_CREATE_DIRECTORY)) {
    return TRUE;
  }
  if ($link_count > $chunk_size) {
    if (!$fp = fopen($parent_directory ."/xsm-$id.xml", 'wb')) {
      watchdog('xmlsitemap', 'Could not create the cache file @file.', array('@file' => $parent_directory ."/xsm-$id.xml"), WATCHDOG_ERROR);
      return FALSE;
    }
    fwrite($fp, '<?xml version="1.0" encoding="UTF-8"?>' . "\n");
    if ($xsl = _xmlsitemap_xsl()) {
      fwrite($fp, '<?xml-stylesheet type="text/xsl" href="' . $xsl . '" ?>' . "\n");
    }
    fwrite($fp, '<sitemapindex xmlns="http://www.sitemaps.org/schemas/sitemap/0.9"' . "\n");
    fwrite($fp, '  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"' . "\n");
    fwrite($fp, '  xsi:schemaLocation="http://www.sitemaps.org/schemas/sitemap/0.9' . "\n");
    fwrite($fp, '  http://www.sitemaps.org/schemas/sitemap/0.9/siteindex.xsd">' . "\n");
    for ($chunk = 0; $chunk < $link_count / $chunk_size; ++$chunk) {
      fwrite($fp, '  <sitemap>' . "\n");
      fwrite($fp, '    <loc>'. url("sitemap$chunk.xml", array('absolute' => TRUE)) .'</loc>' . "\n");
      $from = $chunk * $chunk_size;
      $changed = db_result(db_query_range("SELECT xsm.changed". xmlsitemap_sitemap_query() ."ORDER BY xsm.priority DESC, xsm.changed DESC", $from, $chunk_size));
      fwrite($fp, '    <lastmod>'. gmdate(DATE_W3C, $changed) .'</lastmod>' . "\n");
      fwrite($fp, '  </sitemap>' . "\n");
    }
    fwrite($fp, '</sitemapindex>');
    fclose($fp);
    // Set standard file permissions for webserver-generated files.
    @chmod($parent_directory ."/xsm-$id.xml", 0664);
    for ($chunk = 0; $chunk < $link_count / $chunk_size; ++$chunk) {
      if (!$fp = fopen($parent_directory ."/xsm-$id-$chunk.xml", 'wb')) {
        watchdog('xmlsitemap', 'Could not create the cache file @file.', array('@file' => $parent_directory ."/xsm-$id-$chunk.xml"), WATCHDOG_ERROR);
        return FALSE;
      }
      _xmlsitemap_create_cache_chunk($fp, $chunk_size, $chunk);
      fclose($fp);
      // Set standard file permissions for webserver-generated files.
      @chmod($parent_directory ."/xsm-$id-$chunk.xml", 0664);
    }
  }
  else {
    if (!$fp = fopen($parent_directory ."/xsm-$id.xml", 'wb')) {
      watchdog('xmlsitemap', 'Could not create the cache file @file.', array('@file' => $parent_directory ."/xsm-$id.xml"), WATCHDOG_ERROR);
      return FALSE;
    }
    _xmlsitemap_create_cache_chunk($fp, $chunk_size);
    fclose($fp);
    // Set standard file permissions for webserver-generated files.
    @chmod($parent_directory ."/xsm-$id.xml", 0664);
  }
  return TRUE;
}

/**
 * @} End of "addtogroup xmlsitemap".
 */
