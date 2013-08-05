<?php

/**
 * Customtext variable controller.
 */
class spaces_controller_customtext extends spaces_controller_variable {
  /**
   * Override of init_overrides().
   */
  function init_overrides() {
    parent::init_overrides();
    global $language;
    $language->language = spaces_customtext_cache();
  }
}
