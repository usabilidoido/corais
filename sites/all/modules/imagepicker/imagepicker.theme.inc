<?php
// $Id: imagepicker.theme.inc,v 1.1.2.26 2010/10/09 22:37:53 hutch Exp $
// $Name: DRUPAL-6--2-10 $
// for D6. Version 2
/**
 * @file
 * Themimg functions for imagepicker.
 */

/**
 * theme registry
 */
function imagepicker_theme() {
  return array(
    'imagepicker' => array(
      'arguments' => array('content' => NULL),
      'template' => 'imagepicker',
    ),

    'imagepicker_iframe' => array(
      'arguments' => array(
        'content' => NULL,
        'img'  => NULL,
        'public'  => NULL,
      ),
    ),
    'imagepicker_list' => array(
      'arguments' => array(
        'header' => array(),
        'rows' => array(),
        'max' => 50,
        'message' => "",
        'pref' => "",
        'suff' => "",
        'label' => "",
      ),
    ),
    'imagepicker_stats' => array(
      'arguments' => array(
        'header' => array(),
        'rows' => array(),
        'pref' => "",
        'suff' => "",
        'label' => "",
      ),
    ),
    'imagepicker_browser' => array(
      'arguments' => array(
        'content' => array(),
        'forms' => array(),
        'message' => "",
        'help' => "",
        'label' => "",
      ),
    ),
    'imagepicker_fullpage' => array(
      'arguments' => array(
        'image' => array(),
        'source' => "",
        'link' => 1,
      ),
    ),
    'imagepicker_quota_message' => array(
      'arguments' => array(
        'message1' => "",
        'message2' => "",
        'form' => array(),
        'label' => "",
        'help' => "",
      ),
    ),
    'imagepicker_view' => array(
      'arguments' => array(
        'img' => array(),
        'imgpath' => "",
        'info' => array(),
        'exifinfo' => array(),
      ),
    ),
    'imagepicker_image_edit_header' => array(
      'arguments' => array(
        'image' => array(),
        'source' => "",
      ),
    ),
    'imagepicker_quota' => array(
      'arguments' => array(
        'form' => array(),
        'message' => "",
        'label' => "",
      ),
    ),
    'imagepicker_upload_progress_message' => array(
      'arguments' => array(
        'message' => "",
      ),
    ),
    'imagepicker_user_config' => array(
      'arguments' => array(
        'form' => array(),
        'label' => "",
        'help' => "",
        'message1' => "",
        'message2' => "",
      ),
    ),
    'imagepicker_user_view' => array(
      'arguments' => array(
        'view' => "",
        'form1' => array(),
        'form2' => array(),
        'form3' => array(),
      ),
    ),
    'imagepicker_admin_view' => array(
      'arguments' => array(
        'view' => "",
        'form1' => array(),
        'form2' => array(),
        'form3' => array(),
      ),
    ),
    'imagepicker_insert' => array(
      'arguments' => array(
        'img' => NULL,
        'public' => NULL,
        'form1' => array(),
        'form2' => array(),
      ),
    ),
    'imagepicker_upload_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_user_image_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_user_config_admin_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_admin_image_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_user_search_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_group_search_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_quota_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_edit_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_groups_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_group_delete_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_image_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_browse_groups_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_browse_public_groups_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_group_images_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_browse_order_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_browse_public_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_browse_search_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_copy_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_settings_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_browse_admin_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_browse_admin' => array(
      'arguments' => array(
        'forms' => '',
        'message' => '',
        'pref' => '',
        'suff' => '',
        'label' => '',
      ),
    ),
    'imagepicker_multitask_delete_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_multitask_groups_form' => array(
      'arguments' => array(
        'form' => array(),
      ),
    ),
    'imagepicker_import_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
    ),
    'imagepicker_import_dir_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
    ),
  );
}

// preprocess for imagepicker.tpl.php
function template_preprocess_imagepicker(&$variables) {
  global $language;
  if (module_exists('admin_menu')) {
    admin_menu_suppress();
  }

  $variables['head_title']  = (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal'));
  $variables['styles'] = drupal_get_css();
  $variables['scripts'] = drupal_get_js();
  // fix jquery version thing in the iframe
  if (module_exists('jquery_update')) {
    jquery_update_preprocess_page($variables);
  }
  $variables['tabs'] = theme('menu_local_tasks');
  $variables['messages'] = imagepicker_strip_messages(theme('status_messages'));
  $variables['language'] = $language;

}

// set up the iframe
function theme_imagepicker_iframe($content, $img='', $public=FALSE) {

  drupal_add_css(IMAGEPICKER_PATH .'/imagepicker.css');
  if ($img) {
    global $user;
    $settings = array(
      'imagepicker_iframe' => array(
        'imgpImageAlt' => ($img['img_title'] ? addslashes(check_plain($img['img_title'])) : t('Image') ),
        'imgpImageTitle' => ($img['img_title'] ? addslashes(check_plain($img['img_title'])) : ''),
        'imgpImageDesc' => preg_replace("/(\n|\r)/", '', nl2br($img['img_description'])),
        'imgpFileLink' => imagepicker_get_image_path($img, 'full', ($public ? array('name' => $img['name'], 'uid' => $img['uid'] ) : FALSE) ),
        'imgpThumbLink' => imagepicker_get_image_path($img, 'thumb', ($public ? array('name' => $img['name'], 'uid' => $img['uid'] ) : FALSE)),
        'imgpPageLink' => url('imagepicker/image/'. $img['img_id']),
        'isFCKeditor' => (module_exists('fckeditor') ? 'yes' : 'no' ),
        'isWysiwyg' => (module_exists('wysiwyg') ? 'yes' : 'no' ),
        'use_cssbox' => ((variable_get('imagepicker_use_cssbox', 0) && ! isset($user->imagepicker_use_cssbox) ) ? 1 : (variable_get('imagepicker_use_cssbox', 0) && isset($user->imagepicker_use_cssbox) && $user->imagepicker_use_cssbox) ? 1 : 0),
        'default_align_show' => ((variable_get('imagepicker_default_align_show', 1) && ! isset($user->imagepicker_default_align_show) ) ? 1 : (variable_get('imagepicker_default_align_show', 1) && isset($user->imagepicker_default_align_show) && $user->imagepicker_default_align_show) ? 1 : 0),
        'default_fleft' => (isset($user->imagepicker_default_fleft) ? $user->imagepicker_default_fleft : variable_get('imagepicker_default_fleft', 'style="float: left"')),
        'default_fright' => (isset($user->imagepicker_default_fright) ? $user->imagepicker_default_fright : variable_get('imagepicker_default_fright', 'style="float: right"')),
        'lightbox2_enable' => (module_exists('lightbox2') && variable_get('imagepicker_lightbox2_enable', 0)),
        'lightbox2_insert' => (isset($user->imagepicker_lightbox2_insert) ? $user->imagepicker_lightbox2_insert : variable_get('imagepicker_lightbox2_insert', 'lightbox')),
        'thickbox_enable' => (module_exists('thickbox') && variable_get('imagepicker_thickbox_enable', 0)),
        'colorbox_enable' => (module_exists('colorbox') && variable_get('imagepicker_colorbox_enable', 0)),
        'colorbox_iframe' => (imagepicker_get_colorbox_perms()),
      ),
    );
    drupal_add_js($settings, 'setting');
    drupal_add_js(IMAGEPICKER_PATH .'/imagepicker_iframe.js');
  }
  return $content;
}

function theme_imagepicker_list($header, $rows, $max, $message="", $pref="", $suff="", $label="", $cols = 1) {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  if ($rows) {
    $pager = theme('pager', NULL, $max);
    if ($pager) {
      $rows[] = array(array('data' => $pager, 'colspan' => $cols));
    }
    $output .= $pref . theme('table', $header, $rows) . $suff;
  }
  else {
    $output .= '<div class="messages">'. $message .'</div>';
  }
  $output .= ($label ? '</fieldset>' : '');
  return $output;

}

function theme_imagepicker_stats($header, $rows, $pref="", $suff="", $label="") {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  $output .= $pref . theme('table', $header, $rows) . $suff;
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_browser($content, $forms='', $message='', $help='', $label='') {
  $rows = $content[0];
  $max = $content[1];
  $cols = $content[2];
  $pref1 = $content[3][0];
  $pref2 = $content[3][1];
  $suff1 = $content[4][0];
  $suff2 = $content[4][1];
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  // forms
  if ($forms) {
    $output .= $forms['browse_search'];
    $output .= $forms['browse_groups'];
    $output .= $forms['browse_public'];
    $output .= $forms['browse_public_groups'];
    $output .= $forms['browse_order'];
  }
  if ($rows) {
    // help message
    if ($help) {
      $output .= '<div class="imgp_help">'. $help . '</div>';
    }
    $pager = theme('pager', NULL, $max);
    $output .= $pref1;
    $ct = 0;
    foreach ($rows AS $row) {
      $tooltip =  $row['img_name'] .': '. $row['img_title'] .' '. $row['img_description'];
      $imglink = '<img src="'. $row['imgpath']  .'" alt="'. $row['img_title'] .'" title="'. $tooltip .'" />';
      $output .= $pref2 . l($imglink, $row['imgurl'] . $row['img_id'], array('html' => TRUE)) . $suff2;
      $ct++;
      if ($cols > 0 && $ct >= $cols) {
        $output .= $suff1 . $pref1;
        $ct = 0;
      }
    }
    $output .= $suff1 . $pager;
  }
  else {
    $output .= '<div class="messages">'. $message .'</div>';
  }
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_fullpage($image, $source, $link) {
  $output = '<div id="imgp_page"><div id="imgp_page_img">';
  $output .= '<a href="'. $source .'" alt="'. $image['img_title'] .'" target="_blank">';
  $output .= '<img src="'. $source .'" alt="'. $image['img_title'] .'" /></a></div>';
  $output .= ($image['img_description'] ? '<div>'. nl2br($image['img_description']) .'</div>' : '');
  $output .= ($link ? '<div><a href="#" onclick="history.back()">'. t('Return to page') .'</a></div>' : '');
  $output .= '</div>';
  return $output;
}

function theme_imagepicker_quota_message($message1, $message2, $form, $label, $help) {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  $output .= ($message1 ? '<p class="messages">'. $message1 .'</p>' : '') ;
  $output .= ($help ? '<div class="imgp_help">' . $help . '</div>' : '');
  $output .= $form;
  $output .= ($message2 ? '<p class="messages">'. $message2 .'</p>' : '') ;
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_view($img, $imgpath, $info, $exifinfo='') {
  $output = '<div id="imgp_img_view">';
  $output .= ($img['img_title'] ? '<div id="imgp_img_view_title">'. $img['img_title'] .'</div>' : '');
  $output .= '<img id="imgp_img_view_img" src="'. $imgpath .'" alt="'. $img['img_title'] .'" title="'. $img['img_name'] .'" />';
  $output .= ($img['img_description'] ? '<div id="imgp_img_view_desc">'. nl2br($img['img_description']) .'</div>' : '');
  $output .= '<div>';
  $output .= t('Width')  .": ". $info['width']     ."&nbsp;&nbsp;&nbsp;";
  $output .= t('Height') .": ". $info['height']    ."&nbsp;&nbsp;&nbsp;";
  $output .= t('Type')   .": ". $info['extension'] ."&nbsp;&nbsp;&nbsp;";
  $output .= t('Size')   .": ". $info['file_size'] . '</div>';

  if ($exifinfo) {
    $output .= '<a id="imgp_trig">'. t('Show/Hide Exif Info') .'</a>';
    $output .= '<div id="imgp_targ" >';
    if (is_array($exifinfo)) {
      // using php function exif_read_data().
      foreach ($exifinfo AS $key => $section) {
        $header = array(
          array('data' => drupal_ucfirst(drupal_strtolower($key)), 'colspan' => 2),
        );
        $rows = array();
        foreach ($section as $name => $val) {
          if ($key == 'COMMENT') {
            $a = explode(':', $val);
            $rows[] = array($a[0] . ':', $a[1]);
          }
          else {
            $rows[] = array($name . ': ', $val);
          }
        }
        $output .= theme('table', $header, $rows);
        unset($header);
        unset($rows);
      }
      $output .= '</div>';
    }
    else {
      // from an external source eg exiftool
      $output .= '<pre>';
      $output .= check_plain($exifinfo);
      $output .= '</pre>';
      $output .= '</div>';
    }
  }
  $output .= '</div>';
  return $output;
}

function theme_imagepicker_image_edit_header($image, $source) {
  $output = '<div class="imgp_help">'. t('Edit image details') . '</div>';
  $output .= '<div id="imgp_img_holder"><img src="'. $source .'" alt="'. check_plain($image['img_title']) .'" /></div>';
  return $output;
}

function theme_imagepicker_quota($form, $message, $label) {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  $output .= ($message ? '<p class="messages">'. $message .'</p>' : '') ;
  $output .= $form;
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_upload_progress_message($message) {
  $output = '<div id="sending" class="progress" style="display: none;">';
  $output .= '<div class="status">'. $message .'</div>';
  $output .= '<div class="bar"></div></div>';
  return $output;
}

function theme_imagepicker_user_config($form, $label='', $help='', $message1='', $message2='') {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  $output .= ($help ? '<div class="imgp_help">' . $help . '</div>' : '');
  $output .= ($message1 ? '<p class="messages">'. $message1 .'</p>' : '') ;
  $output .= $form;
  $output .= ($message2 ? '<p class="messages">'. $message2 .'</p>' : '') ;
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_user_view($view, $form1='', $form2='', $form3='') {
  $output = "";
  $output .= $view;
  $output .= '<br />';
  $output .= ($form1 ? $form1 : '') ;
  $output .= '<br />';
  $output .= ($form2 ? $form2 : '') ;
  $output .= ($form3 ? $form3 : '') ;
  return $output;
}

function theme_imagepicker_admin_view($view, $form1='', $form2='', $form3='') {
  $output = "";
  $output .= $view;
  $output .= '<br />';
  $output .= ($form1 ? $form1 : '') ;
  $output .= '<br />';
  $output .= ($form2 ? $form2 : '') ;
  $output .= ($form3 ? $form3 : '') ;
  return $output;
}

function theme_imagepicker_insert($img='', $public=FALSE, $form1='', $form2='') {
  $output = "";
  if ($img) {
    $imgpath = imagepicker_get_image_path($img, 'browser', ($public ? array('name' => $img['name'] , 'uid' => $img['uid']) : FALSE ));
    $output .= '<div class="imgp_help">'. t('Choose the settings you want, place the cursor in the Body box above and Insert image.') .'</div>';
    if ($img['img_title']) {
      $output .= '<div id="imgp_img_holder_title">'. check_plain($img['img_title']) .'</div>';
    }
    if ($img['img_description']) {
      $output .= '<div id="imgp_img_holder_description">'. nl2br($img['img_description']) .'</div>';
    }
    $output .= '<div id="imgp_img_holder">';
    $output .= '<img id="imgp_img" src="'. $imgpath .'" alt="'. check_plain($img['img_title']) .'" title="'. $img['img_name'] .'" />';
    $output .= '</div>';
    $output .= $form1;
    $output .= $form2;
  }
  return $output;
}

function theme_imagepicker_upload_form($form) {
  $output = '';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_user_image_form($form) {
  $output = '';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_user_config_admin_form($form) {
  $output = '';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_align']['#prefix'] = '<div class="imgp_img_options">';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_align']['#suffix'] = '</div>';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_show']['#prefix'] = '<div class="imgp_img_options">';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_show']['#suffix'] = '</div>';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_link']['#prefix'] = '<div class="imgp_img_options">';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_link']['#suffix'] = '</div>';
  $form['imagepicker_watermark']['imagepicker_watermark_image']['#prefix'] = '<div class="container-inline">';
  $form['imagepicker_watermark']['imagepicker_watermark_image_delete']['#suffix'] = '</div>';
  $form['submit']['#prefix'] = '<div class="container-inline">';
  $form['reset']['#suffix'] = '</div>';

  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_admin_image_form($form) {
  $output = '';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_user_search_form($form) {
  $output = '';
  $form['imagepicker_currentuser']['#prefix'] = '<div id="imgp_users_form" class="container-inline">';
  $form['imagepicker_user_search_submit']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_group_search_form($form) {
  $output = '';
  $form['imagepicker_currentgroup']['#prefix'] = '<div id="imgp_groups_form" class="container-inline">';
  $form['imagepicker_group_search_submit']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_quota_form($form) {
  $output = '';
  $form['imagepicker_quota']['#prefix'] = '<div id="imgp_quota_form" class="container-inline">';
  $form['submit']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_edit_form($form) {
  $output = '';
  $form['title']['#prefix'] = '<div id="imgp_edit_form">';
  $form['description']['#suffix'] = '</div>';
  $form['submit']['#prefix'] = '<div id="imgp_controls">';
  $form['cancel']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_groups_form($form) {
  $output = '';
  $form['groupsave']['group_public_roles']['#prefix'] = '<div id="wrap-group-public-roles">';
  $form['groupsave']['group_public_roles']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_group_delete_form($form) {
  $output = '';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_image_form($form) {
  $output = '';
  $form['align']['#prefix'] = '<div class="imgp_img_options">';
  $form['align']['#suffix'] = '</div>';
  $form['show']['#prefix'] = '<div class="imgp_img_options">';
  $form['show']['#suffix'] = '</div>';
  $form['link']['#prefix'] = '<div class="imgp_img_options">';
  $form['link']['#suffix'] = '</div>';
  $form['desc']['#prefix'] = '<div id="imgp_desc_control">';
  $form['desc']['#suffix'] = '</div>';
  $form['insert']['#prefix'] = '<div id="imgp_controls">';
  $form['delete']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_browse_groups_form($form) {
  $output = '';
  $form['gid']['#prefix'] = '<div id="imgp_groups_form" class="container-inline">';
  $form['submit']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_browse_public_groups_form($form) {
  $output = '';
  $form['gid']['#prefix'] = '<div id="imgp_groups_form" class="container-inline">';
  $form['submit']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_group_images_form($form) {
  $output = '';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_browse_order_form($form) {
  $output = '';
  $form['imagepicker_browser_order']['#prefix'] = '<div id="imgp_order_form" class="container-inline">';
  $form['submit']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_browse_public_form($form) {
  $output = '';
  $form['imagepicker_browse_public']['#prefix'] = '<div id="imgp_browse_form" class="container-inline">';
  $form['submit']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_browse_search_form($form) {
  $output = '';
  $form['imagepicker_browser_search']['#prefix'] = '<div id="imgp_search_form" class="container-inline">';
  $form['reset']['#suffix'] = '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_copy_form($form) {
  $output = '';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_settings_form($form) {
  $output = '';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_align']['#prefix'] = '<div class="imgp_img_options">';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_align']['#suffix'] = '</div>';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_show']['#prefix'] = '<div class="imgp_img_options">';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_show']['#suffix'] = '</div>';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_link']['#prefix'] = '<div class="imgp_img_options">';
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_link']['#suffix'] = '</div>';
  $form['imagepicker_import']['imagepicker_import_delete']['#prefix'] = '<div id="wrap-imagepicker-import">';
  $form['imagepicker_import']['imagepicker_import_delete']['#suffix'] = '</div>';
  $form['imagepicker_groups']['imagepicker_public_enabled']['#prefix'] = '<div id="wrap-imagepicker-groups">';
  $form['imagepicker_groups']['imagepicker_groups_in_upload_enabled']['#suffix'] = '</div>';
  $form['imagepicker_progress']['imagepicker_upload_progress_delay']['#prefix'] = '<div id="wrap-imagepicker-upload-progress">';
  $form['imagepicker_progress']['imagepicker_upload_progress_message']['#suffix'] = '</div>';
  $form['imagepicker_exif']['imagepicker_exifinfo_external']['#prefix'] = '<div id="wrap-imagepicker-exifinfo-external">';
  $form['imagepicker_exif']['imagepicker_exifinfo_external']['#suffix'] = '</div>';
  $form['imagepicker_quotas']['imagepicker_quota_role']['#prefix'] = '<div id="wrap_imagepicker_quota_role">';
  $form['imagepicker_quotas']['imagepicker_quota_role']['#suffix'] = '</div>';

  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_browse_admin_form($form) {
  $src = $form['options']['src']['#value'];
  $cols = $form['options']['cols']['#value'];
  $public = $form['options']['public']['#value'];
  $form['options']['#prefix'] = '<div class="container-inline">';
  $form['options']['#suffix'] = '</div>';
  // tablesort
  if ($public && $src == 'admin') {
    $header = array(
      theme('table_select_header_cell'),
      array('data' => t('Name'), 'field' => 'i.img_name'),
      array('data' => t('Title'), 'field' => 'i.img_title'),
      t('Description'),
      array('data' => t('User'), 'field' => 'n.name'),
      array('data' => t('Date'), 'field' => 'i.img_date'),
      array('data' => t('Actions'), 'colspan' => 2),
    );
  }
  else {
    $header = array(
      theme('table_select_header_cell'),
      array('data' => t('Name'), 'field' => 'i.img_name'),
      array('data' => t('Title'), 'field' => 'i.img_title'),
      t('Description'),
      array('data' => t('Date'), 'field' => 'i.img_date'),
      array('data' => t('Actions'), 'colspan' => 2),
    );
  }

  $output = drupal_render($form['options']);

  if (isset($form['img_name']) && is_array($form['img_name'])) {
    foreach (element_children($form['img_name']) as $key) {
      if ($public && $src == 'admin') {
        $rows[] = array(
          drupal_render($form['images'][$key]),
          drupal_render($form['img_name'][$key]),
          drupal_render($form['img_title'][$key]),
          drupal_render($form['img_desc'][$key]),
          drupal_render($form['img_list'][$key]),
          drupal_render($form['img_date'][$key]),
          drupal_render($form['img_edit'][$key]),
          drupal_render($form['img_delete'][$key]),
        );
      }
      else {
        $rows[] = array(
          drupal_render($form['images'][$key]),
          drupal_render($form['img_name'][$key]),
          drupal_render($form['img_title'][$key]),
          drupal_render($form['img_desc'][$key]),
          drupal_render($form['img_date'][$key]),
          drupal_render($form['img_edit'][$key]),
          drupal_render($form['img_delete'][$key]),
        );
      }
    }
  }
  else {
    $rows[] = array(array('data' => t('No images available.'), 'colspan' => $cols));
  }
  if ($form['pager']['#value']) {
    $pager = drupal_render($form['pager']);
    $rows[] = array(array('data' => $pager, 'colspan' => $cols+1));
  }
  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_browse_admin($forms='', $message="", $pref="", $suff="", $label="") {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  if ($forms) {
    $output .= $pref;
    $output .= $forms['browse_search'];
    $output .= $forms['browse_groups'];
    $output .= $forms['browse_public'];
    $output .= $forms['browse_public_groups'];
    $output .= $forms['browse_admin'];
    $output .= $suff;
  }
  else {
    $output .= '<div class="messages">'. $message .'</div>';
  }
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_multitask_delete_form($form) {
  $output = '';
  $count = $form['countnids']['#value'];
  $output .= '<p>'. t('You have selected %c to be deleted', array('%c' => format_plural( $count, '1 image', '@count images'))) . '</p>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_multitask_groups_form($form) {
  $output = '';
  $count = $form['countnids']['#value'];
  $output .= '<p>'. t('You have selected %c to be grouped', array('%c' => format_plural( $count, '1 image', '@count images'))) . '</p>';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_import_form($form) {
  $output = '';
  $output .= drupal_render($form);
  return $output;
}

function theme_imagepicker_import_dir_form($form) {
  $output = '';
  $output .= drupal_render($form);
  return $output;
}
