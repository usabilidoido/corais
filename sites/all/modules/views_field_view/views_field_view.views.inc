<?php
// $Id: views_field_view.views.inc,v 1.1.2.1 2010/08/03 05:25:11 dereine Exp $

/**
* Implements hook_views_data_alter().
*/
function views_field_view_views_data_alter(&$data) {
  $data['views']['view'] = array(
    'title' => t('View'),
    'help' => t('Embed a view as field, this can cause slow performance, so enable some caching.'),
    'field' => array(
      'handler' => 'views_field_view_handler_field_view',
    ),
  );
}

/**
 * Implements hook_views_handlers().
 */
function views_field_view_views_handlers() {
  return array(
    'handlers' => array(
      'views_field_view_handler_field_view' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}