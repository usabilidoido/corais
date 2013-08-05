<?php
/**
 * Lovely filter handler which restricts posts to the current group. Useful for group related blocks.
 **/
class ogur_handler_filter_og_users_roles_picg extends views_handler_filter {
  function query() {
    $table = $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$table.gid  = ***CURRENT_GID***");
  }
}