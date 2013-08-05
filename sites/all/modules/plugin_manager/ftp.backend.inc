<?php

/**
 * @file
 * The FTP Backend for the plugin manager.
 */

/**
 * Implementation of user hook plugin_manager_settings.  Checks
 * if the requirements for the ftp backend are enabled.
 *
 * @return
 *  Return form array of backend configuration form.
 */
function ftp_plugin_manager_settings() {
  $form = array();
  $form['plugin_manager_ftp_path'] = array(
    '#title' => t('FTP Path'),
    '#description' => t('This is the apparent path to your Drupal installation for FTP.  Leave this blank if you wish for the system to attempt to guess the correct location.<br />(ex: If the Drupal installation is at /home/user1/www and your ftp home is /home/user1 then /www would be placed here.)'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_ftp_path', ''),
  );

  return $form;
}

/**
 * Implementation of user hook plugin_manager_backend.  Checks
 * if the requirements for the ftp backend are enabled.
 *
 * @return
 *  Return 'ftp' if the requirements are available.
 */
function ftp_plugin_manager_backend() {
  if (function_exists('ftp_connect') OR ini_get('allow_url_fopen')) {
    return 'ftp';
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
 *   FTP server (usually localhost)
 * @param $username
 *   FTP Username
 * @param $password
 *   FTP Password
 * @return
 *  TRUE if all the files were installed, FALSE otherwise.
 */
function ftp_plugin_manager_copy($files, $type, $host, $username, $password) {
  if (function_exists('ftp_connect')) {
    return ftp_plugin_manager_copy_library($files, $type, $host, $username, $password);
  }
  return ftp_plugin_manager_copy_wrapper($files, $type, $host, $username, $password);
}

/**
 * Install the supplied files to the appropriate locations using ftp without
 * the stream wrapper.
 *
 * @param $files
 *   The files to install.
 * @param $type
 *   Either 'module' or 'theme' to indicate where to install to.
 * @param $host
 *   FTP server (usually localhost)
 * @param $username
 *   FTP Username
 * @param $password
 *   FTP Password
 * @return
 *  TRUE if all the files were installed, FALSE otherwise.
 */
function ftp_plugin_manager_copy_library($files, $type, $host, $username, $password) {
  // Figure out the type of files.
  if (!in_array($type, array("modules", "themes"))) {
    drupal_set_message(t("Cannot install plugin of type @type", array('@type' => $type)), "error");
    return FALSE;
  }
  $dir = 'sites/all/'. drupal_strtolower($type);

  // Connect to the local ftp server, parsing for non-standard port.
  $parts = explode(':', $host);
  if (count($parts)==2)
      $connect = ftp_connect($parts[0], $parts[1]+0);
  else
      $connect = ftp_connect($host);
  if (!$connect) {
    drupal_set_message(t('No ftp server could be found.'), 'error');
    return FALSE;
  }

  // Login to the local ftp server.
  if (!@ftp_login($connect, $username, $password)) {
    drupal_set_message(t('Could not login to the ftp server.'), 'error');
    return FALSE;
  }

  // Try to guess the ftp address for the drupal install.
  // Start by putting the entire address together and replacing \ with /.
  $local_path = str_replace('\\', '/', $_SERVER['DOCUMENT_ROOT'] . base_path());

  // Grab the last on off the end and explode it.
  $local_path = explode('/', rtrim($local_path, '/'));

  $ftp_path = variable_get('plugin_manager_ftp_path', '');

  // Try to guess which how far in we are chrooted...
  if (empty($ftp_path)) {
    foreach ($local_path AS $index => $value) {
      unset($local_path[$index]);
      if (@ftp_nlist($connect, implode('/', $local_path) .'/'. $dir)) {
        $ftp_path = implode('/', $local_path) .'/'. $dir;
        drupal_set_message(t('A drupal install was automatically located on ftp at @ftp_path.', array('@ftp_path' => $ftp_path)));
        break;
      }
    }
  }
  else {
    $ftp_path = $ftp_path .'/'. $dir;
    if (!@ftp_chdir($connect, $ftp_path)) {
      drupal_set_message(t('Your provided drupal install directory is invalid.') . l(t('Change it here.'), 'admin/plugin_manager/settings'),  'error');
      return FALSE;
    }
  }

  // If we couldn't guess it, then quit.
  if (!isset($ftp_path) || !@ftp_chdir($connect, $ftp_path)) {
    drupal_set_message(l(t('Could not guess the ftp directory for drupal.  Set it here.'), 'admin/plugin_manager/settings'), 'error');
    return FALSE;
  }

  // Prepare the directories to use later.
  $extract_dir = file_directory_path() .'/plugin_manager_extraction/';
  ftp_chdir($connect, "/");
  // Process each of the files.
  foreach ($files AS $index => $file) {
    ftp_chdir($connect, "/");
    if (trim($file, "\\/") != $file) {
      if (!@ftp_mkdir($connect, "$ftp_path/$file") && !@ftp_chdir($connect, "$ftp_path/$file")) {
        drupal_set_message(t("Unable to create directory @file", array('@file' => $file)), 'error');
        return FALSE;
      }
    }
    else {
      if (!@ftp_put($connect, "$ftp_path/$file", $extract_dir . $file, FTP_BINARY)) {
        drupal_set_message(t("Unable to upload @file", array('@file' => $file)), 'error');
        return FALSE;
      }
    }
  }
  return TRUE;
}

/**
 * Install the supplied files to the appropriate locations using the ftp
 * stream wrapper.
 *
 * @param $files
 *   The files to install.
 * @param $type
 *   Either 'module' or 'theme' to indicate where to install to.
 * @param $host
 *   FTP server (usually localhost)
 * @param $username
 *   FTP Username
 * @param $password
 *   FTP Password
 * @return
 *  TRUE if all the files were installed, FALSE otherwise.
 */
function ftp_plugin_manager_copy_wrapper($files, $type, $host, $username, $password) {
  // Figure out the type of files.
  if (!in_array($type, array('modules', 'themes'))) {
    drupal_set_message(t("Cannot install plugin of type @type", array('@type' => $type)), "error");
    return FALSE;
  }
  $dir = 'sites/all/'. drupal_strtolower($type);

  // Write the common part of the url
  $ftp_base_dir = "ftp://". urlencode($username) .":". urlencode($password) ."@$host/";

  if (!@is_dir($ftp_base_dir)) {
    drupal_set_message(t('The supplied username/password combination was not accepted, or no local ftp server is running.'), 'error');
    return FALSE;
  }

  // Try to guess the ftp address for the drupal install.
  // Start by putting the entire address together and replacing \ with /.
  $local_path = str_replace('\\', '/', $_SERVER['DOCUMENT_ROOT'] . base_path());

  // Grab the last on off the end and explode it.
  $local_path = explode('/', rtrim($local_path, '/'));

  $ftp_path = variable_get('plugin_manager_ftp_path', '');

  // Try to guess which how far in we are chrooted...
  if (empty($ftp_path)) {
    foreach ($local_path AS $index => $value) {
      unset($local_path[$index]);
      if (is_dir($ftp_base_dir . implode('/', $local_path) .'/'. $dir)) {
        $ftp_path = $ftp_base_dir . implode('/', $local_path) .'/'. $dir;
        break;
      }
    }
  }
  else {
    $ftp_path = $ftp_base_dir . $ftp_path .'/'. $dir;
  }

  // If we couldn't guess it, then quit.
  if (!isset($ftp_path) || !is_dir($ftp_path)) {
    drupal_set_message(t('Could not guess the ftp directory for drupal.'), 'error');
    return FALSE;
  }

  // Prepare the directories to use later.
  $extract_dir = file_directory_path() .'/plugin_manager_extraction/';

  // Process each of the files.
  foreach ($files AS $index => $file) {
    if (trim($file, "\\/") != $file) {
      @mkdir("$ftp_path/$file");
    }
    else {
      @copy($extract_dir . $file, "$ftp_path/$file");
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
 *   FTP server (usually localhost)
 * @param $username
 *   FTP Username
 * @param $password
 *   FTP Password
 * @return
 *  TRUE if all the files were removed, FALSE otherwise.
 */
function ftp_plugin_manager_remove($files, $host, $username, $password) {
  if (function_exists('ftp_connect')) {
    return ftp_plugin_manager_remove_library($files, $host, $username, $password);
  }
  return ftp_plugin_manager_remove_wrapper($files, $host, $username, $password);
}

/**
 * Remove the files using ftp without
 * the stream wrapper.
 *
 * @param $folder
 *   The folder to remove.
 * @param $host
 *   FTP server (usually localhost)
 * @param $username
 *   FTP Username
 * @param $password
 *   FTP Password
 * @return
 *  TRUE if the folder was removed, FALSE otherwise.
 */
function ftp_plugin_manager_remove_library($folder, $host, $username, $password) {
  $dir = '';//sites/all';

  // Connect to the local ftp server.
  $connect = ftp_connect($host);
  if (!$connect) {
    drupal_set_message(t('No ftp server could be found.'), 'error');
    return FALSE;
  }

  // Login to the local ftp server.
  if (!@ftp_login($connect, $username, $password)) {
    drupal_set_message(t('Could not login to the ftp server.'), 'error');
    return FALSE;
  }

  // Try to guess the ftp address for the drupal install.
  // Start by putting the entire address together and replacing \ with /.
  $local_path = str_replace('\\', '/', $_SERVER['DOCUMENT_ROOT'] . base_path());

  // Grab the last on off the end and explode it.
  $local_path = explode('/', rtrim($local_path, '/'));

  $ftp_path = variable_get('plugin_manager_ftp_path', '');

  // Try to guess which how far in we are chrooted...
  if (empty($ftp_path)) {
    foreach ($local_path AS $index => $value) {
      unset($local_path[$index]);
      if (ftp_nlist($connect, implode('/', $local_path) .'/'. $dir)) {
        $ftp_path = implode('/', $local_path) .'/'. $dir;
        drupal_set_message(t('A drupal install was automatically located on ftp at @ftp_path.', array('@ftp_path' => $ftp_path)));
        break;
      }
    }
  }
  else {
    $ftp_path = $ftp_path .'/'. $dir;
    if (!@ftp_chdir($connect, $ftp_path)) {
      drupal_set_message(t('Your provided drupal install directory is invalid.') . l(t('Change it here.'), 'admin/plugin_manager/settings'),  'error');
      return FALSE;
    }
  }

  // If we couldn't guess it, then quit.
  if (!isset($ftp_path) || !@ftp_chdir($connect, $ftp_path)) {
    drupal_set_message(t('Could not guess the ftp directory for drupal.'), 'error');
    return FALSE;
  }
  ftp_chdir($connect, "/");
  if (ftp_plugin_manager_rmdir($connect, "$ftp_path/$folder"))
    return TRUE;
  return FALSE;
}

function ftp_plugin_manager_rmdir($connect, $dir) {
    $pwd = ftp_pwd($connect);
    if (!@ftp_chdir($connect, $dir))
        return FALSE;
    $list = @ftp_nlist($connect, '.');
    foreach ($list as $item) {
        if ($item=='.' || $item=='..') continue;
        if (@ftp_chdir($connect, $item)) {
            ftp_chdir($connect, '..');
            if (!ftp_plugin_manager_rmdir($connect, $item)) return FALSE;
        }
        elseif (!ftp_delete($connect, $item)) return FALSE;
    }
    ftp_chdir($connect, $pwd);
    if (!ftp_rmdir($connect, $dir)) return FALSE;
    return TRUE;
}

function ftp_is_dir($dir) {
  global $ftp_connect;
  if (ftp_chdir($ftp_connect, basename($dir))) {
    ftp_chdir($ftp_connect, '..');
    return TRUE;
  }
  else {
    return FALSE;
  }
}

function ftp_plugin_manager_exists($connect, $file) {
    $pwd = ftp_pwd($connect);
    $dir = dirname($file);
    $name = basename($file);
    if (!@ftp_chdir($connect, $dir)) return FALSE;
    $list = @ftp_nlist($connect, '.');
    ftp_chdir($connect, $pwd);
    foreach ($list as $item) {
        if ($item==$name) return TRUE;
    }
    return FALSE;
}

/**
 * Remove the supplied folder using the ftp
 * stream wrapper.
 *
 * @param $folder
 *   The files to remove.
 * @param $host
 *   FTP server (usually localhost)
 * @param $username
 *   FTP Username
 * @param $password
 *   FTP Password
 * @return
 *  TRUE if all the files were removed, FALSE otherwise.
 */
function ftp_plugin_manager_remove_wrapper($folder, $host, $username, $password) {
  $dir = '';

  // Write the common part of the url
  $ftp_base_dir = "ftp://". urlencode($username) .":". urlencode($password) ."@$host/";

  if (!@is_dir($ftp_base_dir)) {
    drupal_set_message(t('The supplied username/password combination was not accepted, or no ftp server is running.'), 'error');
    return FALSE;
  }

  // Try to guess the ftp address for the drupal install.
  // Start by putting the entire address together and replacing \ with /.
  $local_path = str_replace('\\', '/', $_SERVER['DOCUMENT_ROOT'] . base_path());

  // Grab the last on off the end and explode it.
  $local_path = explode('/', rtrim($local_path, '/'));

  $ftp_path = variable_get('plugin_manager_ftp_path', '');

  // Try to guess which how far in we are chrooted...
  if (empty($ftp_path)) {
    foreach ($local_path AS $index => $value) {
      unset($local_path[$index]);
      if (is_dir($ftp_base_dir . implode('/', $local_path) .'/'. $dir)) {
        $ftp_path = $ftp_base_dir . implode('/', $local_path) .'/'. $dir;
        break;
      }
    }
  }
  else {
    $ftp_path = $ftp_base_dir . $ftp_path .'/'. $dir;
  }

  // If we couldn't guess it, then quit.
  if (!isset($ftp_path) || !is_dir($ftp_path)) {
    drupal_set_message(t('Could not guess the ftp directory for drupal.'), 'error');
    return FALSE;
  }
  // Process each of the files.
  return ftp_plugin_manager_rmdir_stream("$ftp_path/$file");
}

function ftp_plugin_manager_rmdir_stream( $_target ) {
    if (is_dir($_target)) {
        if (is_writeable($_target)) {
            foreach (new DirectoryIterator($_target) as $_res) {
                if ($_res->isDot()) {
                    unset($_res);
                    continue;
                }
                if ($_res->isFile()) {
                    ftp_plugin_manager_rmdir_stream($_res->getPathName());
                }
                elseif ($_res->isDir()) {
                    ftp_plugin_manager_rmdir_stream($_res->getRealPath());
                }
                unset($_res);
            }
            if (@rmdir($_target)) {
                return TRUE;
            }
        }
        return FALSE;
    }
}
