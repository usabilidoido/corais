<?php
// $Id: imagepicker.import.inc,v 1.1.2.6 2010/06/14 18:25:51 hutch Exp $
// $Name: DRUPAL-6--2-10 $

/**
 * @file
 * manage batch imports.
 */

module_load_include('inc', 'imagepicker', 'imagepicker.imagefuncs');

/**
 * Function to check the import folder.
 * @param $account
 *   Optional. user override
 * @return
 *   Returns the number of images.
 */
function imagepicker_import_dir_check($account = FALSE) {
  if (! $importdir = variable_get('imagepicker_import_dir', '')) {
    return FALSE;
  }
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }
  if (file_check_directory($importdir, TRUE)) {

    // clear out the noisy 'created' messages
    drupal_get_messages('status', TRUE);
    $importlist = file_scan_directory($importdir, ".*", array('.', '..', 'CVS'), 0, FALSE);
    $checked_importlist = array();
    $bad_importlist = array();
    if (count($importlist)) {
      foreach ($importlist AS $k => $v) {
        $file = $importlist[$k]->basename;
        $fullfile = $importlist[$k]->filename;
        if ( file_validate_is_image($fullfile)) {
          $checked_importlist[] = $file;
        }
        else {
          $bad_importlist[] = $fullfile;
        }
      }
    }
    else {
      drupal_set_message(t('No files to import.'), 'warning');
    }
  }
  else {
    drupal_set_message(t('Unable to create a directory structure for your import folder.'), 'error');
  }
  return (count($checked_importlist) > 0 ? $checked_importlist : 0);
}

/**
 * Function to display the image import form.
 *
 * @param $total
 *   Required. total number of images being imported.
 * @param $account
 *   Optional. user override
 * @param $admin
 *   Optional. admin switch
 * @return
 *   Returns the image form.
 */
function imagepicker_import_form($form_state, $total, $account=FALSE, $admin = FALSE) {
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }

  // provide checkbox list of groups if any
  // default watermark
  $form['total'] = array(
    '#type' => 'markup',
    '#value' => t('!total images found in the imagepicker import folder.<br />Selected user is %user', array('!total' => $total, '%user' => $user->name )),
  );

  $form['thumb'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail size'),
    '#size' => 10,
    '#default_value' => variable_get('imagepicker_default_thumbnail_size', 100),
    '#description' => t('Size in pixels of thumbnail\'s bigger side'),
    '#required' => TRUE,
  );
  $form['scale'] = array(
    '#type' => 'textfield',
    '#title' => t('Scale image'),
    '#size' => 10,
    '#default_value' => variable_get('imagepicker_default_scale', ''),
    '#description' => t('Scale all images in this import to this size in pixels if not left empty'),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Add a title to all images in this import'),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#rows' => 2,
    '#cols' => 80,
    '#description' => t('Add a description to all images in this import'),
  );

  if ( imagepicker_image_check_functions(TRUE) && variable_get('imagepicker_watermark_enable', 0)) {
    if (! variable_get('imagepicker_watermark_image', '')
      && (isset($user->imagepicker_watermark_image) ? $user->imagepicker_watermark_image : FALSE)
      && isset($user->imagepicker_watermark_enable)
      && $user->imagepicker_watermark_enable ) {
      $form['watermark'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use watermark'),
        '#description' => t('Use watermark on all images in this import.'),
        '#default_value' => (isset($user->imagepicker_watermark_use) ? $user->imagepicker_watermark_use : FALSE),
      );
    }
    elseif (variable_get('imagepicker_watermark_image', '')) {
      $form['watermark'] = array(
        '#type' => 'value',
        '#value' => 1,
      );
    }
  }

  // groups
  if (variable_get('imagepicker_groups_enabled', 1) && variable_get('imagepicker_groups_in_upload_enabled', 1)) {
    $grouplist = imagepicker_get_groups( ($admin ? $user : FALSE));
    if ($grouplist) {
      $form['grouplist'] = array(
        '#type' => 'checkboxes',
        '#options' => $grouplist,
        '#title' => t('Your Groups'),
        '#description' => t('Select a group to add this import to.'),
      );
    }
  }

  if ($admin) {
    $form['admin'] = array(
      '#type' => 'value',
      '#value' => 1,
    );
  }
  if ($account) {
    $form['account'] = array(
      '#type' => 'value',
      '#value' => $user->uid,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;

}

/**
 * Validate form
 */
function imagepicker_import_form_validate($form, &$form_state) {
  foreach ($form_state['values'] as $name => $value) {
    $value = trim($value);
    switch ($name) {
      case 'thumb':
        if (!preg_match('/^[0-9]{1,3}$/', $value) || $value <= 0) {
          form_set_error($name, t('Thumbnail size should be an integer between 1 and 999.'));
        }
        break;

      case 'scale':
        if (!preg_match('/^[0-9]{0,3}$/', $value)) {
          form_set_error($name, t('Scale value should be an integer between 1 and 999 or leave it empty if you don\'t want to scale your image.'));
        }
        break;
    }
  }
}

/**
 * Submit form
 */
function imagepicker_import_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op']: '';
  if ($op == t('Import')) {
    if ( isset($form_state['values']['account']) && isset($form_state['values']['admin'])) {
      $user = user_load(array('uid' => $form_state['values']['account']));
    }
    else {
      global $user;
    }

    $destdir = imagepicker_get_path(FALSE, (isset($form_state['values']['admin']) ? array('name' => $user->name, 'uid' => $user->uid ) : TRUE));
    $thumbsdir = $destdir . IMAGEPICKER_THUMBS_DIR;
    $browserdir = $destdir . IMAGEPICKER_BROWSER_DIR;
    $origdir = $destdir . IMAGEPICKER_ORIG_DIR;
    $sourcedir = variable_get('imagepicker_import_dir', '');

    if (file_check_directory($destdir, TRUE)
      && file_check_directory($thumbsdir, TRUE)
      && file_check_directory($browserdir, TRUE)
      && file_check_directory($origdir, TRUE)
      && file_check_directory($sourcedir, TRUE)
      ) {
      // clear out the noisy 'created' messages
      drupal_get_messages('status', TRUE);
      // Add DIRECTORY_SEPARATORS here because drupals' functions remove trailing slashes
      $options['destdir'] = $destdir . DIRECTORY_SEPARATOR;
      $options['thumbsdir'] = $thumbsdir . DIRECTORY_SEPARATOR;
      $options['browserdir']  = $browserdir . DIRECTORY_SEPARATOR;
      $options['origdir']  = $origdir . DIRECTORY_SEPARATOR;
      $options['sourcedir'] = $sourcedir;
      $selected = imagepicker_import_dir_check();

      $batch = array(
        'title' => t('Importing images'),
        'operations' => array(
          array('imagepicker_import_batch', array($form_state, $selected, $options, $user)),
        ),
        'finished' => 'imagepicker_import_finished',
        'file' =>  drupal_get_path('module', 'imagepicker') ."/imagepicker.import.inc",
        'progress_message' => t('Processed @current out of @total.'),
      );
      batch_set($batch);
      // batch_process();
    }
    else {
      drupal_set_message(t('Unable to create a directory structure for your images.'), 'error');
    }
  }
}

function imagepicker_import_batch($form_state, $selected, $options, $account, &$context) {
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }

  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($selected);
    $context['sandbox']['images'] = $selected;
  }
  // Process images by groups of 5.
  $count = min(5, count($context['sandbox']['images']));
  for ($i = 0; $i < $count; $i++) {
    $index = array_shift($context['sandbox']['images']);

    // Check the file is OK
    $filename = $index;
    $source_filepath = file_check_location($options['sourcedir'] . DIRECTORY_SEPARATOR . $filename, $options['sourcedir']);
    if ($source_filepath and file_validate_is_image($source_filepath)) {
      $source = $source_filepath;
      $file = $filename;

      // copy original
      if (file_copy($source, $options['origdir'], FILE_EXISTS_RENAME)) {
        $file = basename($source);
        $orig = $options['origdir'] . $file;
        $scaleto = ($form_state['values']['scale'] !="" ? $form_state['values']['scale'] : FALSE );
        if ($scaleto) {
          $imagescaled = imagepicker_scale_image($orig, $options['destdir'] . $file, $scaleto);
        }
        else {
          // no scaling, save direct from $origdir to $destdir
          file_copy($source, $options['destdir'] , FILE_EXISTS_RENAME);
          $file = basename($source);
        }
        // if watermark is enabled just apply to destdir image, not orig or the thumbs
        if ( $form_state['values']['watermark'] ) {
          if (! imagepicker_watermark_process($options['destdir'] . $file, $user)) {
            $_SESSION['imagepicker_import_status'] = t('Error while watermarking imported image %im.', array('%im' => $file));
          }
        }
        // thumbsdir
        // not sure why this has to be restated, but if not done the thumbs
        // get wmarked too when not scaling image above
        $orig = $options['origdir']  . $file;
        $maxthumbsize = $form_state['values']['thumb'] ? $form_state['values']['thumb'] : 100;
        if (imagepicker_scale_image($orig, $options['thumbsdir'] . $file, $maxthumbsize)) {
          //
          imagepicker_scale_image($orig, $options['browserdir'] . $file, variable_get('imagepicker_default_browser_thumbnail_size', 100));
          $title = htmlspecialchars($form_state['values']['title']);
          $description = htmlspecialchars($form_state['values']['description']);
          $nextimgid = imagepicker_insert_image($user->uid, $file, $title, $description);
          if ($nextimgid) {
            if ( is_array($form_state['values']['grouplist']) && variable_get('imagepicker_groups_enabled', 1) && variable_get('imagepicker_groups_in_upload_enabled', 1) ) {
              $result = db_query("SELECT img_id FROM {imagepicker} WHERE uid = '%d' AND img_name = '%s'", array($user->uid, $file));
              $row = db_fetch_array($result);
              $record['img_id']  = $row['img_id'];
              foreach ($form_state['values']['grouplist'] AS $gid) {
                $record['gid'] = $gid;
                imagepicker_insert_group_image($record);
              }
            }
            // delete
            if (variable_get('imagepicker_import_delete', 0)) {
              file_delete($source_filepath);
            }
          }
          else {
            file_delete($options['thumbsdir'] . $file);
            file_delete($options['browserdir'] . $file);
            file_delete($options['origdir'] . $file);
            file_delete($options['destdir'] . $file);
            $_SESSION['imagepicker_import_status'] = t('Error while saving information to database for imported image %im.', array('%im' => $file));
          }

        }
        else {
          $_SESSION['imagepicker_import_status'] = t('Could not scale %file', array('%file' => $source_filepath));
        }
      }
      else {
        $_SESSION['imagepicker_import_status'] = t('Could not copy to original: %file', array('%file' => $source_filepath));
      }
    }
    else {
      $_SESSION['imagepicker_import_status'] = t('Invalid image: %file', array('%file' => $source_filepath));
    }
    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['results'][] = $source_filepath;
    if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
    else {
      $context['finished'] = 1;
    }
  }
}

function imagepicker_import_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), 'One image processed.', '@count images processed.');
  }
  else {
    $message = t('Finished with an error.');
  }
  $_SESSION['imagepicker_import_status'] = $message;
}

/**
 * Function to display the import folder form
 *
 * @return
 *   Returns the import folder form.
 */
function imagepicker_import_dir_form($form_state) {

  $form['imagepicker_import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Imagepicker Import Folder'),
    '#description' => t('Path of the import folder, relative to the Drupal root or an absolute path.<br />This is where you upload images to, via FTP or rsync.'),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('imagepicker_import_dir', '') ? TRUE : FALSE),
  );
  $form['imagepicker_import']['imagepicker_import_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Import Folder'),
    '#size' => 55,
    '#maxlength' => 100,
    '#required' => FALSE,
    '#default_value' => variable_get('imagepicker_import_dir', ''),
  );
  $form['imagepicker_import']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate form
 */
function imagepicker_import_dir_form_validate($form, &$form_state) {
  $importdir = $form_state['values']['imagepicker_import_dir'];
  if ($importdir && ! file_check_directory($importdir, TRUE)) {
    form_set_error('imagepicker_import_dir', t('The folder does not exist.'));
  }
}

/**
 * Submit form
 */
function imagepicker_import_dir_form_submit($form, &$form_state) {
  variable_set('imagepicker_import_dir', $form_state['values']['imagepicker_import_dir']);
}
