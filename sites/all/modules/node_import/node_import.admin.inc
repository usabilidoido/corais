<?php

/**
 * @file
 * Administration forms of node_import module.
 */

module_load_include('inc', 'node_import');
drupal_add_css(drupal_get_path('module', 'node_import') .'/node_import.css');

/**
 * Lists the available import tasks and shows their progress.
 */
function node_import_list_tasks_form(&$form_state) {
  $form = array();

  $tasks = node_import_list_tasks(TRUE);
  $form['tasks'] = array(
    '#type' => 'item',
    '#title' => t('Pending tasks'),
    '#theme' => 'node_import_imports_list',
  );

  if (!empty($tasks)) {
    foreach ($tasks as $taskid => $task) {
      $task_owner = user_load(array('uid' => $task['uid']));
      $form['tasks'][$taskid] = array(
        'name' => array('#value' => check_plain($task['name'])),
        'uid' => array('#value' => theme('username', $task_owner)),
        'created' => array('#value' => format_date($task['created'], 'small')),
        'status' => array('#value' => $task['status'] == NODE_IMPORT_STATUS_DONE ? t('Completed') : t('In progress')),
        'view' => array('#value' => l(t('view'), 'admin/content/node_import/'. $taskid)),
        'delete' => array('#value' => $task['status'] == NODE_IMPORT_STATUS_DONE ? l(t('delete'), 'admin/content/node_import/'. $taskid .'/delete') : ''),
      );
    }
  }

  $form['buttons'] = array(
    'add_task_button' => array(
      '#type' => 'markup',
      '#value' => l(t('New import'), 'admin/content/node_import/add'),
    ),
  );

  return $form;
}

function node_import_list_files_form(&$form_state) {
  $form = array();

  $files = node_import_list_files(TRUE);
  $form['files'] = array(
    '#type' => 'item',
    '#theme' => 'node_import_file_select',
  );

  if (!empty($files)) {
    $form['files']['fid'] = array(
      '#type' => 'checkboxes',
      '#options' => node_import_extract_property($files, 'filename'),
    );

    foreach ($files as $fid => $file) {
      $file_owner = user_load(array('uid' => $file->uid));
      $form['files'][$fid] = array(
        'filename' => array('#value' => $file->filename),
        'filepath' => array('#value' => $file->filepath),
        'filesize' => array('#value' => format_size($file->filesize)),
        'timestamp' => array('#value' => format_date($file->timestamp, 'small')),
        'uid' => array('#value' => ($file->uid == 0 ? t('Public FTPd file') : theme('username', $file_owner))),
      );
    }
  }

  $form['buttons'] = array(
    'delete_button' => array(
      '#type' => 'submit',
      '#value' => t('Delete selected files'),
      '#submit' => array('node_import_list_files_form_submit_delete'),
      '#disabled' => empty($files),
    ),
  );

  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  $form['upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload file'),
    '#description' => t('<strong>The file you are uploading must be in UTF8 encoding.</strong>'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($files),
  );
  $form['upload']['file_upload'] = array(
    '#type' => 'file',
  );
  $form['upload']['file_upload_button'] = array(
    '#type' => 'submit',
    '#value' => t('Upload file'),
    '#submit' => array('node_import_add_form_submit_upload_file'),
  );

  return $form;
}

function node_import_list_files_form_submit_delete($form, &$form_state) {
  $files = node_import_list_files();
  foreach ((array)$form_state['values']['fid'] as $fid => $selected) {
    if (isset($files[$fid]) && $selected) {
      drupal_set_message(t('Deleted %filepath from server.', array('%filepath' => $files[$fid]->filepath)));
      db_query("DELETE FROM {files} WHERE fid = %d", $fid);
      file_delete($files[$fid]->filepath);
    }
  }
}

function node_import_list_files_form_submit_upload_file($form, &$form_state) {
  $validators = array();

  $dest = node_import_directory();

  if (($file = file_save_upload('file_upload', $validators, $dest, FILE_EXISTS_RENAME))) {
    drupal_set_message(t('New file %name uploaded to %path.', array('%name' => $file->filename, '%path' => $file->filepath)));
    file_set_status($file, FILE_STATUS_PERMANENT);
  }
}

/**
 * Creates a new import task by letting the user fill in a wizard.
 */
function node_import_add_form(&$form_state) {
  global $user;
  $form = array();

  // Check if the user can create something.
  $types = node_import_types();
  if (empty($types)) {
    form_set_error('', t('You are not allowed to create any content. <a href="!permissions">Assign the correct permissions</a> (such as <em>create story content</em>) on the administration page.', array('!permissions' => url('admin/user/permissions'))));
  }

  // ------------------------------------------------------------
  // Get the currently filled in values of the form.
  $form_state['storage'] = isset($form_state['storage']) ? $form_state['storage'] : array();
  $form_state['values'] = isset($form_state['values']) ? $form_state['values'] : array();
  $values = array_merge($form_state['storage'], $form_state['values']);

  // ------------------------------------------------------------
  // Check and store the page we are on.
  $pages = array(
    'intro' => array(
      'title' => t('Introduction'),
      'description' => t('The <em>Node import</em> module allows you to import content (such as users, vocabularies, terms or nodes) into your site from a tabular file (such as Comma or Tab Separated Values files). This wizard guides you through a number of steps to configure such an import.') . (module_exists('advanced_help') ? ' ' . theme('advanced_help_topic', 'node_import', 'new-import', t('[more help]')) : ''),
    ),
    'type' => array(
      'title' => t('Select content type'),
      'description' => t('Select the content type for the data in the file. You can only select content types you are allowed to create.'),
    ),
    'file' => array(
      'title' => t('Select file'),
      'description' => t('Select the file which contains the data to import. You can select a previously uploaded file or upload a new file now.') . ((variable_get('node_import:ftp:enabled', 0) != 0 && (variable_get('node_import:ftp:user', 0) == $user->uid || variable_get('node_import:ftp:user', 0) == 0) ? ' '. t('Alternatively, you can upload a new file to %path using FTP (or other means). Press %reload after this FTP upload has finished to inform this form about this newly uploaded file.', array('%path' => node_import_directory(), '%reload' => t('Reload'))) : '')),
    ),
    'file_options' => array(
      'title' => t('Set file options'),
      'description' => t('Enter the file format options for the file. The options specify how to interpret the file. Select one of the predefined file formats or enter the record separator, field separator, text delimiter and escape character yourself.'),
    ),
    'map' => array(
      'title' => t('Map file columns'),
      'description' => t('For each content type field, select the columns in the file that contain the data for this field.'),
    ),
    'options' => array(
      'title' => t('Set import options'),
      'description' => t('On this page you can set some additional options.'),
    ),
    'defaults' => array(
      'title' => t('Set default values'),
      'description' => t('For each content type field, you can set the default value to use here. The default value is used when the field is not mapped to a file column or when the data in the mapped file column is empty for a record.'),
    ),
    'preview' => array(
      'title' => t('Preview import'),
      'description' => t('Here you can preview the import before importing the content. If there are errors, you may want to make changes to the previous pages. Press %reload if you change the number of records to preview.', array('%reload' => t('Reload'))),
    ),
    'start' => array(
      'title' => t('Start import'),
      'description' => t('Last chance to check all selected options. If you want to start the import, click %start.', array('%start' => t('Start import'))),
    ),
  );

  $titles = node_import_extract_property($pages, 'title');
  array_shift($titles);

  $page_keys = array_keys($pages);
  $first_page = $page_keys[0];
  $last_page = $page_keys[count($page_keys) - 1];

  $page = isset($values['page']) ? $values['page'] : $first_page;
  $form['#pages'] = $pages;
  $form_state['storage']['page'] = $page;

  // ------------------------------------------------------------
  // Set title and description for the current page.
  $steps = count($pages) - 1;
  $step = array_search($page, $page_keys);

  if ($page !== 'intro') {
    drupal_set_title(t('@title (step @step of @steps)', array('@title' => $pages[$page]['title'], '@step' => $step, '@steps' => $steps)));
  }
  $form['help'] = array(
    '#value' => '<div class="help">'. $pages[$page]['description'] .'</div>',
    '#weight' => -50,
  );

  // ------------------------------------------------------------
  // Introduction page.
  if ($page == 'intro') {
    $form['intro'] = array(
      '#value' => /* t('<h3>Wizard pages</h3>') .'<ol><li>'. implode('</li><li>', $titles) .'</li></ol>'. */ t('<h3>Buttons</h3><dl><dt>%back</dt><dd>Go to the previous page.</dd><dt>%reload</dt><dd>Reload the page without resetting the already filled in values.</dd><dt>%reset</dt><dd>Reset the values of the current page to their defaults.</dd><dt>%next</dt><dd>Go to the next page.</dd></dl>', array('%next' => t('Next'), '%back' => t('Back'), '%reset' => t('Reset page'), '%reload' => t('Reload page'))),
    );

    if (variable_get('node_import:annoy', 1)) {
      $form['intro']['#value'] .= t('<div class="warning"><p><strong>The <em>Node import</em> module is still in development</strong>. Check on the <a href="http://drupal.org/project/node_import" title="Node import project page">project page</a> to see if support for a specific module has been added. You can help the developers by</p><ul><li><a href="@documentation-path">writing documentation</a>,</li><li><a href="http://drupal.org/node/add/project-issue/4840">reporting bugs</a> (please read the issue submission instructions),</li><li><a href="http://drupal.org/project/node_import" title="Node import project page">fund the development by donating some money</a>.</li></ul></div>', array('@documentation-path' => url('admin/advanced_help/node_import')));
    }
  }

  // ------------------------------------------------------------
  // Select or upload a file.
  if ($page == 'file') {
    $files = node_import_list_files(TRUE);

    $form['file_select'] = array(
      '#type' => 'item',
      '#title' => t('Select file'),
      '#theme' => 'node_import_file_select',
    );

    if (!empty($files)) {
      foreach ($files as $fid => $file) {
        $file_owner = user_load(array('uid' => $file->uid));
        $form['file_select'][$fid] = array(
          'filename' => array('#value' => $file->filename),
          'filepath' => array('#value' => $file->filepath),
          'filesize' => array('#value' => format_size($file->filesize)),
          'timestamp' => array('#value' => format_date($file->timestamp, 'small')),
          'uid' => array('#value' => ($file->uid == 0 ? t('Public FTPd file') : theme('username', $file_owner))),
        );
      }

      $form['file_select']['fid'] = array(
        '#type' => 'radios',
        '#options' => node_import_extract_property($files, 'filename'),
        '#default_value' => isset($values['fid']) ? $values['fid'] : 0,
      );
    }
    else {
      $form['file_select']['fid'] = array(
        '#type' => 'item',
        '#value' => t('No files available.'),
      );
    }

    $form['#attributes'] = array('enctype' => 'multipart/form-data');

    $form['upload'] = array(
      '#type' => 'fieldset',
      '#title' => t('Upload file'),
      '#description' => t('<strong>The file you are uploading must be in UTF8 encoding.</strong>'),
      '#collapsible' => TRUE,
      '#collapsed' => !empty($files),
    );
    $form['upload']['file_upload'] = array(
      '#type' => 'file',
    );
    $form['upload']['file_upload_button'] = array(
      '#type' => 'submit',
      '#value' => t('Upload file'),
      '#submit' => array('node_import_add_form_submit_upload_file'),
    );
  }

  // ------------------------------------------------------------
  // Select file options.
  if ($page == 'file_options') {
    $record_separators = node_import_format_options('record separators') + array('' => t('Other'));
    $field_separators = node_import_format_options('field separators') + array('' => t('Other'));
    $text_delimiters = node_import_format_options('text delimiters') + array('' => t('Other'));
    $escape_characters = node_import_format_options('escape characters') + array('' => t('Other'));

    $file_options = isset($values['file_options']) ? $values['file_options'] : array();
    $has_headers = isset($values['has_headers']) ? $values['has_headers'] : 1;

    $form['has_headers'] = array(
      '#type' => 'checkbox',
      '#title' => t('First row contains column names'),
      '#description' => t('Check this option if the first row of the file contains names for each column. If the first row of the file contains data to import, uncheck this option.'),
      '#default_value' => $has_headers,
    );

    $form['file_options'] = array(
      '#title' => t('File options'),
      '#tree' => TRUE,
      '#theme' => 'node_import_file_format_select',
    );

    $file_formats = node_import_format_options('file formats');
    $file_format_options = node_import_extract_property($file_formats, 'title');
    $file_format_options[''] = t('Delimiter Separated Values');

    $form['file_options']['file_format'] = array(
      '#type' => 'radios',
      '#title' => t('File format'),
      '#options' => $file_format_options,
      '#default_value' => $file_options['file_format'],
    );

    foreach ($file_formats as $format => $info) {
      $form['file_options'][$format] = array(
        'record separator' => array('#value' => $record_separators[$info['record separator']]),
        'field separator' => array('#value' => $field_separators[$info['field separator']]),
        'text delimiter' => array('#value' => $text_delimiters[$info['text delimiter']]),
        'escape character' => array('#value' => $escape_characters[$info['escape character']]),
      );
    }

    foreach (array('record separator' => $record_separators, 'field separator' => $field_separators, 'text delimiter' => $text_delimiters, 'escape character' => $escape_characters) as $key => $options) {
      $form['file_options'][$key] = array(
        '#type' => 'radios',
        '#options' => $options,
        '#default_value' => $file_options[$key],
      );
      $form['file_options']['other '. $key] = array(
        '#type' => 'textfield',
        '#size' => 6,
        '#default_value' => isset($file_options['other '. $key]) ? $file_options['other '. $key] : '',
      );
    }

    if (isset($values['samples'])) {
      $form['samples'] = array(
        '#type' => 'item',
        '#title' => t('Sample data'),
        '#value' => theme('node_import_sample_data', $values['samples'], variable_get('node_import:length_of_samples', 200)),
      );
    }
  }

  // ------------------------------------------------------------
  // Select the content type.
  if ($page == 'type') {
    $type_titles = node_import_extract_property($types, 'title');
    $default = count($type_titles) == 1 ? key($type_titles) : '';
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Content type'),
      '#options' => $type_titles,
      '#default_value' => isset($values['type']) ? $values['type'] : $default,
    );
  }

  // ------------------------------------------------------------
  // Map file columns.
  if ($page == 'map') {
    $form['map'] = array(
      '#tree' => TRUE,
      '#theme' => 'node_import_field_table',
      '#node_import-columns' => array(t('Content field'), t('Maps to column')),
      '#node_import-empty' => t('There are no content fields you can map.'),
    );

    $fields = node_import_fields($values['type']);

    $map = $form_state['storage']['headers'];
    $map_required = array('' => t('- Please choose -')) + $map;
    $map_optional = array('' => t('- None -')) + $map;
    foreach ($fields as $fieldname => $fieldinfo) {
      if ($fieldinfo['is_mappable']) {
        $description = '';
        if (count($fieldinfo['tips']) > 1) {
          $description = $form[$fieldname]['#description'] = '<ul class="tips">';
          $description .= '<li>' . implode('</li><li>', $fieldinfo['tips']) . '</li>';
          $description .= '</ul>';
        }
        else if (!empty($fieldinfo['tips'])) {
          $description = implode('', $fieldinfo['tips']);
        }
        $form['map'][$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#description' => $description,
          '#node_import-group' => $fieldinfo['group'],
          '#type' => 'select',
          '#options' => $fieldinfo['map_required'] ? $map_required : $map_optional,
          '#multiple' => $fieldinfo['has_multiple'] || $fieldinfo['has_hierarchy'],
          '#default_value' => isset($values['map'][$fieldname]) ? $values['map'][$fieldname] : '',
        );
      }
    }

    if (isset($values['samples'])) {
      $form['samples'] = array(
        '#type' => 'item',
        '#title' => t('Sample data'),
        '#value' => theme('node_import_sample_data', $values['samples'], variable_get('node_import:length_of_samples', 200)),
      );
    }
  }

  // ------------------------------------------------------------
  // Default values.
  if ($page == 'defaults') {
    $fields = node_import_fields($values['type']);
    $form['defaults'] = array(
      '#tree' => TRUE,
      '#theme' => 'node_import_field_table',
      '#node_import-columns' => array(t('Content field'), t('Default value')),
      '#node_import-empty' => t('There are no default values you can set.'),
    );
    $values['defaults'] = isset($values['defaults']) ? $values['defaults'] : array();
    $form['defaults'] = array_merge($form['defaults'], node_import_defaults($values['type'], $values['defaults'], $fields, $values['map']));
  }

  // ------------------------------------------------------------
  // Options.
  if ($page == 'options') {
    $fields = node_import_fields($values['type']);
    $form['options'] = array(
      '#tree' => TRUE,
      '#theme' => 'node_import_field_table',
      '#node_import-columns' => array(t('Content field'), t('Options')),
      '#node_import-empty' => t('There are no additional options you can set.'),
    );
    $values['options'] = isset($values['options']) ? $values['options'] : array();
    $form['options'] = array_merge($form['options'], node_import_options($values['type'], $values['options'], $fields, $values['map']));
  }

  // ------------------------------------------------------------
  // Preview.
  if ($page == 'preview') {
    $form['preview_count'] = array(
      '#type' => 'select',
      '#title' => t('Number of records to preview'),
      '#default_value' => isset($values['preview_count']) ? $values['preview_count'] : variable_get('node_import:preview_count', 5),
      '#options' => drupal_map_assoc(array(5, 10, 15, 25, 50, 100, 150, 200)),
    );

    $form['preview'] = array(
      '#title' => t('Previews'),
    );
    foreach ($values['previews'] as $i => $preview) {
      $form['preview'][] = array(
        '#type' => 'item',
        '#title' => t('Record @count', array('@count' => $i + 1)),
        '#value' => $preview,
      );
    }
  }

  // ------------------------------------------------------------
  // Start import.
  if ($page == 'start') {
    $files = node_import_list_files();
    $file = $files[$values['fid']];
    $form['name'] = array(
      '#title' => t('Name'),
      '#type' => 'textfield',
      '#default_value' => isset($values['name']) ? $values['name'] : $file->filename,
      '#maxlength' => 64,
      '#description' => t('You can set a name for this import here so you can more easily differentiate between several imports.'),
    );
    $form[] = node_import_task_details($values);
  }

  // ------------------------------------------------------------
  // Add Back, Next and/or Start buttons.
  $form['buttons-bottom'] = array(
    '#weight' => 50,
  );
  $form['buttons-bottom']['back_button'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#submit' => array('node_import_add_form_submit_back'),
    '#disabled' => ($page == $first_page),
  );
  $form['buttons-bottom']['reload_button'] = array(
    '#type' => 'submit',
    '#value' => t('Reload page'),
    '#submit' => array('node_import_add_form_submit_reload'),
    '#disabled' => ($page == $first_page),
  );
  $form['buttons-bottom']['reset_button'] = array(
    '#type' => 'submit',
    '#value' => t('Reset page'),
    '#submit' => array('node_import_add_form_submit_reset'),
    '#disabled' => ($page == $first_page),
  );
  $form['buttons-bottom']['next_button'] = array(
    '#type' => 'submit',
    '#value' => ($page == $last_page) ? t('Start import') : t('Next'),
    '#validate' => array('node_import_add_form_validate_next'),
    '#submit' => array('node_import_add_form_submit_next'),
    '#disabled' => empty($types),
  );

  $form['buttons-top'] = $form['buttons-bottom'];
  $form['buttons-top']['#weight'] = -40;

  return $form;
}

function node_import_add_form_validate_next($form, &$form_state) {
  $values = $form_state['values'];
  $storage = $form_state['storage'];
  $page = $storage['page'];

  // ------------------------------------------------------------
  if ($page == 'file') {
    // If a file is uploaded, select it.
    $validators = array();
    $dest = node_import_directory();
    if (($file = file_save_upload('file_upload', $validators, $dest, FILE_EXISTS_RENAME))) {
      file_set_status($file, FILE_STATUS_PERMANENT);
      drupal_set_message(t('New file %name uploaded to %path.', array('%name' => $file->filename, '%path' => $file->filepath)));
      $values['fid'] = $form_state['values']['fid'] = $file->fid;
    }

    // Validate whether a file has been chosen and the file exists.
    $files = node_import_list_files(TRUE);
    if (!isset($values['fid']) || !isset($files[$values['fid']])) {
      form_set_error('fid', t('%select-file field is required. Upload a file first if there are no files uploaded yet.', array('%select-file' => t('Select file'))));
    }
  }

  // ------------------------------------------------------------
  if ($page == 'file_options') {
    if ($values['file_options']['file_format'] == '') {
      // The user has defined his own format. Make sure that when
      // he selects "Other" for one of the options that a value
      // has been filled in.
      $keys = array(
        'record separator' => t('Record separator'),
        'field separator' => t('Field separator'),
        'text delimiter' => t('Text delimiter'),
        'escape character' => t('Escape character'),
      );
      foreach ($keys as $key => $title) {
        if ($values['file_options'][$key] == '' && strlen($values['file_options']['other '. $key]) == 0) {
          form_set_error('file_options][other '. $key, t('You need to specify a character to use as %title if you choose %other for it.', array('%title' => $title, '%other' => t('Other'))));
        }
      }
    }
  }

  // ------------------------------------------------------------
  if ($page == 'type') {
    $types = node_import_types();
    if (!isset($values['type']) || !isset($types[$values['type']])) {
      form_set_error('type', t('!name field is required.', array('!name' => t('Content type'))));
    }
  }

  // ------------------------------------------------------------
  if ($page == 'map') {
    $fields = node_import_fields($storage['type']);
    foreach ($fields as $fieldname => $fieldinfo) {
      if (empty($fieldinfo['columns'])) {
        if ($fieldinfo['map_required'] && strlen($values['map'][$fieldname]) == 0) {
          form_set_error('map]['. $fieldname, t('!name field is required.', array('!name' => $fieldinfo['title'])));
        }
      }
      else {
        foreach ($fieldinfo['columns'] as $colname => $colinfo) {
          if ($colinfo['map_required'] && strlen($values['map'][$fieldname][$colname]) == 0) {
            form_set_error('map]['. $fieldname .']['. $colname, t('!name field is required.', array('!name' => $colinfo['title'])));
          }
        }
      }
    }
  }
}

function node_import_add_form_submit_upload_file($form, &$form_state) {
  $validators = array();

  $dest = node_import_directory();

  if (($file = file_save_upload('file_upload', $validators, $dest, FILE_EXISTS_RENAME))) {
    drupal_set_message(t('New file %name uploaded to %path.', array('%name' => $file->filename, '%path' => $file->filepath)));
    file_set_status($file, FILE_STATUS_PERMANENT);
    $form_state['values']['fid'] = $file->fid;
  }

  node_import_add_form_submit_reload($form, $form_state);
}

function node_import_add_form_submit_next($form, &$form_state) {
  $page = $form_state['storage']['page'];
  $page_keys = array_keys((array)$form['#pages']);
  $last_page = $page_keys[count($page_keys) - 1];

  if ($page == $last_page) {
    // If we are on the last page, save the task and redirect to task page.
    $form_state['storage'] = array_merge((array)$form_state['storage'], (array)$form_state['values']);
    $taskid = node_import_save_task($form_state['storage']);
    drupal_set_message(t('New import task has been created.'));

    $form_state['rebuild'] = FALSE;
    $form_state['storage'] = array();

    $form_state['redirect'] = 'admin/content/node_import/'. $taskid;
    $form_state['taskid'] = $taskid;
  }
  else {
    // Otherwise process current input and go to next page.
    node_import_add_form_submit_reload($form, $form_state);

    $form_state['storage']['page'] = $page_keys[array_search($form_state['storage']['page'], $page_keys) + 1];

    node_import_add_form_submit_reload($form, $form_state);
  }
}

function node_import_add_form_submit_back($form, &$form_state) {
  $page_keys = array_keys((array)$form['#pages']);
  $form_state['storage']['page'] = $page_keys[array_search($form_state['storage']['page'], $page_keys) - 1];

  node_import_add_form_submit_reload($form, $form_state);
}

function node_import_add_form_submit_reset($form, &$form_state) {
  drupal_set_message(t('The values have been reset to their default values.'), 'warning');
  $page = $form_state['storage']['page'];

  switch ($page) {
    case 'file':
      unset($form_state['values']['fid']);
      unset($form_state['storage']['fid']);
      break;
    default:
      unset($form_state['values'][$page]);
      unset($form_state['storage'][$page]);
      break;
  }

  node_import_add_form_submit_reload($form, $form_state);
}

function node_import_add_form_submit_reload($form, &$form_state) {
  $page = $form_state['storage']['page'];

  $form_state['storage'] = array_merge((array)$form_state['storage'], (array)$form_state['values']);
  $form_state['rebuild'] = TRUE;

  // ------------------------------------------------------------
  if ($page == 'file_options') {
    // Auto detect file format if not set.
    $files = node_import_list_files();
    $file = $files[$form_state['storage']['fid']];

    if (!isset($form_state['storage']['file_options'])) {
      $form_state['storage']['file_options'] = node_import_autodetect($file->filepath);
      drupal_set_message(t('The file options have been autodetected. If this is incorrect, fill in the correct values below.'), 'warning');
    }

    if (!isset($form_state['storage']['has_headers'])) {
      $form_state['storage']['has_headers'] = 1;
    }

    // Read some sample data with current settings.
    $samples = array();

    $i = variable_get('node_import:number_of_samples', 5);
    $num_cols = 0;
    $file_offset = 0;
    $data = array();
    while ($i > 0 && is_array($data)) {
      $i--;
      list($file_offset, $data) = node_import_read_from_file($file->filepath, $file_offset, $form_state['storage']['file_options']);
      if (is_array($data)) {
        $samples[] = $data;
        $num_cols = max($num_cols, count($data));
      }
    }

    if (!$form_state['storage']['has_headers']) {
      $header = array();
      while ($num_cols > 0) {
        $header[] = t('Column @number', array('@number' => $num_cols));
        $num_cols--;
      }
      $header = array_reverse($header);
      array_unshift($samples, $header);
    }

    $form_state['storage']['samples'] = $samples;
    $form_state['storage']['headers'] = $samples[0]; //TODO: what happens if no data?
  }

  // ------------------------------------------------------------
  if ($page == 'map') {
    // Auto detect mapping if not set.
    if (!isset($form_state['storage']['map']) && isset($form_state['storage']['headers'])) {
      $form_state['storage']['map'] = node_import_automap($form_state['storage']['type'], $form_state['storage']['headers']);
    }
  }

  // ------------------------------------------------------------
  if ($page == 'preview') {
    $files = node_import_list_files();
    $file = $files[$form_state['storage']['fid']];

    if (!isset($form_state['storage']['preview_count'])) {
      $form_state['storage']['preview_count'] = variable_get('node_import:preview_count', 5);
    }

    $previews = array();
    $preview_count = $form_state['storage']['preview_count'];
    $has_headers = $form_state['storage']['has_headers'];

    $file_offset = 0;
    $data = array();
    if ($has_headers) {
      list($file_offset, $data) = node_import_read_from_file($file->filepath, $file_offset, $form_state['storage']['file_options']);
    }

    global $node_import_can_continue;
    $node_import_can_continue = TRUE;

    $form_state['storage']['defaults'] = isset($form_state['storage']['defaults']) ? $form_state['storage']['defaults'] : array();
    $form_state['storage']['options'] = isset($form_state['storage']['options']) ? $form_state['storage']['options'] : array();

    while ($preview_count > 0 && is_array($data) && $node_import_can_continue) {
      $preview_count--;
      list($file_offset, $data) = node_import_read_from_file($file->filepath, $file_offset, $form_state['storage']['file_options']);
      if (is_array($data)) {
        $previews[] = node_import_create($form_state['storage']['type'], $data, $form_state['storage']['map'], $form_state['storage']['defaults'], $form_state['storage']['options'], TRUE);
      }
    }

    if (!$node_import_can_continue) {
      $previews[] = t('It is impossible to show more previews as a new object has been created that can be referenced by other rows.');
    }

    $form_state['storage']['previews'] = $previews;
  }

  unset($form_state['values'][$page]);
}

function node_import_task_details($values) {
  $form = array();

  if (isset($values['uid'])) {
    $user = user_load(array('uid' => $values['uid']));
    $form[] = array(
      '#type' => 'item',
      '#title' => t('Created by'),
      '#value' => theme('username', $user),
    );
  }

  if (isset($values['created'])) {
    $form[] = array(
      '#type' => 'item',
      '#title' => t('Created on'),
      '#value' => format_date($values['created']),
    );
    $form[] = array(
      '#type' => 'item',
      '#title' => t('Last updated on'),
      '#value' => format_date($values['changed']),
    );
  }

  if (isset($values['fid'])) {
    $files = node_import_list_files();
    $file = $files[$values['fid']];
    $form[] = array(
      '#type' => 'item',
      '#title' => t('File'),
      '#value' => check_plain($file->filename),
      '#description' => check_plain($file->filepath) .' ('. format_size($file->filesize) .')',
    );
  }

  if (isset($values['file_options'])) {
    $rows = array();
    $rows[] = array(t('First row contains column names'), $values['has_headers'] ? t('Yes') : t('No'));
    foreach (array('record separator' => t('Record separator'), 'field separator' => t('Field separator'), 'text delimiter' => t('Text delimiter'), 'escape character' => t('Escape character')) as $key => $title) {
      if ($values['file_options'][$key] === '') {
        $rows[] = array($title, check_plain($values['file_options']['other '. $key]));
      }
      else {
        $stuff = node_import_format_options($key .'s');
        $rows[] = array($title, $stuff[$values['file_options'][$key]]);
      }
    }
    $form[] = array(
      '#type' => 'item',
      '#title' => t('File options'),
      '#value' => theme('table', NULL, $rows),
    );
  }

  if (isset($values['samples'])) {
    $form[] = array(
      '#type' => 'item',
      '#title' => t('Sample data'),
      '#value' => theme('node_import_sample_data', $values['samples'], variable_get('node_import:length_of_samples', 200)),
    );
  }

  if (isset($values['type'])) {
    $type_titles = node_import_extract_property(node_import_types(), 'title');
    $form[] = array(
      '#type' => 'item',
      '#title' => t('Content type'),
      '#value' => $type_titles[$values['type']],
    );
  }

  if (isset($values['map'])) {
    //TODO: can we construct some reasonable status for this?
  }

  if (isset($values['defaults'])) {
    //TODO: can we construct some reasonable status for this?
  }

  if (isset($values['options'])) {
    //TODO: can we construct some reasonable status for this?
  }

  return $form;
}

/**
 * View a task and its progress.
 */
function node_import_view_form(&$form_state, $task) {
  drupal_set_title(t('Importing %name', array('%name' => $task['name'])));

  $form = array();
  $form['#task'] = $task;

  $form['status'] = array(
    '#type' => 'item',
    '#title' => t('Status'),
    '#value' => $task['status'] == NODE_IMPORT_STATUS_DONE ? t('Completed') : t('In progress'),
  );

  if ($task['status'] == NODE_IMPORT_STATUS_DONE) {
    $form['row_done'] = array(
      '#type' => 'item',
      '#title' => t('Rows imported'),
      '#prefix' => '<div id="node_import-row_done">',
      '#suffix' => '</div>',
      '#value' => format_plural($task['row_done'], t('1 row'), t('@count rows')),
    );
    $form['row_error'] = array(
      '#type' => 'item',
      '#title' => t('Rows with errors'),
      '#prefix' => '<div id="node_import-row_error">',
      '#suffix' => '</div>',
      '#value' => format_plural($task['row_error'], t('1 row'), t('@count rows')),
    );

    $form['download'] = array(
      '#type' => 'fieldset',
      '#title' => t('Download rows'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );
    $form['download']['what_rows'] = array(
      '#type' => 'radios',
      '#title' => t('Rows to download'),
      '#options' => array(
        'all' => t('All rows'),
        'error' => t('Rows with errors'),
        'done' => t('Rows without errors'),
      ),
      '#default_value' => 'error',
    );
    $form['download']['what_cols'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Extra columns'),
      '#options' => array(
        'objid' => t('Object identifier'),
        'errors' => t('Errors'),
      ),
      '#default_value' => array('objid', 'errors'),
    );
    $form['download']['download_button'] = array(
      '#type' => 'submit',
      '#value' => t('Download'),
      '#submit' => array('node_import_view_form_submit_download'),
    );
  }
  else {
    $form['progress'] = array(
      '#value' => '<div id="node_import-progress">'. t('You need to have Javascript enabled to see the progress of this import.') .'</div>',
    );

    drupal_add_js('misc/progress.js', 'core');
    $js_settings = array(
      'nodeImport' => array(
        'uri' => url('admin/content/node_import/'. $task['taskid']),
      ),
    );
    drupal_add_js($js_settings, 'setting');
    drupal_add_js(drupal_get_path('module', 'node_import') .'/node_import.js', 'module');
  }

  $form['details'] = array_merge(array(
    '#type' => 'fieldset',
    '#title' => t('Task details'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  ), node_import_task_details($task));

  $form['buttons'] = array(
    'delete_task_button' => array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#disabled' => $task['status'] != NODE_IMPORT_STATUS_DONE,
      '#submit' => array('node_import_view_form_submit_delete'),
    ),

    'add_task_button' => array(
      '#type' => 'markup',
      '#value' => l(t('New import'), 'admin/content/node_import/add'),
    ),
  );

  if (variable_get('node_import:annoy', 1)) {
    $form['report'] = array(
      '#type' => 'markup',
      '#value' => '<div class="warning">'. t('When you <a href="http://drupal.org/node/add/project-issue/node_import" title="Report an issue with the Node import module">report a bug</a> for the <em>Node import</em> module, it helps the developers a lot if you attach <a href="!node_import-debug" title="Node import debug report">a <em>Node import</em> debug report</a> to the issue. This report contains - in plain text - <ul><li>the Drupal and PHP version,</li><li>the enabled modules and their versions,</li><li>the permissions of the user,</li><li>the task and content fields details,</li><li>the error messages and data of @count non-imported rows.</li></ul><strong>You should not submit this data if you are concerned about the content of the file you try to import</strong>, for example if the file contains e-mail addresses or passwords. You could create another task with the same parameters but with a file with content that can be publically viewed in this case or e-mail the author directly. Thanks!', array('!node_import-debug' => url('admin/content/node_import/'. $task['taskid'] .'/debug'), '@count' => variable_get('node_import:debug:row_count', 5))) .'</div>',
    );
  }

  return $form;
}

function node_import_view_form_submit_download($form, &$form_state) {
  $task = $form['#task'];

  $file_formats = node_import_format_options('file formats');
  $file_options = $task['file_options'];

  $mime = 'text/plain';
  if (isset($file_options['file_format']) && isset($file_formats[$file_options['file_format']]) && isset($file_formats[$file_options['file_format']]['mime'])) {
    $mime = $file_formats[$file_options['file_format']]['mime'];
  }

  $suffix = pathinfo($task['file']->filename, PATHINFO_EXTENSION);
  $filename = basename($task['file']->filename, '.'. $suffix);

  $sql = "SELECT * FROM {node_import_status} WHERE taskid = %d";

  switch ($form_state['values']['download']['what_rows']) {
    case 'error':
      $filename = $filename .'-reject.'. $suffix;
      $sql .= " AND status = ". NODE_IMPORT_STATUS_ERROR;
      break;

    case 'done':
      $filename = $filename .'-done.'. $suffix;
      $sql .= " AND status = ". NODE_IMPORT_STATUS_DONE;
      break;

    default:
    case 'all':
      $filename = $filename .'-all.'. $suffix;
      break;
  }

  $sql .= " ORDER BY file_offset ASC";

  header('Content-Type: '. $mime);
  header('Content-Disposition: attachment; filename='. $filename);

  $objid = in_array('objid', $form_state['values']['download']['what_cols']);
  $errors = in_array('errors', $form_state['values']['download']['what_cols']);

  if ($task['has_headers']) {
    $headers = $task['headers'];
    if ($objid) {
      array_unshift($headers, t('Object'));
    }
    if ($errors) {
      array_unshift($headers, t('Errors'));
    }
    print node_import_write_to_string($headers, $file_options);
  }

  $result = db_query($sql, $task['taskid']);
  while (($row = db_fetch_object($result))) {
    list($file_offset, $data) = node_import_read_from_file($task['file']->filepath, $row->file_offset, $file_options);
    if ($objid) {
      array_unshift($data, $row->objid);
    }
    if ($errors) {
      array_unshift($data, implode("\n", (array)unserialize($row->errors)));
    }
    print node_import_write_to_string($data, $file_options);
  }

  exit();
}

function node_import_view_form_submit_delete($form, &$form_state) {
  $destination = '';
  if (isset($_REQUEST['destination'])) {
    $destination = drupal_get_destination();
    unset($_REQUEST['destination']);
  }
  $task = $form['#task'];
  $form_state['redirect'] = array('admin/content/node_import/'. $task['taskid'] .'/delete', $destination);
}

/**
 * Delete a task.
 */
function node_import_delete_form(&$form_state, $task) {
  $form = array();

  if ($task['status'] != NODE_IMPORT_STATUS_DONE) {
    $form[] = array(
      '#type' => 'markup',
      '#value' => t('This import task cannnot be deleted as it has not finished yet.'),
    );

    return $form;
  }

  $form['taskid'] = array(
    '#type' => 'value',
    '#value' => $task['taskid'],
  );

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $task['name'])),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/node_import/'. $task['taskid'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function node_import_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    node_import_delete_task($form_state['values']['taskid']);
  }

  $form_state['redirect'] = 'admin/content/node_import';
}

/**
 * General settings for node_import module.
 */
function node_import_settings_form(&$form_state) {
  $form = array();

  $form['node_import:directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Import directory'),
    '#description' => t('Enter the directory, relative to %file-directory-path, where the files to import are stored. If FTP is enabled, this is also the location where users can upload files. Default value: %default-directory.', array('%file-directory-path' => variable_get('file_directory_path', 'sites/default/files'), '%default-directory' => 'imports')),
    '#default_value' => variable_get('node_import:directory', 'imports'),
    '#field_prefix' => variable_get('file_directory_path', 'sites/default/files') .'/',
    '#field_suffix' => '/',
  );

  if (function_exists('theme_token_help')) {
    $form['node_import:directory']['#description'] .= theme('token_help', 'user');
  }

  $form['ftp'] = array(
    '#type' => 'fieldset',
    '#title' => t('FTP settings'),
  );

  $form['ftp']['node_import:ftp:enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow FTP uploads'),
    '#description' => t('If checked users can upload files to the import directory by using FTP (or other means) in addition to uploading files using the form. If unsure, uncheck this option.'),
    '#default_value' => variable_get('node_import:ftp:enabled', 0),
  );

  $form['ftp']['node_import:ftp:user'] = array(
    '#type' => 'textfield',
    '#title' => t('File owner'),
    '#description' => t('Files uploaded by FTP are assigned "ownership" to this user. Users will only be allowed to use files they have uploaded themselves and files "owned" by anonymous. If you leave this field blank, all files uploaded by FTP will be "owned" by anonymous and so all users will see those files as being available for them. If you enter a username here, files that are uploaded using FTP will be "owned" by that user and only that user will be able to see those uploaded files (in addition to the ones the user uploaded using the form). If unsure, leave this blank.'),
    '#default_value' => variable_get('node_import:ftp:user', ''),
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['ftp']['node_import:ftp:extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed extensions'),
    '#description' => t('Enter a space-delimited list of allowed file extensions. Only files uploaded with these extensions are allowed, other files will be ignored. Default value: %default-extensions.', array('%default-extensions' => 'csv tsv txt')),
    '#default_value' => variable_get('node_import:ftp:extensions', 'csv tsv txt'),
  );

  return system_settings_form($form);
}

/**
 * Theme the file selection list.
 */
function theme_node_import_file_select($form) {
  $header = array('', t('Filename'), t('Uploaded on'), t('Uploaded by'));
  $rows = array();

  foreach (element_children($form) as $child) {
    if (is_numeric($child)) {
      $element = $form[$child];
      $file = array(
        '#type' => 'item',
        '#value' => $element['filename']['#value'],
        '#description' => $element['filepath']['#value'] .' ('. $element['filesize']['#value'] .')',
      );
      unset($form['fid'][$child]['#title']);
      $rows[] = array(
        drupal_render($form['fid'][$child]),
        drupal_render($file),
        drupal_render($form[$child]['timestamp']),
        drupal_render($form[$child]['uid']),
      );
      unset($form[$child]);
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => t('No files available.'),
          'colspan' => count($header),
        ),
      ),
    );
    unset($form['fid']);
  }

  return theme('table', $header, $rows) . drupal_render($form);
}

/**
 * Theme the import task view list.
 */
function theme_node_import_imports_list($form) {
  $header = array(t('Name'), t('Created on'), t('Created by'), t('Status'), array('colspan' => 2, 'data' => t('Operations')));
  $rows = array();

  foreach (element_children($form) as $child) {
    if (is_numeric($child)) {
      $rows[] = array(
        drupal_render($form[$child]['name']),
        drupal_render($form[$child]['created']),
        drupal_render($form[$child]['uid']),
        drupal_render($form[$child]['status']),
        drupal_render($form[$child]['view']),
        drupal_render($form[$child]['delete']),
      );
      unset($form[$child]);
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No imports pending. You can <a href="!node_import-add">create a new import</a> using the wizard.', array('!node_import-add' => url('admin/content/node_import/add'))),
        'colspan' => count($header) + 1,
      ),
    );
  }

  return theme('table', $header, $rows) . drupal_render($form);
}

/**
 * Theme the file format selection list.
 */
function theme_node_import_file_format_select($form) {
  $header = array(t('File format'), t('Record separator'), t('Field separator'), t('Text delimiter'), t('Escape character'));
  $rows = array();

  $file_formats = $form['file_format']['#options'];
  foreach ($file_formats as $format => $title) {
    $rows[] = array(
      drupal_render($form['file_format'][$format]),
      isset($form[$format]['record separator']) ? drupal_render($form[$format]['record separator']) : drupal_render($form['record separator']) . drupal_render($form['other record separator']),
      isset($form[$format]['field separator']) ? drupal_render($form[$format]['field separator']) : drupal_render($form['field separator']) . drupal_render($form['other field separator']),
      isset($form[$format]['text delimiter']) ? drupal_render($form[$format]['text delimiter']) : drupal_render($form['text delimiter']) . drupal_render($form['other text delimiter']),
      isset($form[$format]['escape character']) ? drupal_render($form[$format]['escape character']) : drupal_render($form['escape character']) . drupal_render($form['other escape character']),
    );
    unset($form[$format]);
  }

  $form['file_format'] = array(
    '#type' => 'item',
    '#title' => $form['#title'],
    '#value' => theme('table', $header, $rows),
    '#description' => $form['#description'],
  );

  return drupal_render($form);
}

/**
 * Theme a table with sample data.
 */
function theme_node_import_sample_data($data, $length = 0) {
  $header = NULL;

  foreach ((array)$data as $i => $fields) {
    foreach ((array)$fields as $j => $value) {
      if ($length > 0 && strlen($value) > $length) {
        $data[$i][$j] = check_plain(substr($value, 0, $length)) .' '. theme('placeholder', format_plural(strlen($value) - $length, t('(1 more character)'), t('(@count more characters)')));
      }
      else {
        $data[$i][$j] = check_plain($value);
      }
    }
  }

  if (!empty($data) > 0) {
    $header = array_shift($data);
  }
  else {
    $data = array(t('Empty data set.'));
  }

  return theme('table', $header, $data);
}

/**
 * Theme the mapping, defaults and options tables.
 */
function theme_node_import_field_table($form) {
  $header = $form['#node_import-columns'];
  $rows = array();
  $groups = array();

  foreach (element_children($form) as $child) {
    if (!isset($form[$child]['#type']) || $form[$child]['#type'] != 'value') {
      $title = check_plain($form[$child]['#title']);
      $description = isset($form[$child]['#description']) ? $form[$child]['#description'] : '';
      $group = isset($form[$child]['#node_import-group']) ? $form[$child]['#node_import-group'] : '';
      unset($form[$child]['#title']);
      unset($form[$child]['#description']);

      if (!isset($groups[$group])) {
        $groups[$group] = array();
      }

      $groups[$group][] = array(
        check_plain($title) . '<div class="description">'. $description .'</div>',
        drupal_render($form[$child]),
      );
    }
  }

  if (isset($groups['']) && !empty($groups[''])) {
    $rows = array_merge($rows, $groups['']);
  }

  foreach ($groups as $group => $items) {
    if ($group !== '' && !empty($items)) {
      $rows[] = array(
        array('data' => $group, 'colspan' => 2, 'class' => 'region'),
      );
      $rows = array_merge($rows, $items);
    }
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => $form['#node_import-empty'], 'colspan' => 2));
  }

  return theme('table', $header, $rows) . drupal_render($form);
}

/**
 * Save a file with a debug report.
 */
function node_import_debug_report($task) {
  drupal_set_header('Content-Type: text/plain');
  drupal_set_header('Content-Disposition: attachment; filename="node_import-'. strtr($task['type'], ':', '_') .'-'. $task['taskid'] .'.txt"');

  print "------------------------------------------------------------\n";
  print "Drupal and PHP version:\n";
  print "------------------------------------------------------------\n";
  print "Drupal version: ". VERSION ." (". DRUPAL_CORE_COMPATIBILITY .")\n";
  print "PHP version: ". PHP_VERSION ."\n";
  print "\n";

  print "------------------------------------------------------------\n";
  print "Enabled modules and versions:\n";
  print "------------------------------------------------------------\n";
  foreach (module_rebuild_cache() as $name => $module) {
    if ($module->status && $module->type == 'module') {
      print $name ." ". $module->info['version'] ." (". $module->info['project'] .")\n";
    }
  }
  print "\n";

  print "------------------------------------------------------------\n";
  print "User permissions:\n";
  print "------------------------------------------------------------\n";
  if ($task['uid'] == 1) {
    print "User #1 has all privileges.\n";
  }
  else {
    $account = user_load(array('uid' => $task['uid']));
    $result = db_query("SELECT p.perm FROM {role} AS r INNER JOIN {permission} AS p ON p.rid = r.rid WHERE r.rid IN (". db_placeholders($account->roles) .")", array_keys($account->roles));
    $perms = array();
    while (($row = db_fetch_object($result))) {
      $perms += array_flip(explode(', ', $row->perm));
    }
    print implode("\n", array_keys($perms)) ."\n";
  }
  print "\n";

  print "------------------------------------------------------------\n";
  print "Task details:\n";
  print "------------------------------------------------------------\n";
  print '$task = '; var_dump($task);
  $fields = node_import_fields($task['type']);
  print '$fields = '; var_dump($fields);
  print "\n";

  print "------------------------------------------------------------\n";
  print "Errors and data from last ". variable_get('node_import:debug:row_count', 5) ." rows with errors:\n";
  print "------------------------------------------------------------\n";
  $result = db_query("SELECT * FROM {node_import_status} WHERE taskid = %d AND status = %d LIMIT %d", $task['taskid'], NODE_IMPORT_STATUS_ERROR, variable_get('node_import:debug:row_count', 5));
  $i = 0;
  while (($row = db_fetch_object($result))) {
    $i++;
    print "\n";
    print "----- Row $i -------------------------------------------------\n";
    $row->errors = unserialize($row->errors);
    print '$row_status = '; var_dump($row);

    list($file_offset, $data) = node_import_read_from_file($task['file']->filepath, $row->file_offset, $task['file_options']);
    print '$data = '; var_dump($data);
  }
  print "\n";

  exit();
}

