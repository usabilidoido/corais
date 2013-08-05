<?php
// $Id: views_handler_filter_mc_currency.inc,v 1.1.2.2 2010/12/22 20:21:51 matslats Exp $
/**
 * Simple filter to handle matching of multiple options selectable via checkboxes
 *
 * Definition items:
 * - numeric: If set to true, this item will use numeric operators instead of string.
 *
 */
class views_handler_filter_mc_currency extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $this->value_options = mc_node_select_options(currencies_load());
  }
}
