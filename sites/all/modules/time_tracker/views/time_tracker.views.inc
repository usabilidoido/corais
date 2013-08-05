<?php

/**
 * @file
 * Views integration for time_tracker.
 */

/**
 * Implements hook_views_data().
 */
function time_tracker_views_data() {
  $data['time_tracker_entry']['table']['group'] = t('Time Tracker');
  $data['time_tracker_entry']['table']['base'] = array(
    'field' => 'teid',
    'title' => t('Time Tracker'),
    'help' => t('Content generated from time tracker entries.'),
  );

  $data['time_tracker_entry']['teid'] = array(
    'title' => t('ID'),
    'help' => t('Time Entry ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('Timestamp of the Entry.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'time_tracker_views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['start'] = array(
    'title' => t('Start Time'),
    'help' => t('Start time of the Entry.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'time_tracker_views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['end'] = array(
    'title' => t('End Time'),
    'help' => t('End time of the Entry.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'time_tracker_views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['duration'] = array(
    'title' => t('Duration'),
    'help' => t('The Raw Time Entry Duration (without deductions).'),
    'field' => array(
      'handler' => 'time_tracker_views_handler_field_numeric',
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['note'] = array(
    'title' => t('Note'),
    'help' => t('Notes for node-based time entries'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['deductions'] = array(
    'title' => t('Deductions'),
    'help' => t('Time Entry Deductions.'),
    'field' => array(
      'handler' => 'time_tracker_views_handler_field_numeric',
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  // $data['time_tracker_entry']['total_duration'] = array(
  //   'title' => t('Total Duration'),
  //   'help' => t('Time Entry Total Duration, minus the deductions'),
  //   'field' => array(
  //     'handler' => 'time_tracker_views_handler_field_total_duration',
  //     'float' => TRUE,
  //   ),
  //   'sort' => array(
  //     'handler' => 'views_handler_sort',
  //   ),
  //   'filter' => array(
  //     'handler' => 'views_handler_filter_numeric',
  //   ),
  //   'argument' => array(
  //     'handler' => 'views_handler_argument',
  //   ),
  // );
  // This isn't being used right now so I'm de-exposing it to views
  $data['time_tracker_entry']['billable'] = array(
    'title' => t('Billable'),
    'help' => t('Is the Entry Billable?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['billed'] = array(
    'title' => t('Billed'),
    'help' => t('Has the Entry been billed?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['locked'] = array(
    'title' => t('Locked'),
    'help' => t('Is the Entry Locked?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
  $data['time_tracker_entry']['uid'] = array(
    'title' => t('Author'),
    'help' => t('Relate an entry to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('User'),
    ),
  );
  $data['time_tracker_entry']['nid'] = array(
    'title' => t('Node'),
    'help' => t('Relate an entry to a comments node.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'label' => t('Node'),
    ),
  );
  $data['time_tracker_entry']['cid'] = array(
    'title' => t('Comment'),
    'help' => t('Relate an entry to a comment.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'comments',
      'base field' => 'cid',
      'label' => t('Comment'),
    ),
  );
  $data['time_tracker_entry']['activity'] = array(
    'title' => t('Activity'),
    'help' => t('Relate an entry to an Activity Name.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'time_tracker_activity',
      'base field' => 'taid',
      'label' => t('Activity'),
    ),
  );

  $data['time_tracker_activity']['table']['join']['time_tracker_entry'] = array(
    'left_field' => 'activity',
    'field' => 'taid',
  );
  $data['time_tracker_activity']['table']['group'] = t('Time Tracker');

  $data['time_tracker_activity']['name'] = array(
    'title' => t('Activity Name'),
    'help' => t('Activity Name.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['time_estimate_entry']['table']['join'] = array(
    'time_tracker_entry' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['time_estimate_entry']['table']['group'] = t('Time Tracker');

  $data['time_estimate_entry']['estimate'] = array(
    'title' => t('Time Estimate'),
    'help' => t('Time Estimate.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Access Time Tracker Data from the comments base table
  $data['comments']['cid'] = array(
    'relationship' => array(
      'title' => t('Time Tracker'),
      'help' => t('Access time tracker data for comments.'),
      'handler' => 'views_handler_relationship',
      'base' => 'time_tracker_entry',
      'base field' => 'cid',
      'label' => t('Time Tracker'),
    ),
  );

  // Access Time Tracker Data from the node base table
  $data['node']['nid'] = array(
    'relationship' => array(
      'title' => t('Time Tracker'),
      'help' => t('Access time tracker data for nodes.'),
      'handler' => 'views_handler_relationship',
      'base' => 'time_tracker_entry',
      'base field' => 'nid',
      'label' => t('Time Tracker'),
    ),
  );

  // Define a "table" called 'time_tracker_totals', which will actually be
  // a subquery that will add up all the time tracked on the node.
  $data['time_tracker_totals'] = array(
    'table' => array(
      'group' => t('Time Tracker'),
      // Join this "table" to the Node table, so this can be used in node views,
      // which is the whole point.
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
          // Use a custom join handler, which constructs the query. This is
          // defined below.
          'handler' => 'time_tracker_node_join_handler',
        ),
      ),
    ),

    // Create a field called total_time, to add up all the time tracked on
    // a node. Allow it to be used as a numeric Field, Filter, and Sort.
    'total_time' => array(
      'title' => t('Total time'),
      'help' => t('The total time tracked on the node (minus deductions)'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
        'float' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
  );

  // Define another "table" called 'time_tracker_total_duration'.
  // This one creates a compound field "total_duration" (duration - deduction)
  // and joins it back into the time_tracker_entry table.
  $data['time_tracker_total_duration'] = array(
    'table' => array(
      'group' => t('Time Tracker'),
      // Join this "table" to the Node table, so this can be used in node views,
      // which is the whole point.
      'join' => array(
        'time_tracker_entry' => array(
          'left_field' => 'teid',
          'field' => 'teid',
          // Use a custom join handler, which constructs the query. This is
          // defined below.
          'handler' => 'time_tracker_total_duration_join_handler',
        ),
      ),
    ),

    // Create a field called total_duration, which is the duration minus the
    // deductions. This belongs to the pseudo table "time_tracker_total_duration"
    'total_duration' => array(
      'title' => t('Total Duration'),
      'help' => t('Time Entry Total Duration, minus the deductions'),
      'field' => array(
        'handler' => 'time_tracker_views_handler_field_numeric',
        'click sortable' => TRUE,
        'float' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function time_tracker_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'time_tracker') .'/views',
    ),
    'handlers' => array(
      'time_tracker_views_handler_filter_date' => array(
        'parent' => 'views_handler_filter_numeric',
      ),
      'time_tracker_views_handler_field_numeric' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      // 'time_tracker_views_handler_field_total_duration' => array(
      //   'parent' => 'time_tracker_views_handler_field_numeric',
      // ),
    ),
  );
}


function time_tracker_form_views_exposed_form_alter(&$form, $form_state) {

}

/**
 * Implementation of hook_date_api_tables().
 */
function time_tracker_date_api_tables() {
  return array('time_tracker_entry');
}

/**
 * Implementation of hook_date_api_fields().
 *
 * This is required to allow timetracker timestamp
 * as a date argument in views.
 */
function time_tracker_date_api_fields($field) {
  $values = array(
    'sql_type' => DATE_UNIX,
    'tz_handling' => 'site',
    'timezone_field' => '',
    'offset_field' => '',
    'related_fields' => array(),
    'granularity' => array('year', 'month', 'day', 'hour', 'minute', 'second'),
  );

  switch ($field) {
    case 'time_tracker_entry.timestamp':
    case 'time_tracker_entry.start':
    case 'time_tracker_entry.end':
      return $values;
  }
}

/**
 * Implementation of hook_views_bulk_operations_object_info().
 */
function time_tracker_views_bulk_operations_object_info() {
  $object_info = array(
    'time_tracker_entry' => array(
      'type' => 'time_tracker',
      'base_table' => 'time_tracker_entry',
      'load' => '_time_tracker_entry_load',
      'oid' => 'teid',
      'title' => 'title',
    ),
  );
  return $object_info;
}


/**
 * Implementation of hook_action_info().
 */
function time_tracker_action_info() {
  return array(
    'time_tracker_time_tracker_billed_action' => array(
      'description' => t('Mark entries as billed'),
      'type' => 'time_tracker',
      'configurable' => FALSE,
      'hooks' => array(
        'time_tracker' => array('billed'),
      ),
    ),
  );
}


/***********************************************************************
 * HELPER FUNCTIONS
 ***********************************************************************/


function time_tracker_time_tracker_billed_action($teids, $context) {
  foreach ($teids as $teid) {
    db_query("UPDATE {time_tracker_entry} SET billed = 1 WHERE teid = %d", $teid);
  }
}

function _time_tracker_entry_load($teid) {
  return db_fetch_object(db_query("SELECT *, n.title FROM {time_tracker_entry} AS te JOIN {node} AS n ON te.nid = n.nid WHERE teid = %d", $teid));
}


/***********************************************************************
 * VIEWS CUSTOM JOIN HANDLERS
 ***********************************************************************/


/**
 * Handles the join between the node table and the time information.
 *
 * Note that this handler is in the views.inc file rather than in its own file,
 * because we need to load it. Views doesn't auto-load join handlers, unlike
 * other handlers (i.e., join handlers are not Views handlers per se, despite
 * the name).
 */
class time_tracker_node_join_handler extends views_join {

  /**
   * Overrides the views_join::join() method.
   *
   * Custom join that makes a sub-query.
   */
  function join($table, &$query) {
    $alias = $table['alias'];
    $left = $query->get_table_info('node');
    $left_field = $left['alias'] . ".nid";

    $output = $this->type . " JOIN (SELECT SUM(`duration`) - SUM(`deductions`) as total_time, `nid` FROM {time_tracker_entry} GROUP BY nid) as $alias ON $left_field = $alias.nid";
    return $output;
  }
}

/**
 * Handles the join between the time entry table and itself. Wait, what?
 *
 * We do this to create a new "Table" that defines the "total_duration" field
 * which is the duration minus the deductions. It's probably not the best
 * solution performance-wise, but it's compatible with views calc, and that's
 * more important for now. My original solution was a custom field handler
 * that overrode the query method. I kept it for now in:
 *   time_tracker_views_handler_field_total_duration.inc
 */
class time_tracker_total_duration_join_handler extends views_join {

  /**
   * Overrides the views_join::join() method.
   *
   * Custom join that makes a sub-query.
   */
  function join($table, &$query) {
    $alias = $table['alias'];
    $left = $query->get_table_info('time_tracker_entry');
    $left_field = $left['alias'] . ".teid";

    $output = $this->type . " JOIN (SELECT duration - deductions as total_duration, teid FROM {time_tracker_entry}) as $alias ON $left_field = $alias.teid";
    return $output;
  }
}
