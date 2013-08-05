<?php

/**
 * @file
 * Handler for 'finder_views' display.
 */
class finder_views_plugin_display_finder extends views_plugin_display {

  function execute() {
    return $this->view->render($this->display->id);
  }

  function render() {
    return !empty($this->view->result) || !empty($this->view->style_plugin->definition['even empty']) ? $this->view->style_plugin->render($this->view->result) : '';
  }

  function query() {
    // Get variables passed through Views' "options" functionality.
    extract($this->get_option('finder_views_options'));
    $field_info = $this->display->display_options['finder_views_field_info'];

    // Add relationships for choices fields.
    if ($mode == 'choices' && isset($finder->settings['views']['relationships'])) {
      $element = &finder_element($finder, $finder_element_id);
      $element_fields = &$element->settings['choices']['field'];
      foreach ($element_fields as $key => $field) {
        foreach ($finder->settings['views']['relationships'] as $rel_key => $rel_fields) {
          if (in_array($field, $rel_fields)) {
            $field_info[$finder_element_id][$key] = finder_split_field($field);
            $alias = finder_table_alias($feid, $field_info[$finder_element_id][$key]['table'], $field_info[$finder_element_id][$key]['field']);
            $relationship = $this->view->relationship[$rel_key]->alias;
            $join = NULL;
            $this->view->query->add_table($field_info[$finder_element_id][$key]['table'], $relationship, $join, $alias);
          }
        }
      }
    }


    if (!empty($keywords)) {

      $where_group_created = FALSE;

      if ($mode == 'choices') {
        $this->view->query->set_where_group('OR', 'finder_choices');
      }

      foreach ($keywords as $feid => $keyword_array) {

        $element = &finder_element($finder, $feid);
        $element_fields = &$element->settings['choices']['field'];
        $field_combination = $element->settings['advanced']['field_combination'] ? 'AND' : 'OR';
        $value_combination = $element->settings['advanced']['value_combination'] ? 'AND' : 'OR';
        $nesting_order = $element->settings['advanced']['nesting_order'];
        $results_match = finder_match_operator($element->settings['advanced']['match']);

        $match = ($mode == 'results') ? $results_match : $match;

        if (!empty($keyword_array)) {

          $clauses = array();
          $clause_args = array();

          foreach ($element_fields as $key => $field) {

            $field_info[$feid][$key] = finder_split_field($field);

            foreach (array_values($keyword_array) as $keyword_position => $keyword) {

              // Get the info we need to add the table/field.
              $delta = ($value_combination == 'AND' && count($keyword_array) > 1) ? $keyword_position : NULL;
              $alias = finder_table_alias($feid, $field_info[$feid][$key]['table'], $field_info[$feid][$key]['field'], $delta);
              $relationship = NULL;
              if (isset($finder->settings['views']['relationships'])) {
                foreach ($finder->settings['views']['relationships'] as $rel_key => $rel_fields) {
                  if (in_array($field, $rel_fields)) {
                    $relationship = $this->view->relationship[$rel_key]->alias;
                    // This code is now handled below by the code that fixes the left join.
                    //$join = $this->view->query->get_join_data($field_info[$feid][$key]['table'], $this->view->relationship[$rel_key]->definition['base']);
                    break;
                  }
                }
              }


              // This chunk of code will fix the join directly to the left.  Should we actually be recursing through all the left tables right back to the base table?
              if (empty($relationship)) {
                $relationship = $this->view->query->base_table;
              }
              $join = $this->view->query->get_join_data($field_info[$feid][$key]['table'], $this->view->query->relationships[$relationship]['base']);
              if (!empty($join)) {
                $join = $this->view->query->adjust_join($join, $relationship);
              }

              if (!empty($join->left_table) && $join->left_table != $this->view->query->base_table) {

                // Check for the long-chain join case that we probably screwed up, and plead for assistance.
                $left_table_join_data = $this->view->query->get_join_data($join->left_table, $this->view->query->relationships[$relationship]['base']);
                if (!empty($left_table_join_data->left_table) && $left_table_join_data->left_table != $this->view->query->base_table && user_access('administer finder')) {
                  // Purposefully not enclosed in t() because this is not a typical UI string.
                  drupal_set_message("Views join configuration not supported by finder.  Please post in the <a href=\"http://drupal.org/node/add/project-issue/finder\">finder issue queue</a> and attach an export of your finder, or explain what fields you are using.  We need your help to solve this problem.", 'error');
                }

                // Table name supplied here is a combo of the table joined here, and the table we ultimately want to join.
                $join_alias = finder_table_alias($feid, $field_info[$feid][$key]['table'] . '_' . $join->left_table, $field_info[$feid][$key]['field'], $delta);
                $join_alias = $this->view->query->add_table($join->left_table, NULL, NULL, $join_alias);
                // This line below actually seems to break stuff. Issue #1248608.  But removing it breaks other stuff #1321070.
                $join->left_table = $join_alias;
              }

              $field_alias = finder_field_alias($feid, $field_info[$feid][$key]['table'], $field_info[$feid][$key]['field'], $delta);

              $join = $join ? $join : $this->view->query->get_join_data($alias, $this->view->base_table);
              $table = isset($join->table) ? $join->table : $field_info[$feid][$key]['table'];

              if ($table == $this->view->base_table) {
                 // This is KAOS.  Ve don't alias base table here.
                $alias = $this->view->base_table;
              }

              $this->view->query->add_table($field_info[$feid][$key]['table'], $relationship, $join, $alias);

              // Select the field.
              if (($mode == 'results' && $finder->settings['views']['results']) || ($mode == 'choices' && $feid == $finder_element_id)) {
                // If we're doing a custom results page or a choices list, grab the field value, and store the alias.

                $this->view->query->add_field($alias, $field_info[$feid][$key]['field'], $field_alias);
                $field_info[$feid][$key]['field_alias'][$delta] = $field_alias;

                // This if-statement also assumes that $feid == $finder_element id, but it is not needed to specify this because of the outer if-statement.
                if ($mode == 'choices') {
                  $this->view->query->add_where('finder_choices', $alias . '.' . $field_info[$feid][$key]['field'] . ' IS NOT NULL');
                }

              }

              // Add the field to where clauses.
              if ($keyword !== '' && $keyword !== NULL) {
                $placeholder = finder_placeholder($match, $table, $field_info[$feid][$key]['field']);
                $outer_key = $nesting_order ? $key : $keyword_position;
                $clauses[$outer_key][] = $alias .'.'. $field_info[$feid][$key]['field'] . $placeholder;
                $clause_args[$outer_key][] = $keyword;

                // Using same conditions as for when we add a field, because there's no point in adding the match field if not selecting the field.
                if ($mode == 'choices' && $feid == $finder_element_id) {
                  $expression = $alias .'.'. $field_info[$feid][$key]['field'] . finder_placeholder($match, $table, $field_info[$feid][$key]['field'], $keyword);
                  $this->view->query->add_field(NULL, $expression, $field_alias . '_matched');
                }

              }

            }
          }

          // Convert where clauses into sql strings and add to view with add_where().
          if (!empty($clauses)) {
            $inner_operator = $nesting_order ? $value_combination : $field_combination;
            $outer_operator = $nesting_order ? $field_combination : $value_combination;
            $inner_clauses = array();
            $inner_clause_args = array();
            foreach ($clauses as $clause) {
              $inner_clauses[] = '('. implode(' ' . $inner_operator . ' ', $clause) .')';
            }
            foreach ($clause_args as $clause_arg) {
              foreach ($clause_arg as $inner_clause_arg) {
                $inner_clause_args[] = $inner_clause_arg;
              }
            }

	          if (!$where_group_created) {
	            // Create a where group called 'finder', for element combination.
	            $element_combination = $finder->settings['advanced']['element_combination'] ? 'OR' : 'AND';
	            $this->view->query->set_where_group($element_combination, 'finder');
	            $where_group_created = TRUE;
	          }
            $this->view->query->add_where('finder', '('. implode(' ' . $outer_operator . ' ', $inner_clauses) .')', $inner_clause_args);
          }

        }
      }
    }

    // Save $field_info into the view, so we can use it in the style plugin.
    $this->view->display_handler->set_option('finder_views_field_info', $field_info);

    // Add groups.
    foreach ($groups as $group) {
      $this->view->query->add_groupby($group);
    }

  }

}