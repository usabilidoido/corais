<?php
/**
 * @file
 * Public API of the Node import module.
 */

/**
 * @mainpage Public API of Node import
 *
 * Global variable: $node_import_can_continue
 *
 *   Boolean. Some imports may not be able to do more then one row at a
 *   time because of all kinds of (static) caching in Drupal. For example
 *   if you import a node with taxonomy terms and select to create the
 *   non-existing terms on import, taxonomy_get_tree() will not have an
 *   up-to-date internal list of terms and so the submission of the form
 *   will fail due to "An illegal choice has been detected. Please contact
 *   the site administrator.".
 *
 * TODO
 */

/**
 * @defgroup node_import_constants Node import constants
 * @{
 * The constants are used in {node_import_tasks} and {node_import_status}
 * database tables to specify the status of either the complete task or
 * one individual row.
 */

/**
 * Status pending.
 *
 * - In {node_import_tasks} : the task is not yet finished and there are
 *    more pending rows to import.
 *
 * - In {node_import_status} : the import of the row has started, but has
 *   not yet finished. This means the row may have to be reimported. Note
 *   that we don't really know what part of it didn't finish :-(
 */
define('NODE_IMPORT_STATUS_PENDING', 0);

/**
 * Status error.
 *
 * - In {node_import_tasks} : the task has finished with some errors and
 *   the user has chosen to retry importing the rows that had errors.
 *   Possibly after editing the task. This has not yet been implemented.
 *
 * - In {node_import_status} : the row did not import and there were
 *   errors.
 */
define('NODE_IMPORT_STATUS_ERROR', 1);

/**
 * Status finished.
 *
 * - In {node_import_tasks} : the task is fully finished and file and task
 *   can be deleted.
 *
 * - In {node_import_status} : the row has been imported succesfully.
 */
define('NODE_IMPORT_STATUS_DONE', 2);

/**
 * @}
 */

// Load all support files.
foreach ((array)drupal_system_listing('.*\.inc', drupal_get_path('module', 'node_import'), 'name') as $name => $file) {
  if (module_exists($name)) {
    require_once('./'. $file->filename);
  }
}

// Make sure the import directory is created.
node_import_directory();

/**
 * Returns the API version of node_import.
 *
 * Currently versions are/were:
 * - 1.x : 4.7.x version (unsupported),
 * - 2.x : 5.x version (unsupported),
 * - 3.x : 6.x-1.x version (current).
 *
 * @return
 *   A string similar to PHP or Drupal version. Major versions
 *   will not change function prototypes or return values. Minor
 *   versions may add additional keys to returned arrays and
 *   add non-required parameters to functions.
 *
 *   Note that the version does not reflect the version of the
 *   support files only of the API.
 */
function node_import_version() {
  return '3.0';
}

/**
 * @defgroup node_import_hooks Node import hooks
 * @{
 */

/**
 * Returns a list of available content types.
 *
 * @param $check_access
 *   Boolean. If TRUE, only the types the user can create are
 *   returned. If FALSE, all types are returned.
 *
 * @param $reset
 *   Boolean. If TRUE, the internal cache is rebuilt.
 *
 * @return
 *   Array of types. See hook_node_import_types().
 */
function node_import_types($check_access = TRUE, $reset = FALSE) {
  static $types;
  static $allowed_types;

  if (!isset($types) || $reset) {
    $defaults = array(
      'title' => '',
      'can_create' => FALSE,
      'create' => '',
    );

    $utypes = (array)module_invoke_all('node_import_types');
    foreach ($utypes as $type => $typeinfo) {
      $utypes[$type] = array_merge($defaults, $typeinfo);
    }
    drupal_alter('node_import_types', $utypes);

    $types = array_map('strip_tags', node_import_extract_property($utypes, 'title'));
    asort($types);
    foreach ($types as $type => $title) {
      $types[$type] = $utypes[$type];
    }

    $allowed_types = array();
    foreach ($types as $type => $typeinfo) {
      if ($typeinfo['can_create'] === TRUE || (function_exists($function = $typeinfo['can_create']) && $function($type) == TRUE)) {
        $allowed_types[$type] = $typeinfo;
      }
    }
  }

  return $check_access ? $allowed_types : $types;
}

/**
 * Returns a list of available content fields for given
 * node_import type.
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $reset
 *   Boolean. If TRUE, the internal cache is rebuilt.
 *
 * @return
 *   Array of fields. See hook_node_import_fields().
 */
function node_import_fields($type, $reset = FALSE) {
  static $fields;

  if (!isset($fields[$type]) || $reset) {
    $defaults = array(
      'title' => '',
      'tips' => array(),
      'group' => '',
      'module' => '',
      'weight' => 0,

      'is_mappable' => TRUE,
      'map_required' => FALSE,

      'has_multiple' => FALSE,
      'multiple_separator' => variable_get('node_import:multiple_separator', '||'),

      'has_hierarchy' => FALSE,
      'hierarchy_separator' => variable_get('node_import:hierarchy_separator', '>>'),
      'hierarchy_reverse' => FALSE,

      'input_format' => '',
      'preprocess' => array(),
      'allowed_values' => array(),

      'default_value' => NULL,
      'allow_empty' => FALSE,
      'map_required' => FALSE,
      'is_checkboxes' => FALSE,
    );

    $fields[$type] = (array)module_invoke_all('node_import_fields', $type);

    foreach ($fields[$type] as $fieldname => $fieldinfo) {
      // Merge sane defaults.
      $fields[$type][$fieldname] = $fieldinfo = array_merge($defaults, $fieldinfo);

      // Add preprocessors for builtin input_formats.
      if (!empty($fieldinfo['allowed_values'])) {
        $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_values';
        $s = '';
        foreach ($fieldinfo['allowed_values'] as $key => $value) {
          if (drupal_strlen($s) > 60) {
            $s .= ', &#8230;';
            break;
          }
          $s .= ($s == '' ? '' : ', ') . check_plain(drupal_strtolower($key)) . ': ' . check_plain(drupal_strtolower($value));
        }
        $fields[$type][$fieldname]['tips'][] = t('Allowed values (!values).', array('!values' => $s));
      }

      switch ($fieldinfo['input_format']) {
        case 'boolean':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_boolean';
          $fields[$type][$fieldname]['tips'][] = t('Boolean (0/1, off/on, no/yes, false/true).');
          break;

        case 'date':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_date';
          $fields[$type][$fieldname]['tips'][] = t('Date ("YYYY-MM-DD HH:MM" or specified custom format).');
          break;

        case 'email':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_email';
          break;

        case 'filepath':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_filepath';
          break;

        case 'node_reference':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_node_reference';
          $fields[$type][$fieldname]['tips'][] = t('Node reference (by nid or title).');
          break;

        case 'user_reference':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_user_reference';
          $fields[$type][$fieldname]['tips'][] = t('User reference (by uid, name or mail).');
          break;

        case 'weight':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_weight';
          break;
      }
    }

    drupal_alter('node_import_fields', $fields[$type], $type);

    // Sort by weight.
    $count = 0;
    $max_count = count($fields[$type]) * 10;
    foreach ($fields[$type] as $fieldname => $fieldinfo) {
      $fields[$type][$fieldname]['weight'] += $count / $max_count;
      $count++;
    }
    uasort($fields[$type], 'node_import_sort');
  }

  return $fields[$type];
}

/**
 * Returns a list of default (form elements).
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $defaults
 *   Array of currently filled in values.
 *
 * @param $fields
 *   Array of available fields.
 *
 * @param $map
 *   Array of how fields are mapped.
 *
 * @return
 *   FAPI array. See hook_node_import_defaults().
 */
function node_import_defaults($type, $defaults, $fields, $map) {
  if (!is_array($defaults)) {
    $defaults = array();
  }

  $form = (array)module_invoke_all('node_import_defaults', $type, $defaults, $fields, $map);

  foreach ($fields as $fieldname => $fieldinfo) {
    // Set #node_import-group if not set.
    if (isset($form[$fieldname]) && !isset($form[$fieldname]['#node_import-group'])) {
      $form[$fieldname]['#node_import-group'] = $fieldinfo['group'];
    }

    // Set #weight if not set.
    if (isset($form[$fieldname]) && !isset($form[$fieldname]['#weight'])) {
      $form[$fieldname]['#weight'] = $fieldinfo['weight'];
    }

    // Set #description if not set.
    if (isset($form[$fieldname]) && !isset($form[$fieldname]['#description'])) {
      if (count($fieldinfo['tips']) > 1) {
        $form[$fieldname]['#description'] = '<ul class="tips">';
        $form[$fieldname]['#description'] .= '<li>' . implode('</li><li>', $fieldinfo['tips']) . '</li>';
        $form[$fieldname]['#description'] .= '</ul>';
      }
      else if (!empty($fieldinfo['tips'])) {
        $form[$fieldname]['#description'] = implode('', $fieldinfo['tips']);
      }
    }

    // Set default_value as value.
    if (!empty($fieldinfo['default_value']) && !isset($form[$fieldname])) {
      $form[$fieldname] = array(
        '#type' => 'value',
        '#value' => $fieldinfo['default_value'],
      );
    }
  }

  drupal_alter('node_import_defaults', $form, $type, $defaults, $fields, $map);

  return $form;
}

/**
 * Returns a list of options (form elements).
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $options
 *   Array of currently filled in values.
 *
 * @param $fields
 *   Array of available fields.
 *
 * @param $map
 *   Array of how fields are mapped.
 *
 * @return
 *   FAPI array. See hook_node_import_options().
 */
function node_import_options($type, $options, $fields, $map) {
  if (!is_array($options)) {
    $options = array();
  }

  $form = (array)module_invoke_all('node_import_options', $type, $options, $fields, $map);

  // Copy from modules/system/system.admin.inc
  $date_short = array('Y-m-d H:i', 'm/d/Y - H:i', 'd/m/Y - H:i', 'Y/m/d - H:i',
     'd.m.Y - H:i', 'm/d/Y - g:ia', 'd/m/Y - g:ia', 'Y/m/d - g:ia',
     'M j Y - H:i', 'j M Y - H:i', 'Y M j - H:i',
     'M j Y - g:ia', 'j M Y - g:ia', 'Y M j - g:ia');
  $date_short_choices = array();
  foreach ($date_short as $f) {
    $date_short_choices[$f] = format_date(time(), 'custom', $f);
  }
  $date_short_choices['custom'] = t('Custom format');
  $timezones = date_timezone_names(TRUE);

  foreach ((array)$fields as $fieldname => $fieldinfo) {
    // Set #node_import-group if not set.
    if (isset($form[$fieldname]) && !isset($form[$fieldname]['#node_import-group'])) {
      $form[$fieldname]['#node_import-group'] = $fieldinfo['group'];
    }

    // Set #description if not set.
    if (isset($form[$fieldname]) && !isset($form[$fieldname]['#description'])) {
      if (count($fieldinfo['tips']) > 1) {
        $form[$fieldname]['#description'] = '<ul class="tips">';
        $form[$fieldname]['#description'] .= '<li>' . implode('</li><li>', $fieldinfo['tips']) . '</li>';
        $form[$fieldname]['#description'] .= '</ul>';
      }
      else if (!empty($fieldinfo['tips'])) {
        $form[$fieldname]['#description'] = implode('', $fieldinfo['tips']);
      }
    }

    // Set #weight if not set.
    if (isset($form[$fieldname]) && !isset($form[$fieldname]['#weight'])) {
      $form[$fieldname]['#weight'] = $fieldinfo['weight'];
    }

    $map_count = node_import_field_map_count($fieldname, $map);

    // Add multiple_separator option for fields that can have multiple
    // values AND that is mapped to exactly one file column.
    if ($fieldinfo['has_multiple'] &&
        $map_count == 1 &&
        (!isset($form[$fieldname]) || !isset($form[$fieldname]['multiple_separator']))) {

      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      $form[$fieldname]['multiple_separator'] = array(
        '#type' => 'textfield',
        '#title' => t('Multiple values are separated by'),
        '#size' => 6,
        '#default_value' => isset($options[$fieldname]['multiple_separator']) ? $options[$fieldname]['multiple_separator'] : $fieldinfo['multiple_separator'],
      );
    }

    // Add hierarchy_separator option for fields that can have
    // hierarchical values AND (that can have multiple values OR
    // that can not have multiple values but is mapped to exactly
    // one file column).
    if ($fieldinfo['has_hierarchy'] &&
        ($fieldinfo['has_multiple'] || $map_count == 1) &&
        $map_count > 0 &&
        (!isset($form[$fieldname]) || !isset($form[$fieldname]['hierarchy_separator']))) {

      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      $form[$fieldname]['hierarchy_separator'] = array(
        '#type' => 'textfield',
        '#title' => t('Hierarchy is specified by'),
        '#size' => 6,
        '#default_value' => isset($options[$fieldname]['hierarchy_separator']) ? $options[$fieldname]['hierarchy_separator'] : $fieldinfo['hierarchy_separator'],
      );
    }

    // Add hierarchy_reverse option for fields that can have
    // hierarchical values AND that can not have multiple values
    // AND that is mapped to more than one file column.
    if ($fieldinfo['has_hierarchy'] &&
        $map_count > 1 && !$fieldinfo['has_multiple'] &&
        (!isset($form[$fieldname]) || !isset($form[$fieldname]['hierarchy_reverse']))) {

      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      if ($map_count > 1 && !$fieldinfo['has_multiple']) {
        $form[$fieldname]['hierarchy_reverse'] = array(
          '#type' => 'checkbox',
          '#title' => t('Reverse file columns for hierarchy'),
          '#default_value' => isset($options[$fieldname]['hierarchy_reverse']) ? $options[$fieldname]['hierarchy_reverse'] : $fieldinfo['hierarchy_reverse'],
        );
      }
    }

    // Add a custom date format option for fields that are dates.
    if ($fieldinfo['input_format'] == 'date' && $map_count > 0) {
      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      $form[$fieldname]['timezone'] = array(
        '#type' => 'select',
        '#title' => t('Timezone'),
        '#default_value' => isset($options[$fieldname]['timezone']) ? $options[$fieldname]['timezone'] : date_default_timezone_name(),
        '#options' => $timezones,
        '#description' => t('Select the default time zone. If in doubt, choose the timezone that is closest to your location which has the same rules for daylight saving time.'),
      );

      $form[$fieldname]['date_format'] = array(
        '#type' => 'select',
        '#title' => t('Date format'),
        '#default_value' => isset($options[$fieldname]['date_format']) ? $options[$fieldname]['date_format'] : variable_get('date_format_short', 'm/d/Y - H::i'),
        '#options' => $date_short_choices,
        '#description' => t('Select the date format for import. If you choose <em>Custom format</em> enter the custom format below.'),
      );
      $form[$fieldname]['date_custom'] = array(
        '#type' => 'textfield',
        '#title' => t('Custom date format'),
        '#attributes' => array('class' => 'custom-format'),
        '#default_value' => isset($options[$fieldname]['date_custom']) ? $options[$fieldname]['date_custom'] : variable_get('date_format_short', 'm/d/Y - H::i'),
        '#description' => t('See the <a href="@url" target="_blank">PHP manual</a> for available options.', array('@url' => 'http://php.net/manual/function.date.php')),
      );
    }

    // Add directory selection for fields that are filepaths.
    if ($fieldinfo['input_format'] == 'filepath' && $map_count > 0) {
      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      $form[$fieldname][] = array(
        '#value' => t('You need to FTP the files you reference in this field manually to the correct location (%path) before doing the import.', array('%path' => file_create_path(isset($fieldinfo['to_directory']) ? $fieldinfo['to_directory'] : ''))),
      );
      $form[$fieldname]['manually_moved'] = array(
        '#type' => 'hidden',
        '#value' => TRUE,
      );

/*
TODO: we disable this until I get the time to figure out why files are not
moved to the correct location if you do not move them manually.

//TODO: as the directory must be relative... we could let the user choose from a select box
//TODO: we might use the sample data to find the correct directory
      $form[$fieldname]['from_directory'] = array(
        '#type' => 'textfield',
        '#title' => t('Copy from'),
        '#field_prefix' => node_import_directory() . '/',
        '#field_suffix' => '/',
        '#default_value' => isset($options[$fieldname]['from_directory']) ? $options[$fieldname]['from_directory'] : '',
        '#description' => t('Fill in the directory which contains the files.'),
      );

      $form[$fieldname]['manually_moved'] = array(
        '#type' => 'checkbox',
        '#title' => t('Files have been manually moved'),
        '#default_value' => isset($options[$fieldname]['manually_moved']) ? $options[$fieldname]['manually_moved'] : 0,
        '#description' => t('Check this box if you have already moved the files to the correct location on your server (%path).', array('%path' => file_create_path(isset($fieldinfo['to_directory']) ? $fieldinfo['to_directory'] : ''))),
      );
*/

/* TODO
      $form[$fieldname]['delete_on_success'] = array(
        '#type' => 'checkbox',
        '#title' => t('Delete files after import'),
        '#default_value' => isset($options[$fieldname]['delete_on_success']) ? $options[$fieldname]['delete_on_success'] : 1,
        '#description' => t('Check this box if you want to delete the files from the server after a succesful import.'),
      );
*/
    }
  }

  drupal_alter('node_import_options', $form, $type, $options, $fields, $map);

  return $form;
}

/**
 * Create an array of values to submit to the form.
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $data
 *   Array of data from the file as ($col_index => $value).
 *
 * @param $map
 *   Array of how the data maps to fields.
 *
 * @param $defaults
 *   Array of default values.
 *
 * @param $options
 *   Array of options.
 *
 * @param $fields
 *   Array of available fields.
 *
 * @param $preview
 *   Boolean. If TRUE a preview will be created. If FALSE construct the
 *   values for real.
 *
 * @return
 *   Array of values to submit to a form. See hook_node_import_values().
 */
function node_import_values($type, $data, $map, $defaults, $options, $fields, $preview) {
  $values = module_invoke_all('node_import_values', $type, $defaults, $options, $fields, $preview);

  foreach ($fields as $fieldname => $fieldinfo) {
    $map[$fieldname] = isset($map[$fieldname]) ? $map[$fieldname] : '';
    $map[$fieldname] = is_array($map[$fieldname]) ? $map[$fieldname] : array($map[$fieldname]);
    $map_count = node_import_field_map_count($fieldname, $map);

    $mseparator = isset($options[$fieldname]['multiple_separator']) ? $options[$fieldname]['multiple_separator'] : $fieldinfo['multiple_separator'];
    $hseparator = isset($options[$fieldname]['hierarchy_separator']) ? $options[$fieldname]['hierarchy_separator'] : $fieldinfo['hierarchy_separator'];
    $hreverse = isset($options[$fieldname]['hierarchy_reverse']) ? $options[$fieldname]['hierarchy_reverse'] : $fieldinfo['hierarchy_reverse'];

    // Merge default value for each field.
    if (isset($defaults[$fieldname])) {
      if ($fieldinfo['is_checkboxes']) {
        $values[$fieldname] = array_keys(array_filter($defaults[$fieldname]));
      }
      else {
        $values[$fieldname] = $defaults[$fieldname];
      }
    }
    else if (isset($fieldinfo['default_value'])) {
      $values[$fieldname] = $fieldinfo['default_value'];
    }

    // Map the data ONLY IF the data to map is not empty.
    if ($fieldinfo['has_multiple']) {
      if ($map_count > 0) {
        $fieldvalues = array();

        foreach ($map[$fieldname] as $col) {
          $value = isset($data[$col]) ? (string)$data[$col] : '';
          if ($map_count == 1 && strlen($mseparator) > 0) {
            $fieldvalues = strlen($value) > 0 ? array_map('trim', explode($mseparator, $value)) : array();
            break;
          }
          $fieldvalues[] = $value;
        }

        if (!$fieldinfo['allow_empty']) {
          $fieldvalues = array_filter($fieldvalues, 'drupal_strlen');
        }

        if ($fieldinfo['has_hierarchy'] && strlen($hseparator) > 0) {
          foreach ($fieldvalues as $i => $value) {
            $fieldvalues[$i] = strlen($value) > 0 ? array_map('trim', explode($hseparator, $value)) : array($value);
          }
        }

        if (empty($fieldvalues) && isset($values[$fieldname])) {
          $values[$fieldname] = $values[$fieldname];
        }
        else {
          $values[$fieldname] = $fieldvalues;
        }
      }
    }
    else {
      if ($map_count > 0 && $fieldinfo['has_hierarchy']) {
        $fieldvalues = array();

        foreach ($map[$fieldname] as $col) {
          $value = isset($data[$col]) ? (string)$data[$col] : '';
          if ($map_count == 1 && strlen($hseparator) > 0) {
            $fieldvalues = drupal_strlen($value) > 0 ? array_map('trim', explode($hseparator, $value)) : array();
            break;
          }
          $fieldvalues[] = $value;
        }

        if (!$fieldinfo['allow_empty']) {
          $fieldvalues = array_filter($fieldvalues, 'drupal_strlen');
        }

        if ($hreverse) {
          $fieldvalues = array_reverse($fieldvalues);
        }

        $values[$fieldname] = empty($fieldvalues) ? $values[$fieldname] : $fieldvalues;
      }
      else if ($map_count == 1) {
        foreach ($map[$fieldname] as $col) {
          $value = isset($data[$col]) ? (string)$data[$col] : '';
        }

        $values[$fieldname] = drupal_strlen($value) > 0 ? $value : (isset($values[$fieldname]) ? $values[$fieldname] : '');
      }

      $values[$fieldname] = array(isset($values[$fieldname]) ? $values[$fieldname] : '');
    }

    // Preprocess the data as long as the value is not empty and it
    // validates.
    $values[$fieldname] = isset($values[$fieldname]) ? $values[$fieldname] : array('');
    foreach ((array)$values[$fieldname] as $i => $value) {
      foreach ($fieldinfo['preprocess'] as $function) {
        if ((is_array($values[$fieldname][$i]) && !empty($values[$fieldname][$i])) // has_multiple values are arrays
            || (is_string($values[$fieldname][$i]) && drupal_strlen($values[$fieldname][$i]) > 0)) {
          $options[$fieldname] = isset($options[$fieldname]) ? $options[$fieldname] : array();

          $value = $values[$fieldname][$i];
          $return = $function($value, $fieldinfo, $options[$fieldname], $preview);
          $values[$fieldname][$i] = $value;

          if ($return === FALSE) {
            // No longer validates - stop.
            $values[$fieldname][$i] = '';
            continue 2;
          }
          else if ($return === TRUE) {
            // Final preprocess reported - stop.
            continue 2;
          }
        }
      }
    }

    // If empty values are not allowed, filter them out.
    if (!$fieldinfo['allow_empty']) {
      $values[$fieldname] = array_filter((array)$values[$fieldname], 'drupal_strlen');
    }

    // Handle files specially. The preprocess function only returns
    // the path - we need to make sure we save the file now into the
    // db and set the value to the fid. We need to do this here instead
    // of in the preprocess function because we need $values['uid'].
    if ($fieldinfo['input_format'] == 'filepath') {
      foreach ($values[$fieldname] as $i => $value) {
        if (drupal_strlen($value) > 0) {
          $result = db_result(db_query("SELECT fid FROM {files} WHERE filepath = '%s'", $value));
          if ($result) {
            $values[$fieldname][$i] = $result;
          }
          else {
            // TODO: don't we need more stuff - eg run the validators?
            global $user;
            $file = new stdClass();
            $file->uid = isset($values['uid']) ? $values['uid'] : $user->uid;
            $file->filename = basename($value);
            $file->filepath = $value;
            $file->filesize = filesize($value);
            $file->filemime = file_get_mimetype($file->filename);
            $file->status = FILE_STATUS_TEMPORARY;
            $file->timestamp = time();
            drupal_write_record('files', $file);
            $values[$fieldname][$i] = $file->fid;
          }
        }
      }
    }

    // If only a single value is allowed, get the first one.
    if (!$fieldinfo['has_multiple']) {
      $values[$fieldname] = array_shift($values[$fieldname]);
    }

    // Convert checkboxes fields to a format FAPI understands.
    if ($fieldinfo['is_checkboxes'] && !empty($values[$fieldname])) {
      // Only in PHP > 5.2: $values[$fieldname] = array_fill_keys($values[$fieldname], 1);
      $values[$fieldname] = array_combine($values[$fieldname], array_fill(0, count($values[$fieldname]), 1));
    }
  }

  drupal_alter('node_import_values', $values, $type, $defaults, $options, $fields, $preview);

  return $values;
}

/**
 * Get a list of options for different stuff presented to the
 * user in the wizard form such as 'record separators', ...
 *
 * @param $op
 *   String. See hook_node_import_format_options().
 *
 * @param $reset
 *   Boolean. If TRUE, the internal cache is reset.
 *
 * @return
 *   Array.
 */
function node_import_format_options($op, $reset = FALSE) {
  static $options;

  if (!isset($options) || ($reset && !isset($op))) {
    $options = array();
  }

  if (isset($op) && (!isset($options[$op]) || $reset)) {
    $options[$op] = module_invoke_all('node_import_format_options', $op);
    drupal_alter('node_import_format_options', $options[$op], $op);
  }

  return isset($op) ? $options[$op] : array();
}

/**
 * @}
 */

/**
 * Import a number of rows from all available tasks. Should only be called
 * from within hook_cron() or from a JS callback as this function may take
 * a long time.
 *
 * The function ends when $count $units have been finished. For example
 * @code
 * node_import_do_all_tasks('all');
 * node_import_do_all_tasks('rows', 10);
 * node_import_do_all_tasks('bytes', 4096);
 * node_import_do_all_tasks('ms', 1000);
 * @endcode
 *
 * @param $unit
 *   String. Either 'rows', 'bytes', 'ms' (milliseconds) or 'all'.
 *   Defaults to 'all'.
 *
 * @param $count
 *   Integer. Number of $units to do. Defaults to 0 (in which case
 *   exactly one row will be imported if $unit != 'all').
 *
 * @return
 *   Nothing.
 */
function node_import_do_all_tasks($unit = 'all', $count = 0) {
  global $node_import_can_continue;

  $byte_count = 0;
  $row_count = 0;
  timer_start('node_import:do_all_tasks');

  foreach (node_import_list_tasks(TRUE) as $taskid => $task) {
    $bytes = $task['file_offset'];
    $rows = $task['row_done'] + $task['row_error'];

    node_import_do_task($task, $unit, $count);

    $byte_count += $task['file_offset'] - $bytes;
    $row_count += $task['row_done'] + $task['row_error'] - $rows;

    if ($node_import_can_continue && ($unit == 'all'
        || ($unit == 'bytes' && $byte_count < $count)
        || ($unit == 'rows' && $row_count < $count)
        || ($unit == 'ms' && timer_read('node_import:do_all_tasks') < $count))) {
      continue;
    }

    break;
  }

  timer_stop('node_import:do_all_tasks');
}

/**
 * Import a number of rows from the specified task. Should only be called
 * from within hook_cron() or from a JS callback as this function may take
 * a long time.
 *
 * The function ends when $count $units have been finished. For example
 * @code
 * node_import_do_task($task, 'all');
 * node_import_do_task($task, 'rows', 10);
 * node_import_do_task($task, 'bytes', 4096);
 * node_import_do_task($task, 'ms', 1000);
 * @endcode
 *
 * @param $task
 *   Array. The task to continue. Note that this is passed by reference!
 *   So you can check the status of the task after running this function
 *   without having to query the database.
 *
 * @param $unit
 *   String. Either 'rows', 'bytes', 'ms' (milliseconds) or 'all'.
 *   Defaults to 'all'.
 *
 * @param $count
 *   Integer. Number of $units to do. Defaults to 0 (in which case
 *   exactly one row will be imported if $unit != 'all').
 *
 * @return
 *   The status of each imported row (error or not) is stored in the
 *   database. @see node_import_constants.
 */
function node_import_do_task(&$task, $unit = 'all', $count = 0) {
  global $node_import_can_continue;
  $node_import_can_continue = TRUE;

  if ($task['status'] != NODE_IMPORT_STATUS_DONE && node_import_lock_acquire()) {
    global $user;
    $backup_user = $user;
    if ($task['uid'] != $user->uid) {
      session_save_session(FALSE);
      $user = user_load(array('uid' => $task['uid']));
    }

    $taskid = $task['taskid'];

    $file_offset = 0;
    $byte_count = 0;
    $row_count = 0;
    timer_start('node_import:do_task:'. $taskid);

    $data = array();
    switch ($task['status']) {
      case NODE_IMPORT_STATUS_PENDING:
        if ($task['file_offset'] == 0 && $task['has_headers']) {
          list($file_offset, $data) = node_import_read_from_file($task['file']->filepath, $file_offset, $task['file_options']);
        }
        else {
          $file_offset = $task['file_offset'];
        }
        break;

      case NODE_IMPORT_STATUS_ERROR:
        $task['status'] = NODE_IMPORT_STATUS_DONE;
        $file_offset = $task['file']->filesize; //TODO
        break;
    }

    module_invoke_all('node_import_task', $task, 'continue');

    while ($task['status'] != NODE_IMPORT_STATUS_DONE) {
      list($new_offset, $data) = node_import_read_from_file($task['file']->filepath, $file_offset, $task['file_options']);

      if (is_array($data)) {
        switch ($task['status']) {
          case NODE_IMPORT_STATUS_PENDING:
            db_query("DELETE FROM {node_import_status} WHERE taskid = %d AND file_offset = %d", $taskid, $file_offset);
            db_query("INSERT INTO {node_import_status} (taskid, file_offset, errors) VALUES (%d, %d, '%s')", $taskid, $file_offset, serialize(array()));
            break;

          case NODE_IMPORT_STATUS_ERROR:
            db_query("UPDATE {node_import_status} SET errors = '%s', status = %d WHERE taskid = %d AND file_offset = %d", serialize(array()), NODE_IMPORT_STATUS_PENDING, $taskid, $file_offset);
            break;
        }

        db_query("UPDATE {node_import_tasks} SET file_offset = %d, changed = %d WHERE taskid = %d", $new_offset, time(), $taskid);
        $task['file_offset'] = $new_offset;

        $errors = node_import_create($task['type'], $data, $task['map'], $task['defaults'], $task['options'], FALSE);

        if (is_array($errors)) {
          db_query("UPDATE {node_import_status} SET status = %d, errors = '%s' WHERE taskid = %d AND file_offset = %d", NODE_IMPORT_STATUS_ERROR, serialize($errors), $taskid, $file_offset);
          db_query("UPDATE {node_import_tasks} SET row_error = row_error + 1 WHERE taskid = %d", $taskid);
          $task['row_error']++;
        }
        else {
          db_query("UPDATE {node_import_status} SET status = %d, objid = %d WHERE taskid = %d AND file_offset = %d", NODE_IMPORT_STATUS_DONE, $errors, $taskid, $file_offset);
          db_query("UPDATE {node_import_tasks} SET row_done = row_done + 1 WHERE taskid = %d", $taskid);
          $task['row_done']++;
        }

        $byte_count += $new_offset - $file_offset;
        $row_count++;
      }
      else {
        db_query("UPDATE {node_import_tasks} SET status = %d, file_offset = %d WHERE taskid = %d", NODE_IMPORT_STATUS_DONE, $task['file']->filesize, $taskid);
        $task['status'] = NODE_IMPORT_STATUS_DONE;
        $task['file_offset'] = $task['file']->filesize;
      }

      switch ($task['status']) {
        case NODE_IMPORT_STATUS_PENDING:
          $file_offset = $new_offset;
          break;

        case NODE_IMPORT_STATUS_ERROR:
          $file_offset = $task['file']->filesize; //TODO
          break;
      }

      if ($node_import_can_continue && ($unit == 'all'
          || ($unit == 'bytes' && $byte_count < $count)
          || ($unit == 'rows' && $row_count < $count)
          || ($unit == 'ms' && timer_read('node_import:do_task:'. $taskid) < $count))) {
        continue;
      }

      break;
    }

    module_invoke_all('node_import_task', $task, 'pause');

    // Cleanup before exit.
    $user = $backup_user;
    session_save_session(TRUE);
    timer_stop('node_import:do_task:'. $taskid);
    node_import_lock_release();
  }
}

/**
 * JS callback to continue the specified task and returns the status
 * of it. This function will take at most one second.
 *
 * @param $task
 *   Full loaded $task.
 *
 * @return
 *   JSON.
 */
function node_import_js($task) {
  node_import_do_task($task, 'ms', 1000);
  echo drupal_json(array(
    'status' => 1,
    'message' => format_plural($task['row_done'], t('1 row imported'), t('@count rows imported')) .'<br />'.
                 format_plural($task['row_error'], t('1 row with errors'), t('@count rows with errors')),
    'percentage' => $task['status'] == NODE_IMPORT_STATUS_DONE ? 100 : round(floor(100.0 * $task['file_offset'] / $task['file']->filesize), 0),
  ));
  exit();
}

/**
 * Create a new object of specified $type.
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $data
 *   Array of data from the file as ($col_index => $value).
 *
 * @param $map
 *   Array of how the data maps to fields.
 *
 * @param $defaults
 *   Array of default values.
 *
 * @param $options
 *   Array of options.
 *
 * @param $preview
 *   Boolean.
 *
 * @return
 *   The return value is: if $preview is TRUE, a string with the preview
 *   of the object is returned. If $preview is FALSE, an unique identifier
 *   is returned or an array with errors if failed.
 */
function node_import_create($type, $data, $map, $defaults, $options, $preview) {
  $output = $preview ? '' : array();

  // Reset execution time. Note that this only works when SAFE_MODE is OFF but
  // it has no side-effects if SAFE_MODE is ON. See
  // http://php.net/manual/en/function.set-time-limit.php
  set_time_limit(variable_get('node_import:set_time_limit', 60));

  $types = node_import_types();
  $fields = node_import_fields($type);

  // We need to clean out the form errors before submitting.
  form_set_error(NULL, '', TRUE);

  // Create a list of values to submit.
  $values = node_import_values($type, $data, $map, $defaults, $options, $fields, $preview);

  // Submit it for preview or creation.
  if (function_exists($function = $types[$type]['create'])) {
    $output = $function($type, $values, $preview);
  }
  module_invoke_all('node_import_postprocess', $type, $values, $options, $preview);

  // Check for errors and clear them again.
  if ($preview) {
    $output = theme('status_messages') . $output;
  }
  if (($errors = form_get_errors())) {
    if ($preview) {
      $output .= '<pre>values = '. print_r($values, TRUE) .'</pre>'; //TODO: show data instead?
    }
    else {
      $output = $errors;
    }
  }
  form_set_error(NULL, '', TRUE);
  drupal_get_messages(NULL, TRUE); // Otherwise they are still showed.

  return $output;
}

/**
 * @defgroup node_import_tasks Node import tasks
 * @{
 */

/**
 * Create a new import task.
 *
 * @param $values
 *   Array of filled in values.
 *
 * @return
 *   Integer. Unique identifier for the task or FALSE if the task could
 *   not be saved to the database.
 */
function node_import_save_task($values) {
  global $user;

  if (!isset($values['uid'])) {
    $values['uid'] = $user->uid;
  }
  if (!isset($values['created'])) {
    $values['created'] = time();
  }
  if (!isset($values['changed'])) {
    $values['changed'] = time();
  }

  if (drupal_write_record('node_import_tasks', $values) === SAVED_NEW) {
    module_invoke_all('node_import_task', $values, 'insert');
    return $values['taskid'];
  }
  return FALSE;
}

/**
 * Get a list of available tasks.
 *
 * @param $all
 *   Boolean. If TRUE, all tasks are returned. If FALSE, only the tasks
 *   the current user has access to.
 *
 * @return
 *   Array of tasks.
 */
function node_import_list_tasks($all = FALSE) {
  global $user;
  $tasks = array();

  if ($all || user_access('administer imports')) {
    $result = db_query("SELECT * FROM {node_import_tasks} ORDER BY created ASC");
  }
  else {
    $result = db_query("SELECT * FROM {node_import_tasks} WHERE uid = %d ORDER BY created ASC", $user->uid);
  }

  while (($task = db_fetch_array($result))) {
    foreach (array('file_options', 'headers', 'map', 'defaults', 'options') as $key) {
      $task[$key] = isset($task[$key]) ? unserialize($task[$key]) : array();
    }

    $task['file'] = db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = %d", $task['fid']));
    $tasks[$task['taskid']] = $task;
  }

  return $tasks;
}

/**
 * Delete an import task.
 *
 * @param $taskid
 *   Unique identifier.
 *
 * @return
 *   Nothing.
 */
function node_import_delete_task($taskid) {
  module_invoke_all('node_import_task', $taskid, 'delete');
  db_query("DELETE FROM {node_import_tasks} WHERE taskid = %d", $taskid);
  db_query("DELETE FROM {node_import_status} WHERE taskid = %d", $taskid);
}

/**
 * @}
 */

/**
 * @defgroup node_import_preprocess Node import preprocess functions
 * @{
 */

/**
 * Check if the value is a valid boolean (1, 0, true, false, yes, no, on, off).
 *
 * Uses: nothing.
 */
function node_import_check_boolean(&$value, $field, $options, $preview) {
  static $trues;
  static $falses;

  if (!isset($trues)) {
    $trues = array(
      '1',
      'on', drupal_strtolower(t('On')),
      'yes', drupal_strtolower(t('Yes')),
      'true', drupal_strtolower(t('True')),
    );
    $falses = array(
      '0',
      'off', drupal_strtolower(t('Off')),
      'no', drupal_strtolower(t('No')),
      'false', drupal_strtolower(t('False')),
    );
  }

  if (in_array(drupal_strtolower($value), $trues, TRUE)) {
    $value = '1';
    return TRUE;
  }
  else if (in_array(drupal_strtolower($value), $falses, TRUE)) {
    $value = '0';
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a boolean).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is a valid date.
 *
 * Uses: $field['output_format'] (output format type - defaults to DATE_UNIX).
 * Uses: $options['date_format'], $options['date_custom'] and $options['timezone'] (default to date_default_timezone_name()).
 */
function node_import_check_date(&$value, $field, $options, $preview) {
  $timezone = isset($options['timezone']) ? $options['timezone'] : date_default_timezone_name();
  $input_format = $options['date_format'] == 'custom' ? $options['date_custom'] : $options['date_format'];
  $output_format = isset($field['output_format']) ? $field['output_format'] : DATE_UNIX;

  if (date_is_valid($value, DATE_ISO)) {
    $value = date_convert($value, DATE_ISO, $output_format, $timezone);
    return TRUE;
  }

  module_load_include('inc', 'date_api', 'date_api_elements');

  if (($date = date_convert_from_custom($value, $input_format))) {
    // It is useless to check for date_is_valid() as it is a DATE_DATETIME already.
    $value = date_convert($date, DATE_DATETIME, $output_format, $timezone);
    return TRUE;
  }

  if (date_is_valid($value, DATE_UNIX)) {
    $value = date_convert($value, DATE_UNIX, $output_format, $timezone);
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a date in %date format).', array('%value' => $value, '%name' => $field['title'], '%date' => format_date(time(), 'custom', $input_format))));
  return FALSE;
}

/**
 * Check if the value is a valid email address.
 *
 * Uses: nothing.
 */
function node_import_check_email(&$value, $field, $options, $preview) {
  if (!valid_email_address($value)) {
    node_import_input_error(t('Input error: %value is not a valid e-mail address.', array('%value' => $value)));
    return FALSE;
  }
  return TRUE;
}

/**
 * Check if the value points to a valid filepath.
 *
 * Uses: $field['to_directory'], $options['from_directory'], $options['manually_moved'].
 */
function node_import_check_filepath(&$value, $field, $options, $preview) {
  // No need to check empty values.
  if (drupal_strlen($value) == 0) {
    return TRUE;
  }

  // Where should be file be located?
  $find_in = isset($options['from_directory']) ? $options['from_directory'] : '';
  $find_in = node_import_directory() . (strlen($find_in) > 0 ? '/'. $find_in : '');
  if (isset($options['manually_moved']) && $options['manually_moved']) {
    $find_in = isset($field['to_directory']) ? $field['to_directory'] : '';
  }
  $find_in = file_create_path($find_in);

  // Check if the file exists.
  $filepath = $find_in .'/'. $value;
  if (file_check_location($filepath, $find_in) && file_exists($filepath)) {
    $value = $filepath;
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a file in %path).', array('%value' => $value, '%name' => $field['title'], '%path' => $find_in)));
  $value = '';
  return FALSE;
}

/**
 * Check if the value is a valid node reference (by nid or title).
 *
 * Uses: $field['output_format']. Either 'nid' (default) or 'title'.
 */
function node_import_check_node_reference(&$value, $field, $options, $preview) {
  if (($nid = node_import_get_object('node', $value)) !== NULL ||
      ($nid = db_result(db_query("SELECT nid FROM {node} WHERE nid = %d OR LOWER(title) = '%s' LIMIT 1", is_numeric($value) && intval($value) > 0 ? $value : -1, drupal_strtolower($value))))) {

    node_import_set_object('node', $value, $nid);
    $value = $nid;

    $field['output_format'] = isset($field['output_format']) ? $field['output_format'] : 'nid';
    switch ($field['output_format']) {
      case 'title':
        if (($title = node_import_get_object('node:title', $nid)) ||
            ($title = db_result(db_query("SELECT title FROM {node} WHERE nid = %d LIMIT 1", $nid)))) {
          $value = $title;
          node_import_set_object('node:title', $nid, $title);
        }
        break;

      case 'nid':
      default:
        break;
    }

    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a node reference).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is a valid user (by uid, name or email).
 *
 * Uses: $field['output_format']. Either 'uid' (default), 'name' or 'email'.
 */
function node_import_check_user_reference(&$value, $field, $options, $preview) {
  if (($uid = node_import_get_object('user', $value)) !== NULL ||
      ($uid = db_result(db_query("SELECT uid FROM {users} WHERE uid = %d OR LOWER(name) = '%s' OR LOWER(mail) = '%s' LIMIT 1", is_numeric($value) && intval($value) > 0 ? $value : -1, drupal_strtolower($value), drupal_strtolower($value))))) {

    node_import_set_object('user', $value, $uid);
    $value = $uid;

    $field['output_format'] = isset($field['output_format']) ? $field['output_format'] : 'uid';
    switch ($field['output_format']) {
      case 'name':
        if (($name = node_import_get_object('user:name', $uid)) ||
            ($name = db_result(db_query("SELECT name FROM {users} WHERE uid = %d LIMIT 1", $uid)))) {
          $value = $name;
          node_import_set_object('user:name', $uid, $name);
        }
        break;

      case 'email':
        if (($email = node_import_get_object('user:email', $uid)) ||
            ($email = db_result(db_query("SELECT mail FROM {users} WHERE uid = %d LIMIT 1", $uid)))) {
          $value = $email;
          node_import_set_object('user:email', $uid, $email);
        }
        break;

      case 'uid':
      default:
        break;
    }

    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not an user).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is in the list of allowed values (by key or value).
 *
 * Uses: $field['allowed_values'].
 */
function node_import_check_values(&$value, $field, $options, $preview) {
  foreach ($field['allowed_values'] as $key => $title) {
    $tmp = drupal_strtolower($value);
    if ($tmp === drupal_strtolower($key) || $tmp === drupal_strtolower($title)) {
      $value = (string)$key;
      return TRUE;
    }
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not in allowed values list).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is a valid weight (integer between -X and X).
 *
 * Uses: $field['delta'].
 */
function node_import_check_weight(&$value, $field, $options, $preview) {
  $weight = isset($field['delta']) ? $field['delta'] : 10;

  if (is_numeric($value) && intval($value) <= $weight && intval($value) >= -$weight) {
    $value = intval($value);
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a weight).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * @}
 */

/**
 * @defgroup node_import_util Various node import utility functions.
 * @{
 */

/**
 * Store an object-id in the node_import cache.
 *
 * As some string->object-id lookups can be expensive (in db queries) and
 * most of the time the same strings are looked up (eg users), we have a
 * cache of object-ids we already have looked up.
 *
 * This function is used to store an object-id in the cache.
 *
 * @param $type
 *   String. The type of object (eg 'user').
 *
 * @param $value
 *   String or array. The value we haved looked up.
 *
 * @param $oid
 *   Integer. The looked-up object-id. If NULL, the currently stored
 *   object-id is returned without setting the $type/$value to NULL.
 *   If you want to reset (eg make it NULL) a value, use
 *   @code
 *   node_import_set_object($type, $value, NULL, TRUE);
 *   @endcode
 *
 * @param $reset
 *   Boolean. Whether to reset the cache. If $type is NULL, the whole
 *   cache is reset. If $value is not NULL only the cache for that
 *   specific $type/$value is reset.
 *
 * @return
 *   Integer. The looked-up object-id. NULL if not found.
 *
 * @see node_import_get_object().
 */
function node_import_set_object($type, $value, $oid = NULL, $reset = FALSE) {
  static $cache;

  if (!isset($cache)) {
    $cache = array();
  }
  if (isset($type) && !isset($cache[$type])) {
    $cache[$type] = array();
  }

  $stored_value = NULL;
  if (isset($value)) {
    $stored_value = is_array($value) ? implode("\n", array_map('drupal_strtolower', $value)) : drupal_strtolower($value);
  }

  if ($reset) {
    if (isset($type)) {
      if (isset($value)) {
        unset($cache[$type][$stored_value]);
      }
      else {
        $cache[$type] = array();
      }
    }
    else {
      $cache = array();
    }
    return;
  }

  if (isset($oid)) {
    $cache[$type][$stored_value] = $oid;
  }

  return isset($cache[$type][$stored_value]) ? $cache[$type][$stored_value] : NULL;
}

/**
 * Get an object-id from the node_import cache.
 *
 * @param $type
 *   String. The type of object (eg 'user').
 *
 * @param $value
 *   String or array. The value to get the object-id of.
 *
 * @return
 *   NULL if not yet in cache. Otherwise an integer.
 *
 * @see node_import_set_object().
 */
function node_import_get_object($type, $value) {
  return node_import_set_object($type, $value);
}

/**
 * Get a property from each element of an array.
 *
 * @param $array
 *   Array of ($key => $info).
 *
 * @param $property
 *   String.
 *
 * @return
 *   Array of ($key => $info->$property).
 */
function node_import_extract_property($array, $property = 'title') {
  $result = array();
  foreach ((array)$array as $key => $info) {
    if (is_array($info) || is_object($info)) {
      $info = (array)$info;
      $result[$key] = isset($info[$property]) ? $info[$property] : '';
    }
    else {
      $result[$key] = $info;
    }
  }
  return $result;
}

/**
 * Function used by uasort to sort structured arrays by weight.
 */
function node_import_sort($a, $b) {
  $a_group = (is_array($a) && isset($a['group'])) ? $a['group'] : '';
  $b_group = (is_array($b) && isset($b['group'])) ? $b['group'] : '';
  if ($a_group !== $b_group && ($a_group === '' || $b_group === '')) {
    return ($a_group === '') ? -1 : 1;
  }

  $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
  $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    return 0;
  }

  return ($a_weight < $b_weight) ? -1 : 1;
}

/**
 * Returns the number of columns given $fieldname is mapped
 * to.
 *
 * @param $fieldname
 *   String.
 *
 * @param $map
 *   Array of file column mapping.
 *
 * @return
 *   Integer. Number of file columns the field is mapped to.
 */
function node_import_field_map_count($fieldname, $map) {
  if (!isset($map[$fieldname])) {
    return 0;
  }
  if (!is_array($map[$fieldname])) {
    return strlen($map[$fieldname]) > 0;
  }
  $count = 0;
  foreach ($map[$fieldname] as $col) {
    if ($col !== '') {
      $count++;
    }
  }
  return $count;
}

/**
 * Set an error on a random form element.
 */
function node_import_input_error($message, $args = array()) {
  static $count = 0;
  form_set_error('node_import-'. $count, $message, $args);
  $count++;
}

/**
 * @}
 */

/**
 * @defgroup node_import_files Node import file functions
 * @{
 */

/**
 * Return the full path where files to import are stored.
 *
 * @param $reset
 *   Boolean. Reset internal cache.
 *
 * @return
 *   String. The directory is created if needed.
 */
function node_import_directory($reset = FALSE) {
  static $path;

  if (!isset($path) || $reset) {
    $path = variable_get('node_import:directory', 'imports');

    if (function_exists('token_replace')) {
      global $user;
      $path = token_replace($path, 'user', user_load($user->uid));
    }

    if (strlen($path) > 0) {
      $parts = array_filter(explode('/', $path));
      $path = '';
      while (!empty($parts)) {
        $path .= array_shift($parts) . '/';
        $path_to_check = file_create_path($path);
        file_check_directory($path_to_check, FILE_CREATE_DIRECTORY, 'node_import:directory');
      }
    }

    $path = rtrim($path, '/');
    $path = file_create_path($path);
  }

  return $path;
}

/**
 * Returns a list of available files.
 */
function node_import_list_files($reset = FALSE) {
  global $user;
  static $files;

  if (!isset($files) || $reset) {
    $files = array();
    $path = node_import_directory();

    // If FTP uploads of files is allowed, rescan the directory.
    if (variable_get('node_import:ftp:enabled', 0)) {
      $ftp_user = user_load(array('name' => variable_get('node_import:ftp:user', '')));
      //TODO: use the $validators functionality of file_save_upload() ?
      $extensions = array_map('drupal_strtolower', array_filter(explode(' ', variable_get('node_import:ftp:extensions', 'csv tsv txt'))));
      foreach ($extensions as $extension) {
        $extensions[] = drupal_strtoupper($extension);
      }

      if (!empty($extensions)) {
        $existing_files = array();
        $result = db_query("SELECT filepath FROM {files} WHERE filepath LIKE '%s%%' AND status = %d", $path, FILE_STATUS_PERMANENT);
        while (($file = db_fetch_object($result))) {
          $existing_files[$file->filepath] = TRUE;
        }
        foreach (file_scan_directory($path, '.*\.(('. implode(')|(', $extensions) .'))') as $filename => $file) {
          if (!isset($existing_files[$file->filename])) {
            $record = (object)array(
              'uid' => $ftp_user->uid,
              'filename' => $file->basename,
              'filepath' => $file->filename,
              'filemime' => 'text/plain', //TODO: how to get real MIME?
              'filesize' => filesize($file->filename),
              'status' => FILE_STATUS_PERMANENT,
              'timestamp' => time(),
            );
            drupal_write_record('files', $record);
            drupal_set_message(t('A new file %name was detected in %path.', array('%name' => $record->filename, '%path' => $path)));
          }
        }
      }
    }

    // Users with 'administer imports' permission can see all files.
    //TODO: we should also filter out files that are already in use by another task.
    if (user_access('administer imports')) {
      $result = db_query("SELECT * FROM {files} WHERE filepath LIKE '%s%%' AND status = %d ORDER BY filename, timestamp", $path, FILE_STATUS_PERMANENT);
    }
    else {
      $result = db_query("SELECT * FROM {files} WHERE filepath LIKE '%s%%' AND (uid = %d OR uid = 0) AND status = %d ORDER BY filename, timestamp", $path, $user->uid, FILE_STATUS_PERMANENT);
    }

    // Create the list, keep only the still existing files.
    while (($file = db_fetch_object($result))) {
      if (!file_exists(file_create_path($file->filepath))) {
        drupal_set_message(t('The previously uploaded file %name is no longer present. Removing it from the list of uploaded files.', array('%name' => $file->filepath)));
        file_set_status($file, FILE_STATUS_TEMPORARY);
      }
      else {
        $files[$file->fid] = $file;
      }
    }
  }

  return $files;
}

/**
 * Return an autodetected mapping for given headers and content type.
 *
 * The automapping is done by checking the column titles in the file,
 * whether they match with the field name or field title.
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $headers
 *   Array of column titles in the file.
 *
 * @return
 *   Array of mapping.
 */
function node_import_automap($type, $headers) {
  global $user;

  if (user_access('administer imports')) {
    $result = db_query("SELECT map FROM {node_import_tasks} WHERE type = '%s' AND LOWER(headers) = '%s' ORDER BY created DESC LIMIT 1", $type, strtolower(serialize($headers)));
  }
  else {
    $result = db_query("SELECT map FROM {node_import_tasks} WHERE type = '%s' AND LOWER(headers) = '%s' AND uid = %d ORDER BY created DESC LIMIT 1", $type, strtolower(serialize($headers)), $user->uid);
  }
  if (($map = db_result($result))) {
    return unserialize($map);
  }

  $map = array();
  $headers = array_map('drupal_strtolower', $headers);

  foreach (node_import_fields($type) as $fieldname => $fieldinfo) {
    if ($fieldinfo['is_mappable']) {
      $map[$fieldname] = '';
      if (($col = array_search(drupal_strtolower($fieldname), $headers)) !== FALSE
          || ($col = array_search(drupal_strtolower($fieldinfo['title']), $headers)) !== FALSE) {
        $map[$fieldname] = $col;
      }
    }
  }

  return $map;
}

/**
 * Return an autodetected file format and file options for given
 * file.
 *
 * This checks which file format options cause the file to contain
 * the most fields. The file options tested are the ones used
 * by the user (last 5) and the builtin ones.
 *
 * @param $filepath
 *   String. Path to file.
 *
 * @return
 *   Array of file options.
 */
function node_import_autodetect($filepath) {
  global $user;

  if (user_access('administer imports')) {
    $result = db_query("SELECT DISTINCT file_options FROM {node_import_tasks} ORDER BY created DESC LIMIT 5");
  }
  else {
    $result = db_query("SELECT DISTINCT file_options FROM {node_import_tasks} WHERE uid = %d ORDER BY created DESC LIMIT 5");
  }

  $file_formats = node_import_format_options('file formats');
  while (($used_format = db_fetch_array($result))) {
    $file_formats[] = unserialize($used_format['file_options']);
  }

  $format = 'csv';
  $max_cols = 0;

  foreach ($file_formats as $file_format => $file_options) {
    $file_offset = 0;

    $num_rows = variable_get('node_import:detect:row_count', 5);
    $num_cols = 0;
    $fields = array();

    while ($num_rows > 0 && is_array($fields)) {
      list($new_offset, $fields) = node_import_read_from_file($filepath, $file_offset, $file_options);
      $num_rows--;

      if (is_array($fields) && count($fields) > $num_cols) {
        $num_cols = count($fields);
      }
    }

    if ($num_cols > $max_cols) {
      $format = $file_format;
      $max_cols = $num_cols;
    }
  }

  $file_options = $file_formats[$format];
  $file_options['file_format'] = is_int($format) ? '' : $format;

  return $file_options;
}

/**
 * Returns one record from the file starting at file offset using
 * the supplied file options.
 *
 * @param $filepath
 *   String. Path to file.
 *
 * @param $file_offset
 *   Integer. Starting point of record.
 *
 * @param $file_options
 *   Array with 'record separator', 'field separator', 'text delimiter'
 *   and 'escape character'. If not set, the options default to the
 *   CSV options ("\n", ',', '"', '"').
 *
 * @return
 *   Array ($file_offset, $record). The $file_offset is the start offset
 *   of the next record. The $record is an array of fields (strings).
 *
 *   On error or when the end of the file has been reached we return
 *   FALSE.
 */
function node_import_read_from_file($filepath, $file_offset, $file_options) {
  // Open file and set to file offset.
  if (($fp = fopen($filepath, 'r')) === FALSE) {
    return FALSE;
  }
  if (fseek($fp, $file_offset)) {
    return FALSE;
  }

  // File options.
  _node_import_sanitize_file_options($file_options);

  $rs = $file_options['record separator'];
  $fs = $file_options['field separator'];
  $td = $file_options['text delimiter'];
  $ec = $file_options['escape character'];

  // The current record is stored in the $fields array. The $new_offset
  // contains the file position of the end of the returned record. Note
  // that if $new_offset == $file_offset we have reached the end of the
  // file.
  $fields = array();
  $new_offset = $file_offset;
  $start = 0;

  // We read $length bytes at a time in the $buffer.
  $length = variable_get('node_import:fgets:length', 1024);
  $buffer = fgets($fp, $length);

  // A field can be enclosed in text delimiters or not. If this variable is
  // TRUE, we need to parse until we find the next unescaped text delimiter.
  // If FALSE, the field value was not enclosed.
  $enclosed = FALSE;

  // Read until the EOF or until end of record.
  while (!feof($fp) || $start < strlen($buffer)) {
    if (!$enclosed) {
      // Find the next record separator, field separator and text delimiter.
      if ($rs === "\n") {
        $pos_rs = strpos($buffer, "\n", $start);
        $pos_rs = $pos_rs !== FALSE ? $pos_rs : strpos($buffer, "\r", $start);
      }
      else {
        $pos_rs = strpos($buffer, $rs, $start);
      }
      $pos_fs = strpos($buffer, $fs, $start);
      $pos_td = strlen($td) > 0 ? strpos($buffer, $td, $start) : FALSE;

      // Check for begin of text delimited field.
      if ($pos_td !== FALSE && ($pos_rs === FALSE || $pos_td <= $pos_rs) && ($pos_fs === FALSE || $pos_td <= $pos_fs)) {
        $enclosed = TRUE;
        $buffer = substr($buffer, 0, $pos_td) . substr($buffer, $pos_td + strlen($td));
        $new_offset += strlen($td);
        $start = $pos_td;
        continue;
      }

      // Check for end of record.
      if ($pos_rs !== FALSE && ($pos_fs === FALSE || $pos_rs <= $pos_fs)) {
        if ($pos_rs >= 0) {
          $fields[] = substr($buffer, 0, $pos_rs);
          $buffer = '';
          $new_offset += $pos_rs;
          $start = 0;
        }
        else if (empty($fields)) {
          $buffer = substr($buffer, strlen($rs), strlen($buffer) - strlen($rs));
          $new_offset += strlen($rs);
          $start = 0;
          continue;
        }
        $new_offset += strlen($rs);
        break;
      }

      // Check for end of field.
      if ($pos_fs !== FALSE) {
        $fields[] = substr($buffer, 0, $pos_fs);
        $buffer = substr($buffer, $pos_fs + strlen($fs));
        $new_offset += $pos_fs + strlen($fs);
        $start = 0;
        continue;
      }
    }
    else {
      // Find the next text delimiter and escaped text delimiter.
      $pos_td = strpos($buffer, $td, $start);
      $pos_ec = strlen($ec) > 0 ? strpos($buffer, $ec . $td, $start) : FALSE;

      // Check for end of text delimited field.
      if ($pos_td !== FALSE && ($pos_ec === FALSE || $pos_td <= ($pos_ec - strlen($td)))) {
        $enclosed = FALSE;
        $buffer = substr($buffer, 0, $pos_td) . substr($buffer, $pos_td + strlen($td));
        $new_offset += strlen($td);
        $start = $pos_td;
        continue;
      }

      // Check for escaped text delimiter.
      if ($pos_ec !== FALSE) {
        $buffer = substr($buffer, 0, $pos_ec) . substr($buffer, $pos_ec + strlen($ec));
        $new_offset += strlen($ec);
        $start = $pos_ec + strlen($td);
        continue;
      }
    }

    // Nothing found... read more data.
    $start = strlen($buffer);
    $buffer .= fgets($fp, $length);
  }

  // Check if we need to add the last field.
  if (feof($fp) && strlen($buffer) > 0) {
    $fields[] = $buffer;
    $new_offset += strlen($buffer);
  }

  // Remove extra white space.
  $fields = array_map('trim', $fields);

  // Check whether the whole row is empty.
  $empty_row = TRUE;
  foreach ($fields as $field) {
    if (strlen($field) > 0) {
      $empty_row = FALSE;
      break;
    }
  }
  if ($empty_row && !feof($fp) && !empty($fields)) {
    return node_import_read_from_file($filepath, $new_offset, $file_options);
  }

  // Cleanup and return.
  $result = (!feof($fp) || !empty($fields)) ? array($new_offset, $fields) : FALSE;
  unset($buffer);
  fclose($fp);

  return $result;
}

function _node_import_sanitize_file_options(&$file_options) {
  // When file_format is not '', we need to load the options from the
  // predefined file_format.
  if (isset($file_options['file_format']) && $file_options['file_format'] !== '') {
    $file_formats = node_import_format_options('file formats');
    $file_options = array_merge($file_options, $file_formats[$file_options['file_format']]);
  }

  // The default options (CSV).
  $options = array(
    'record separator' => '<newline>',
    'field separator'  => ',',
    'text delimiter'   => '"',
    'escape character' => '"',
  );

  // Use 'other ...' when selected.
  foreach ($options as $key => $default) {
    if (isset($file_options[$key]) && drupal_strlen($file_options[$key]) > 0) {
      $options[$key] = $file_options[$key];
    }
    else if (isset($file_options['other '. $key]) && drupal_strlen($file_options['other '. $key]) > 0) {
      $options[$key] = $file_options['other '. $key];
    }
  }

  // Replace our special characters.
  $replaces = array(
    '<newline>' => "\n",
    '<none>'    => '',
    '<space>'   => ' ',
    '<tab>'     => "\t",
  );
  $file_options = str_replace(array_keys($replaces), array_values($replaces), $options);
}

/**
 * Returns one line in the specified file format of the array of
 * values.
 *
 * @param $values
 *   Array of strings.
 *
 * @param $file_options
 *   Array with 'record separator', 'field separator', 'text delimiter'
 *   and 'escape character'. If not set, the options default to the
 *   CSV options ("\n", ',', '"', '"').
 *
 * @return
 *   String.
 */
function node_import_write_to_string($values, $file_options) {
  // File options.
  _node_import_sanitize_file_options($file_options);

  $rs = $file_options['record separator'];
  $fs = $file_options['field separator'];
  $td = $file_options['text delimiter'];
  $ec = $file_options['escape character'];

  // Write data.
  $output = '';

  if (is_array($values) && !empty($values)) {
    // TODO: we could avoid writing $td if the $value does not contain $td, $fs or $rs.
    if (drupal_strlen($td) > 0) {
      foreach ($values as $i => $value) {
        $values[$i] = $td . str_replace($td, $ec . $td, $value) . $td;
      }
    }
    $output = implode($fs, $values);
  }

  return $output . $rs;
}

/**
 * @}
 */

/**
 * @defgroup node_import_form_hacks Hacks for includes/form.inc
 * @{
 * The problem node_import has by design - where the design is that we
 * want to use the normal form validation - is that when a form is
 * submitted more then once, the validation of the form is not done
 * correctly by the core includes/form.inc
 *
 * This file tries to lift this limitation. The solution is based (or
 * rather copied) from sites/all/modules/views/includes/form.inc of
 * the views module which needs to do the same crappy stuff (and even
 * more).
 *
 * Short explanation: instead of
 * @code
 *   drupal_execute($form_id, $form_state, ...);
 * @endcode
 * use
 * @code
 *   node_import_drupal_execute($form_id, $form_state, ...);
 * @endcode
 * whenever the form you want to execute can be executed more than
 * once in the same page request.
 *
 * For the core bug, see http://drupal.org/node/260934 : Static caching:
 * cannot call drupal_validate_form on the same form more than once.
 *
 * Note that another bug for multiple form validation and submission
 * (one that could not be lifted) was fixed in Drupal 6.5, see
 * http://drupal.org/node/180063 : No way to flush form errors during
 * iterative programatic form submission.
 *
 * Many, many thanks to merlinofchaos!!
 */

/**
 * The original version of drupal_execute() calls drupal_process_form().
 * The modified version sets $form_state['must_validate'] = TRUE and
 * calls node_import_drupal_process_form() instead.
 */
function node_import_drupal_execute($form_id, &$form_state) {
  $args = func_get_args();
  $args[1] = &$form_state;
  $form = call_user_func_array('drupal_retrieve_form', $args);
  $form['#post'] = $form_state['values'];
  $form_state['must_validate'] = TRUE;
  drupal_prepare_form($form_id, $form, $form_state);
  node_import_drupal_process_form($form_id, $form, $form_state);
}

/**
 * The original version of drupal_process_form() calls drupal_validate_form().
 * The modified version calls node_import_drupal_validate_form() instead.
 */
function node_import_drupal_process_form($form_id, &$form, &$form_state) {
  $form_state['values'] = array();

  $form = form_builder($form_id, $form, $form_state);
  // Only process the form if it is programmed or the form_id coming
  // from the POST data is set and matches the current form_id.
  if ((!empty($form['#programmed'])) || (!empty($form['#post']) && (isset($form['#post']['form_id']) && ($form['#post']['form_id'] == $form_id)))) {
    node_import_drupal_validate_form($form_id, $form, $form_state);

    // form_clean_id() maintains a cache of element IDs it has seen,
    // so it can prevent duplicates. We want to be sure we reset that
    // cache when a form is processed, so scenerios that result in
    // the form being built behind the scenes and again for the
    // browser don't increment all the element IDs needlessly.
    form_clean_id(NULL, TRUE);

    if ((!empty($form_state['submitted'])) && !form_get_errors() && empty($form_state['rebuild'])) {
      $form_state['redirect'] = NULL;
      form_execute_handlers('submit', $form, $form_state);

      // We'll clear out the cached copies of the form and its stored data
      // here, as we've finished with them. The in-memory copies are still
      // here, though.
      if (variable_get('cache', CACHE_DISABLED) == CACHE_DISABLED && !empty($form_state['values']['form_build_id'])) {
        cache_clear_all('form_'. $form_state['values']['form_build_id'], 'cache_form');
        cache_clear_all('storage_'. $form_state['values']['form_build_id'], 'cache_form');
      }

      // If batches were set in the submit handlers, we process them now,
      // possibly ending execution. We make sure we do not react to the batch
      // that is already being processed (if a batch operation performs a
      // drupal_execute).
      if ($batch =& batch_get() && !isset($batch['current_set'])) {
        // The batch uses its own copies of $form and $form_state for
        // late execution of submit handers and post-batch redirection.
        $batch['form'] = $form;
        $batch['form_state'] = $form_state;
        $batch['progressive'] = !$form['#programmed'];
        batch_process();
        // Execution continues only for programmatic forms.
        // For 'regular' forms, we get redirected to the batch processing
        // page. Form redirection will be handled in _batch_finished(),
        // after the batch is processed.
      }

      // If no submit handlers have populated the $form_state['storage']
      // bundle, and the $form_state['rebuild'] flag has not been set,
      // we're finished and should redirect to a new destination page
      // if one has been set (and a fresh, unpopulated copy of the form
      // if one hasn't). If the form was called by drupal_execute(),
      // however, we'll skip this and let the calling function examine
      // the resulting $form_state bundle itself.
      if (!$form['#programmed'] && empty($form_state['rebuild']) && empty($form_state['storage'])) {
        drupal_redirect_form($form, $form_state['redirect']);
      }
    }
  }
}

/**
 * The original version of drupal_validate_form() keeps a static array
 * of validated forms. The modified version checks $form_state['must_validate']
 * to see if the form needs validation. If set and TRUE, validation is
 * forced even if it was already done.
 */
function node_import_drupal_validate_form($form_id, $form, &$form_state) {
  static $validated_forms = array();

  if (isset($validated_forms[$form_id]) &&
      (!isset($form_state['must_validate']) || $form_state['must_validate'] !== TRUE)) { //Changed!
    return;
  }

  // If the session token was set by drupal_prepare_form(), ensure that it
  // matches the current user's session.
  if (isset($form['#token'])) {
    if (!drupal_valid_token($form_state['values']['form_token'], $form['#token'])) {
      // Setting this error will cause the form to fail validation.
      form_set_error('form_token', t('Validation error, please try again. If this error persists, please contact the site administrator.'));
    }
  }

  _form_validate($form, $form_state, $form_id);
  $validated_forms[$form_id] = TRUE;
}

/**
 * @}
 */

/**
 * @defgroup node_import_locking Node import locking functions
 * @{
 * Locking functions for node_import. This code is based on #251792
 * (Implement a locking framework for long operations). We need
 * locking to avoid both hook_cron() and node_import_view_form() to
 * process the same task at the same time (which would result in
 * rows being processed twice).
 *
 * The code below uses the database-based locking mechanism. Note
 * that is does not use the full implementation as in the patch
 * because we can have only one global lock. One lock for each task
 * does not work because a task can spawn the creation of something
 * else (such as a taxonomy term).
 *
 * We only need one _acquire() and _release() function.
 *
 * When a more general locking framework is committed to Drupal,
 * we can easily replace this.
 */

/**
 * Acquire or release our node_import lock.
 *
 * @param $release
 *   Boolean. If TRUE, release the lock. If FALSE, acquire the
 *   lock.
 *
 * @return
 *   Boolean. Whether the lock was acquired.
 */
function node_import_lock_acquire($release = FALSE) {
  static $lock_id, $locked;

  if (!isset($lock_id)) {
    $lock_id = md5(uniqid());
    $locked = FALSE;
    register_shutdown_function('node_import_lock_release');
  }

  if ($release) {
    db_query("DELETE FROM {variable} WHERE name = '%s'", 'node_import:lock');
    $locked = FALSE;
  }
  else if (!$locked) {
    if (@db_query("INSERT INTO {variable} (name, value) VALUES ('%s', '%s')", 'node_import:lock', $lock_id)) {
      $locked = TRUE;
    }
  }

  return $locked;
}

/**
 * Release our node_import lock.
 *
 * @return
 *   Nothing.
 */
function node_import_lock_release() {
  node_import_lock_acquire(TRUE);
}

/**
 * @}
 */

