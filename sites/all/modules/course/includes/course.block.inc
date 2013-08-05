<?php

/**
 * @file course blocks
 * @todo use context
 */

/**
 * Course block callback: outline view.
 */
function _course_block_outline_view() {
  if ($node = course_get_context()) {
    if ($output = course_outline_list($node)) {
      return array('subject' => t('Course outline'), 'content' => $output);
    }
  }
}

// @todo move this to a new config class interface. Keep for notes for now.
///**
// * Course block callback: outline configure.
// */
//function _course_block_outline_configure() {
//  // Course outline display handler.
//  $outlines = array();
//  $handlers = course_get_handlers('outline');
//  foreach ($handlers as $module => $outline_handlers) {
//    foreach ($outline_handlers as $key => $outline_handler)
//      $outlines[$key] = $outline_handler['name'];
//  }
//
//  $form['outline'] = array(
//    '#title' => t('Available outline displays'),
//    '#type' => 'radios',
//    '#options' => $outlines,
//    '#default_value' => variable_get('course_block_outline', 0),
//  );
//}
//
///**
// * Course block callback: outline save.
// */
//function _course_block_outline_save($edit) {
//  variable_set('course_block_outline', $edit['outline']);
//}

/**
 * Course block callback: navigation view.
 */
function _course_block_navigation_view() {
  global $user;
  $node = course_get_context();
  if ($node) {
    $course = course_get_course($node, $user);
    $links = $course->getNavigation();

    // Strip array keys so theme_item_list() works properly.
    $items = array_values($links);

    // Add javascript poller to update the next step button.
    drupal_add_js(drupal_get_path('module', 'course') . '/js/nav.js', 'module', 'header', FALSE, FALSE, FALSE);

    return array(
      'subject' => '',
      'content' => theme('item_list', $items, NULL, 'ul', array('id' => 'course-nav')),
    );
  }
}
