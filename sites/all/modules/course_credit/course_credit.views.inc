<?php

module_load_include('inc', 'views', 'handlers/views_handler_relationship');
include_once 'includes/views_join_course_credit.inc';

/**
 * Implements hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function course_credit_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'course_credit') . '/includes',
    ),
    'handlers' => array(
      'views_handler_field_course_credit_title' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_filter_course_credit_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'views_handler_field_course_credit_node_active_types' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'course_credit_credit_awarded_credit_instance_vhr' => array(
        'parent' => 'views_handler_relationship',
      ),
      'course_credit_course_report_credit_awarded_vhr' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}

/**
 * Relate node to awarded credit.
 *
 * Join node.nid to awarded.nid, USER awarded.uid
 *
 * This is for course relationships mostly.
 */
class course_credit_node_awarded_credit_vhr extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    global $user;
    //detect the user from the path to the report (instead of deafualt global $user)
    if ($this->view->argument['uid']) {
      $account = user_load($this->view->args[0]);
    }
    else {
      $account = $user;
    }
    // Figure out what base table this relationship brings to the party.
    $join = new views_join();
    $join->definition = array(
      'table' => 'course_credit_awarded',
      'field' => 'nid',
      'left_table' => 'node',
      'left_field' => 'nid',
      'extra' => array(
        array(
          'field' => 'uid',
          'numeric' => TRUE,
          'value' => $account->uid,
        ),
      ),
    );

    $join->construct();
    $this->ensure_my_table();

    $alias = $join->definition['table'] . '_' . $join->definition['left_table'];
    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }

}

/**
 * Implements hook_views_data().
 */
function course_credit_views_data() {
  $data = array();

  // Expose course_credit schema to views.
  module_load_install('course_credit');
  $schema = course_credit_schema();

  foreach ($schema as $table => $schemata) {
    $data[$table]['table']['group'] = t($table);
    foreach ($schemata['fields'] as $key => $field) {
      switch ($field['type']) {
        case 'int':
        case 'numeric':
        case 'float':
          $type = "_numeric";
          break;
        default:
          $type = "";
      }
      $data[$table][$key] = array(
        'title' => ucwords($key),
        'help' => $field['description'],
        'field' => array(
          'handler' => "views_handler_field$type",
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => "views_handler_sort$type",
        ),
        'filter' => array(
          'handler' => "views_handler_filter$type",
        ),
        'argument' => array(
          'handler' => "views_handler_argument",
        ),
      );
      if (isset($field['scale'])) {
        $data[$table][$key]['field']['float'] = TRUE;
      }
    }
  }

  $data['course_credit_awarded']['table']['group'] = t('Course credit awarded');
  $data['course_credit']['table']['group'] = t('Course credit instance');
  $data['course_credit_type']['table']['group'] = t('Course credit type');

  $data['course_report']['credit_awarded']['relationship'] = array(
    'handler' => 'course_credit_course_report_credit_awarded_vhr',
    'label' => t('Credit awarded'),
    'title' => t('Credit awarded'),
    'help' => t('Credit awarded for this record'),
    'base' => 'course_credit_awarded',
  );

  $data['course_credit_awarded']['ctid_global']['relationship'] = array(
    'relationship field' => 'type',
    'base' => 'course_credit_type',
    'field' => 'type',
    'base field' => 'type',
    'handler' => 'views_handler_relationship',
    'label' => t('Credit type'),
    'title' => t('Credit type'),
    'help' => t('Relate to global credit type'),
  );

  $data['course_credit_awarded']['ctid_instance']['relationship'] = array(
    'handler' => 'course_credit_credit_awarded_credit_instance_vhr',
    'label' => t('Credit instance'),
    'title' => t('Credit instance'),
    'help' => t('Relate awarded credit to per-course credit instance'),
    'base' => 'course_credit',
  );

  $data['course_credit']['ctid']['relationship'] = array(
    'relationship field' => 'type',
    'base' => 'course_credit_type',
    'base field' => 'type',
    'field' => 'type',
    'handler' => 'views_handler_relationship',
    'label' => t('Default credit type'),
    'title' => t('Default credit type'),
    'help' => t('Relate to global credit type.'),
  );

  $data['node']['ctid_instance']['relationship'] = array(
    'relationship field' => 'nid',
    'base' => 'course_credit',
    'field' => 'nid',
    'base field' => 'nid',
    'handler' => 'views_handler_relationship',
    'label' => t('Credit instance'),
    'title' => t('Credit instance'),
    'help' => t('Relate node to credit instances.'),
  );

  $data['node']['credit_awarded']['relationship'] = array(
    'handler' => 'course_credit_node_awarded_credit_vhr',
    'label' => t('Credit awarded'),
    'title' => t('Credit awarded'),
    'help' => t("Relate node to user's awarded credit"),
    'base' => 'course_credit_awarded',
  );

  $data['node']['course_credit_types'] = array(
    'title' => 'Active credit types',
    'help' => 'The credit types active on this node.',
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field_course_credit_node_active_types',
    ),
  );

  $data['course_credit_type']['type']['filter']['handler'] = 'views_handler_filter_course_credit_type';
  $data['course_credit_type']['title']['field']['handler'] = 'views_handler_field_course_credit_title';

  // Extras for credit dates.
  $datefields = array(
    'course_credit_awarded' => array(
      'expiration',
      'date',
    ),
    'course_credit' => array(
      'expiration_date',
    ),
  );

  foreach ($datefields as $table => $fields) {
    foreach ($fields as $field_name) {
      $data[$table][$field_name]['field'] = array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      );
      $data[$table][$field_name]['filter']['handler'] = 'views_handler_filter_date';
      $data[$table][$field_name]['sort']['handler'] = 'views_handler_sort_date';
    }
  }

  return $data;
}
