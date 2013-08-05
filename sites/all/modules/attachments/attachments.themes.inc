<?php
// $Id: attachments.themes.inc,v 1.4 2010/06/26 14:02:15 adaddinsane Exp $
/**
 * @file
 * Modify the user interface and the display of file attachments - themes
 */

/**
 * Theme the whole attachments description for the given (or current) user
 */
function theme_attachments_allowed($maxfiles = 0, $account = NULL) {
  $output = '';

  if ($account===NULL) {
    global $user;
    $account = drupal_clone($user);
  }

  $limits = _upload_file_limits($account);

  $output .= $limits['resolution'] ? t('Images are larger than %res will be resized. ', array('%res' => $limits['resolution'])) : '';
  $output .= t('The maximum upload size is %filesize. ', array('%filesize' => format_size($limits['file_size'])));
  $output .= $maxfiles ? t('You can upload a maximum of !x files for this type of node. ', array('!x' => $maxfiles)) : '';
  $output .= theme('attachments_extensions', $limits['extensions']);

  return '<div style="clear:both;">' . $output . '</div>';
}

/**
 * Theme the extensions the user is allowed to upload using the "grouped" style
 */
function theme_attachments_extensions($extensions, $sep=' ') {
  $struct = variable_get(Attachments::VAR_EXTENSIONS, Attachments::exts());
  $extensions = explode($sep, $extensions);

  $out = array();
  foreach ($struct as $key => $options) {
    $out[$key] = '';
    foreach ($options as $opt) {
      $i = array_search($opt, $extensions);
      if ($i!==FALSE) {
        $out[$key][] = $opt;
        unset($extensions[$i]);
      }
    }
  }
  if (!empty($extensions)) {
    $out[check_plain(variable_get(Attachments::VAR_OTHER_EXTS, Attachments::OTHER_EXTS))] = $extensions;
  }

  $output = '';
  foreach ($out as $group => $exts) {
    $output .= t('<li>@group: %exts</li>', array('@group' => check_plain(drupal_ucfirst($group)), '%exts' => check_plain(implode(', ', $exts))));
  }
  return $output ? t('Only files with the following extensions may be uploaded:') . "<ul>{$output}</ul>" : '';
}

/**
 * Theme the extension groups and their contents for the settings page
 */
function theme_attachments_settings_table($element = array()) {
  $header = array(t('Extension group name'), t('Extension list'));
  $rows = array();
  foreach (element_children($element) as $key) {
    $rows[] = array(
      drupal_render($element[$key]['group']),
      drupal_render($element[$key]['exts']),
    );

  }
  return theme('table', $header, $rows);
}

/**
 * Theme the attachments display style
 *
 * This code already includes the possibility that the file
 * attachment display style is defined in the node itself.
 */
function theme_attachments_style($node) {
  $output = $node->content['files']['#value'];
  $style = isset($node->attachments['style']) ? $node->attachments['style'] : variable_get(Attachments::VAR_STYLE, Attachments::STYLE_DEFAULT);
  if ($style!=Attachments::STYLE_DEFAULT) {
    $title = variable_get(Attachments::VAR_TITLE, Attachments::VAR_TITLE_DEFAULT);
    if ($title=='<none>') $title = '';
    $output = theme($style, $node, $title);
  }
  return $output;
}

function theme_attachments_list($node, $title = 'Attachments') {
  $items = array();
  foreach ($node->files as $file) {
    if ($file->list) {
      list(, $ext) = explode('/', drupal_strtoupper($file->filemime));
      $name = check_plain($file->description) . sprintf(" (%s, %s)", $ext, format_size($file->filesize));
      $items[] = l($name, $file->filepath);
    }
  }
  if (!empty($items)) {
    $output = theme('item_list', $items, t($title));
  }
  return $output;
}



