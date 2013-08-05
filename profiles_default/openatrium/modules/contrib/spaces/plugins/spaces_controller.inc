<?php

/**
 * Abstract base class for a spaces controller. Classes that extend
 * spaces_controller must implement load_original_values().
 */
abstract class spaces_controller {
  var $controller;
  var $space_type;
  var $space_id;
  var $values;
  protected $loaded;
  protected $loaded_all;

  function __construct($controller, $space_type, $space_id) {
    $this->controller = $controller;
    $this->space_type = $space_type;
    $this->space_id = $space_id;

    // Stores actual values on the controller instance.
    $this->values = array(
      'original' => array(),
      'preset' => array(),
      'space' => array(),
    );
    // Stores boolean flags for whether values for a given environment
    // have had at least 1 attempt to be loaded. Note that this may be
    // set to TRUE even if no value exists for the given key.
    $this->loaded = array(
      'original' => array(),
      'preset' => array(),
      'space' => array(),
    );
    // Stores boolean flags for whether all values have been loaded for
    // an environment.
    $this->loaded_all = array(
      'original' => FALSE,
      'preset' => FALSE,
      'space' => FALSE,
    );
  }

  /**
   * Initialize overrides. Assume this controller is now active.
   * This method is re-run when the caller wants to ensure the values
   * provided by the controller are refreshed.
   */
  function init_overrides() {
    $this->reset_values('original');
    $this->reset_values('preset');
    $this->reset_values('space');
  }

  /**
   * Clear out loaded values.
   */
  protected function reset_values($environment) {
    $this->loaded[$environment] = array();
    $this->loaded_all[$environment] = FALSE;
  }

  /**
   * Wrapper around load_[environment]_values.
   */
  protected function load_values($environment = NULL, $id = NULL) {
    switch ($environment) {
      case 'original':
        return $this->load_original_values($id);
      case 'preset':
        return $this->load_preset_values($id);
      case 'space':
        return $this->load_space_values($id);
      default:
        $this->load_original_values();
        $this->load_preset_values();
        $this->load_space_values();
        break;
    }
  }

  /**
   * Protected method that ensures a space-level override for the
   * provided object has been loaded.
   */
  protected function load_space_values($id = NULL) {
    if (!$this->loaded_all['space']) {
      if (!isset($id)) {
        $result = db_query("
          SELECT object_id AS id, value
          FROM {spaces_overrides}
          WHERE type = '%s'
            AND id = '%s'
            AND object_type = '%s'",
          $this->space_type, $this->space_id, $this->controller);
        while ($row = db_fetch_object($result)) {
          $this->values['space'][$row->id] = unserialize($row->value);
        }
        $this->loaded_all['space'] = TRUE;
      }
      else if (!isset($this->loaded['space'][$id])) {
        $result = db_query("
          SELECT object_id AS id, value
          FROM {spaces_overrides}
          WHERE type = '%s'
            AND id = '%s'
            AND object_type = '%s'
            AND object_id = '%s'",
          $this->space_type, $this->space_id, $this->controller, $id);
        while ($row = db_fetch_object($result)) {
          $this->values['space'][$row->id] = unserialize($row->value);
        }
        $this->loaded['space'][$id] = TRUE;
      }
    }
  }

  /**
   * Protected method that ensures a preset-level override for the
   * provided object has been loaded. Since presets values are
   * stored in aggregate, the $id argument here is largely for consistency.
   */
  protected function load_preset_values($id = NULL) {
    if (!$this->loaded_all['preset']) {
      $preset_name = variable_get("spaces_preset_{$this->space_type}", NULL);
      if ($preset_name && $preset = spaces_preset_load($preset_name)) {
        if (isset($preset->value[$this->controller])) {
          $this->values['preset'] = $preset->value[$this->controller];
        }
      }
      $this->loaded_all['preset'] = TRUE;
    }
  }

  /**
   * Protected method for loading an original object.
   * Must be overridden by extending classes.
   */
  abstract protected function load_original_values($id = NULL);

  /**
   * Get a value for this controller.
   *
   * @param $id
   *   The id of the value to retrieve. Optional.
   *
   * @param $environment
   *   The realm of to fetch the setting for either 'space', 'preset', or
   *   'original'. Optional.
   */
  function get($id = NULL, $environment = NULL) {
    if (isset($environment, $id)) {
      $this->load_values($environment, $id);
      return isset($this->values[$environment][$id]) ? $this->values[$environment][$id] : NULL;
    }
    else if (isset($environment)) {
      $this->load_values($environment);
      return isset($this->values[$environment]) ? $this->values[$environment] : NULL;
    }
    else if (isset($id)) {
      $environments = array('space', 'preset', 'original');
      $environment = array_shift($environments);
      while (isset($this->values[$environment])) {
        $this->load_values($environment, $id);
        if (isset($this->values[$environment][$id])) {
          return $this->values[$environment][$id];
        }
        $environment = array_shift($environments);
      }
      return array();
    }
    // Neither argument provided, return merged set of all values.
    $this->load_values();
    return array_merge($this->values['original'], $this->values['preset'], $this->values['space']);
  }

  /**
   * Set override values for a given controller object in this space.
   */
  function set($id, $value) {
    $override = array(
      'type' => $this->space_type,
      'id' => $this->space_id,
      'object_type' => $this->controller,
      'object_id' => $id,
    );
    $exists = db_result(db_query("SELECT id FROM {spaces_overrides} WHERE type = '%s' AND id = '%s' AND object_type = '%s' AND object_id = '%s'", $override));
    $keys = array_keys($override);
    $override['value'] = $value;
    if ($exists) {
      drupal_write_record('spaces_overrides', $override, $keys);
    }
    else {
      drupal_write_record('spaces_overrides', $override);
    }
    $this->values['space'][$id] = $value;
    // @todo Throw an exception if set failed.
    return TRUE;
  }

  /**
   * Delete a controller object override for this space.
   */
  function del($id = NULL) {
    $query = "{spaces_overrides} WHERE type = '%s' AND id = '%s' AND object_type = '%s'";
    $override = array(
      'type' => $this->space_type,
      'id' => $this->space_id,
      'object_type' => $this->controller,
    );
    if (isset($id)) {
      $query .= " AND object_id = '%s'";
      $override['object_id'] = $id;
    }
    $exists = db_result(db_query("SELECT id FROM {$query}", $override));
    if ($exists) {
      db_query("DELETE FROM {$query}", $override);
      if (isset($this->values['space'][$id])) {
        unset($this->values['space'][$id]);
      }
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Provide a readable display of an object override for administration.
   */
  function summary($id, $value = NULL) {
    switch (gettype($value)) {
      case 'string':
      case 'integer':
      case 'boolean':
      case 'double':
        return filter_xss_admin($value);
      case 'array':
      case 'object':
        return filter_xss_admin(var_export($value, TRUE));
    }
  }
}
