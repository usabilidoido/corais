<?php

/**
 * @file
 * Provides a views field handler class for the sticky_notes priority field.
 */

/**
 * Views field handler for the field priority.
 *
 * @author berliner
 */

class views_handler_field_sticky_notes_priority extends views_handler_field {

  function render($values) {
    $priority = isset($values->{$this->table . '_priority'}) ? $values->{$this->table . '_priority'} : STICKY_NOTES_DEFAULT_PRIORITY;
    return sticky_notes_get_priority_array($priority);
  }
}
