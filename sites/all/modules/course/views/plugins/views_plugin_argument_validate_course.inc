<?php

/**
 * Validate whether an argument is a course.
 */
class views_plugin_argument_validate_course extends views_plugin_argument_validate {

  function validate_argument($argument) {
    $node = node_load($argument);
    $this->argument->validated_title = check_plain($node->title);
    return course_node_is_course($node);
  }

}
