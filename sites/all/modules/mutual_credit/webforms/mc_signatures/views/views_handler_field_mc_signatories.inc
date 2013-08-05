<?php
// $Id: views_handler_field_mc_signatories.inc,v 1.1.2.1 2010/12/06 13:19:46 hutch Exp $

/**
 * Field handler to present a link node edit.
 */
class views_handler_field_mc_signatories extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  function render($values) {
    module_load_include('inc', 'mc_webforms');
    $exchange = node_load($values->nid); //additional field from above
    $links = array();
    mc_append_sign_links($exchange, $links);

    return theme('links', $links);
  }
}
