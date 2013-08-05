<?php
// $Id: views_handler_field_node_link_convert.inc,v 1.1.2.1 2010/12/06 13:19:46 hutch Exp $

/**
 * Field handler to present a link node edit.
 */
class views_handler_field_node_link_convert extends views_handler_field_node_link {
  function construct() {
    parent::construct();
  }

  function render($values) {
    if (!user_access('exchange')) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('exchange...');
    return l($text, "exchange/1stparty/convert/". $values->{$this->aliases['nid']});
  }
}
