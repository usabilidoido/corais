<?php
// $Id: imagepicker.user.inc,v 1.1.2.15 2010/09/17 23:26:49 hutch Exp $
// $Name: DRUPAL-6--2-10 $

/**
 * @file
 * my imagepicker in my account
 */
function imagepicker_user_page($uid, $path="", $func="", $id=0) {
  drupal_add_css(IMAGEPICKER_PATH .'/imagepicker.css');
  drupal_add_js(IMAGEPICKER_PATH .'/imagepicker.js');
  $content = '';
  // path/func/id
  if ($path == 'images') {
    if ( ($func == 'browse' || $func == 'browseadmin') && is_numeric($id) && $id > 0) {
      $content .= imagepicker_user_view($id);
    }
    elseif ($func == 'edit' && is_numeric($id) && $id > 0) {
      module_load_include('inc', 'imagepicker', 'imagepicker.edit');
      $content .= imagepicker_user_image_edit($id);
    }
    elseif ($func == 'browse') {
      $content .= imagepicker_user_browse();
    }
    elseif ($func == 'browse_public') {
      if (is_numeric($id) && $id > 0) {
        $content .= imagepicker_user_view_public($id);
      }
      else {
        $content .= imagepicker_user_browse_public();
      }
    }
    elseif ($func == 'browseadmin') {
      $content .= imagepicker_user_browse_admin();
    }
    elseif ($func == 'delete' && is_numeric($id) && $id > 0) {
      imagepicker_image_delete($id, FALSE, 'account');
    }
  }
  elseif (variable_get('imagepicker_groups_enabled', 1) && $path == 'groups') {
    module_load_include('inc', 'imagepicker', 'imagepicker.group');
    if ($func == 'edit' && is_numeric($id) && $id > 0) {
      $content .= imagepicker_user_groups($func, $id);
    }
    elseif ($func == 'browse' && is_numeric($id) && $id > 0) {
      imagepicker_set_user_group_state(1, $id);
      $content .= imagepicker_user_browse();
    }
    elseif ($func == 'browse') {
      $content .= imagepicker_user_groups();
    }
    elseif ($func == 'delete') {
      $content .= drupal_get_form('imagepicker_group_delete_form', $id);
    }
  }
  elseif ($path == 'stats') {
    $content .= imagepicker_group_stats(-1);
  }
  elseif ($path == 'config') {
    $content .= imagepicker_user_config_admin();
  }
  else {
    module_load_include('inc', 'imagepicker', 'imagepicker.upload');
    $content .= imagepicker_user_upload();
  }
  return $content;
}

/**
 * main thumbnails page in my imagepicker
 */
function imagepicker_user_browse() {
  $content = _imagepicker_browse("account");
  return $content;
}

function imagepicker_user_browse_public() {
  $content = _imagepicker_browse_public("account", 1);
  return $content;
}

function imagepicker_user_browse_admin() {
  $content = _imagepicker_browse_admin("account");
  return $content;
}

function imagepicker_user_view($img_id) {
  drupal_add_js(IMAGEPICKER_PATH .'/imagepicker.js');

  $img = _imagepicker_get_img($img_id);
  if ($img) {
    $imgpath = imagepicker_get_image_path($img, 'full');
    $info = image_get_info(imagepicker_get_path(FALSE, TRUE) . $img['img_name']);
    $exifinfo = "";
    if ($info['extension'] != 'gif' ) {
      $exifinfo = imagepicker_get_exifinfo(imagepicker_get_path(FALSE, TRUE) . $img['img_name']);
    }
    $view = theme('imagepicker_view', $img, $imgpath, $info, $exifinfo);
    $form1 = drupal_get_form('imagepicker_user_image_form', $img_id);
    $form2 = '';
    $form3 = '';
    // copy form
    if (variable_get('imagepicker_copy_enabled', 0)) {
      $form2 = drupal_get_form('imagepicker_copy_form', $img['img_id'], $img['img_name']);
    }
    // group form
    if (_imagepicker_has_groups()) {
      $form3 = drupal_get_form('imagepicker_group_images_form', $img['img_id']);
    }
    $content = theme('imagepicker_user_view', $view, $form1, $form2, $form3);
  }
  else {
    drupal_set_message(t('Image not found.'), 'error');
    $content = '';
  }
  return $content;
}

function imagepicker_user_view_public($img_id) {

  $img = _imagepicker_get_img($img_id, FALSE);
  if ($img) {

    $result = db_query("SELECT name FROM {users} WHERE uid=%d AND status=1", array($img['uid']));
    $row = db_fetch_array($result);
    $img['name'] = ($row['name'] ? $row['name'] : FALSE);

    $imgpath = imagepicker_get_image_path($img, 'full', array('name' => $img['name'] , 'uid' => $img['uid'] ));
    $info = image_get_info(imagepicker_get_path(FALSE, array('name' => $img['name'] , 'uid' => $img['uid'] )) . $img['img_name']);
    $content = theme('imagepicker_view', $img, $imgpath, $info);
  }
  else {
    drupal_set_message(t('Image not found.'), 'error');
    $content = '';
  }
  return $content;
}

function imagepicker_user_image_form(&$form_state, $img_id) {
  $form['edit'] = array(
    '#type' => 'submit',
    '#value' => t('Edit image'),
    '#submit' => array('imagepicker_user_image_form_edit'),
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete image'),
    '#submit' => array('imagepicker_user_image_form_delete'),
  );
  $form['img_id'] = array(
    '#type' => 'value',
    '#value' => $img_id,
  );
  return $form;
}

function imagepicker_user_image_form_delete($form, &$form_state) {
  imagepicker_image_delete($form_state['values']['img_id'] , FALSE, 'account');
}

function imagepicker_user_image_form_edit($form, &$form_state) {
  global $user;
  drupal_goto('user/'. $user->uid .'/imagepicker/images/edit/'. $form_state['values']['img_id']);
}

/**
 * Menu callback; presents the configuration settings form for imagepicker user.
 */
function imagepicker_user_config_admin() {
  $content = drupal_get_form('imagepicker_user_config_admin_form');
  return theme('imagepicker_user_config', $content, '', t('Imagepicker configuration page.'), '', '');
}

/**
 * Submit form
 */
function imagepicker_user_config_admin_form(&$form_state, $account = FALSE) {
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }

  $form['#attributes']['enctype'] = 'multipart/form-data';

  // default options
  $form['imagepicker_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set imagepicker defaults'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -38,
  );
  $form['imagepicker_defaults']['imagepicker_default_thumbnail_size'] = imagepicker_thumbnail_size_elem((isset($user->imagepicker_default_thumbnail_size) ? $user->imagepicker_default_thumbnail_size : NULL));
  $form['imagepicker_defaults']['imagepicker_default_scale'] = imagepicker_scale_elem((isset($user->imagepicker_default_scale) ? $user->imagepicker_default_scale : NULL));
  $form['imagepicker_defaults']['imagepicker_default_pagelink'] = imagepicker_pagelink_elem((isset($user->imagepicker_default_pagelink) ? $user->imagepicker_default_pagelink : NULL));
  $form['imagepicker_defaults']['imagepicker_rows_per_page'] = imagepicker_rows_per_page_elem((isset($user->imagepicker_rows_per_page) ? $user->imagepicker_rows_per_page : NULL));

  // insert settings
  $form['imagepicker_insert_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set imagepicker insert defaults'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -36,
  );
  if (variable_get('imagepicker_default_align_show', 1)) {
    $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_align'] = imagepicker_get_align_opts((isset($user->imagepicker_insert_defaults_align) ? $user->imagepicker_insert_defaults_align : NULL));
  }
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_show'] = imagepicker_get_show_opts((isset($user->imagepicker_insert_defaults_show) ? $user->imagepicker_insert_defaults_show : NULL));
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_link'] = imagepicker_get_link_opts((isset($user->imagepicker_insert_defaults_link) ? $user->imagepicker_insert_defaults_link : NULL));

  if (variable_get('imagepicker_default_align_show', 1)) {
    // alignment settings
    $form['imagepicker_align'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image alignment settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => -34,
    );
    $form['imagepicker_align']['imagepicker_default_fleft'] = imagepicker_fleft_elem((isset($user->imagepicker_default_fleft) ? $user->imagepicker_default_fleft : NULL));
    $form['imagepicker_align']['imagepicker_default_fright'] = imagepicker_fright_elem((isset($user->imagepicker_default_fright) ? $user->imagepicker_default_fright : NULL));
  }

  // browser
  $form['imagepicker_browser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Imagepicker browser settings'),
    '#description' => t('Configure browser.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -20,
  );
  $form['imagepicker_browser']['imagepicker_default_browser_thumbnail_size'] = imagepicker_browser_thumbnail_size_elem((isset($user->imagepicker_default_browser_thumbnail_size) ? $user->imagepicker_default_browser_thumbnail_size : NULL));
  $form['imagepicker_browser']['imagepicker_advanced_browser_columns'] = imagepicker_browser_columns_elem((isset($user->imagepicker_advanced_browser_columns) ? $user->imagepicker_advanced_browser_columns : NULL));
  $form['imagepicker_browser']['imagepicker_advanced_browser_page'] = imagepicker_browser_page_elem((isset($user->imagepicker_advanced_browser_page) ? $user->imagepicker_advanced_browser_page : NULL));
  if (variable_get('imagepicker_show_browse_order_form', 1)) {
    $form['imagepicker_browser']['imagepicker_show_browse_order_form'] = imagepicker_show_browse_order_form_elem((isset($user->imagepicker_show_browse_order_form) ? $user->imagepicker_show_browse_order_form : NULL));
  }
  $form['imagepicker_browser']['imagepicker_default_browser_order'] = imagepicker_browser_order_elem((isset($user->imagepicker_default_browser_order) ? $user->imagepicker_default_browser_order : NULL));
  if (variable_get('imagepicker_show_browse_search_form', 1)) {
    $form['imagepicker_browser']['imagepicker_show_browse_search_form'] = imagepicker_show_browse_search_form_elem((isset($user->imagepicker_show_browse_search_form) ? $user->imagepicker_show_browse_search_form : NULL));
  }

  if ( imagepicker_image_check_functions(TRUE)
    && variable_get('imagepicker_watermark_enable', 0)
    && ! variable_get('imagepicker_watermark_image', '')
  ) {

    $desc = "";
    $wfiles = imagepicker_get_watermarks($user);
    if ($wfiles) {
      foreach ($wfiles AS $k => $v) {
        $wf[$v] = "$v";
      }
    }
    else {
      $desc = t('You have no uploaded watermark images');
    }
    $form['imagepicker_watermark'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image watermark settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['imagepicker_watermark']['imagepicker_watermark_enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable watermark'),
      '#description' => $desc,
      '#default_value' => (isset($user->imagepicker_watermark_enable) ? $user->imagepicker_watermark_enable : FALSE),
      '#return_value' => 1,
    );

    if ($wf) {
      $form['imagepicker_watermark']['imagepicker_watermark_image'] = array(
      '#type' => 'select',
      '#default_value' => (isset($user->imagepicker_watermark_image) ? $user->imagepicker_watermark_image : ''),
      '#options' => $wf,
      '#title' => t('Select Watermark image'),
      );
      $form['imagepicker_watermark']['imagepicker_watermark_image_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('imagepicker_user_config_admin_form_delete'),
      );
    }
    $form['imagepicker_watermark']['file_upload'] = array(
      '#type' => 'file',
      '#title' => t('Upload Watermark Image'),
      '#description' => t('Browse your computer for watermark image file'),
      '#required' => FALSE,
      '#value' => 1,
      '#size' => 40,
    );
    $form['imagepicker_watermark']['imagepicker_watermark_position'] = array(
      '#type' => 'select',
      '#default_value' => ($user->imagepicker_watermark_position ? $user->imagepicker_watermark_position : 0),
      '#options' =>  imagepicker_watermark_opts(),
      '#title' => t('Select watermark position'),
    );
    $form['imagepicker_watermark']['imagepicker_watermark_use'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use watermark by default'),
      '#description' => t('Check the "Use watermark" checkbox on the upload form by default'),
      '#default_value' => ($user->imagepicker_watermark_use ? $user->imagepicker_watermark_use : FALSE),
      '#return_value' => 1,
    );

    // watermark scaling
    $form['imagepicker_watermark']['imagepicker_user_watermark'] = array(
      '#type' => 'fieldset',
      '#title' => t('Watermark scaling'),
      '#collapsible' => TRUE,
      '#collapsed' => ($user->imagepicker_watermark_scaling ? TRUE : FALSE),
    );
    $form['imagepicker_watermark']['imagepicker_user_watermark']['imagepicker_watermark_scaling'] = imagepicker_watermark_scaling_elem((isset($user->imagepicker_watermark_scaling) ? $user->imagepicker_watermark_scaling : NULL));
    $form['imagepicker_watermark']['imagepicker_user_watermark']['imagepicker_watermark_scaling_percentage'] = imagepicker_watermark_scaling_percentage_elem((isset($user->imagepicker_watermark_scaling_percentage) ? $user->imagepicker_watermark_scaling_percentage : NULL));
    $form['imagepicker_watermark']['imagepicker_user_watermark']['imagepicker_watermark_min_width'] = imagepicker_watermark_min_width_elem((isset($user->imagepicker_watermark_min_width) ? $user->imagepicker_watermark_min_width : NULL));
  }

  if (module_exists('lightbox2') && variable_get('imagepicker_lightbox2_enable', 1)) {
    // Iframe
    $form['imagepicker_lightbox2'] = array(
      '#type' => 'fieldset',
      '#title' => t('Imagepicker Lightbox2 settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => -23,
    );
    $form['imagepicker_lightbox2']['imagepicker_lightbox2_insert'] = imagepicker_lightbox2_insert_elem((isset($user->imagepicker_lightbox2_insert) ? $user->imagepicker_lightbox2_insert : NULL));
  }

  // Iframe
  $form['imagepicker_iframe'] = array(
    '#type' => 'fieldset',
    '#title' => t('Imagepicker Iframe settings'),
    '#description' => t('You can alter the default styling of the iframe here,<br />useful if the iframe is not fitting in with your theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -22,
  );
  if (module_exists('thickbox') && file_exists(drupal_get_path('module', 'imagepicker') .'/imagepicker_thickbox.js') && variable_get('imagepicker_advanced_thickbox', 0) ) {
    $form['imagepicker_iframe']['imagepicker_advanced_thickbox'] = imagepicker_thickbox_elem((isset($user->imagepicker_advanced_thickbox) ? $user->imagepicker_advanced_thickbox : NULL));
  }
  if (module_exists('colorbox') && variable_get('colorbox_load', 0)) {
    $form['imagepicker_iframe']['imagepicker_advanced_colorbox'] = imagepicker_colorbox_elem((isset($user->imagepicker_advanced_colorbox) ? $user->imagepicker_advanced_colorbox : NULL));
  }
  $form['imagepicker_iframe']['imagepicker_advanced_iframe_width'] = imagepicker_iframe_width_elem((isset($user->imagepicker_advanced_iframe_width) ? $user->imagepicker_advanced_iframe_width : NULL));
  $form['imagepicker_iframe']['imagepicker_advanced_iframe_height'] = imagepicker_iframe_height_elem((isset($user->imagepicker_advanced_iframe_height) ? $user->imagepicker_advanced_iframe_height : NULL));
  $form['imagepicker_iframe']['imagepicker_advanced_iframe_border'] = imagepicker_iframe_border_elem((isset($user->imagepicker_advanced_iframe_border) ? $user->imagepicker_advanced_iframe_border : NULL));
  $form['imagepicker_iframe']['imagepicker_advanced_collapsed'] = imagepicker_collapsed_elem((isset($user->imagepicker_advanced_collapsed) ? $user->imagepicker_advanced_collapsed : NULL));

  if ($account) {
    $form['uid'] = array(
      '#type' => 'value',
      '#value' => $user->uid,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('imagepicker_user_config_form_reset'),
  );
  return $form;
}

function imagepicker_user_config_admin_form_validate($form, &$form_state) {
  if (isset($form_state['values']['uid']) && is_numeric($form_state['values']['uid']) ) {
    $user = user_load(array('uid' => $form_state['values']['uid']));
  }
  else {
    global $user;
  }
  if (! drupal_strlen($form_state['values']['imagepicker_default_thumbnail_size'])) {
    form_set_error('imagepicker_default_thumbnail_size', t('You must fill in the Thumbnail field'));
  }
  if (! is_numeric($form_state['values']['imagepicker_default_thumbnail_size'])) {
    form_set_error('imagepicker_default_thumbnail_size', t('The Thumbnail field must be a number'));
  }
  if (drupal_strlen($form_state['values']['imagepicker_default_scale']) && ! is_numeric($form_state['values']['imagepicker_default_scale'])) {
    form_set_error('imagepicker_default_scale', t('The Scale field must be a number'));
  }
  if (drupal_strlen($form_state['values']['imagepicker_rows_per_page']) && ! is_numeric($form_state['values']['imagepicker_rows_per_page'])) {
    form_set_error('imagepicker_rows_per_page', t('Rows per page must be a number'));
  }

  if (variable_get('imagepicker_default_align_show', 1)) {
    if (! drupal_strlen($form_state['values']['imagepicker_default_fleft'])) {
      form_set_error('imagepicker_default_fleft', t('You must fill in the Left alignment field'));
    }
    if (! drupal_strlen($form_state['values']['imagepicker_default_fright'])) {
      form_set_error('imagepicker_default_fright', t('You must fill in the Right alignment field'));
    }
  }
  if (! is_numeric($form_state['values']['imagepicker_default_browser_thumbnail_size'])) {
    form_set_error('imagepicker_default_browser_thumbnail_size', t('The Browser Thumbnail field must be a number'));
  }
  if (! is_numeric($form_state['values']['imagepicker_advanced_browser_columns']) && $form_state['values']['imagepicker_advanced_browser_columns'] < 0) {
    form_set_error('imagepicker_advanced_browser_columns', t('The browser columns must be a positive number or zero'));
  }
  if (! is_numeric($form_state['values']['imagepicker_advanced_browser_page']) && $form_state['values']['imagepicker_advanced_browser_page'] < 0) {
    form_set_error('imagepicker_advanced_browser_page', t('The browser page must be a positive number or zero'));
  }

  if (! is_numeric($form_state['values']['imagepicker_advanced_browser_page']) && $form_state['values']['imagepicker_advanced_browser_page'] < 0) {
    form_set_error('imagepicker_advanced_browser_page', t('The browser page must be a positive number or zero'));
  }
  if (! is_numeric($form_state['values']['imagepicker_advanced_iframe_height'])) {
    form_set_error('imagepicker_advanced_iframe_height', t('The Iframe height must be a number'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_advanced_iframe_width'])) {
    form_set_error('imagepicker_advanced_iframe_width', t('You must fill in the Iframe width'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_advanced_iframe_border'])) {
    form_set_error('imagepicker_advanced_iframe_border', t('You must fill in the Iframe border'));
  }
  if (module_exists('lightbox2') && variable_get('imagepicker_lightbox2_enable', 1)) {
    if (! drupal_strlen($form_state['values']['imagepicker_lightbox2_insert'])) {
      form_set_error('imagepicker_lightbox2_insert', t('You must fill in the Lightbox2 insert'));
    }
  }
  // also used by the imagepicker admin form
  if (isset($form_state['values']['imagepicker_watermark_image'])) {
    $wdir = imagepicker_get_watermarks_dir($user);
    $file = $wdir . DIRECTORY_SEPARATOR . $form_state['values']['imagepicker_watermark_image'];
    if (! file_exists($file)) {
      form_set_error('imagepicker_watermark_image', t('This file does not exist or has invalid path: %file.', array('%file' => $file)));
    }
    if ($form_state['values']['imagepicker_watermark_scaling']) {
      if (! is_numeric($form_state['values']['imagepicker_watermark_scaling_percentage']) || $form_state['values']['imagepicker_watermark_scaling_percentage'] <= 0 || $form_state['values']['imagepicker_watermark_scaling_percentage'] >= 100) {
        form_set_error('imagepicker_watermark_scaling_percentage', t('Please enter a number between 1 and 100 for scaling percentage.'));
      }
      elseif ($form_state['values']['imagepicker_watermark_scaling_percentage'] < 10 || $form_state['values']['imagepicker_watermark_min_width'] < 50) {
        // we do not disable scaling for low values, e.g. if the user chooses to scale down to 1%, we just warn
        drupal_set_message(t('Warning: Scaling is set to @percentage%, minimum width is only @min_width px. This may result in very small or almost invisible watermarks. It is recommended to check twice that an adequate minimum width has been set.',
          array('@percentage' => $form_state['values']['imagepicker_watermark_scaling_percentage'], '@min_width' => $form_state['values']['imagepicker_watermark_min_width'])));
      }
      if (intval($form_state['values']['imagepicker_watermark_min_width']) < 0) {
        form_set_error('imagepicker_watermark_min_width', t('Please enter a positive number for minimum width.'));
      }
    }
  }

  if (! empty($_FILES['files']['name']['file_upload'])) {
    module_load_include('inc', 'imagepicker', 'imagepicker.upload');
    if (!isset($_FILES['files']['tmp_name']['file_upload']) || !file_exists($_FILES['files']['tmp_name']['file_upload'])) {
      form_set_error('file_upload', t('Error while uploading file.'));
    }
    elseif (!image_get_info($_FILES['files']['tmp_name']['file_upload'])) {
      form_set_error('file_upload', t('Uploaded file is not an image.'));
    }
    elseif (!imagepicker_get_uploaded_file_extension('file_upload')) {
      form_set_error('file_upload', t('Only .jpg, .gif and .png image files are accepted.'));
    }
  }
}

function imagepicker_user_config_admin_form_submit($form, &$form_state) {
  if (isset($form_state['values']['uid'])) {
    $user = user_load(array('uid' => $form_state['values']['uid']));
  }
  else {
    global $user;
  }

  if (! empty($_FILES['files']['name']['file_upload'])) {
    module_load_include('inc', 'imagepicker', 'imagepicker.upload');
    $wdir = imagepicker_get_watermarks_dir();
    if (file_check_directory($wdir, TRUE)) {
      // clear out the noisy 'created' messages
      drupal_get_messages('status', TRUE);
      $wdir = $wdir . DIRECTORY_SEPARATOR;
      // $destination gets altered in imagepicker_copy_uploaded_file to give us the final path + name
      $destination = $wdir;
      // save the original and use that from here on
      if ( $imagemoved = imagepicker_copy_uploaded_file($destination, 'file_upload') ) {
        // $destination has been changed in imagepicker_copy_uploaded_file()
        // to point to the new file
        $file = basename($destination);
        $source = $wdir . $file;
      }
    }
  }
  // got one
  if (isset($file)) {
    $form_state['values']['imagepicker_watermark_image'] = $file;
  }
  // save all
  if (isset($form_state['values']['imagepicker_watermark_image'])) {
    user_save($user, array('imagepicker_watermark_enable' => $form_state['values']['imagepicker_watermark_enable']));
    user_save($user, array('imagepicker_watermark_image' => $form_state['values']['imagepicker_watermark_image']));
    user_save($user, array('imagepicker_watermark_position' => $form_state['values']['imagepicker_watermark_position']));
    user_save($user, array('imagepicker_watermark_use' => $form_state['values']['imagepicker_watermark_use']));
    if ($form_state['values']['imagepicker_watermark_scaling']) {
      user_save($user, array('imagepicker_watermark_scaling' => $form_state['values']['imagepicker_watermark_scaling'] ));
      user_save($user, array('imagepicker_watermark_scaling_percentage' => $form_state['values']['imagepicker_watermark_scaling_percentage']));
      user_save($user, array('imagepicker_watermark_min_width' => $form_state['values']['imagepicker_watermark_min_width']));
    }
  }
  else {
    user_save($user, array('imagepicker_watermark_image' => NULL));
  }

  user_save($user, array('imagepicker_default_thumbnail_size' => $form_state['values']['imagepicker_default_thumbnail_size']));
  user_save($user, array('imagepicker_default_scale' => $form_state['values']['imagepicker_default_scale']));
  user_save($user, array('imagepicker_rows_per_page' => $form_state['values']['imagepicker_rows_per_page']));

  user_save($user, array('imagepicker_default_pagelink' => $form_state['values']['imagepicker_default_pagelink']));

  if (variable_get('imagepicker_default_align_show', 1)) {
    user_save($user, array('imagepicker_insert_defaults_align' => $form_state['values']['imagepicker_insert_defaults_align']));
  }
  user_save($user, array('imagepicker_insert_defaults_show' => $form_state['values']['imagepicker_insert_defaults_show']));
  user_save($user, array('imagepicker_insert_defaults_link' => $form_state['values']['imagepicker_insert_defaults_link']));
  if (variable_get('imagepicker_default_align_show', 1)) {
    user_save($user, array('imagepicker_default_fleft' => $form_state['values']['imagepicker_default_fleft']));
    user_save($user, array('imagepicker_default_fright' => $form_state['values']['imagepicker_default_fright']));
  }
  user_save($user, array('imagepicker_default_browser_thumbnail_size' => $form_state['values']['imagepicker_default_browser_thumbnail_size']));
  user_save($user, array('imagepicker_advanced_browser_columns' => $form_state['values']['imagepicker_advanced_browser_columns']));
  user_save($user, array('imagepicker_advanced_browser_page' => $form_state['values']['imagepicker_advanced_browser_page']));
  if (variable_get('imagepicker_show_browse_order_form', 1)) {
    user_save($user, array('imagepicker_show_browse_order_form' => $form_state['values']['imagepicker_show_browse_order_form']));
  }
  user_save($user, array('imagepicker_default_browser_order' => $form_state['values']['imagepicker_default_browser_order']));
  if (variable_get('imagepicker_show_browse_search_form', 1)) {
    user_save($user, array('imagepicker_show_browse_search_form' => $form_state['values']['imagepicker_show_browse_search_form']));
  }
#  if (variable_get('imagepicker_upload_progress_enabled', 1)) {
#    user_save($user, array('imagepicker_upload_progress_enabled' => $form_state['values']['imagepicker_upload_progress_enabled']));
#    user_save($user, array('imagepicker_upload_progress_delay' => $form_state['values']['imagepicker_upload_progress_delay']));
#    user_save($user, array('imagepicker_upload_progress_message' => $form_state['values']['imagepicker_upload_progress_message']));
#  }
#  if ( imagepicker_image_check_functions(TRUE)) {
#    user_save($user, array('imagepicker_exifinfo_enable' => $form_state['values']['imagepicker_exifinfo_enable']));
#    user_save($user, array('imagepicker_exifinfo_external' => $form_state['values']['imagepicker_exifinfo_external']));
#  }
  if (isset($form_state['values']['imagepicker_advanced_thickbox']) && module_exists('thickbox') && file_exists(drupal_get_path('module', 'imagepicker') .'/imagepicker_thickbox.js')) {
    user_save($user, array('imagepicker_advanced_thickbox' => $form_state['values']['imagepicker_advanced_thickbox']));
  }
  if (isset($form_state['values']['imagepicker_advanced_colorbox']) && module_exists('colorbox')) {
    user_save($user, array('imagepicker_advanced_colorbox' => $form_state['values']['imagepicker_advanced_colorbox']));
  }
  user_save($user, array('imagepicker_advanced_iframe_width' => $form_state['values']['imagepicker_advanced_iframe_width']));
  user_save($user, array('imagepicker_advanced_iframe_height' => $form_state['values']['imagepicker_advanced_iframe_height']));
  user_save($user, array('imagepicker_advanced_iframe_border' => $form_state['values']['imagepicker_advanced_iframe_border']));
  user_save($user, array('imagepicker_advanced_collapsed' => $form_state['values']['imagepicker_advanced_collapsed']));
  if (module_exists('lightbox2') && variable_get('imagepicker_lightbox2_enable', 0)) {
    user_save($user, array('imagepicker_lightbox2_insert' => $form_state['values']['imagepicker_lightbox2_insert']));
  }

  drupal_set_message(t('Configuration settings saved'));
}

function imagepicker_user_config_form_reset($form, &$form_state) {
  global $user;
  user_save($user, array('imagepicker_watermark_enable' => NULL));
  user_save($user, array('imagepicker_watermark_image' => NULL ));
  user_save($user, array('imagepicker_watermark_position' => NULL));
  user_save($user, array('imagepicker_watermark_use' => NULL));
  user_save($user, array('imagepicker_watermark_scaling' => NULL));
  user_save($user, array('imagepicker_watermark_scaling_percentage' => NULL));
  user_save($user, array('imagepicker_watermark_min_width' => NULL));
  user_save($user, array('imagepicker_default_thumbnail_size' => NULL));
  user_save($user, array('imagepicker_default_scale' => NULL));
  user_save($user, array('imagepicker_rows_per_page' => NULL));
  user_save($user, array('imagepicker_insert_defaults_align' => NULL));
  user_save($user, array('imagepicker_insert_defaults_show' => NULL));
  user_save($user, array('imagepicker_insert_defaults_link' => NULL));
  user_save($user, array('imagepicker_default_fleft' => NULL));
  user_save($user, array('imagepicker_default_fright' => NULL));
  user_save($user, array('imagepicker_default_browser_thumbnail_size' => NULL));
  user_save($user, array('imagepicker_advanced_browser_columns' => NULL));
  user_save($user, array('imagepicker_advanced_browser_page' => NULL));
  user_save($user, array('imagepicker_show_browse_order_form' => NULL));
  user_save($user, array('imagepicker_default_browser_order' => NULL));
  user_save($user, array('imagepicker_show_browse_search_form' => NULL));
  user_save($user, array('imagepicker_default_pagelink' => NULL));
  user_save($user, array('imagepicker_advanced_thickbox' => NULL));
  user_save($user, array('imagepicker_advanced_colorbox' => NULL));
  user_save($user, array('imagepicker_advanced_iframe_width' => NULL));
  user_save($user, array('imagepicker_advanced_iframe_height' => NULL));
  user_save($user, array('imagepicker_advanced_iframe_border' => NULL));
  user_save($user, array('imagepicker_advanced_collapsed' => NULL));
  user_save($user, array('imagepicker_lightbox2_insert' => NULL));
  drupal_set_message(t('Configuration settings reset'));
}

function imagepicker_user_config_admin_form_delete($form, &$form_state) {
  if ( $form_state['values']['uid'] ) {
    $user = user_load(array('uid' => $form_state['values']['uid']));
  }
  else {
    global $user;
  }
  if ($form_state['values']['imagepicker_watermark_image']) {
    $file = imagepicker_get_watermarks_dir() . DIRECTORY_SEPARATOR . $form_state['values']['imagepicker_watermark_image'];
    if (file_exists($file)) {
      file_delete($file);
    }
    user_save($user, array('imagepicker_watermark_image' => NULL));
    drupal_set_message(t('Watermark File deleted'));
  }
}

