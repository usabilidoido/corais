<?php
// $Id: casetracker_views_handler_filter_project.inc,v 1.1.2.1 2009/03/02 15:59:55 jmiccolis Exp $
/**
 * Filter to only show casetracker cases.
 */
class casetracker_views_handler_filter_project extends views_handler_filter {
  function query() {
    if ($project_types = variable_get('casetracker_project_node_types', array('casetracker_basic_project'))) {
      // Weed out disabled types
      foreach ($project_types as $type => $enabled) {
        if (empty($enabled)) {
          unset($project_types[$type]);
        }
      }
      $placeholders = db_placeholders($project_types, 'varchar');
      $table = $this->query->ensure_table('node');
      $this->query->add_where($this->options['group'], "$table.type IN ($placeholders)", $project_types);
    }
    else {
      $this->query->add_where($this->options['group'], "FALSE");
      drupal_set_message(t('You have no node types which are acting as projects.', 'error'));
    }
  }
}
