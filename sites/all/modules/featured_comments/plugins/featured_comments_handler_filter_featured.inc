<?php


class featured_comments_handler_filter_featured extends views_handler_filter {

  function query() {
    if ($this->relationship) {
      $base_table = $this->view->query->relationships[$this->relationship]['table'];
    }
    else {
      $base_table = $this->view->base_table;
    }
    
    switch ($base_table) {
      case 'comments':
        $table = $this->view->query->ensure_table('featured_comments', $this->relationship);
        $this->view->query->add_where(0, "$table.status = 1");
        break;
    }
  }

  function admin_summary() {
    return 'Featured comments';
  }
}
