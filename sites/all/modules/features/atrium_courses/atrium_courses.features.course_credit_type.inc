<?php

/**
 * Implementation of hook_course_credit_default_types().
 */
function atrium_courses_course_credit_default_types() {
$course_credit_type = array();
  $course_credit_type['learning_credits'] = array(
  'type' => 'learning_credits',
  'title' => 'Learning Credits',
  'description' => '',
  'increments' => '10.00',
  'min' => '0.00',
  'max' => '100.00',
  'enable_variable_credit' => '0',
);
return $course_credit_type;
}
