<?php

/**
 * Implements hook_clone_node_alter().
 *
 * Display warning messages if there are objects which might not properly clone.
 */
function course_clone_node_alter(&$node, $original_node, $method) {
  if ($method == 'prepopulate') {
    $course = course_get_course($original_node);
    course_clone_warning_messages($course);
  }
}

/**
 * Show warning messages for a Course.
 *
 * @param Course $course
 */
function course_clone_warning_messages($course) {
  $messages = array();
  foreach ($course->getObjects() as $courseObject) {
    $warning = $courseObject->getCloneAbility();
    if (is_string($warning)) {
      $messages[] = $warning;
    }
  }
  if ($messages) {
    drupal_set_message(filter_xss_admin(theme('item_list', $messages, t('Cloning issues')), 'warning'));
  }
}

/**
 * Implements hook_node_export_node_alter().
 *
 * Allow course objects to freeze data into the export.
 */
function course_node_export_node_alter($node, $original_node, $op) {
  if ($op == 'export') {
    if (course_node_is_course($node)) {
      $course = course_get_course($node);
      if (arg(2) == 'node_export') {
        course_clone_warning_messages($course);
        foreach ($course->getObjects() as $object) {
          if ($object->getCloneAbility() !== FALSE) {
            $node->course['objects'][$object->getId()]->freeze = $object->freeze();
          }
        }
      }
    }
  }
}

/**
 * Freeze course objects onto a node if necessary.
 */
function course_handle_clone($node) {
  if (isset($node->clone_from_original_nid) && $original = node_load($node->clone_from_original_nid)) {
    $node->course['objects'] = $original->course['objects'];
    switch ($node->course['clone_type']) {
      case 'clone':
        foreach ($node->course['objects'] as &$object_data) {
          $courseObject = course_get_course_object_by_id($object_data->coid);
          $object_data->freeze = $courseObject->freeze();
        }
        break;
      case 'reference':
        break;
      case 'new':
        foreach ($node->course['objects'] as $object_data) {
          unset($object_data->instance);
        }
        break;
    }
  }
}
