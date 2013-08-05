<?php
/**
 * @file
 * Views hooks and utility functions.
 */

/**
 * Implementation of hook_views_data().
 * 
 * Dynamically create views integration for any table Data manages.
 */
function data_views_data() {
  $data = array();

  $tables = data_get_all_tables();

  foreach ($tables as $table) {

    // Get schema and check wether there are field definitions.
    $schema = $table->get('table_schema');
    $meta = $table->get('meta');
    if (!isset($schema['fields'])) {
      continue;
    }

    $table_data = array();
    $table_data['table'] = array(
      'group' => check_plain($table->get('title')),
      );
    foreach ($schema['fields'] as $field_name => $field) {
      // If there is no label, generate one from field name.
      $title = empty($meta['fields'][$field_name]['label']) ? data_natural_name($field_name) : $meta['fields'][$field_name]['label'];

      $table_data[$field_name] = array(
        'title' => $title,
        'help' => $title,
        'field' => array(
          'handler' => data_get_views_handler('field', $table, $field_name),
          'help' => $title,
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => data_get_views_handler('filter', $table, $field_name),
          'allow empty' => TRUE,
          'help' => t('Filter on %field', array('%field' => $title)),
        ),
        'argument' => array(
          'handler' => data_get_views_handler('argument', $table, $field_name),
          'help' => $title,
        ),
        'sort' => array(
          'handler' => data_get_views_handler('sort', $table, $field_name),
          'help' => t('Sort by %field', array('%field' => $title)),
        ),
      );
    }

    // Tables with a primary key are base tables.
    if (isset($schema['primary key']) && count($schema['primary key']) >= 1) {
      $table_data['table']['base'] = array(
        'field' => current($schema['primary key']),
        'title' => check_plain($table->get('title')),
        'help' => t('Data table'),
        'weight' => 10, 
      );
    }

    // Add join information.
    if (isset($meta['join']) && is_array($meta['join'])) {
      $table_data['table']['join'] = array();
      foreach ($meta['join'] as $left_table => $join) {
        // @todo: See if left table has other tables it is linked to and link
        // all the way to the leftmost table.
        $table_data['table']['join'][$left_table] = array(
          'left_field' => $join['left_field'],
          'field' => $join['field'],
          'type' => $join['inner_join'] ? 'INNER' : 'LEFT',
        );
        // Add an explicit relationship for every join added.
        $table_data[$join['field']]['relationship'] = array(
          'label' => "{$table->get('name')}.{$join['field']} -> {$left_table}.{$join['left_field']}",
          'base' => $left_table,
          'base field' => $join['left_field'],
        );
      }
    }

    $data[$table->get('name')] = $table_data;
  }

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function data_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'data') . '/views',
    ),
    'handlers' => array(
      'views_handler_field_data_markup' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Return all available field handlers.
 *
 * @param $type
 *   Optional: the view handler type whose options should be provided
 *   ('field', 'filter', 'sort', 'argument'). If omitted, a full array
 *   keyed on type is returned.
 * @param $reset
 *   Boolean to reset the static cache.
 *
 * @return
 *   An array suitable for use as options in a FormAPI element.
 */
function data_get_views_handler_options($type = NULL, $reset = FALSE) {
  static $handlers;
  if (!isset($handlers) || $reset) {
    $handlers = array();
    module_load_include('inc', 'views', 'includes/base');
    module_load_include('inc', 'views', 'includes/handlers');
    $available_handlers = views_discover_handlers();
    $handlers = array(
      'field',
      'filter',
      'argument',
      'sort',
    );
    foreach ($available_handlers as $available_handler => $def) {
      foreach ($handlers as $handler) {
        // Look for handler names of the form 'views_handler_TYPE.*'.
        $pattern = '/^views_handler_';
        $pattern .= $handler;
        $pattern .= '([_][^_.]+)*$/';
        if (preg_match($pattern, $available_handler)) {
          $handlers[$handler][$available_handler] = $available_handler;
        };
      };
    };
    // Allow other modules to alter the list of available handlers.
    drupal_alter('data_views_handlers', $handlers);
  }
  return isset($type) && isset($handlers[$type]) ? $handlers[$type] : $handlers;
}

/**
 * Get the handler (class name) for a specified data table field.
 *
 * @param $type
 *   The view handler type ('field', 'filter', 'sort', 'argument').
 * @param $table
 *   A data table object.
 * @param $field_name
 *   String: name of the field.
 * @param $default
 *   Boolean for whether to return the default handler for the given
 *   db column type.
 *
 * @return
 *   String: A views handler class name.
 */
function data_get_views_handler($type, $table, $field_name, $default = FALSE) {
  // Return the handler's custom setting if available
  if (!$default) {
    $meta = $table->get('meta');
    if (isset($meta['fields'][$field_name]["views_{$type}_handler"])) {
      return $meta['fields'][$field_name]["views_{$type}_handler"];
    }
  }

  $schema = $table->get('table_schema');
  switch ($type) {

    case 'field':
      switch ($schema['fields'][$field_name]['type']) {
        case 'int':
        case 'float':
        case 'serial':
        case 'numeric':
          return 'views_handler_field_numeric';
        case 'datetime':
          return 'views_handler_field_date';
      }
      return 'views_handler_field';

    case 'filter':
      switch ($schema['fields'][$field_name]['type']) {
        case 'float':
        case 'numeric':
          return 'views_handler_filter_float';
        case 'int':
        case 'serial':
          return 'views_handler_filter_numeric';
        case 'datetime':
          // @TODO: make this conditional on whether the time is stored as a timestamp/datetime
          if (module_exists('date_api')) {
            return 'date_api_filter_handler';
          }
          return 'views_handler_filter_date';
      }
      return 'views_handler_filter_string';

    case 'argument':
      switch ($schema['fields'][$field_name]['type']) {
        case 'int':
        case 'float':
        case 'serial':
        case 'numeric':
          return 'views_handler_argument_numeric';
        case 'datetime':
          return 'views_handler_argument_date';
        case 'varchar':
        return 'views_handler_argument_string';
      }
      return 'views_handler_argument';

    case 'sort':
      switch ($schema['fields'][$field_name]['type']) {
        case 'datetime':
          return 'views_handler_sort_date';
      }
      return 'views_handler_sort';

  }
}
