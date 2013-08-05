<?php

class crayon_views_handler_field_project extends casetracker_views_handler_field_project_name {
  /**
   * Override of render().
   */
  function render($values) {
    static $projects = array();
    if (!isset($projects[$values->{$this->field_alias}])) {
      $projects[$values->{$this->field_alias}] = db_fetch_object(db_query(db_rewrite_sql("SELECT n.nid, n.title FROM {node} n WHERE n.nid = %d"), $values->{$this->field_alias}));
    }
    if ($node = $projects[$values->{$this->field_alias}]) {
      return theme('crayon_swatch', $this->acronym($node->title), $this->color($node->nid), $node->title, "node/{$node->nid}");
    }
  }

  /**
  * Generate an acronym for a given string value.
  * Override this in a subclass to provide custom acronym generation logic.
  */
  protected function acronym($value) {
    return crayon_generate_acronym($value);
  }

  /**
  * Override this in a subclass to provide custom color generation logic.
  */
  protected function color($value) {
    return $value;
  }
}
