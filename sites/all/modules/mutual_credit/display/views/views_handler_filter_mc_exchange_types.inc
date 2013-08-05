<?php
// $Id: views_handler_filter_mc_exchange_types.inc,
/**
 * Filter handler for exchange types
 */
class views_handler_filter_mc_exchange_types extends views_handler_filter_many_to_one {

  function get_value_options() {
    $types = module_invoke_all('exchange_type_info');
    foreach ($types as $type => $info) {
      $this->value_options[$type] = $info['title'];
    }
  }
}
