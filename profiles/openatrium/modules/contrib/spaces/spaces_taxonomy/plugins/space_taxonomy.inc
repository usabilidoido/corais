<?php

/**
 * Taxonomy integration for Spaces. 
 */
class space_taxonomy extends space_type_purl {
  var $term = NULL;

  /**
   * Override of title().
   */
  function title() {
    return $this->term->name;
  }

  /**
    * Override of load().
    */
  function load() {
    // The 0 id means a new term is being saved. Instantiate a space
    // so preset values can become active.
    if ($this->id === 0) {
      return TRUE;
    }
    else if ($this->term = taxonomy_get_term($this->id)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Override of router().
   */
  function router($op, $object = NULL) {
    switch ($op) {
      case 'init':
        if (implode('/', array(arg(0), arg(1))) === 'taxonomy/term' && arg(2)) {
          $terms = taxonomy_terms_parse_string(arg(2));
          if (!empty($terms['tids'])) {
            foreach ($terms['tids'] as $tid) {
              if ($term = taxonomy_get_term($tid)) {
                $this->router_term($term);
              }
            }
          }
        }
        break;
      case 'node':
        $node = $object;
        if (!empty($node->taxonomy)) {
          foreach ($node->taxonomy as $term) {
            if ($term->vid == variable_get('spaces_taxonomy_vid', 0)) {
              $this->router_term($term);
            }
          }
        }
        break;
    }
    return;
  }

  /**
   * Router helper with logic for individual taxonomy terms.
   */
  protected function router_term($term) {
    if ($term->vid == variable_get('spaces_taxonomy_vid', 0)) {
      $space = spaces_load('taxonomy', $term->tid);
      // If there is no active taxonomy space or the spaces do not match, activate this term's space.
      if ($space && (!$this->active || ($this->id != $space->id))) {
        $space->activate();
      }
    }
  }

  /**
   * Override of views_filter().
   */
  function views_filter(&$query, $base_table = '', $relationship = '') {
    switch ($base_table) {
      case 'node':
        $table = $query->ensure_table('term_node', $relationship);
        $query->add_where(0, "$table.tid = %d", $this->id);
        break;
    }
  }
}
