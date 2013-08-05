<?php
// $Id: linkimagefield_formatter.inc,v 1.4 2009/08/27 19:50:30 johnfyoung Exp $
/**
  * @file
  *
  * linkimagefield formatter code
  * @author John Young <john@codeandcreative.com>
  */
function theme_linkimagefield_formatter_linkimage($element) {
  // Inside a view $element may contain null data. In that case, just return.
  if (empty($element['#item']['fid'])) {
    return '';
  }

  $field = content_fields($element['#field_name']);
  $item = $element['#item'];

  $item['data']['alt'] = _linkimagefield_format_set_attribute('alt', $item, $field);
  $item['data']['title'] = _linkimagefield_format_set_attribute('title', $item, $field);
  $item['data']['url'] = _linkimagefield_format_set_attribute('url', $item, $field);
  $item['data']['target'] = _linkimagefield_format_set_attribute('target', $item, $field);

  $class = 'linkimagefield linkimagefield-'. $field['field_name'];
  return theme('linkimagefield_image', $item, $item['data']['alt'], $item['data']['title'], array('class' => $class), TRUE, $item['data']['url'], $item['data']['target'], $item['data']['nofollow']);
}

function theme_linkimagefield_formatter_linkimage_justlink($element) {
  $item = $element['#item'];
  $field = content_fields($element['#field_name']);
  
  // Inside a view $element may contain null data. In that case, just return.
  if (empty($item['data']['url']) && empty($field['widget']['url'])) {
    return '';
  }

  $item['data']['title'] = _linkimagefield_format_set_attribute('title', $item, $field);
  $item['data']['url'] = _linkimagefield_format_set_attribute('url', $item, $field);
  $item['data']['target'] = _linkimagefield_format_set_attribute('target', $item, $field);

  $class = 'linkimagefield linkimagefield-justlink linkimagefield-'. $field['field_name'];
  return theme('linkimagefield_justlink', $item['data']['url'], $item['data']['target'], $item['data']['title'], $class, $item['data']['nofollow']);
}
