<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Course credit'),
  'description' => t('A list of eligible course credit on this course and current user.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Node'),
);

function course_credit_course_credit_content_type_render($subtype, $conf, $panel_args, $context) {
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();
  $block->module = 'course_credit';

  $block->title = t('Course credit');
  if ($node) {
    $block->content = course_credit_render_credit($node);
  }
  else {
    $block->content = t('Course credit goes here.');
  }

  return $block;
}

function course_credit_course_credit_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" Course credit', array('@s' => $context->identifier));
}

function course_credit_course_credit_content_type_edit_form(&$form, &$form_state) {
}
