<?php

/**
 * This plugin array is more or less self documenting.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Course take course button'),
  'description' => t('The take course button rendered html, if the user can take the course.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Node'),
  'icon' => '',
);

/**
 * Render callback.
 */
function course_course_take_course_button_content_type_render($subtype, $conf, $panel_args, $context) {
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();
  $block->module = 'course';

  $block->title = t('Course take course button');
  if ($node) {
    $block->content = course_render_button($node);
  }
  else {
    $block->content = t('Course take course button goes here.');
  }

  return $block;
}

/**
 * Return the translated admin string for the take course button.
 */
function course_course_take_course_button_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" Course take course button', array('@s' => $context->identifier));
}

/**
 * The edit form for the take course button.
 */
function course_course_take_course_button_content_type_edit_form(&$form, &$form_state) {
}
