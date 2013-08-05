<?php

/**
 * @file
 * The SSH Backend for the plugin manager.
 */

/**
 * Implementation of user hook plugin_manager_backend.  Checks
 * if the requirements for the ssh backend are enabled.
 *
 * @return
 *  Return 'ssh' if the requirements are available.
 */
function ssh_plugin_manager_backend() {
  if (function_exists('ssh2_connect')) {
    return 'ssh';
  }
}

/**
 * Install the supplied files to the appropriate locations.
 *
 * @param $files
 *   The files to install.
 * @param $type
 *   Either 'module' or 'theme' to indicate where to install to.
 * @param $host
 *   Server (usually localhost)
 * @param $username
 *   Username
 * @param $password
 *   Password
 * @return
 *  TRUE if all the files were installed, FALSE otherwise.
 */

function ssh_plugin_manager_copy($files, $type, $host, $username, $password) {
  // Figure out the type of files.
  if (!in_array($type, array('modules', 'themes'))) {
    return FALSE;
  }
  $dir = 'sites/all/'. drupal_strtolower($type);

  // Connect to the server via ssh, parsing for non-standard port.
  $parts = explode(':', $host);
  if (count($parts)==2)
      $connection = ssh2_connect($parts[0], $parts[1]+0);
  else
      $connection = ssh2_connect($host);

  if ($connection == FALSE) {
    drupal_set_message(t('Could not connect to ssh on this host.'), 'error');
    return FALSE;
  }

  if (!ssh2_auth_password($connection, $username, $password)) {
    drupal_set_message(t('The supplied username/password combination '
                      .'was not accepted.'), 'error');
    return FALSE;
  }

  // Prepare the directories to use later.
  $extract_dir = file_directory_path() .'/plugin_manager_extraction';


  // Try to guess the ftp address for the drupal install.
  // Start by putting the entire address together and replacing \ with /.
  $local_path = rtrim(str_replace('\\', '/', $_SERVER['DOCUMENT_ROOT'] . base_path()), '/');

  // Process each of the files.
  foreach ($files AS $file) {
    if (trim($file, "\\/") != $file) {
      ssh2_exec($connection, "mkdir ". escapeshellarg("$local_path/$dir/$file"));
    }
    else {
      ssh2_scp_send($connection, "$extract_dir/$file", "$local_path/$dir/$file");
    }
  }

  return TRUE;
}

/**
 * Remove the supplied files.
 *
 * @param $files
 *   The files to remove.
 * @param $host
 *   Server (usually localhost)
 * @param $username
 *   Username
 * @param $password
 *   Password
 * @return
 *  TRUE if all the files were installed, FALSE otherwise.
 */

function ssh_plugin_manager_remove($files, $type, $host, $username, $password) {
  $dir = 'sites/all';

  // Connect to the server via ssh, parsing for non-standard port.
  $parts = explode(':', $host);
  if (count($parts)==2)
      $connection = ssh2_connect($parts[0], $parts[1]+0);
  else
      $connection = ssh2_connect($host);

  if ($connection == FALSE) {
    drupal_set_message(t('Could not connect to ssh on this host.'), 'error');
    return FALSE;
  }

  if (!ssh2_auth_password($connection, $username, $password)) {
    drupal_set_message(t('The supplied username/password combination '
                      .'was not accepted.'), 'error');
    return FALSE;
  }


  // Try to guess the ftp address for the drupal install.
  // Start by putting the entire address together and replacing \ with /.
  $local_path = rtrim(str_replace('\\', '/', $_SERVER['DOCUMENT_ROOT'] . base_path()), '/');

  // Process each of the files.
  foreach ($files AS $file) {
    if (trim($file, "\\/") != $file) {
      ssh2_exec($connection, "rmdir ". escapeshellarg("$local_path/$dir/$file"));
    }
    else {
      ssh2_exec($connection, "rm ". escapeshellarg("$local_path/$dir/$file"));
    }
  }

  return TRUE;
}
