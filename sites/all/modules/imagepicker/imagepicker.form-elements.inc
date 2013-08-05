<?php
// $Id: imagepicker.form-elements.inc,v 1.1.2.6 2010/09/08 10:56:16 hutch Exp $
// $Name: DRUPAL-6--2-10 $
// for D6. Version 2
/**
 * @file
 * Provides various form elements.
 */

function imagepicker_browser_thumbnail_size_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Default Browser Thumbnail Size'),
    '#description' => t('Configure the default browser thumbnail size'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_default_browser_thumbnail_size', 100)),
  );
}
function imagepicker_pagelink_elem($default = NULL) {
  return  array(
    '#type' => 'checkbox',
    '#title' => t('Show return link in page'),
    '#description' => t('Setting this option will add a link back to the thumbnail when using the page option. Uses javascript history(back).'),
    '#return_value' => 1,
    '#default_value' => (is_numeric($default) ? $default : variable_get('imagepicker_default_pagelink', 1)),
  );
}
function imagepicker_thumbnail_size_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Default Thumbnail Size'),
    '#description' => t('Configure the default thumbnail size'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_default_thumbnail_size', 100)),
  );
}
function imagepicker_scale_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Default Scale'),
    '#description' => t('Configure the default scale. leave empty for no default scale'),
    '#size' => 10,
    '#required' => FALSE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_default_scale', '')),
  );
}
function imagepicker_fleft_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Default Float left code'),
    '#description' => t('Configure the default code used for float left styling.'),
    '#size' => 25,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_default_fleft', 'style="float: left"')),
  );
}
function imagepicker_fright_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Default Float right code'),
    '#description' => t('Configure the default code used for float right styling.'),
    '#size' => 25,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_default_fright', 'style="float: right"')),
  );
}
// watermarks
function imagepicker_watermark_scaling_elem($default = NULL) {
  return array(
    '#type' => 'checkbox',
    '#title' => t('Scale watermark'),
    '#description' => t('Use scaling, if you want the watermark to be proportional to the image width. Please note that the watermark is scaled only when it is too big.'),
    '#default_value' => (is_numeric($default) ? $default : variable_get('imagepicker_watermark_scaling', 0)),
  );
}
function imagepicker_watermark_scaling_percentage_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Percentage'),
    '#description' => t('Percentage to apply the watermark to the image.'),
    '#size' => 3,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_watermark_scaling_percentage', 50)),
  );
}
function imagepicker_watermark_min_width_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Minimum width'),
    '#description' => t('The mimimum of the watermark in pixels. Type 0 to turn this option off.'),
    '#size' => 3,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_watermark_min_width', 0)),
  );
}

function imagepicker_thickbox_elem($default = NULL) {
  return array(
      '#type' => 'checkbox',
      '#title' => t('Thickbox Iframe'),
      '#description' => t('Use thickbox iframe instead of the builtin one. See the contribs folder in Imagepicker'),
      '#return_value' => 1,
      '#default_value' => (is_numeric($default) ? $default : variable_get('imagepicker_advanced_thickbox', 0)),
    );
}
function imagepicker_colorbox_elem($default = NULL) {
  return array(
      '#type' => 'checkbox',
      '#title' => t('Colorbox Iframe'),
      '#description' => t('Use colorbox iframe instead of the builtin one.'),
      '#return_value' => 1,
      '#default_value' => (is_numeric($default) ? $default : variable_get('imagepicker_advanced_colorbox', 0)),
    );
}
function imagepicker_iframe_width_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Iframe width'),
    '#description' => t('Configure the iframe width. This can be a number or a percentage, eg 400 or 100%'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_advanced_iframe_width', "100%")),
  );
}
function imagepicker_iframe_height_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Iframe height'),
    '#description' => t('Configure the iframe height. This must be a number, eg 500'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_advanced_iframe_height', "500")),
  );
}
function imagepicker_iframe_border_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Iframe border'),
    '#description' => t('Configure the iframe border. This can be a number, eg 0 or 1, or a css entry such as 1px solid #808080'),
    '#size' => 25,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_advanced_iframe_border', "0")),
  );
}
function imagepicker_collapsed_elem($default = NULL) {
  return array(
    '#type' => 'checkbox',
    '#title' => t('Fieldset state'),
    '#description' => t('Setting this option will collapse the fieldset the iframe is in by default.'),
    '#return_value' => 1,
    '#default_value' => (is_numeric($default) ? $default : variable_get('imagepicker_advanced_collapsed', 0)),
  );
}
function imagepicker_browser_columns_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Browser columns'),
    '#description' => t('Configure the number of columns in the image browser. This must be a number, 0 for normal wraparound'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_advanced_browser_columns', 0)),
  );
}
function imagepicker_browser_page_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Images per page'),
    '#description' => t('Configure the number of images shown per page in the image browser. This must be a number, 0 for no paging'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_advanced_browser_page', 20)),
  );
}
function imagepicker_show_browse_order_form_elem($default = NULL) {
  return array(
    '#type' => 'checkbox',
    '#title' => t('Show the order select box in the browser'),
    '#return_value' => 1,
    '#default_value' => (is_numeric($default) ? $default : variable_get('imagepicker_show_browse_order_form', 1)),
  );
}
function imagepicker_browser_order_elem($default = NULL) {
  return array(
    '#type' => 'select',
    '#title' => t('Default order'),
    '#description' => t('The default order used to sort the browser. This will be applied wether or not the select box is visible'),
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_default_browser_order', 'img_id DESC')),
    '#options' => array(
      'img_id DESC' => t('Newest first'),
      'img_id ASC' => t('Newest last'),
      'img_date DESC' => t('Edited first'),
      'img_date ASC' => t('Edited last'),
      'img_name' => t('By name'),
    ),
  );
}
function imagepicker_show_browse_search_form_elem($default = NULL) {
  return array(
    '#type' => 'checkbox',
    '#title' => t('Show the search form in the browser'),
    '#return_value' => 1,
    '#default_value' => (is_numeric($default) ? $default : variable_get('imagepicker_show_browse_search_form', 1)),
  );
}

// align options
function imagepicker_get_align_opts($default = NULL) {
  $alignoptions = array('none' => t('None'), 'fleft' => t('Float Left'), 'fright' => t('Float Right'));
  return array(
    '#type' => 'radios',
    '#title' => t('Align'),
    '#options' => $alignoptions,
    '#description' => '',
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_insert_defaults_align', 'none')),
  );
}
function imagepicker_get_show_opts($default = NULL) {
  $showoptions = array('full' => t('Full size'), 'thumb' => t('Thumbnail'), 'title' => t('Title'));
  return array(
    '#type' => 'radios',
    '#id' => 'show',
    '#title' => t('Show'),
    '#options' => $showoptions,
    '#description' => '',
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_insert_defaults_show', 'full')),
  );
}
function imagepicker_get_link_opts($default = NULL) {
  $linkoptions = array('none' => t('None'), 'file' => t('File'), 'page' => t('Page') );
  if (module_exists('lightbox2') && variable_get('imagepicker_lightbox2_enable', 0) ) {
    $linkoptions['lightbox'] = t('Lightbox');
  }
  if (module_exists('thickbox') && variable_get('imagepicker_thickbox_enable', 0) ) {
    $linkoptions['thickbox'] = t('Thickbox');
  }
  if (module_exists('colorbox') && variable_get('imagepicker_colorbox_enable', 0) ) {
    $linkoptions['colorbox'] = t('Colorbox');
  }
  return array(
    '#type' => 'radios',
    '#title' => t('Link'),
    '#options' => $linkoptions,
    '#description' => '',
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_insert_defaults_link', 'none')),
  );
}

function imagepicker_lightbox2_insert_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Default Lightbox insert'),
    '#description' => t('Configure the default code inserted into the "rel" attribute.'),
    '#size' => 15,
    '#maxlength' => 30,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_lightbox2_insert', 'lightbox')),
  );
}

function imagepicker_rows_per_page_elem($default = NULL) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Rows per page in tables'),
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => ($default != NULL ? $default : variable_get('imagepicker_rows_per_page', 25)),
  );
}
