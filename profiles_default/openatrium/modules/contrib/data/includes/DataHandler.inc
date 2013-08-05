<?php
/**
 * @file
 * Definition of DataHandler class.
 */

/**
 * Simple access methods to table data. Can be used on any table, not just Data
 * managed tables.
 */
class DataHandler {

  // Holds the name of the table that this handler is responsible for.
  protected $table;

  /**
   * Constructor, call indirectly through DataHandler::instance();
   */
  protected function __construct($table) {
    $this->table = $table;
  }

  /**
   * Instantiate a DataHandler object.
   *
   * @param $table
   *   The name of the table to access with this DataHandler object.
   */
  public static function instance($table) {
    static $handlers;
    if (!isset($handlers[$table])) {
      $handlers[$table] = new DataHandler($table);
    }
    return $handlers[$table];
  }

  /**
   * Getter.
   */
  public function __get($name) {
    return $this->$name;
  }

  /**
   * Load a record.
   */
  public function load($keys) {
    $where = $values = array();
    $schema = drupal_get_schema($this->table);
    $fields = $schema['fields'];
    foreach ($keys as $key => $value) {
      // Return if a key does not exist.
      if (!isset($fields[$key]['type'])) {
        return FALSE;
      }
      $where[] = db_escape_string($key) ." = ". db_type_placeholder($fields[$key]['type']);
      $values[] = $value;
    }

    if (!empty($where)) {
      $result = db_query('SELECT * FROM {'. db_escape_table($this->table) .'} WHERE '. implode(' AND ', $where), $values);
      $results = array();
      while ($row = db_fetch_array($result)) {
        $results[] = $row;
      }
      return count($results) ? $results : FALSE;
    }
    return FALSE;
  }

  /**
   * Insert a record.
   *
   * @see drupal_write_record().
   *
   * @param $record
   *   An array that is the record to save to this handler's table.
   */
  public function insert(&$record) {
    $result = drupal_write_record($this->table, $record);
    module_invoke_all('data_insert', $record, $this->table);
    return $result;
  }

  /**
   * Update a record.
   *
   * @see drupal_write_record().
   *
   * @param $record
   *   An array that is the record to save to this handler's table.
   * @param $update
   *   A string or an array of strings that defines the keys to use for
   *   this update.
   */
  public function update(&$record, $update) {
    $result = drupal_write_record($this->table, $record, $update);
    module_invoke_all('data_update', $record, $this->table);
    return $result;
  }

  /**
   * Save one or more records to the table.
   *
   * If $update is given, method will try to update before.
   *
   * This method is more comfortable, but slower than using insert() or
   * update().
   *
   * @param $record
   *   An array that is the record to save to this handler's table.
   * @param
   *   A string or an array of strings that defines the keys to use for
   *   this update.
   */
  public function save(&$record, $update = array()) {
    if (is_string($update)) {
      $update = array($update);
    }
    if (is_array($update)) {
      $keys = array();
      foreach ($update as $key) {
        $keys[$key] = $record[$key];
      }
      if (count($keys)) {
        if ($this->load($keys)) {
          return $this->update($record, $update);
        }
      }
    }
    return $this->insert($record);
  }

  /**
   * Delete one or more records from the table.
   *
   * @param $clause
   *   An array where the keys are columns in the data table and the values are
   *   the values to filter on. This array will be turned into the where clause
   *   of the delete query.
   *
   *   Example:
   *
   *   array(
   *     'feed_nid' => 10,
   *     'timestamp' => array(
   *       '<',
   *       1255623780,
   *     ),
   *   );
   */
  public function delete($clause) {
    $query = new DataQuery($this->table);
    $schema = drupal_get_schema($this->table);
    $fields = $schema['fields'];
    foreach ($clause as $key => $value) {
      $operator = '=';
      if (is_array($value)) {
        $operator = array_shift($value);
        $value = array_shift($value);
      }
      $query->addWhere(db_escape_table($this->table) .'.'. db_escape_string($key) ." ". $operator ." ". db_type_placeholder($fields[$key]['type']), $value);
    }
    drupal_alter('data_delete_query', $query, $this->table);
    if (!empty($query->where)) {
      $query->execute();
    }
    return db_affected_rows();
  }

  /**
   * Empty data table.
   */
  public function truncate() {
    db_query('TRUNCATE TABLE {'. db_escape_table($this->table) .'}');
  }
}

/**
 * Builds and executes a query, only delete queries are supported at the moment.
 */
class DataQuery {

  public $subject, $table, $from, $join, $where;

  /**
   * Creates an empty query.
   *
   * @param $table
   *   Base table to operate on, also the subject table to delete
   *   from.
   */
  public function __construct($table) {
    $this->table = db_escape_table($table);
    $this->subject = array($this->table => $this->table);
    $this->from = $this->table;
    $this->join = array();
    $this->where = array();
  }

  /**
   * Add a subject table.
   */
  public function addSubject($table) {
    $this->subject[$table] = $table;
  }

  /**
   * Add a join.
   *
   * @param $table
   *   The table to join to.
   * @param $join
   *   The columns to join on.
   */
  public function addJoin($table, $join, $type = 'INNER JOIN') {
    $this->join[$table] = array(
      $join,
      $type,
    );
  }

  /**
   * Add a where clause.
   *
   * @param $clause
   *   The actual clause. Caller is responsible for referring to existing fields
   *   and supplying correct placeholder for value.
   * @param $value
   *   The value the clause compares against.
   */
  public function addWhere($clause, $value = NULL) {
    $this->where[$clause] = $value;
  }

  /**
   * Build and execute a query.
   */
  public function execute() {
    $table = db_escape_table($this->table);
    $query = "DELETE ". implode(', ', $this->subject);
    $query .= " FROM {{$table}} {$table}";
    if (!empty($this->join)) {
      foreach ($this->join as $table => $join) {
        $table = db_escape_table($table);
        $clause = array_shift($join);
        $join = array_shift($join);
        $query .= " $join {{$table}} $table ON $clause";
      }
    }
    $where = $values = array();
    foreach ($this->where as $k => $v) {
      $where[] = $k;
      if ($v !== NULL) {
        $values[] = $v;
      }
    }
    if (!empty($where)) {
      $query .= " WHERE ". implode(' AND ', $where);
    }
    return db_query($query, $values);
  }
}
