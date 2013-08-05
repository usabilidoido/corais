<?php
// $Id$

/**
 * @file
 * Views 2 support for location taxonomy.
 */

/**
 * Implementation of hook_views_data().
 */
function location_taxonomy_views_data() {
  // Tell the base tables about us.

  // location table.
  // Location links to taxonomy terms via lid.
  $data['location']['table']['join']['term_data'] = array(
    'left_table' => 'location_instance',
    'left_field' => 'lid',
    'field' => 'lid',
  );
  // location_instance table.
  $data['location_instance']['table']['join']['term_data'] = array(
    'handler' => 'location_join_taxonomy',
    'left_field' => 'tid',
    'field' => 'genid',
  );
  // term_data table
  $data['term_data']['table']['join']['location'] = array(
    'handler' => 'location_join_taxonomy',
    'left_table' => 'location_instance',
    'left_field' => 'genid',
    'field' => 'tid',
  );

  return $data;
}

/**
 * Custom views join handler to join the term_data and location_instance
 * tables via genid.
 */
class location_join_taxonomy extends views_join {
  // PHP 4 doesn't call constructors of the base class automatically from a
  // constructor of a derived class. It is your responsibility to propagate
  // the call to constructors upstream where appropriate.
  function construct($table = NULL, $left_table = NULL, $left_field = NULL, $field = NULL, $extra = array(), $type = 'LEFT') {
    parent::construct($table, $left_table, $left_field, $field, $extra, $type);
  }

  /**
   * Build the SQL for the join this object represents.
   */
  function join($table, &$query) {
    $left = $query->get_table_info($this->left_table);
    switch ($this->field) {
      case 'genid':
        switch ($GLOBALS['db_type']) {
          case 'mysql':
          case 'mysqli':
            $output = " {$this->type} JOIN {{$this->table}} {$table['alias']} ON CONCAT('taxonomy:', {$left['alias']}.{$this->left_field}) = {$table['alias']}.{$this->field}";
          break;
          case 'pgsql':
            $output = " {$this->type} JOIN {{this->table}} {$table['alias']} ON ('taxonomy:' || {$left['alias']}.{$this->left_field}) = {$table['alias']}.{$this->field}";
          break;
        }
        break;
      case 'tid':
        switch ($GLOBALS['db_type']) {
          case 'mysql':
          case 'mysqli':
            $output = " {$this->type} JOIN {{$this->table}} {$table['alias']} ON {$left['alias']}.{$this->left_field} = CONCAT('taxonomy:', {$table['alias']}.{$this->field})";
          break;
          case 'pgsql':
            $output = " {$this->type} JOIN {{$this->table}} {$table['alias']} ON {$left['alias']}.{$this->left_field} = ('taxonomy:' || {$table['alias']}.{$this->field})";
          break;
        }
        break;
    }

    // Tack on the extra (this part is taken from the parent views_join).
    if (isset($this->extra)) {
      if (is_array($this->extra)) {
        $extras = array();
        foreach ($this->extra as $info) {
          $extra = '';
          // Figure out the table name. Remember, only use aliases provided
          // if at all possible.
          $join_table = '';
          if (!array_key_exists('table', $info)) {
            $join_table = $table['alias'] . '.';
          }
          elseif (isset($info['table'])) {
            $join_table = $info['table'] . '.';
          }

          // And now deal with the value and the operator.  Set $q to
          // a single-quote for non-numeric values and the
          // empty-string for numeric values, then wrap all values in $q.
          $raw_value = $this->db_safe($info['value']);
          $q = (empty($info['numeric']) ? "'" : '');

          if (is_array($raw_value)) {
            $operator = !empty($info['operator']) ? $info['operator'] : 'IN';
            // Transform from IN() notation to = notation if just one value.
            if (count($raw_value) == 1) {
              $value = $q . array_shift($raw_value) . $q;
              $operator = $operator == 'NOT IN' ? '!=' : '=';
            }
            else {
              $value = "($q" . implode("$q, $q", $raw_value) . "$q)";
            }
          }
          else {
            $operator = !empty($info['operator']) ? $info['operator'] : '=';
            $value = "$q$raw_value$q";
          }
          $extras[] = "$join_table$info[field] $operator $value";
        }

        if ($extras) {
          if (count($extras) == 1) {
            $output .= ' AND ' . array_shift($extras);
          }
          else {
            $output .= ' AND (' . implode(' ' . $this->extra_type . ' ', $extras) . ')';
          }
        }
      }
      else if ($this->extra && is_string($this->extra)) {
        $output .= " AND ($this->extra)";
      }
    }
    return $output;
  }
}
