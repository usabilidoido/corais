<?php
//=================
// THEME FUNCTIONS
//=================

/**
 * Themes the taskbar.
 */
function theme_taskbar_taskbar($taskbar) {
  $output = '<div id="taskbar-container"><div id="taskbar" class="taskbar"><div class="taskbar-inner">';

  $regions = element_children($taskbar);
  foreach ($regions as $region) {
    $output .= theme($taskbar[$region]['#type'], $taskbar[$region]);
  }
  $output .= '</div></div></div>';

  return $output;
}

/**
 * Themes the taskbar region.
 */
function theme_taskbar_region($region) {
  $output = '<div id="taskbar-region-' . $region['#name'] . '" class="taskbar-region"><div class="taskbar-region-inner">';

  $items = element_children($region);
  foreach ($items as $item) {;
    $output .= theme($region[$item]['#type'], $region[$item]);
  }
  $output .= '</div></div>';

  return $output;
}

/**
 * Themes the taskbar item.
 *
 * Each taskbar item can has 3 elements:
 * - content: the content to be put in the taskbar
 * - count: the number to be highlighted beside the content
 * - extra: the extra information  to be displayed on click (if not Ajax)
 */
function theme_taskbar_item($item, $content_only = FALSE) {
  $content = '<div class="taskbar-item-content" title="' . check_plain($item['#item']->settings['description']) . '">' . $item['#value']['content'];
  if (isset($item['#value']['count'])) {
    $content .= '<span class="taskbar-item-count"><span class="taskbar-item-count-inner">' . $item['#value']['count'] . '</span></span>';
  }
  $content .= '</div>';
  if ($content_only) {
    return $content;
  }

  $class = array('taskbar-item', 'taskbar-provider-' . $item['#provider']);
  foreach ($item['#options'] as $key => $option) {
    if ($option) {
      $class[] = 'taskbar-item-options-' . $key;
    }
  }

  $output = '<div id="taskbar-item-' . $item['#name'] . '" class="' . implode(' ', $class) .'">';
  $output .= $content;
  if (isset($item['#value']['extra'])) {
    $output .= '<div class="taskbar-item-extra">' . _taskbar_item_titlebar($item['#item']) . $item['#value']['extra'] . '</div>';
  }
  $output .= '</div>';
  return $output;
}

function _taskbar_item_titlebar($item) {
  return variable_get('taskbar_titlebar', FALSE) ? '<div class="taskbar-item-titlebar">' . $item->settings['text'] . '</div>' : '';
}
