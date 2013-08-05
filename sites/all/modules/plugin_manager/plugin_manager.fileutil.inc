<?php

/**
 * Attempts to get a file using drupal_http_request and to store it locally.
 *
 * @param $path
 *   The URL of the file to grab.
 * @return
 *   On success the address the files was saved to, FALSE on failure.
 */
function plugin_manager_get($path) {
  // Get each of the specified files.
  $parsed_url = parse_url($path);
  $local = file_directory_path() ."/plugin_manager_cache/". basename($parsed_url['path']);
  $todir = file_directory_path() ."/plugin_manager_cache/";
  if (!file_check_directory($todir)){
    drupal_set_message(t("The directory @path either does not exist or is not writeable. Please create it manually.",array('@path'=>$todir)),'error');
    return FALSE;
  }
  // Check the cache and download the file if needed
  if (!file_exists($local)) {

    // $result->data is the actual contents of the downloaded file.  This saves it
    // into a local file, whose path is stored in $local.  $local is stored relative
    // to the drupal installation.
    $result = drupal_http_request($path);
    if ($result->code != 200 OR !file_save_data($result->data, $local)) {
      drupal_set_message(t('@remote could not be saved.', array('@remote' => $path)), 'error');
      return FALSE;
    }
  }
  return $local;
}

/**
 * Untars a file.
 *
 * @param $file
 *   The file to untar.
 * @return
 *   An array containing the locations of the extracted files.
 */
function plugin_manager_untar($file) {
  $dir = file_directory_path() .'/plugin_manager_extraction/';
  $file_safe = escapeshellarg($file);
  $dir_safe  = escapeshellarg($dir);
  $file_list = array();

  // Try to use tar to extract the files.
  if (function_exists('popen')) {
    $handle = popen("tar -zvxf $file_safe -C $dir_safe", "r");
    while ($line = fgets($handle)) {
      $file_list[] = trim($line);
    }
    pclose($handle);
  }

  // If tar returned something (a.k.a, is present) then return it
  if (!empty($file_list)) {
    return $file_list;
  }

  // If we get this far then tar failed.  Attempt to extract using PEAR's Archive/Tar.
  // This should still be GPL safe since this modules does not depend on PEAR.
  @include_once("Archive/Tar.php");

  // If it doesn't exist either, then we can't explode the files.
  if (!class_exists("Archive_Tar")) {
    drupal_set_message(t('Neither the tar executable nor the Archive/Tar package could be located.'), "error");
    return FALSE;
  }

  // Attempt to explode the file with PEAR.
  $tar = new Archive_Tar($file);
  $tar->extract($dir);

  // Get a list of the extracted files.
  foreach ($tar->listcontent() AS $file) {
    $file_list[] = trim($file['filename']);
  }

  // Return the list
  return $file_list;
}
