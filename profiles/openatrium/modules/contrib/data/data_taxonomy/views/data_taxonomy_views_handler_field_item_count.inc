<?php
/**
 * @file
 * Class defition for data_node_views_handler_field_item_count.
 */

class data_taxonomy_views_handler_field_item_count extends views_handler_field {
  function query() { return; }

  function pre_render($result) {
    $this->items = array();
    foreach ($result as $row) {
      if (!empty($row->{$this->view->base_field})) {
        $tid = $row->{$this->view->base_field};
        if ($this->view->base_table == 'node') {
          $this->items[$tid] = db_result(db_query("SELECT COUNT(DISTINCT dt.id) AS count FROM {node} n LEFT JOIN {term_node} tn ON tn.nid = n.nid LEFT JOIN {data_taxonomy} dt ON dt.tid = tn.tid WHERE n.nid = %d", $tid));
        } else {
          $this->items[$tid] = db_result(db_query("SELECT COUNT(id) AS count FROM {data_taxonomy} WHERE tid = %d", $tid));
        }
      }
    }
  }

  function render($values) {
    if (!empty($values->{$this->view->base_field})) {
      $tid = $values->{$this->view->base_field};
      if (isset($this->items[$tid])) {
        return check_plain($this->items[$tid]);
      }
    }
    return 0;
  }
}
