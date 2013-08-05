<?php

/**
 * Common functionality for space types that use a PURL modifier to trigger
 * their activation. Examples: space_og, space_taxonomy.
 */
class space_type_purl extends space_type {
  /**
   * Verify that this space's PURL modifier is present for the current
   * page request.
   */
  protected function verify_purl() {
    $method = variable_get("purl_method_spaces_{$this->type}", 'path');
    $elements = purl_active()->get($method);
    foreach ($elements as $element) {
      if ($element->provider == "spaces_{$this->type}" && $element->id == $this->id) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Return an array of paths from which the space should never be active.
   */
  protected function excluded_paths() {
    return array('features', 'features/*', 'admin', 'admin/*');
  }

  /**
   * Override of activate(). Ensure that the PURL modifier is present when the
   * space is active.
   */
  function activate() {
    if (!$this->verify_purl()) {
      $this->purge_request_destination();
      // @TODO: This will drop other PURL providers. Probably not the desired behavior!
      purl_goto($_GET['q'], array('query' => drupal_query_string_encode($_GET, array('q')), 'purl' => array('provider' => "spaces_{$this->type}", 'id' => $this->id)));
    }

    // Activate the space before checking for excluded paths. This prevents
    // certain corner case badness, e.g. a stale $_GET['q'] set for the site
    // space triggering an excluded path check.
    $activated = parent::activate();
    $paths = implode("\n", $this->excluded_paths());
    if (drupal_match_path($_GET['q'], $paths)) {
      $this->deactivate();
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Override of deactivate(). Ensure that the PURL modifier is not present
   * when the space is not active.
   */
  function deactivate() {
    $this->purge_request_destination();
    purl_goto($_GET['q'], array('query' => drupal_query_string_encode($_GET, array('q')), 'purl' => array('remove' => array("spaces_{$this->type}"))));
  }

  /**
   * Pull the destination out of the $_REQUEST to prevent a redirect directly to
   * it within purl_goto. This function should be used immediately before a call
   * to purl_goto.
   */
  function purge_request_destination() {
    if (isset($_REQUEST['destination'])) {
      unset($_REQUEST['destination']);
    }
    if (isset($_REQUEST['edit']['destination'])) {
      unset($_REQUEST['edit']['destination']);
    }
  }
}
