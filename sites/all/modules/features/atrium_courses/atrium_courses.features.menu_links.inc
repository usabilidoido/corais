<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function atrium_courses_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:courses
  $menu_links['features:courses'] = array(
    'menu_name' => 'features',
    'link_path' => 'courses',
    'router_path' => 'courses',
    'link_title' => 'Courses',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Exported menu link: navigation:node/add/course
  $menu_links['navigation:node/add/course'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/course',
    'router_path' => 'node/add/course',
    'link_title' => 'Course',
    'options' => array(
      'attributes' => array(
        'title' => 'A <em>course</em> containing Drupal learning objects.',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'node/add',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Course');
  t('Courses');


  return $menu_links;
}
