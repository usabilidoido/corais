<?php

/**
 * @file
 * An extended subclass for field handling that adds multiple field grouping.
 */
class views_rss_handler_field_multiple extends content_handler_field_multiple {

  function render_link($data, $values) {
    // If $data is an array and key #markup exists, this means we have received
    // extended data from one of Views RSS formatters (D7 way). However, D6
    // doesn't know how to handle this, so let's just add extended data to
    // view result, and pass normal markup only to parent handler.
    if (is_array($data) && !empty($data['#markup'])) {
      // Add extended data to view result.
      if (isset($this->view->row_index)) {
        $row_index = $this->view->row_index;
        $field_name = 'field_' . $this->real_field;
        $delta = $data['#item']['#delta'];
        $this->view->result[$row_index]->{$field_name}[$delta] = array(
          'rendered' => $data,
          'raw' => $data['#item'],
        );
      }
      // Only #markup value should be passes to core handler.
      $data = $data['#markup'];
    }
    // Standard CCK render_link().
    return parent::render_link($data, $values);
  }

}
