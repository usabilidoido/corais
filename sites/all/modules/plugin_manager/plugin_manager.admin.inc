<?php

/**
 * @file
 * The Forms used by the plugin manager.
 */

function plugin_manager_modules_redirect() {
    drupal_set_message(t("Search for the modules you want to install, add them to your queue, and then install them."));
    drupal_goto('admin/plugin_manager/search');
}

function plugin_manager_themes_redirect() {
    drupal_set_message(t("Search for the themes you want to install, add them to your queue, and then install them."));
    drupal_goto('admin/plugin_manager/search');
}

/**
 * FAPI callback for the manual plugin manager installer.
 */
function plugin_manager_manual($form_state) {
  drupal_set_message(t('WARNING: This manual install page is to be used with great caution. *Always* inspect an archive before you upload it to your site if you are unsure of the source. It is suggested that you not use this page during normal operations, but use instead the regular install page'), 'error');
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['tarball'] = array(
    '#type' => 'file',
    '#title' => t('Plugin Tarball'),
  );
  $form['backend'] = array(
    '#title' => t('Installation method'),
    '#type' => 'select',
    '#options' => plugin_manager_backends(),
  );
  $form['host'] = array(
    '#title' => t('Hostname'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_hostname', 'localhost'),
  );
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('plugin_manager_username', ''),
  );
  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => '',
  );
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#default_value' => 'modules',
    '#options' => array('themes' => 'Theme', 'modules' => 'Module'),
    '#description' => t('This lets the system know where to install the plugin.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Install'),
  );

  return $form;
}

/**
 * Submit callback for plugin_manager_manual().
 */
function plugin_manager_manual_submit($form, &$form_state) {
  $backend_list = plugin_manager_backends();
  $backend = $backend_list[$form_state['values']['backend']];
  $file = file_save_upload('tarball');
  $host = $form_state['values']['host'];
  $user = $form_state['values']['username'];
  $pass = $form_state['values']['password'];
  $type = $form_state['values']['type'];

  // Get the file
  if (!$file) {
    drupal_set_message(t('File could not be uploaded.'), 'error');
    return;
  }

  // Then finally install it.
  $files = plugin_manager_untar($file->filepath);
  //plugin_manager_ftp_copy($extracted, $type, $host, $user, $pass);

  // Copy the extracted files
  $copy = call_user_func_array($backend .'_plugin_manager_copy',
    array($files, $type, $host,
    $user, $pass));

  // If it failed, stop now.
  if (!$copy) {
    drupal_set_message(t("Unable to install @name", array('@name' => $name)), 'error');
    return;
  }

  foreach (array_reverse($files) AS $file) {
    // Remove the extracted files.
    $file = file_directory_path() .'/plugin_manager_extraction/'. $file;
    if (is_dir($file)) {
      rmdir($file);
    }
    else {
      unlink($file);
    }
  }

  drupal_set_message(l(t('Successfully installed plugin.  Go Here to enable it.'), 'admin/build/'. drupal_strtolower($type)));
}

/**
 * FAPI for the first page of the installer
 *
 * @ingroup forms
 * @see plugin_manager_install_1()
 */
function plugin_manager_page_1() {
  $queue_info = variable_get("plugin_manager_queue_info", array());

  // Put a version drop down for each of the files
  foreach ($queue_info AS $project => $info) {
    $form[$project .'_version'] = array(
      '#title' => t('%title version', array('%title' => $info['title'])),
      '#type' => 'select',
      '#options' => $info['version'],
    );
  }

  // Put the continue button up.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue to Step 2'),
  );
  return $form;
}

/**
 * FAPI for the third page of the installer
 *
 * @ingroup forms
 * @see plugin_manager_install_2()
 */
function plugin_manager_page_2($form_state) {
  $form['backend'] = array(
    '#title' => t('Installation method'),
    '#type' => 'select',
    '#options' => plugin_manager_backends(),
  );
  $form['host'] = array(
    '#title' => t('Hostname'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_hostname', 'localhost'),
  );
  $form['username'] = array(
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_username', ''),
  );
  $form['save_username'] = array(
    '#title' => t('Save Username'),
    '#type' => 'checkbox',
  );
  $form['password'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Install'),
  );
  return $form;
}

function plugin_manager_page_3($form_state) {
  update_get_available(TRUE); // refresh the update database
  $form[] = array(
    '#value' => t("Now you're done.  Unless it is stated otherwise above, your modules and/or themes should be properly installed now.  So what should you do now?") .'<p>'. l(t("If you installed module updates, you should run update.php."), 'update.php') .'<p>'. l(t("If you installed new modules, you can enable them here."), 'admin/build/modules') .'</p><p>'. l(t('If you installed new themes, you can select them here.'), 'admin/build/themes') .'</p>',
  );
  
  return $form;
}

/**
 * A page to list all of the queued changes and begin installation of them.
 */
function plugin_manager_install_form($form_state) {
  // Make sure that the plugin manager can be run.
  if (!plugin_manager_runnable()) {
    return array();
  }

  // Show the congrats page.
  if (isset($form_state['storage']['page']) AND $form_state['storage']['page'] == 3) {
    return plugin_manager_page_3($form_state);
  }

  // If there aren't any, then just tell the just that.  And quit.
  $queue = plugin_manager_get_queue();
  if (empty($queue)) {
    drupal_set_message(t("Nothing has been queued for install."));
    return array();
  }

  // @todo Examine this code.
  // Get the total list and grab the release_history for each and set the download links.
  $queue_info = plugin_manager_get_release_history($queue);
  variable_set("plugin_manager_queue_info", $queue_info);

  // If we don't need to select a version, then skip that page.
  $latest = variable_get('plugin_manager_latest_version', 0);
  if ($latest AND !isset($form_state['storage']['page'])) {
    $files = array();
    foreach ($queue_info AS $name => $plugin) {
      $link = $plugin['release'][0]['download_link'];
      $files[$name] = plugin_manager_get($link);
      // Check for an error.
      if ($files[$name] == FALSE) {
        drupal_set_message(t('Could not download @name', array('@name' => $name)), 'error');
        return;
      }
      else {
        drupal_set_message(t('Downloaded @name', array('@name' => $name)));
      }
    }
    variable_set("plugin_manager_downloaded_files", $files);
    $form_state['storage']['page'] = 2;
    foreach ($queue AS $name) {
      $form_state['storage'][1][$name .'_version'] = 0;
    }
  }

  // Allow the choice of versions on the first page.
  if (!isset($form_state['storage']['page'])) {
    return plugin_manager_page_1();
  }
  elseif ($form_state['storage']['page'] == 2) {
    return plugin_manager_page_2($form_state);
  }
  else {
    return plugin_manager_page_3($form_state);
  }
}

/**
 * Process the first page of the installer, by downloading the needed files.
 */
function plugin_manager_install_1($form, &$form_state) {
  // Download the files.
  $files = array();
  $queue_info = variable_get("plugin_manager_queue_info", array());
  foreach ($queue_info AS $name => $plugin) {
    $version = $form_state['storage'][1][$name .'_version'];
    $link = $plugin['release'][$version]['download_link'];
    $files[$name] = plugin_manager_get($link);

    // Check for an error.
    if ($files[$name] == FALSE) {
      drupal_set_message(t('Could not download @name', array('@name' => $name)), 'error');
      return;
    }
  }
  variable_set("plugin_manager_downloaded_files", $files);

  // Proceed to the next page.
  $form_state['storage']['page'] = 2;
  return;
}

/**
 * Process the third page of the installer.  Extract, install, log and
 * clean up the files.
 */
function plugin_manager_install_2($form, &$form_state) {
  // Load the appropriate backend.
  $backend_list = plugin_manager_backends();
  $backend = $backend_list[$form_state['values']['backend']];
  $form_state['storage']['page'] = 3;

  // Save the ftp_host variable
  variable_set("plugin_manager_ftp_hostname", $form_state['values']['host']);
  if ($form_state['values']['save_username'] == '1') {
    variable_set("plugin_manager_username", $form_state['values']['username']);
  }

  // Go through each on of the files.
  $plugins = variable_get("plugin_manager_downloaded_files", array());
  foreach ($plugins AS $name => $plugin) {
    // Extract the files
    $files = plugin_manager_untar($plugin);

    // Find out whether the thing is a module or theme.
    $row = db_fetch_array(
      db_query(
        "SELECT lower(tag) AS tag
         FROM {plugin_manager_taxonomy}
         WHERE short_name = '%s'
         AND tag IN('Modules', 'Themes')", $name
      )
    );

    // Copy the extracted files
    $copy = call_user_func_array($backend .'_plugin_manager_copy',
      array($files, $row['tag'], $form_state['values']['host'],
        $form_state['values']['username'], $form_state['values']['password']));

    foreach (array_reverse($files) AS $file) {
      // Remove the extracted files.
      $file = file_directory_path() .'/plugin_manager_extraction/'. $file;
      if (is_dir($file)) {
        rmdir($file);
      }
      else {
        unlink($file);
      }
    }

    // If it failed, stop now.
    if (!$copy) {
      drupal_set_message(t("Unable to install @name", array('@name' => $name)), 'error');
      return;
    }

    global $base_url;
    drupal_set_message(t("Successfully installed @name.", array('@name' => $name)) . l(t('View README.txt'), $base_url .'/sites/all/modules/'. check_plain($name) .'/README.txt'));
    // rebuild caches
    drupal_rebuild_theme_registry();
    node_types_rebuild();
    menu_rebuild();
    cache_clear_all('schema', 'cache');
    module_rebuild_cache();
    
    unlink($plugin);
  }
  
  $missing = plugin_manager_missing_dependencies();
  if ($missing['#alert']){
    drupal_set_message(t("You have missing dependencies! ").l(t("Go here"),"admin/plugin_manager/deps").t(" to resolve them."),"error");
  }
  variable_set("plugin_manager_queue_info", array());
  plugin_manager_clear_queue();
  $form_state['storage']['page'] = 3;
}

/**
 * Process the data presented in the plugin_manager_install form.
 */
function plugin_manager_install_form_submit($form, &$form_state) {
  // If page one was submitted, download the requested files.
  if (!isset($form_state['storage']['page']) OR ($form_state['storage']['page'] === 1)) {
    $form_state['storage'][1] = $form_state['values'];
    return plugin_manager_install_1($form, $form_state);
  }
  return plugin_manager_install_2($form, $form_state);
}

/**
 * FAPI for the resolve dependencies page
 *
 * Searches for missing dependencies, and gives the user to opportunity to install them
 * @ingroup forms
 * @see plugin_manager_dependencies_submit
 */
function plugin_manager_dependencies_form($form_state) {
  // Get a list of the non-installed plugins.
  $missing = plugin_manager_missing_dependencies();
  $files = module_rebuild_cache();
  if (!is_array($missing)){
    drupal_set_message(t("You have no missing dependencies!").l(t("Search for modules to install."),'admin/plugin_manager/search'));
    return;
  }
  foreach($missing as $dep=>$info){
    if ($dep=='#alert')continue;
    if (!$info['#in-repository']){ /** massive error checking and reporting **/
      $basetext = t('I can\'t find "@dep" in the repository.',array('@dep'=>$dep));
      if (strstr($dep,'_')!=FALSE){
        $parts = split('_',$dep);
        $ppage = l($parts[0],'http://drupal.org/project/'.$parts[0]);
        // do you have this guy installed?
        if (!isset($files[$parts[0]])){
          // does it exist in the rep? if not, you're out of luck.
          $row = db_fetch_array(db_query("SELECT title FROM {plugin_manager_repository} WHERE short_name = '%s'", $parts[0]));
          if (is_array($row))
            drupal_set_message($basetext.t(" It looks like it's a sub module of ").$ppage.t("--try installing that"),'error');
          else
            drupal_set_message($basetext.t(" It looks like it's a sub module of ").$ppage.t(", however, I also can't find that module in the repository. This means it's probably broken for your version of drupal"),'error');
        }else
          drupal_set_message($basetext.t(" It looks like it's a sub module of ").$ppage.t(", however, that module is already installed -- try checking the project page."),'error');
      }else
        drupal_set_message($basetext.t(" Either the repository is out of date (you can ").l(t('update it here'),'admin/plugin_manager/reload').t("), or it's permanently broken for your version of drupal. Try checking its ").l(t('project page'),'http://drupal.org/project/'.$dep).".",'error');
    }
    $form[$dep] = array(
      '#type' => 'checkbox',
      '#name' => $dep,
      '#title' => htmlentities($dep),
      '#description' => t('Required by ').$info['#required-by'],
      '#disabled' => !$info['#in-repository']
    );
  }
  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Install'),
  );
  return $form;
}

/**
 * Add dependencies to the queue and redirect to the install page
 */
function plugin_manager_dependencies_form_submit($form, &$form_state) {
  foreach ($form_state['values'] AS $plugin => $value) {
    // If it isn't supposed to be removed, then skip it.
    if ($value != 1) {
      continue;
    }
    if (!plugin_manager_add_to_queue($plugin)){
      drupal_set_message(t("Couldn't find @plugin to install. Try ",array('@plugin'=>$plugin)).l(t('updating the repository?'),'admin/plugin_manager/reload'),'error'); 
    }
  }
  drupal_goto('admin/plugin_manager/install');
  return;
}

/**
 * FAPI for the uninstall page.
 *
 * Shows a list of packages to the user, allowing them to mark packages for removal.
 * @ingroup forms
 * @see plugin_manager_uninstall_submit()
 */
function plugin_manager_uninstall_form($form_state) {

  // Get a list of the non-installed plugins.
  $result = db_query(
    "SELECT filename,name,type
     FROM {system}
     WHERE status = 0
     ORDER BY type"
  );
  while ($row = db_fetch_array($result)) {
    if (strpos($row['filename'], 'sites/all/'. $row['type'] .'s/'. $row['name']) !==0 || !file_exists($row['filename'])) continue;
    $info_file = join('.', array_slice(explode('.', $row['filename']), 0, -1)) .'.info';
    $info = drupal_parse_info_file($info_file);
    if (!$info['name'])$info['name'] = $row['name'];
    if (!$form[$row['type']]) {
        $form[$row['type']] = array(
        '#type' => 'fieldset',
        '#title' => htmlentities('Disabled '. ucwords($row['type']) .'s'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        );
    }
    $form[$row['type']][$row['name']] = array(
      '#type' => 'checkbox',
      '#name' => $row['name'],
      '#title' => htmlentities($info['name']),
    );
  }
  $form['backend'] = array(
    '#title' => t('Removal method'),
    '#type' => 'select',
    '#options' => plugin_manager_backends(),
  );
  $form['host'] = array(
    '#title' => t('Hostname'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_hostname', 'localhost'),
  );
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('plugin_manager_username', ''),
  );
  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => '',
  );
  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Uninstall'),
  );
  return $form;
}

/**
 * Remove package files from the server.
 */
function plugin_manager_uninstall_form_submit($form, &$form_state) {
  $backend_list = plugin_manager_backends();
  $backend = $backend_list[$form_state['values']['backend']];

  foreach ($form_state['values'] AS $plugin => $value) {

    // If it isn't supposed to be removed, then skip it.
    if ($value != 1) {
      continue;
    }

    // Get the files to remove.
    $row = db_fetch_array(db_query(
        "SELECT filename, name, type
         FROM {system}
         WHERE name='%s'", array($plugin)));
    if (strpos($row['filename'], 'sites/all/'. $row['type'] .'s/'. $row['name'] .'/') !==0 || !file_exists($row['filename'])) {
        drupal_set_message(t("Illegal Uninstall. Please report this bug."));
        return;
    }
    // Remove the files / folders requested.
    $remove = call_user_func_array($backend .'_plugin_manager_remove',
      array('sites/all/'. $row['type'] .'s/'. $row['name'], $form_state['values']['host'],
        $form_state['values']['username'], $form_state['values']['password']));
    if ($remove) {
        drupal_set_message(t("Successfully uninstalled @plugin.", array('@plugin' => $plugin)));
    }
    else
        drupal_set_message(t("Unable to uninstall @plugin.", array('@plugin' => $plugin)), 'error');
  }
}

/** Package Update Section **/

/**
 * @callback for admin/plugin_manager/update
 * List packages w/ available updates and give the option to install.
 * Makes use of the core "update" module.
 */
function plugin_manager_update_form() {
  $available = update_get_available(TRUE);
  $projects = update_calculate_project_data($available);

  $some = FALSE;
  foreach ($projects as $name => $project) {
     if (($project['status'] == UPDATE_NOT_CURRENT 
       | $project['status'] == UPDATE_NOT_SECURE
       | $project['status'] == UPDATE_NOT_SUPPORTED
        )
		&& $project['title'] != "Drupal" 
		&& $project['title'] != "Drupal core"
		) {
      $some=TRUE;
      $form[$name] = array(
        '#type' => 'checkbox',
        '#title' => $project['title'],
        '#default_value' => 'true'
      );
    }
  }//}
  if (!$some)
      drupal_set_message(t("There are no available updates for your drupal installation."));
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Install Updates'
  );
  return $form;
}

function plugin_manager_update_form_submit($form, &$form_state) {
  foreach ($form_state['values'] AS $name => $value) {
    if ($value == '1') {
      plugin_manager_add_to_queue($name);
    }
    else {
      plugin_manager_remove_from_queue($name);
    }
  }
  $queue = plugin_manager_get_queue();
  if ($queue) {
    drupal_goto("admin/plugin_manager/install");
  }
}

/** Plugin Manager Search Section **/

/**
 * This is the main function for the Ajax Search page
 * of Plugin Manager.
 */
function plugin_manager_find(){

  if (variable_get('plugin_manager_last_reload', 0) + 604800 < date('U')) {
    drupal_set_message(l(t("The repository is out of date.  Update it by clicking here."), 'admin/plugin_manager/reload'));
  }
  if (variable_get('plugin_manager_last_reload', 0) == 0) {
    return array();
  }

  drupal_add_js(drupal_get_path("module", "plugin_manager") ."/plugin_manager_search.js", 'module');
  drupal_add_css(drupal_get_path("module", "plugin_manager") ."/plugin_manager_search.css", 'module');
  drupal_add_js(array(
    'plugin_manager' => array(
      'ajaxPrefix' => url('admin/plugin_manager/search/'),
      ),
    ), 'setting');
  
  $letters = array('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z');

  // Get a list of categories.
  $result = db_query(
    "SELECT tag
     FROM {plugin_manager_taxonomy}
     WHERE tag NOT IN('Modules')
     GROUP BY tag
     ORDER BY tag"
  );
  $tags = array();
  while ($row = db_fetch_array($result)) {
    $tags[$row['tag']] = $row['tag'];
  }
  
  $form['sinput'] = array(
    '#title' => t('Search'),
    '#type' => 'textfield',
    '#prefix' => '<table><tr><td>',
    '#suffix' => '</td>',
  );
  $form['search_button'] = array(
    '#value' => t('Search'),
    '#type' => 'submit',
    '#attributes' => array('onclick' => 'search_term(); return false;'),
    '#prefix' => '<td>',
    '#suffix' => '</td></tr></table>',
  );
  $form['tag_button'] = array(
    '#value' => t('Tags'),
    '#type' => 'submit',
    '#attributes' => array('onclick' => 'show_tags(); return false;','disabled'=>'true','class'=>'selected'),
    '#prefix' => '<table id="which"><tr><td>'.t('Browse By').'</td><td>',
    '#suffix' => '</td>',
  );
  $form['letter_button'] = array(
    '#value' => t('Alphabetical'),
    '#type' => 'submit',
    '#attributes' => array('onclick' => 'show_letters(); return false;'),
    '#prefix' => '<td>',
    '#suffix' => '</td>',
  );
  $form['queue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Install Queue'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('id'=>'install_queue','name'=>'Install Queue'),
    '#prefix' => '<td style="width:100%">',
    '#suffix' => '</td>',
    '#value' => '<table></table>',
  );
  $form['install_button'] = array(
    '#value' => t('Install Queued Plugins'),
    '#type' => 'submit',
    '#attributes' => array('onclick' => 'document.location="'.check_url(url('admin/plugin_manager/install')).'"; return false;'),
    '#prefix' => '<td>',
    '#suffix' => '</td></tr></table>',
  );
  
  $form['tags'] = array('#prefix'=>'<div id="tags">','#suffix'=>'</div>');
  $form['letters'] = array('#prefix'=>'<div id="letters" class="search_hidden">','#suffix'=>'</div>');
  $form['results'] = array(
            '#type' => 'fieldset',
            '#title' => t('Search Results'),
            '#collapsible' => TRUE,
            '#collapsed' => FALSE,
            '#attributes' => array('onclick' => 'return false;','name'=>'Search Results'),
            '#prefix' => '<div id="results" class="search_hidden">',
            '#suffix' => '</div>',
  );
  
  foreach($tags as $tag){
      $form['tags'][$tag] = array(
            '#type' => 'fieldset',
            '#title' => t($tag),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#attributes' => array('onclick' => 'list_tag(this,"'+$tag+'");','name'=>$tag),
      );
  }
  foreach($letters as $letter){
      $form['letters'][$letter] = array(
            '#type' => 'fieldset',
            '#title' => t($letter),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#attributes' => array('onclick' => 'list_letter(this,"'+$letter+'");','name'=>$letter),
      );
  }
  
  return $form;
}

function plugin_manager_settings_form() {

  // If there are no backends, then stop.
  $backends = plugin_manager_backends();
  if (empty($backends)) {
    drupal_set_message(t("There are no available backends for use."), "error");
    return array();
  }

  $form = array();

  $form['plugin_manager_username'] = array(
    '#title' => t('Stored Username'),
    '#type' => 'textfield',
    '#default_value' => variable_get('plugin_manager_username', ''),
  );
  $form['plugin_manager_hostname'] = array(
    '#title' => t('Stored Hostname'),
    '#type' => 'textfield',
    '#description' => t('(Usually localhost) For non-standard port, put host:port (eg. localhost:27).'),
    '#default_value' => variable_get('plugin_manager_hostname', 'localhost'),
  );
  $form['plugin_manager_latest_version'] = array(
    '#type' => 'checkbox',
    '#title' => t('Choose Latest Version'),
    '#description' => t('If this is selected, the plugin manager will skip version selection and automatically select the most recent version.'),
    '#default_value' => variable_get('plugin_manager_latest_version', 0),
  );

  // Get settings from the individual backends.
  foreach ($backends AS $backend) {
    $form[$backend] = array(
      '#type' => 'fieldset',
      '#title' => t('@backend Settings', array('@backend' => drupal_strtoupper($backend))),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    if (function_exists($backend .'_plugin_manager_settings')) {
      $form[$backend][] = call_user_func($backend .'_plugin_manager_settings');
    }
    else {
      // Show that there are no configuration options.
      $form[$backend][] = array(
        '#value' => t('This backend provides no configuration options.'),
      );
    }
  }

  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );

  return $form;
}

function plugin_manager_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values'] AS $name => $value) {
    if (!strcmp(drupal_substr($name, 0, drupal_strlen('plugin_manager_')), 'plugin_manager_')) {
      variable_set($name, $value);
    }
  }
  drupal_set_message(t("The settings have been saved."));
}
