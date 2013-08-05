<?php
// $Id$
/**
 * @file
 */

define('GITOSIS_REPOSITORY', sys_get_temp_dir() . DIRECTORY_SEPARATOR . 'gitosis-admin');

/**
 * Implementation of hook_drush_command().
 *
 * Clones gitosis-admin repository, updates keys and gitosis-conf, commits and pushes changes
 *
 * @return
 *   An associative array describing your command(s).
 */
function gitosis_drush_command() {
  $items = array();

  $items['gitosis-load-config'] = array(
    'description' => "Loads existing configuration at gitosis.conf into your module information.\n
                      This is very important on first run to load the information of the gitosis-admin group at gitosis.conf.",
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'drush gitosis-load-config',
    ),
    'aliases' => array(),
    'callback' => 'gitosis_drush_load_config',
  );

  $items['gitosis-load-keys'] = array(
    'description' => "Loads existing public keys into sshkey table, looking for them in the keydir directory of 'gitosis-admin' repository.",
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'drush gitosis-load-keys',
    ),
    'aliases' => array(),
    'callback' => 'gitosis_drush_load_keys',
  );
 
  $items['gitosis'] = array(
    'description' => "Clones gitosis-admin repository, updates keys and gitosis-conf, commits and pushes changes.",
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'drush gitosis',
    ),
    'aliases' => array(),
    'callback' => 'gitosis_drush_gitosis',
  );

  return $items;
}

/**
 * Loads existing configuration at gitosis.conf into your module information.
 * This is very important on first run to load the information of the gitosis-admin group at gitosis.conf.
 */
function gitosis_drush_load_config() {
  _gitosis_load_gitosis_admin();
  $current_conf = file_get_contents(GITOSIS_REPOSITORY . '/gitosis.conf');
  variable_set('gitosis_main_config', trim($current_conf));
  drush_print("\nThe information at gitosis.conf could be loaded successfully. Go to admin/gitosis/settings to verify it.\n");
}

/**
 * Loads all public keys in keydir which match an email in the users table into sshkey table.
 */
function gitosis_drush_load_keys() {
  module_load_include('inc', 'sshkey', 'sshkey.pages');
  _gitosis_load_gitosis_admin();
  $iterator = new DirectoryIterator(GITOSIS_REPOSITORY . '/keydir');
  foreach ($iterator as $fileinfo) {
    if ($fileinfo->isFile() && preg_match('/\.pub$/', $fileinfo->getFilename())) {
      $key = file_get_contents($fileinfo->getPathname());
      // Match public key with user through email
      $user = db_fetch_object(db_query('SELECT * 
                                        FROM {users} u 
                                        WHERE u.mail = \'%s\'',
                                        preg_replace('/\.pub$/', '', $fileinfo->getFilename())));
      if ($user->uid) {
        // Check that the key does not exist already
        $exists = db_result(db_query('SELECT 1
                                      FROM {sshkey}
                                      WHERE entity_id = %d AND value = \'%s\'',
                                      $user->uid, $key));
        if (!$exists) {
          _gitosis_add_key($fileinfo->getFilename(), $key, $user->uid);
        }
      }
      else {
        _gitosis_add_key($fileinfo->getFilename(), $key, 1); 
      }
    }
  } 
}

/**
 * Updates gitosis configuration based on db info 
 * Clones repository into temp folder, updates it and pushes changes
 */
function gitosis_drush_gitosis() {
  _gitosis_load_gitosis_admin();
  $main_config  = variable_get('gitosis_main_config', '');
  $extra_config = variable_get('gitosis_extra_config', '');

  if (!preg_match('/group gitosis-admin/', $main_config)) {
    throw new Exception('Group gitosis-admin has not been set at Main configuration field at adming/gitosis/settings. Enter it yourself or run gitosis-load-config to grab it from the existing gitosis.conf file');
  }

  $new_conf = $main_config . "\n\n";
 
  // Iterate groups with repositories
  $result_groups = db_query('SELECT DISTINCT n.nid, n.title
                             FROM {node} n
                             INNER JOIN {content_field_repository} cfr USING (nid)
                             WHERE n.type = \'group\'');
  while ($group = db_fetch_object($result_groups)) {  
    $group_name = _gitosis_zen_id_safe($group->title);
    
    // Iterate group repositories
    $result_repos = db_query('SELECT field_repository_value AS name
                              FROM {content_field_repository}
                              WHERE NID = %d', $group->nid);
    $repos = array();
    while ($group_repo = db_fetch_object($result_repos)) {
      $repos[] = $group_repo->name; 
    } 
    if (count($repos)) {
      // Iterate users within a group and add those with a public key
      $result_users = db_query('SELECT u.mail, u.uid
                                FROM {users} u
                                INNER JOIN {og_uid} og USING(uid)
                                WHERE og.nid = %d AND u.status = 1
                                  AND u.uid <> 1', $group->nid);
      $users = array();
      while ($user = db_fetch_object($result_users)) {
        if (db_result(db_query('SELECT 1
                                FROM {sshkey}
                                WHERE entity_type = \'user\'
                                AND entity_id = %d', $user->uid))) {
          $users[] = $user->mail;
        }
      }
      if (count($users)) {
        $new_conf .= '[group ' . $group_name . ']' . "\n";
        $new_conf .= 'writable = ' . implode(' ', $repos) . "\n"; 
        $new_conf .= 'members = ' . implode(' ', $users) . "\n";
        $new_conf .= "\n";
      }
    }
  }
  
  $new_conf .= "\n\n" . trim($extra_config);
  
  chdir(GITOSIS_REPOSITORY);
  $fh = fopen('gitosis.conf', 'w');
  fwrite($fh, $new_conf);
  fclose($fh);

  // Add keys to keydir (ignoring if they already exist or not)
  $result_keys = db_query('SELECT u.mail, s.*
                           FROM {users} u
                           INNER JOIN {sshkey} s ON u.uid = s.entity_id');
  while ($key = db_fetch_object($result_keys)) {
    // Admin user are named by key title and not by email
    // This is because the admin user will probably have other keys
    $key_title = $key->entity_id == 1?$key->title:$key->mail; 
    $fh = fopen('keydir/' . $key_title . '.pub', 'w');
    fwrite($fh, $key->value);
    fclose($fh);
  }
  
  $result = exec('git add .', $response, $return_code); 
  $result = exec('git commit -m "Updated repository"', $response, $return_code); 
  $result = exec('git push', $response, $return_code);
}

/**
 * Private function to load 'gitosis-admin' repository
 * 
 * @throws Exception
 */
function _gitosis_load_gitosis_admin() {
  $repository = variable_get('gitosis_repository', '');
  if (!$repository) {
    throw new Exception('The gitosis repository URL has not been set');
  }
  else {
    if (is_dir(GITOSIS_REPOSITORY)) {
      $result = exec('rm -rf ' . GITOSIS_REPOSITORY);
    }
    $command = 'git clone ' . variable_get('gitosis_repository', '') . ' ' . GITOSIS_REPOSITORY; 

    $result = exec($command, $response, $return_code);
  }
  if (!file_exists(GITOSIS_REPOSITORY . '/gitosis.conf')) {
    throw new Exception('The gitosis repository could not be cloned');
  }
}

/**
 * Private function to add an ssh key for a user
 *
 * @param $title
 *   The key title, which is used to store the key name
 *   and then referenced in each repository of gitosis.conf
 *
 * @param $key
 *   The string of characters which form the ssh public key.
 *
 * @param $uid
 *   The user's id
 */
function _gitosis_add_key($title, $key, $uid) {
  $form_state = array();
  $form_state['values']['title'] = $title;
  $form_state['values']['value'] = $key;
  drupal_execute('sshkey_edit_form', $form_state, 'user', $uid);
  $user = user_load($uid);
  if (form_get_errors()) {
    drush_print('The public key ' . $title . ' could not be saved for user ' . $user->mail . ' in database 
                 because of the following error(s): ' . print_r(form_get_errors(), TRUE));
  } 
  else {
    drush_print('Successfully added public key with title ' . $title . ' for user with email ' . $user->mail);
  }
}

/**
 * Private function to create safe ids
 *
 * @credits http://api.lullabot.com/zen_id_safe/5
 */
function _gitosis_zen_id_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9_-]+/', '-', $string));
  // If the first character is not a-z, add 'id' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id' . $string;
  }
  return $string;
}

