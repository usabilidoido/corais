<?php
// $Id: imagepicker.imagefuncs.inc,v 1.1.2.1 2009/09/23 11:54:07 hutch Exp $
// $Name: DRUPAL-6--2-10 $
// for D6. Version 2
/**
 * @file
 * Image functions for imagepicker.
 */

function imagepicker_scale_image($source, $destination, $maxsize) {
  $info = image_get_info($source);
  if ($info) {
    $width = ($maxsize >= $info['width']) ? $info['width'] : $maxsize;
    $height = ($maxsize >= $info['height']) ? $info['height'] : $maxsize;

    $aspect = $info['height'] / $info['width'];
    if ($aspect < $height / $width) {
      $width = (int)min($width, $info['width']);
      $height = (int)round($width * $aspect);
    }
    else {
      $height = (int)min($height, $info['height']);
      $width = (int)round($height / $aspect);
    }
    return image_toolkit_invoke('resize', array($source, $destination, $width, $height));
  }
  return FALSE;
}


/**
 * Adapted from watermark module.
 *
 */
function imagepicker_watermark_process($image_path, $account=FALSE) {
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }
  $watermark_path = variable_get('imagepicker_watermark_image', FALSE);
  $use_global_watermark = ($watermark_path ? TRUE : FALSE);
  if (! $watermark_path ) {
    $watermark_path = ($account->imagepicker_watermark_image ? $account->imagepicker_watermark_image : FALSE);
  }
  if (! $watermark_path ) {
    return FALSE;
  }
  // init return status
  $status = FALSE;

  if ($use_global_watermark) {
    $scaling = variable_get('imagepicker_watermark_scaling', FALSE);
    $position = variable_get('imagepicker_watermark_position', 0);
  }
  else {
    $scaling = ($user->imagepicker_watermark_scaling ? $user->imagepicker_watermark_scaling : FALSE);
    $position = ($user->imagepicker_watermark_position ? $user->imagepicker_watermark_position : 0);
    $wdir = imagepicker_get_watermarks_dir($user);
    $watermark_path = $wdir . DIRECTORY_SEPARATOR . $watermark_path;
  }

  if (!$wm_info = _imagepicker_watermark_make_image($watermark_path)) {
    return FALSE;
  }
  $wm = $wm_info['handle'];

  if (!$im_info = _imagepicker_watermark_make_image($image_path)) {
    return FALSE;
  }
  $im = $im_info['handle'];

  if (!$im_info['truecolor']) {
    // create truecolor image for alpha blending and better resampling
    $new_im = imagecreatetruecolor($im_info['width'], $im_info['height']);
    imagecopy($new_im, $im, 0, 0, 0, 0, $im_info['width'], $im_info['height']);
    imagedestroy($im);
    $im = $new_im;
  }
  imagealphablending($im, TRUE);

  $im_x = $im_info['width'];
  $im_y = $im_info['height'];

  if ($scaling) {
    // rescale wm image to desired percentage of dest image WIDTH
    if ($use_global_watermark) {
      $percentage = variable_get('imagepicker_watermark_scaling_percentage', 50) / 100;
      $min_width = intval(variable_get('imagepicker_watermark_min_width', 0));
    }
    else {
      $percentage = $user->imagepicker_watermark_scaling_percentage / 100;
      $min_width = intval($user->imagepicker_watermark_min_width);
    }

    $desired_width = $im_x * $percentage;

    if ($desired_width > $wm_info['width'] ) {
      $desired_width = $wm_info['width'];
    }

    if ($desired_width < $min_width) {
      $desired_width = $min_width;
    }
    $scale_ratio = $desired_width / $wm_info['width'];

    // new size
    $wm_x = $wm_info['width'] * $scale_ratio;
    $wm_y = $wm_info['height'] * $scale_ratio;
  }
  else {
    $wm_x = $wm_info['width'];
    $wm_y = $wm_info['height'];
  }

  switch ($position) {
    case 0:
      //middle
      $dest_x = ($im_x / 2) - ($wm_x / 2);
      $dest_y = ($im_y / 2) - ($wm_y / 2);
      break;
    case 1:
      //middle right
      $dest_x = $im_x - $wm_x;
      $dest_y = ($im_y / 2) - ($wm_y / 2);
      break;
    case 2:
      //middle left
      $dest_x = 0;
      $dest_y = ($im_y / 2) - ($wm_y / 2);
      break;
    case 3:
      //top middle
      $dest_x = (($im_x - $wm_x) / 2);
      $dest_y = 0;
      break;
    case 4:
      //top left
      $dest_x = 0;
      $dest_y = 0;
      break;
    case 5:
      //top right
      $dest_x = $im_x - $wm_x;
      $dest_y = 0;
      break;
    case 6:
      //bottom middle
      $dest_x = (($im_x - $wm_x) / 2);
      $dest_y = $im_y - $wm_y;
      break;
    case 7:
      //bottom right
      $dest_x = $im_x - $wm_x;
      $dest_y = $im_y - $wm_y;
      break;
    case 8:
      //bottom left
      $dest_x = 0;
      $dest_y = $im_y - $wm_y;
      break;
  }

  // image create function according to original image type
  $img_create_func = 'image'. $im_info['type'];

  // scaling on - resampling image
  if ($scaling && !imagecopyresampled($im, $wm, $dest_x, $dest_y, 0, 0, $wm_x, $wm_y, $wm_info['width'], $wm_info['height'])) {
      drupal_set_message(t('Failed to merge image with watermark.'), 'error');
      watchdog('imagepicker', 'Failed to merge image with watermark.', array(), WATCHDOG_ERROR);
  }
  // scaling off
  elseif (!$scaling && !imagecopy($im, $wm, $dest_x, $dest_y, 0, 0, $wm_x, $wm_y)) {
    drupal_set_message(t('Failed to merge image with watermark.'), 'error');
    watchdog('imagepicker', 'Failed to merge image with watermark.', array(), WATCHDOG_ERROR);
  }
  // create an image of the same type!
  elseif (!$img_create_func($im, $image_path)) {
    drupal_set_message(t('Failed to save merged image.'), 'error');
    watchdog('imagepicker', 'Failed to save merged image.', array(), WATCHDOG_ERROR);
  }
  else {
    $status = TRUE;
  }

  imagedestroy($im);
  imagedestroy($wm);

  return $status;
}

/**
 * Adapted from watermark module.
 *
 */
function _imagepicker_watermark_make_image($file) {

  if (!file_exists($file)) {
    drupal_set_message(t('Image file %file not found.', array('%file' => $file ) ), 'error');
    return FALSE;
  }

  $type = exif_imagetype($file);

  switch ($type) {
    case IMAGETYPE_GIF:
      $type = 'gif';
      break;
    case IMAGETYPE_JPEG:
      $type = 'jpeg';
      break;
    case IMAGETYPE_PNG:
      $type = 'png';
      break;
    case IMAGETYPE_WBMP:
      $type = 'wbmp';
      break;
    default:
      // Unsupported type
      drupal_set_message(t('Image file %file is an unsupported format type=%type.', array( '%file' => $file, '%type' => $type ) ), 'error');
      return FALSE;
  }

  $function = 'imagecreatefrom'. $type;
  $handle = $function($file);

  if (!$handle) {
    drupal_set_message(t('Failed to create handle for image file %file via function %function.', array( '%file' => $file, '%function' => $function )), 'error');
    return FALSE;
  }

  // create image info
  $img_info = array(
    'type' => $type,
    'truecolor' => imageistruecolor($handle),
    'colors' => imagecolorstotal($handle),
    'handle' => $handle,
    'width'  => imagesx($handle),
    'height' => imagesy($handle),
  );

  return $img_info;
}
