<?php

/**
 * @file
 * A FileField extension to allow referencing of files from IMCE.
 *
 * The "hooks" in this file are not true hooks, they're called individually
 * from the main filefield_sources.module in the corresponding hook by the
 * same name. Any of these hooks could be broken out into a separate module.
 */

/**
 * Implements hook_filefield_source_info().
 */
function filefield_source_imce_info() {
  $source = array();

  if (!module_exists('imce') || !imce_access()) {
    return $source;
  }

  $source['imce'] = array(
    'name' => t('IMCE file browser'),
    'label' => t('File browser'),
    'description' => t('Select a file to use from a file browser.'),
    'process' => 'filefield_source_imce_process',
    'value' => 'filefield_source_imce_value',
    'weight' => -1,
  );
  return $source;
}

/**
 * Implements hook_menu().
 */
function filefield_source_imce_menu() {
  $items = array();
  $items['filefield/imce/%/%'] = array(
    'page callback' => 'filefield_source_imce_page',
    'page arguments' => array(2, 3),
    'access callback' => 'filefield_edit_access',
    'access arguments' => array(2, 3),
    'file' => 'sources/imce.inc',
    'type' => MENU_CALLBACK,
  );
  return $items;
}

/**
 * Implements hook_theme().
 */
function filefield_source_imce_theme() {
  return array(
    'filefield_source_imce_element' => array(
      'arguments' => array('element' => NULL),
      'file' => 'sources/imce.inc',
    ),
 );
}

/**
 * Implements hook_filefield_source_settings().
 */
function filefield_source_imce_settings($op, $field) {
  $return = array();

  if ($op == 'form') {
    $return['sources_imce'] = array(
      '#title' => t('IMCE file browser settings'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $return['sources_imce']['filefield_source_imce_mode'] = array(
      '#type' => 'radios', 
      '#title' => t('File browser mode'),
      '#options' => array(
        0 => t('Restricted: Users can only browse the field directory. No file operations are allowed.'),
        1 => t('Full: Browsable directories are defined by <a href="!imce-admin-url">IMCE configuration profiles</a>. File operations are allowed for IMCE 6.x-2.x or above.', array('!imce-admin-url' => url('admin/settings/imce'))),
      ),
      '#default_value' => isset($field['filefield_source_imce_mode']) ? $field['filefield_source_imce_mode'] : 0,
    );
  }
  elseif ($op == 'save') {
    $return[] = 'filefield_source_imce_mode';
  }

  return $return;

}

/**
 * A #process callback to extend the filefield_widget element type.
 */
function filefield_source_imce_process($element, $edit, &$form_state, $form) {
  $field = content_fields($element['#field_name'], $element['#type_name']);

  $element['filefield_imce'] = array(
    '#weight' => 100.5,
    '#theme' => 'filefield_source_imce_element',
    '#filefield_source' => TRUE, // Required for proper theming.
    '#description' => filefield_sources_element_validation_help($element['#upload_validators']),
  );

  $filepath_id = $element['#id'] . '-imce-path';
  $display_id = $element['#id'] . '-imce-display';
  $select_id = $element['#id'] . '-imce-select';
  $element['filefield_imce']['file_path'] = array(
    '#type' => 'hidden',
    '#value' => '',
    '#id' => $filepath_id,
    '#attributes' => array(
      'onchange' => "if (!jQuery('#$select_id').attr('disabled')) { jQuery('#$select_id').mousedown().disable(); jQuery('#$display_id').html(this.value); }",
    ),
  );

  $imce_function = 'window.open(\'' . url('filefield/imce/' . $element['#type_name'] . '/' . $element['#field_name'], array('query' => 'app=' . rawurlencode($field['widget']['label']) . '|url@' . $filepath_id)) . '\', \'\', \'width=760,height=560,resizable=1\'); return false;';
  $element['filefield_imce']['display_path'] = array(
    '#type' => 'markup',
    '#value' => '<span id="' . $display_id . '" class="filefield-sources-imce-display">' . t('No file selected') . '</span> (<a class="filefield-sources-imce-browse" href="#" onclick="' . $imce_function . '">' . t('browse') . '</a>)',
  );

  $element['filefield_imce']['select'] = array(
    '#name' => implode('_', $element['#array_parents']) . '_imce_select',
    '#type' => 'submit',
    '#value' => t('Select'),
    '#validate' => array(),
    '#submit' => array('node_form_submit_build_node'),
    '#name' => $element['#name'] . '[filefield_imce][button]',
    '#id' => $select_id,
    '#attributes' => array('style' => 'display: none;'),
    '#ahah' => array(
      'path' => 'filefield/ahah/' . $element['#type_name'] . '/' . $element['#field_name'] . '/' . $element['#delta'],
      'wrapper' => $element['#id'] .'-ahah-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $element;
}

/**
 * A #filefield_value_callback function.
 */
function filefield_source_imce_value($element, &$item) {
  if (isset($item['filefield_imce']['file_path']) && $item['filefield_imce']['file_path'] != '') {
    if (variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PUBLIC) {
      $file_path = preg_replace('/^' . preg_quote(base_path(), '/') . '/', '', $item['filefield_imce']['file_path']);
    }
    else {
      $file_path = preg_replace('/^' . preg_quote(url('system/files'), '/') . '/', file_directory_path(), $item['filefield_imce']['file_path']);
    }

    // Resolve the file path to an FID.
    if ($fid = db_result(db_query("SELECT fid FROM {files} WHERE filepath = '%s'", rawurldecode($file_path)))) {
      $file = field_file_load($fid);
      if (filefield_sources_element_validate($element, (object) $file)) {
        $item = array_merge($item, $file);
      }
    }
    else {
      form_error($element, t('The selected file could not be used because the file does not exist in the database.'));
    }
    // No matter what happens, clear the value from the file path field.
    $item['filefield_imce']['file_path'] = '';
  }
}

/**
 * Theme the output of the autocomplete field.
 */
function theme_filefield_source_imce_element($element) {
  $output = theme('markup', $element['display_path']) . theme('hidden', $element['file_path']) . theme('submit', $element['select']);
  return '<div class="filefield-source filefield-source-imce clear-block">' . theme('form_element', $element, $output) . '</div>';
}

/**
 * Outputs the IMCE browser for FileField.
 */
function filefield_source_imce_page($content_type, $field_name) {
  global $conf;

  // Check access.
  if (!module_exists('imce') || !imce_access() || !($field = content_fields($field_name, $content_type))) {
    return drupal_access_denied();
  }

  // Full mode
  if (!empty($field['widget']['filefield_source_imce_mode'])) {
    $conf['imce_custom_scan'] = 'filefield_source_imce_custom_scan_full';
  }
  // Restricted mode
  else {
    $conf['imce_custom_scan'] = 'filefield_source_imce_custom_scan_restricted';
    $conf['imce_custom_field'] = $field;
  }

  // Disable absolute URLs.
  $conf['imce_settings_absurls'] = 0;

  // IMCE 6.x-2.x.
  if (module_hook('imce', 'file_references')) {
    module_load_include('inc', 'imce', 'inc/imce.page');
    return imce();
  }
  // IMCE 6.x-1.x.
  else {
    module_load_include('inc', 'imce', 'inc/page');
    return imce_page();
  }
}

/**
 * Scan directory and return file list, subdirectories, and total size for Full Mode.
 */
function filefield_source_imce_custom_scan_full($dirname, &$imce) {
  // Get a list of files in the database for this directory.
  $sql_dir_name = $dirname == '.' ? file_directory_path() : file_directory_path() .'/'. $dirname;

  $result = db_query("SELECT filepath FROM {files} WHERE filepath LIKE '%s' AND filepath NOT LIKE '%s'", $sql_dir_name .'/%', $sql_dir_name .'/%/%');
  $db_files = array();
  while ($row = db_fetch_object($result)) {
    $db_files[basename($row->filepath)] = 1;
  }

  // Get the default IMCE directory scan, then filter down to database files.
  $directory = imce_scan_directory($dirname);
  foreach ($directory['files'] as $filename => $file) {
    if (!isset($db_files[$filename])) {
      unset($directory['files'][$filename]);
      $directory['dirsize'] -= $file['size'];
    }
  }

  // Disable file operations for IMCE 6.x-1.x
  if (!module_hook('imce', 'file_references')) {
    filefield_source_imce_disable_perms($imce, array('browse', 'subnav'));
  }

  return $directory;
}

/**
 * Scan directory and return file list, subdirectories, and total size for Restricted Mode.
 */
function filefield_source_imce_custom_scan_restricted($dirname, &$imce) {
  $field = $GLOBALS['conf']['imce_custom_field'];
  $fdp = file_directory_path();
  $field_path = $field['widget']['file_path'] == '' ? $fdp : filefield_widget_file_path($field);
  $db_info = content_database_info($field);

  // Get field files
  $result = db_query("SELECT f.* FROM {" . $db_info['table'] . "} c INNER JOIN {files} f ON c." . $db_info['columns']['fid']['column'] . " = f.fid WHERE f.status = 1 AND f.filepath LIKE '%s' AND f.filepath NOT LIKE '%s'", $field_path .'/%', $field_path .'/%/%');

  // Create directory info
  $directory = array('dirsize' => 0, 'files' => array(), 'subdirectories' => array(), 'error' => FALSE);
  while ($file = db_fetch_object($result)) {
    // Get real name
    $name = basename($file->filepath);
    // Get dimensions
    $width = $height = 0;
    if ($img = imce_image_info($file->filepath)) {
      $width = $img['width'];
      $height = $img['height'];
    }
    $directory['files'][$name] = array(
      'name' => $name,
      'size' => $file->filesize,
      'width' => $width,
      'height' => $height,
      'date' => $file->timestamp,
    );
    $directory['dirsize'] += $file->filesize;
  }

  // Process IMCE. Make field directory the only accessible one.
  $imce['dir'] = $field_path == $fdp ? '.' : substr($field_path, strlen($fdp) + 1);
  $imce['directories'] = array();
  if (!empty($imce['perm'])) {
    filefield_source_imce_disable_perms($imce, array('browse'));
  }

   return $directory;
 }

/**
 * Disable IMCE profile permissions.
 */
function filefield_source_imce_disable_perms(&$imce, $exceptions = array()) {
  $disable_all = empty($exceptions);
  foreach ($imce['perm'] as $name => $val) {
    if ($disable_all || !in_array($name, $exceptions)) {
      $imce['perm'][$name] = 0;
    }
  }
  $imce['directories'][$imce['dir']] = array('name' => $imce['dir']) + $imce['perm'];
}
