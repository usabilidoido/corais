<?php

/**
 * Context controller.
 */
class spaces_controller_context extends spaces_controller {
  /**
   * Load the context described by an override identifier like
   * 'foo:reaction:block'.
   */
  protected function load_from_override($override_id) {
    if ($split = explode(':', $override_id)) {
      $context_name = array_shift($split);
      $context = context_load($context_name);
      return $context;
    }
    return FALSE;
  }

  /**
   * Split the single context object into array of individual plugin values.
   */
  protected function plugin_values($context) {
    $keys = array('conditions', 'reactions');
    $plugins = array();
    if (!empty($context->conditions)) {
      foreach ($context->conditions as $plugin => $values) {
        $plugins["{$context->name}:condition:{$plugin}"] = $values;
      }
    }
    if (!empty($context->reactions)) {
      foreach ($context->reactions as $plugin => $values) {
        $plugins["{$context->name}:reaction:{$plugin}"] = $values;
      }
    }
    return $plugins;
  }

  /**
   * Override of load_space_values().
   *
   * All overrides are retrieved at once for a given space to reduce the number
   * of queries run when a space is active.
   */
  protected function load_space_values($id = NULL) {
    if (!$this->loaded_all['space']) {
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
  }

  /**
   * Override of load_original_values().
   *
   * Note that $id here is *NOT* $context->name, but rather a derived plugin
   * id allowing overrides to be set on a granular, per-plugin level. The
   * format of $id is "{$context->name}:{$plugin_type}:{$plugin}", e.g.
   * "blog:reaction:block".
   */
  protected function load_original_values($id = NULL) {
    if (empty($this->loaded_all['original'])) {
      if (!isset($id)) {
        $contexts = context_load();
        foreach ($contexts as $name => $context) {
          array_merge($this->values['original'], $this->plugin_values($context));
        }
        $this->loaded_all['original'] = TRUE;
      }
      else if (!isset($this->loaded['original'][$id])) {
        if ($context = $this->load_from_override($id)) {
          $plugins = $this->plugin_values($context);
          foreach ($plugins as $plugin_id => $plugin_values) {
            $this->values['original'][$plugin_id] = $plugin_values;
            $this->loaded['original'][$plugin_id] = TRUE;
          }
        }
      }
    }
  }
}
