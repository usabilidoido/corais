<?php
// $Id: casetracker_views_handler_filter_project_options.inc,v 1.1 2009/02/09 23:22:58 jmiccolis Exp $

/**
 * Provides a filter (exposable) with projects filtered by the casetracker_project_options view.
 */
class casetracker_views_handler_filter_project_options extends views_handler_filter_many_to_one {
  function get_value_options() {
    $this->value_options = casetracker_project_options();
  }
}