<?php
// $Id: mc_webform_1stparty.views.inc,v 1.1.2.1 2010/12/06 13:19:46 hutch Exp $

function mc_webform_1stparty_views_handlers() {
  $handlers = array(
    'info' => array(
      'path' => drupal_get_path('module', 'mc_webform_1stparty') .'/views',
    ),
    'handlers' => array(
      'views_handler_field_node_link_convert' => array(
        'parent' => 'views_handler_field_node_link'
      ),
    ),
  );
  return $handlers;
}



function mc_webform_1stparty_views_data() {
  $data = array();
  $data['node']['convert_node'] = array(
    'field' => array(
      'title' => t('Convert to 1stparty exchange'),
      'help' => t('Use the node to populate 1stparty exchange form'),
      'handler' => 'views_handler_field_node_link_convert',
    ),
  );
  return $data;
}
