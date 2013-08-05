<?php

/**
 * @file
 * Provides an Better Exposed Filters exposed form plugin for View 3.x.
 */
class better_exposed_filters_exposed_form_plugin extends views_plugin_exposed_form_basic {

  function summary_title() {
    return t('Better Exposed Filters');
  }

  function option_definition() {
    $options = parent::option_definition();

    // Add Better Exposed Filters options to those saved by Views
    $options['bef'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $bef_options = array();

    /*
     * Add options for exposed sorts
     */
    $exposed = FALSE;
    foreach ($this->display->handler->get_handlers('sort') as $label => $sort) {
      if ($sort->options['exposed']) {
        $exposed = TRUE;
        break;
      }
    }
    if ($exposed) {
      $bef_options['sort']['bef_format'] = array(
        '#type' => 'select',
        '#title' => t('Display exposed sort options as'),
        '#default_value' => $this->options['bef']['sort']['bef_format'],
        '#options' => array(
          'default' => t('Default select list'),
          'bef' => t('Radio Buttons'),
          'bef_links' => t('Links'),
        ),
        '#description' => t('Select a format for the exposed sort options.'),
      );
      $bef_options['sort']['combine'] = array(
        '#type' => 'checkbox',
        '#title' => t('Combine sort order with sort by'),
        '#default_value' => $this->options['bef']['sort']['combine'],
        '#description' => t('
          Combines the sort by options and order (ascending or decending) into a single list.  Use this to
          display "Option1 (ascending)", "Option1 (descending)", "Option2 (ascending)", "Option2 (descending)"
          in a single form element.
        '),
      );
      $bef_options['sort']['reset'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include a "Reset sort" option'),
        '#default_value' => $this->options['bef']['sort']['reset'],
        '#description' => t('Adds a "Reset sort" link; Views will use the default sort order.'),
      );
      $bef_options['sort']['reset_label'] = array(
        '#type' => 'textfield',
        '#title' => t('"Reset sort" label'),
        '#default_value' => $this->options['bef']['sort']['reset_label'],
        '#description' => t('Text to use if the above option is checked'),
      );
    }

    /*
     * Add options for exposed pager
     */
    if ($this->display->display_options['pager'] && $this->display->display_options['pager']['options']['expose']['items_per_page']) {
      $bef_options['pager']['bef_format'] = array(
        '#type' => 'select',
        '#title' => t('Display exposed pager options as'),
        '#default_value' => $this->options['bef']['pager']['bef_format'],
        '#options' => array(
          'default' => t('Default select list'),
          'bef' => t('Radio Buttons'),
          'bef_links' => t('Links'),
        ),
        '#description' => t('Select a format for the exposed pager options.'),
      );
    }

    // Only add the description text once -- it was getting a little long to be added to
    // each filter
    $bef_filter_intro = FALSE;

    // Go through each filter and add the same options we used to add in hook_form_alter()
    foreach ($this->display->handler->get_handlers('filter') as $label => $filter) {
      if (!$filter->options['exposed']) {
        continue;
      }

      // If we're adding BEF filter options, add an intro to explain what's going on
      if (!$bef_filter_intro) {
        $bef_options['bef_intro'] = array(
          '#value' => '<div class="form-item"><h3>'
            . t('Exposed Filter Settings')
            . '</h3><p>'
            . t('Select a format and additional options for each exposed filter.')
            . '</p><p>'
            . t('If <em>Force single</em> option is checked in the <em>Configure
                filter</em> form then radio buttons will be used, otherwise
                checkboxes. The "Nested" option allows hierarchical taxonomy
                filters will be rendered as nested, unordered lists. The "Hidden"
                option is generally used for multi-step filters.')
            . '</p><p>'
            . t('Additional options are available for each filter by clicking the
                <em>More options</em> link.')
            . '</p></div>',
        );
        $bef_filter_intro = TRUE;
      }

      // Is this a type of field we can't tweak?  (eg: text fields)
      if (('=' == $filter->operator) && !is_bool($filter->value)) {
        // others?
        continue;
      }

      // Main BEF option: default/checkboxes/hidden
      $bef_options[$label]['bef_format'] = array(
        '#type' => 'select',
        '#title' => t('Display "@label" exposed filter as', array('@label' => $filter->options['expose']['label'])),
        '#default_value' => $this->options['bef'][$label]['bef_format'],
        '#options' => array(
          'default' => t('Default select list'),
          'bef' => t('Checkboxes/Radio Buttons'),
          'bef_ul' => t('Nested Checkboxes/Radio Buttons'),
          'bef_hidden' => t('Hidden'),
        ),
      );

      // Fieldset to keep the UI from getting out of hand
      $bef_options[$label]['more_options'] = array(
        '#type' => 'fieldset',
        '#title' => t('More options for "@label"', array('@label' => $filter->options['expose']['label'])),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      // Select all checkbox
      $bef_options[$label]['more_options']['bef_select_all_none'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add select all/none links'),
        '#default_value' => $this->options['bef'][$label]['more_options']['bef_select_all_none'],
        '#disabled' => $filter->options['expose']['single'],
        '#description' => t(
          'Add a "Select All/None" link when rendering the exposed filter using
            checkboxes. If this option is disabled, edit the filter and uncheck
            "Force single". NOTE: The link is built at page load, so it will not appear
            in the "Live Preview" which is loaded dynamically.'
        ),
      );

      // Put filter in collapsible fieldset option
      $bef_options[$label]['more_options']['bef_collapsible'] = array(
        '#type' => 'checkbox',
        '#title' => t('Make this filter collapsible'),
        '#default_value' => $this->options['bef'][$label]['more_options']['bef_collapsible'],
        '#description' => t(
          'Puts this filter in a collapsible fieldset'
        ),
      );

      // Build a description option form element
      $bef_options[$label]['more_options']['bef_filter_description'] = array(
        '#type' => 'textarea',
        '#title' => t('Description'),
        '#default_value' => $this->options['bef'][$label]['more_options']['bef_filter_description'],
        '#description' => t('Adds descriptive text to the exposed filter.  This is usually
                              rendered in smaller print under the label or the options.'),
      );
    }             // foreach ($filters as $filter) {

    // Add BEF form elements to the exposed form options form
    $form['bef'] = $bef_options;
  }

  /*
   * Tweak the exposed filter form to show Better Exposed Filter options.
   */
  function exposed_form_alter(&$form, &$form_state) {
    parent::exposed_form_alter($form, $form_state);

    // If we have no visible elements, we don't show the Apply button.
    $show_apply = FALSE;

    /*
     * Handle exposed sort elements. Sort options can be removed after BEF
     * settings are specified, so we sanity check here.
     */
    if (isset($this->options['bef']['sort'])
      && !empty($form['sort_by'])
      && !empty($form['sort_order'])) {

      $show_apply = TRUE;

      // Check for combined sort_by and sort_order
      if ($this->options['bef']['sort']['combine']) {
        // Combine sort_by and sort_order into a single element
        $options = array();

        // Add reset sort option at the top of the list
        if ($this->options['bef']['sort']['reset']) {
          $options[' '] = t($this->options['bef']['sort']['reset_label']);
        }

        $selected = '';
        foreach ($form['sort_by']['#options'] as $by_key => $by_val) {
          foreach ($form['sort_order']['#options'] as $order_key => $order_val) {
            // Use a space to separate the two keys, we'll unpack them in our submit handler
            $options["$by_key $order_key"] = "$by_val $order_val";
          }
        }

        $form['sort_bef_combine'] = array(
          '#type' => 'radios',
          '#options' => $options,
          '#title' => $form['sort_by']['#title'],
        );

        // Handle display-specific details
        switch ($this->options['bef']['sort']['bef_format']) {
          case 'bef':
            $form['sort_bef_combine']['#prefix'] = '<div class="bef-sort-combined bef-select-as-radios">';
            $form['sort_bef_combine']['#suffix'] = '</div>';
            break;

          case 'bef_links':
            $form['sort_bef_combine']['#theme'] = 'select_as_links';
            unset($form['sort_bef_combine']['#title']);
            break;

          case 'default':
            $form['sort_bef_combine']['#type'] = 'select';
            break;
        }

        // Add our submit routine to process
        $form['#submit'][] = 'bef_sort_combine_submit';

        // Remove the existing sort_by and sort_order elements
        unset($form['sort_by']);
        unset($form['sort_order']);
      }                 // if ($this->options['bef']['sort']['combine']) {
      else {
        // Leave sort_by and sort_order as separate elements
        if ('bef' == $this->options['bef']['sort']['bef_format']) {
          $form['sort_by']['#type'] = 'radios';
          $form['sort_by']['#process'] = array('expand_radios', 'views_process_dependency');
          $form['sort_by']['#prefix'] = '<div class="bef-sortby bef-select-as-radios">';
          $form['sort_by']['#suffix'] = '</div>';

          $form['sort_order']['#type'] = 'radios';
          $form['sort_order']['#process'] = array('expand_radios', 'views_process_dependency');
          $form['sort_order']['#prefix'] = '<div class="bef-sortorder bef-select-as-radios">';
          $form['sort_order']['#suffix'] = '</div>';
        }
        else if ('bef_links' == $this->options['bef']['sort']['bef_format']) {
          $form['sort_by']['#theme'] = 'select_as_links';
          $form['sort_order']['#theme'] = 'select_as_links';
        }

        // Add reset sort option if selected
        if ($this->options['bef']['sort']['reset']) {
          array_unshift($form['sort_by']['#options'], $this->options['bef']['sort']['reset_label']);
        }
      }                 // if ($this->options['bef']['sort']['combine']) { ... } else {
    }                   // if (isset($this->options['bef']['sort'])) {

    /*
     * Handle exposed pager elements
     */
    if (isset($this->options['bef']['pager'])) {
      $show_apply = TRUE;

      switch($this->options['bef']['pager']['bef_format']) {
        case 'bef':
          $form['items_per_page']['#type'] = 'radios';
          $form['items_per_page']['#process'] = array('expand_radios', 'views_process_dependency');
          $form['items_per_page']['#prefix'] = '<div class="bef-sortby bef-select-as-radios">';
          $form['items_per_page']['#suffix'] = '</div>';
          break;

        case 'bef_links':
          if (count($form['items_per_page']['#options']) > 1) {
            $form['items_per_page']['#theme'] = 'select_as_links';
            $form['items_per_page']['#items_per_page'] = max($form['items_per_page']['#default_value'], key($form['items_per_page']['#options']));
          }
        break;
      }
    }

    // Shorthand for all filters in this view
    $filters = $form_state['view']->display_handler->handlers['filter'];

    // Go through each saved option looking for Better Exposed Filter settings
    foreach ($filters as $label => $filter) {

      // Sanity check: Ensure this filter is an exposed filter
      if (!$filter->options['exposed']) {
        continue;
      }

      // Grab the options for this filter
      $options = array();
      if (empty($this->options['bef'][$label])) {
        $show_apply = TRUE;
        continue;
      }
      else {
        $options = $this->options['bef'][$label];
      }

      // Form element is designated by the element ID which is user-configurable
      $field_id = $form['#info']["filter-$label"]['value'];

      // Add a description to the exposed filter
      if (!empty($options['more_options']['bef_filter_description'])) {
        $form[$field_id]['#description'] = $options['more_options']['bef_filter_description'];
      }

      switch ($options['bef_format']) {
        case 'bef_links':
          $show_apply = TRUE;
          $form[$field_id]['#theme'] = 'select_as_links';
          break;

        case 'bef_ul':
          $show_apply = TRUE;
          $form[$field_id]['#bef_nested'] = TRUE;
          // Intentionally falling through to case 'bef':

        case 'bef':
          $show_apply = TRUE;
          if (empty($form[$field_id]['#multiple'])) {
            // Single-select -- display as radio buttons
            $form[$field_id]['#type'] = 'radios';
            $form[$field_id]['#process'] = array('expand_radios', 'views_process_dependency');

            // Clean up objects from the options array (happens for taxonomy-based filters)
            $opts = $form[$field_id]['#options'];
            $form[$field_id]['#options'] = array();
            foreach ($opts as $index => $opt) {
              if (is_object($opt)) {
                foreach ($opt->option as $key => $val) {
                  $form[$field_id]['#options'][$key] = $val;
                }
              }
              else {
                $form[$field_id]['#options'][$index] = $opt;
              }
            }

            if (isset($form[$field_id]['#options']['All'])) {
              // @TODO: The terms 'All' and 'Any' are customizable in Views
              if (!$filters[$label]->options['expose']['optional']) {
                // Some third-party filter handlers still add the "Any" option even if this is not
                // an optional filter.  Zap it here if they do.
                unset($form[$field_id]['#options']['All']);
              }
              else {
                // Otherwise, make sure the "Any" text is clean
                $form[$field_id]['#options']['All'] = check_plain($form[$field_id]['#options']['All']);
              }
            }

            // Render as radio buttons or radio buttons in a collapsible fieldset
            if (!empty($options['more_options']['bef_collapsible'])) {
              // Use the option label for the title of the fieldset
              $form[$field_id]['#title'] = $form['#info']["filter-$label"]['label'];
              unset($form['#info']["filter-$label"]['label']);

              // Pass the description and title along in a way such that it doesn't get rendered as part of
              // the exposed form widget.  We'll render them as part of the fieldset.
              $form[$field_id]['#bef_description'] = $form[$field_id]['#description'];
              unset($form[$field_id]['#description']);
              $form[$field_id]['#bef_title'] = $form[$field_id]['#title'];
              unset($form[$field_id]['#title']);

              // Take care of adding the fieldset in the theme layer
              $form[$field_id]['#theme'] = 'select_as_radios_fieldset';
            }
            else {
              // Render select element as radio buttons
              $form[$field_id]['#theme'] = 'select_as_radios';
            }
          }           // if (empty($form[$field_id]['#multiple'])) {
          else {
            // Render as checkboxes or checkboxes enclosed in a collapsible fieldset
            if (!empty($options['more_options']['bef_collapsible'])) {
              // Use exposed filter widget label as legend for this fieldset
              $form[$field_id]['#title'] = $form['#info']["filter-$label"]['label'];
              unset($form['#info']["filter-$label"]['label']);

              $form[$field_id]['#theme'] = 'select_as_checkboxes_fieldset';
            }
            else {
              $form[$field_id]['#theme'] = 'select_as_checkboxes';
            }

            // Add BEF's JavaScript to the mix to handle select all/none functionality
            drupal_add_js(drupal_get_path('module', 'better_exposed_filters') .'/better_exposed_filters.js');

            // Add select all/none functionality to this filter.
            if ($options['more_options']['bef_select_all_none']) {
              if (!isset($form[$field_id]['#attributes']['class'])) {
                $form[$field_id]['#attributes']['class'] = 'bef-select-all-none';
              }
              else {
                $form[$field_id]['#attributes']['class'] .= ' bef-select-all-none';
              }
            }
          }           // if (empty($form[$field_id]['#multiple'])) { ... } else {
          break;      // case 'bef':

        case 'bef_hidden':
          $form['#info']["filter-$field_id"]['label'] = '';     // Hide the label
          $form["filter-$field_id"]['label'] = '';
          if (empty($form[$field_id]['#multiple'])) {
            $form[$field_id]['#type'] = 'hidden';
          }
          else {
            $form[$field_id]['#theme'] = 'select_as_hidden';
          }
          break;

        case 'default':
          $show_apply = TRUE;
          break;

      }     // switch ($options['bef_format'])
    }       // foreach ($this->options['bef']...)

    // If our form has no visible filters, hide the submit button.
    $form['submit']['#access'] = $show_apply;

  }         // function exposed_form_alter(...)
}