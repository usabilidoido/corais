<?php
// $Id: views_handler_field_node_link_convert_3rdparty_incoming.inc,v 1.1.2.1 2010/12/15 22:04:04 matslats Exp $

/**
 * Field handler to present a link node edit.
 */
class views_handler_field_node_link_convert_3rdparty_incoming extends views_handler_field_node_link {
  function construct() {
    parent::construct();
  }

  function render($values) {
    if (!user_access('exchange')) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('exchange...');
    return l($text, "exchange/3rdparty/incoming/". $values->{$this->aliases['nid']});
  }
}
