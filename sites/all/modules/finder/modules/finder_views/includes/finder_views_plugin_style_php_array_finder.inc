<?php

/**
 * @file
 * Handler for 'finder_views_php_array_finder' style.
 */
class finder_views_plugin_style_php_array_finder extends views_plugin_style {

  function render() {
    $results = array();
    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    $options = $this->display->display_options['finder_views_options'];
    $field_info = $this->display->display_options['finder_views_field_info'];

    foreach ($sets as $title => $records) {
      foreach ($records as $label => $row) {
        $row->base_table = $this->view->base_table;
        $row->base_field = $this->view->base_field;
        $row->set = $title;
        if ($options['mode'] == 'choices' && isset($field_info[$options['finder_element_id']])) {
          /*

          This code is probably not needed ??

          foreach ($field_info[$options['finder_element_id']] as $key => $field) {
            $field_alias = finder_field_alias($options['finder_element_id'], $field['table'], $field['field']);
            $alias = &$this->view->field[$field_alias]->field_alias;
            if (isset($row->$alias)) {
              $row->$field_alias = $row->$alias;
            }
          }
          */
        }
        elseif ($options['mode'] == 'results') {
          foreach ($field_info as $feid => $info) {
            foreach ($info as $key => $field) {
              if (isset($field['field_alias'])) {
                foreach ($field['field_alias'] as $field_alias) {
                  $row->field_names[$feid][] = $field_alias;
                  $alias = &$this->view->field[$field_alias]->field_alias;
                  if ($alias) {
                    $row->$field_alias = $row->$alias;
                  }
                }
              }
            }
          }
        }
        $results[] = $row;
      }
    }

    return $results;
  }

}