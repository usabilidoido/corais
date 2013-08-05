<?php

/**
 * Base class describing a space.
 *
 * Provides methods for:
 * - Instantiating & loading a space by its type & id
 * - Activating a space
 * - Deactivating a space
 * - Accessing controllers
 */
class space {
  var $controllers;
  var $type;
  var $id;
  var $active;

  /**
   * Constructor.
   */
  function __construct($type, $id = NULL) {
    $this->type = $type;
    $this->id = $id;
    $this->get_controllers();
  }

  /**
   * Extending classes can use this method to load any associated data or
   * objects. Return FALSE to abort the load process.
   */
  function load() {
    return TRUE;
  }

  /**
   * Called from spaces_init_space(). Determine whether this space can be set
   * as the current active space. Override to provide custom logic for bailing
   * the spaces bootstrap.
   */
  function activate() {
    $this->active = TRUE;
    $this->init_overrides();
    return TRUE;
  }

  /**
   * Called when this space should be deactivated. If a space type uses a PURL
   * modifier to be instantiated, this is the time to remove that condition.
   * Other spaces may need to remove their custom conditions need to be at this
   * point.
   */
  function deactivate() {
    return;
  }

  /**
   * Instantiate controllers for this space.
   */
  protected function get_controllers() {
    // Create instances of each controller.
    if (!isset($this->controllers)) {
      $this->controllers = new stdClass();
      module_load_include('inc', 'spaces', 'spaces.overrides');
      ctools_include('plugins');
      $plugins = ctools_get_plugins('spaces', 'plugins');
      foreach (spaces_controllers() as $c => $info) {
        if (isset($plugins[$info['plugin']]) && $class = ctools_plugin_get_class($plugins[$info['plugin']], 'handler')) {
          $this->controllers->{$c} = new $class($c, $this->type, $this->id);
        }
      }
    }
  }

  /**
   * Initialize any overrides as necessary.
   */
  function init_overrides() {
    foreach (spaces_controllers() as $c => $info) {
      if (isset($this->controllers->{$c})) {
        $this->controllers->{$c}->init_overrides();
      }
    }
  }
}
