<?php
// $Id: mc_webforms.views.inc,v 1.1.2.2 2010/12/15 22:04:04 matslats Exp $

function mc_webforms_views_handlers() {
  $handlers = array(
    'info' => array(
      'path' => drupal_get_path('module', 'mc_webforms') .'/views',
    ),
    'handlers' => array(
      'views_handler_field_node_link_convert_3rdparty_outgoing' => array(
        'parent' => 'views_handler_field_node_link'
      ),
      'views_handler_field_node_link_convert_3rdparty_incoming' => array(
        'parent' => 'views_handler_field_node_link'
      ),
    ),
  );
  return $handlers;
}


function mc_webforms_views_data() {
  $data = array();
  $data['node']['convert_node_3rdparty_outgoing'] = array(
    'field' => array(
      'title' => t('Convert node to outgoing exchange'),
      'help' => t('Populate the thirdparty exchange form'),
      'handler' => 'views_handler_field_node_link_convert_3rdparty_outgoing',
    ),
  );
  $data['node']['convert_node_3rdparty_incoming'] = array(
    'field' => array(
      'title' => t('Convert node to incoming exchange'),
      'help' => t('Populate the thirdparty exchange form'),
      'handler' => 'views_handler_field_node_link_convert_3rdparty_incoming',
    ),
  );
  return $data;
}
