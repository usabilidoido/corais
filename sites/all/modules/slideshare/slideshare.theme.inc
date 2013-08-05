<?php

/**
 * @file
 *
 * Theme functions for Slideshare module
 */

/**
 * Flash embed
 */
function theme_slideshare_flash($embed, $width, $height, $field, $item, $node, $autoplay) {
  if (is_numeric($embed)) {
    $embedded_content = $item['data']['EMBED'][0];
  }
  else {
    $embedded_content = $item['embed'];
  }
  $matches = array();
  preg_match_all('/<object[^>]*>(.*?)<\/object>/', filter_xss($embedded_content, array('object', 'embed')), $matches);
  $output = '<object width="'. $width .'" height="'. $height .'" style="margin: 0;">'. $matches[1][0] .'</object>';
  $output = preg_replace('/width="(\d*)"/', 'width="'. $width .'"', $output);
  $output = preg_replace('/height="(\d*)"/', 'height="'. $height .'"', $output);
  return $output;
}
