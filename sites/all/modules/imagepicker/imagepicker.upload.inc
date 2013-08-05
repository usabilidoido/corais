<?php
// $Id: imagepicker.upload.inc,v 1.3.2.18 2010/10/06 12:03:49 hutch Exp $
// $Name: DRUPAL-6--2-10 $

/**
 * @file
 * upload functions
 */

module_load_include('inc', 'imagepicker', 'imagepicker.imagefuncs');

/**
 * Menu callback; presents the upload form for imagepicker
 */
// iframe
function imagepicker_upload() {
  variable_del('imagepicker_advanced_browser_pagestart');
  if (variable_get('imagepicker_upload_progress_enabled', 1)) {
    $content = imagepicker_upload_progress_get_script(variable_get('imagepicker_upload_progress_delay', 0));
  }
  $content .= imagepicker_quota_ok('iframe', FALSE, '', t('Upload images. You can give them a title and description'));
  return theme('imagepicker_iframe', $content);
}
// account
function imagepicker_user_upload() {
  global $user;
  variable_del('imagepicker_advanced_browser_pagestart');
  if (variable_get('imagepicker_upload_progress_enabled', 1)) {
    $content = imagepicker_upload_progress_get_script(variable_get('imagepicker_upload_progress_delay', 0));
  }
  $content .= imagepicker_quota_ok('user', $user, '', t('Upload images. You can give them a title and description'));
  return $content;
}

function imagepicker_upload_form($form_state, $account=FALSE, $admin = FALSE) {
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['file_upload'] = array(
    '#type' => 'file',
    '#title' => t('Image file'),
    '#description' => t('Browse your computer for image file'),
    '#required' => TRUE,
    '#value' => 1,
    '#size' => 40,
  );
  $form['thumb'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail size'),
    '#size' => 10,
    '#default_value' => (isset($user->imagepicker_default_thumbnail_size) ? $user->imagepicker_default_thumbnail_size : variable_get('imagepicker_default_thumbnail_size', 100)),
    '#description' => t('Size in pixels of thumbnail\'s bigger side'),
    '#required' => TRUE,
  );
  $form['scale'] = array(
    '#type' => 'textfield',
    '#title' => t('Scale image'),
    '#size' => 10,
    '#default_value' => (isset($user->imagepicker_default_scale) ? $user->imagepicker_default_scale : variable_get('imagepicker_default_scale', '')),
    '#description' => t('Scale image to this size in pixels if not left empty'),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Add a title for your image'),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#rows' => 2,
    '#cols' => 80,
    '#description' => t('Add a description for your image'),
  );
  if ( imagepicker_image_check_functions(TRUE) && variable_get('imagepicker_watermark_enable', 0)) {
    if (! variable_get('imagepicker_watermark_image', '')
      && ($user->imagepicker_watermark_image ? $user->imagepicker_watermark_image : FALSE)
      && $user->imagepicker_watermark_enable) {
      $form['watermark'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use watermark'),
        '#description' => t('Use watermark on this image.'),
        '#default_value' => ($user->imagepicker_watermark_use ? $user->imagepicker_watermark_use : FALSE),
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
        '#description' => t('Select a group to add your image to.'),
      );
    }
  }

  if ($account) {
    $form['account'] = array(
      '#type' => 'value',
      '#value' => $user->uid,
    );
  }
  if ($admin) {
    $form['admin'] = array(
      '#type' => 'value',
      '#value' => 1,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );
  if (variable_get('imagepicker_upload_progress_enabled', 1)) {
    $form['#suffix'] = imagepicker_upload_progress_busy();
  }
  return $form;
}

/**
 * Validate form
 */
function imagepicker_upload_form_validate($form, &$form_state) {
  foreach ($form_state['values'] as $name => $value) {
    switch ($name) {
      case 'file_upload':
        if (empty($_FILES['files']['name']['file_upload'])) {
          form_set_error($name, t('Image file field is required.'));
        }
        elseif (!isset($_FILES['files']['tmp_name']['file_upload']) || !file_exists($_FILES['files']['tmp_name']['file_upload'])) {
          form_set_error($name, t('Error while uploading file.'));
        }
        elseif (!image_get_info($_FILES['files']['tmp_name']['file_upload'])) {
          form_set_error($name, t('Uploaded file is not an image.'));
        }
        elseif (!imagepicker_get_uploaded_file_extension('file_upload')) {
          form_set_error($name, t('Only .jpg, .gif and .png image files are accepted.'));
        }
        break;

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
function imagepicker_upload_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Upload')) {
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

    if (file_check_directory($destdir, TRUE)
      && file_check_directory($thumbsdir, TRUE)
      && file_check_directory($browserdir, TRUE)
      && file_check_directory($origdir, TRUE)
      ) {
      // clear out the noisy 'created' messages
      drupal_get_messages('status', TRUE);
      // Add DIRECTORY_SEPARATORS here because drupals' functions remove trailing slashes
      $destdir .= DIRECTORY_SEPARATOR;
      $thumbsdir = $thumbsdir . DIRECTORY_SEPARATOR;
      $browserdir = $browserdir . DIRECTORY_SEPARATOR;
      $origdir = $origdir . DIRECTORY_SEPARATOR;
      $maxthumbsize = $form_state['values']['thumb'] ? $form_state['values']['thumb'] : 100;
      $scaleto = $form_state['values']['scale'] ? $form_state['values']['scale'] : FALSE;

      // $destination gets altered in imagepicker_copy_uploaded_file to give us the final path + name
      $destination = $origdir;

      // save the original and use that from here on
      if ( $imagemoved = imagepicker_copy_uploaded_file($destination, 'file_upload') ) {
        // $destination has been changed in imagepicker_copy_uploaded_file()
        // to point to the new file
        $file = basename($destination);
        $source = $origdir . $file;
        if ($scaleto) {
          $imagescaled = imagepicker_scale_image($source, $destdir . $file, $scaleto);
        }
        else {
          // no scaling, save direct from $origdir to $destdir
          file_copy($source, $destdir, FILE_EXISTS_RENAME);
        }
      }

      if (!$scaleto && $imagemoved || $scaleto && $imagescaled) {

        // Source file should still be an uploaded one, as scaled image
        // might have some watermarks etc. from drupal's filters/hooks.

        // if watermark is enabled just apply to destdir image, not orig or the thumbs
        if ( isset($form_state['values']['watermark']) && $form_state['values']['watermark'] ) {
          if (! imagepicker_watermark_process($destdir . $file, $user)) {
            drupal_set_message(t('Error while watermarking an uploaded image.'), 'error');
          }
        }

        // not sure why this has to be restated, but if not done the thumbs
        // get wmarked too when not scaling image above
        $source = $origdir . $file;
        if (imagepicker_scale_image($source, $thumbsdir . $file, $maxthumbsize)) {
          imagepicker_scale_image($source, $browserdir . $file, ($user->imagepicker_default_browser_thumbnail_size ? $user->imagepicker_default_browser_thumbnail_size : variable_get('imagepicker_default_browser_thumbnail_size', 100)));
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
            drupal_set_message(t('Image was successfully uploaded.'));
            if (isset($form_state['values']['admin'])) {
              $outpath = IMAGEPICKER_ADMIN_PATH .'/images/user/'. $user->uid .'/browse';
            }
            elseif (isset($form_state['values']['account'])) {
              $outpath = 'user/'. $user->uid .'/imagepicker/images/browse';
            }
            else {
              $outpath = 'imagepicker/browse/'. $nextimgid;
            }
            // external functions
            module_invoke_all('imagepicker_upload_extra', $nextimgid);
            drupal_goto($outpath);
          }
          else {
            file_delete($thumbsdir . $file);
            file_delete($browserdir . $file);
            file_delete($origdir . $file);
            file_delete($destdir . $file);
            drupal_set_message(t('Error while saving information to database for uploaded image.'), 'error');
          }
        }
        else {
          drupal_set_message(t('Error while creating a thumbnail for uploaded image.'), 'error');
        }
      }
      else {
        if (!$scaleto && !$imagemoved) {
          drupal_set_message(t('Error while moving uploaded file to its destination.'), 'error');
        }
        else {
          drupal_set_message(t('Error while scaling uploaded file.'), 'error');
        }
      }
      file_delete($destdir . $file);
    }
    else {
      drupal_set_message(t('Unable to create a directory structure for your images.'), 'error');
    }
  }
  if (isset($form_state['values']['admin'])) {
    $outpath = IMAGEPICKER_ADMIN_PATH .'/images';
  }
  elseif (isset($form_state['values']['account'])) {
    $outpath = 'user/'. $user->uid .'/imagepicker';
  }
  else {
    $outpath = 'imagepicker';
  }
  drupal_goto($outpath);
}


function imagepicker_copy_uploaded_file(&$destination, $name) {
  $source = $_FILES['files']['tmp_name'][$name];

  if (file_copy($source, $destination, FILE_EXISTS_RENAME)) {
    // Fix bug in drupal's file_copy function which uses '/' instead of
    // DIRECTORY_SEPARATOR for making directories. This causes problems on
    // Windows mashines.
    $source = str_replace('/', DIRECTORY_SEPARATOR, $source);

    $file = imagepicker_get_uploaded_file_name($destination, $name);
    $destination = $destination . $file;
    return @rename($source, $destination);
  }

  return FALSE;
}

function imagepicker_get_uploaded_file_extension($name) {
  switch ($_FILES['files']['type'][$name]) {
    // For some reason Internet Explorer 7 and later decides to give us
    // non-standard MIME types for jpg and png files, this is supposedly
    // for 'backwards compatibility'. For more details see:
    // http://msdn.microsoft.com/en-us/library/ms775147(VS.85).aspx#_replace

    // Non-standard MIME type:
    case 'image/pjpeg':
    case 'image/jpeg': $fileext = '.jpg'; break;
    case 'image/gif': $fileext = '.gif'; break;
    // Non-standard MIME type:
    case 'image/x-png':
    case 'image/png': $fileext = '.png'; break;
    default: $fileext = '';
  }

  return $fileext;
}

function imagepicker_get_uploaded_file_name($destination, $name) {
  $fileext = imagepicker_get_uploaded_file_extension($name);

  if (FALSE !== strpos($_FILES['files']['name'][$name], '.')) {
    $filename = drupal_substr($_FILES['files']['name'][$name], 0, strrpos($_FILES['files']['name'][$name], '.'));
  }
  else {
    $filename = $_FILES['files']['name'][$name];
  }

  $file = $filename . $fileext;
  $i = 0;
  while (file_exists($destination . $file)) {
    $i++;
    $file = $filename .'_'. $i . $fileext;
  }
  return $file;
}

function imagepicker_upload_progress_get_script($delay) {
  drupal_add_js(IMAGEPICKER_PATH .'/imagepicker.js');
  drupal_add_js(array('imagepicker_upload_progress' => array('delay' => $delay[0])), 'setting');
}

function imagepicker_upload_progress_busy() {
  $message = variable_get('imagepicker_upload_progress_message', t('Processing form... please wait.'));
  return theme('imagepicker_upload_progress_message', $message);
}

