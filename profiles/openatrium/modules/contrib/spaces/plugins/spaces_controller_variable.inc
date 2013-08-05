<?php

/**
 * Variable controller.
 */
class spaces_controller_variable extends spaces_controller {
  /**
   * Override of init_overrides().
   */
  function init_overrides() {
    // Load original variables. Only do this once, and it may not be reset.
    $this->load_original_values();

    // Load the preset values.
    $this->reset_values('preset');
    $this->load_preset_values();

    // Load space variables.
    $this->reset_values('space');
    $this->load_space_values();

    // Alter global variable conf as we don't have any other way of
    // influencing the result of variable_get().
    global $conf;
    $conf = $this->get();

    // If we have overridden 'site_frontpage', we need to do some extra
    // work to reinit the drupal path.
    if ($this->get('site_frontpage') !== $this->get('site_frontpage', 'original') && function_exists('purl_language_strip')) {
      $_GET['q'] = purl_language_strip($_REQUEST['q']);
      drupal_init_path();
    }
  }

  /**
   * Override of load_preset_values(). We cannot rely on variable_get() --
   * we must first check our space override values.
   */
  protected function load_preset_values($id = NULL) {
    if (empty($this->loaded_all['preset'])) {
      $preset_name = $this->get("spaces_preset_{$this->space_type}", 'space') ?
        $this->get("spaces_preset_{$this->space_type}", 'space') :
        variable_get("spaces_preset_{$this->space_type}", NULL);
      if ($preset_name && $preset = spaces_preset_load($preset_name)) {
        if (isset($preset->value[$this->controller])) {
          $this->values['preset'] = $preset->value[$this->controller];
        }
      }
      $this->loaded_all['preset'] = TRUE;
    }
  }

  /**
   * Override of load_original_values().
   */
  protected function load_original_values($id = NULL) {
    global $conf;
    if (empty($this->values['original'])) {
      $this->values['original'] = $conf;
      $this->loaded_all['original'] = TRUE;
    }
  }
}
