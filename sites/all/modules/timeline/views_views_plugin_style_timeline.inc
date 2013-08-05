<?php

/**
 * @file
 * Timeline views style plugin
 */

/**
 * Extending the view_plugin_style class to provide a timeline view style.
 */
class views_views_plugin_style_timeline extends views_plugin_style {

  // Basic properties that let the row style follow relationships.
  var $base_table = 'node';
  var $base_field = 'nid';

  /**
   * Set default options
   */
  function option_definition() {

    $options = parent::option_definition();
    $options['display']['contains'] = array(
      'width' => array('default' => variable_get('timeline_default_width', '100%')),
      'height' => array('default' => variable_get('timeline_default_height', '400px')),
      'appearance' => array(
        'contains' => array(
          'theme' => array('default' => 'ClassicTheme'),
          'bubble_width' => array('default' => '250'),
          'bubble_height' => array('default' => ''),
        ),
      ),
    );
    $options['bands']['contains'] = array(
      'focus' => array('default' => 'today'),
      'band1_unit' => array('default' => 'day'),
      'band2_unit' => array('default' => 'month'),
    );
    $options['misc']['contains'] = array('controls' => array('default' => FALSE), );
    $options['fields']['contains'] = array(
      'title' => array('default' => ''),
      'link' => TRUE,
      'start' => array('default' => ''),
      'end' => array('default' => ''),
      'advanced' => array(
        'contains' => array(
          'earliestStart' => '',
          'latestEnd' => '',
          'icon' => '',
          'imagecache_icon' => '',
        ),
      ),
    );
    return $options;
  }

  /**
   * Add settings for the particular timeline.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['colors'] = array(
      '#type' => 'fieldset',
      '#title' => t('Grouping colors'),
      '#collapsible' => TRUE, '#collapsed' => TRUE,
      '#description' => 'You can define the colors that are going to be used for the grouped events. Leave blank to use standard values',
    );

    $form['colors']['values'] = array(
      '#type' => 'textarea',
      '#title' => t('Color coding'),
      '#default_value' => isset($this->options['colors']['values']) ? $this->options['colors']['values'] : '',
      '#description' => t('You can override the colors for your groupings. Enter one hexadecimal RGB color value per line. Examples: #FF00FF #FF0099 #998811'),
    );

    $form['display'] = array('#type' => 'fieldset', '#title' => t('Display settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    $form['display']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => isset($this->options['display']['width']) ? $this->options['display']['width'] : variable_get('timeline_default_width', '100%'),
      '#size' => 6,
      '#maxlength' => 6,
      '#description' => t('The width of the timeline (in units of em, px or %), e.g. 600px or 90%. Leave blank to use default value.'),
    );
    $form['display']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#default_value' => isset($this->options['display']['height']) ? $this->options['display']['height'] : variable_get('timeline_default_height', '400px'),
      '#size' => 6,
      '#maxlength' => 6,
      '#description' => t('The height of the timeline (in units of em, px or %), e.g. 400px. Leave blank to use default value.'),
    );
    // Appearance settings
    ctools_include('plugins');
    $timeline_themes = ctools_get_plugins('timeline', 'themes');
    $timeline_themes_select = array('ClassicTheme' => 'Classic Theme');
    foreach ($timeline_themes as $theme_name => $theme_info) {
      $timeline_themes_select[$theme_name] = $theme_info['name'];
    }
    $form['display']['appearance'] = array('#type' => 'fieldset', '#title' => t('Appearance and themes'), '#collapsible' => TRUE, '#collapsed' => TRUE);
    $form['display']['appearance']['theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme'),
      '#default_value' => $this->options['display']['appearance']['theme'],
      '#options' => $timeline_themes_select,
      '#description' => t('Select a theme. You can create our own themes. Read documentation.'),
    );
    $form['display']['appearance']['bubble_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width of bubbles'),
      '#default_value' => $this->options['display']['appearance']['bubble_width'],
      '#size' => 6,
      '#maxlength' => 6,
      '#description' => t('The width of the bubbles in the timeline (in units of px), e.g. 250. Leave blank to use default value from theme.'),
    );
    $form['display']['appearance']['bubble_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Max height of bubbles'),
      '#default_value' => $this->options['display']['appearance']['bubble_height'],
      '#size' => 6,
      '#maxlength' => 6,
      '#description' => t('The maximum height of the bubbles in the timeline (in units of px), e.g. 400 (Scrollbar will be added for taller bubbles). Leave blank to let the bubbles scale automatically.'),
    );

    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
    }
    else {
      // initializing select arrays for field selection form elements
      $field_names = array('' => '--');
      $date_field_names = array('' => '--');
      $iconimage_field_names = array('' => '--');

      // load all plugins for date and image field supports. provided by this or other modules
      ctools_include('plugins');
      $date_sources = ctools_get_plugins('timeline', 'date_sources');
      $image_sources = ctools_get_plugins('timeline', 'image_sources');

      // go through all available fields' handlers to check support for certain functionality
      foreach ($handlers as $field => $handler) {

        // obtain nice name of field for the ui
        $field_names[$field] = $handler->ui_name();
        if ($label = $handler->label()) {
          $field_names[$field] .= " (\"$label\")";
        }

        // check if this field is supported as a date source
        foreach ($date_sources as $date_source) {
          if (get_class($handler) == $date_source['handler_name']) {
            $date_field_names[$field] = $field_names[$field];
          }
        }

        // check if this field is supported as an image source
        foreach ($image_sources as $image_source) {
          if (get_class($handler) == $image_source['handler_name'] || $handler->content_field['widget']['module'] == 'imagefield') {
            $iconimage_field_names[$field] = $field_names[$field];
          }
        }
      }

      $form['fields'] = array(
        '#type' => 'fieldset',
        '#title' => 'Field usage',
        '#description' => t('Select the fields that contain the title, start time and end time of each item.'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['fields']['title'] = array(
        '#type' => 'select',
        '#title' => 'Title',
        '#options' => $field_names,
        '#default_value' => $this->options['fields']['title'],
        '#required' => TRUE,
      );

      $form['fields']['link'] = array(
        '#type' => 'checkbox',
        '#title' => 'Link to node',
        '#default_value' => $this->options['fields']['link'],
        '#required' => FALSE,
      );

      $form['fields']['start'] = array(
        '#type' => 'select',
        '#title' => 'Start',
        '#options' => $date_field_names,
        '#default_value' => $this->options['fields']['start'],
        '#required' => TRUE,
      );

      $form['fields']['end'] = array(
        '#type' => 'select',
        '#title' => 'End',
        '#options' => $date_field_names,
        '#default_value' => $this->options['fields']['end'],
      );

      $form['fields']['advanced'] = array(
        '#type' => 'fieldset',
        '#title' => t('Advanced visualization settings'),
        '#collapsible' => TRUE, '#collapsed' => TRUE,
        '#description' => t('For more information on these fields visit the <a href="@url">advanced documentation</a>', array('@url' => url('http://drupal.org/node/561738'))),
      );
      $form['fields']['advanced']['earliestStart'] = array(
        '#type' => 'select',
        '#title' => t('Earliest start'),
        '#options' => $date_field_names,
        '#default_value' => $this->options['fields']['advanced']['earliestStart'],
      );
      $form['fields']['advanced']['latestEnd'] = array(
        '#type' => 'select',
        '#title' => t('Latest end'),
        '#options' => $date_field_names,
        '#default_value' => $this->options['fields']['advanced']['latestEnd'],
      );
      $form['fields']['advanced']['icon'] = array(
        '#type' => 'select',
        '#title' => t('Icon'),
        '#options' => $iconimage_field_names,
        '#default_value' => $this->options['fields']['advanced']['icon'],
      );
      if (module_exists('imagecache')) {
        $imagecache_presets = array('' => t('Orginal image size'));
        foreach (imagecache_presets() as $preset) {
          $imagecache_presets[$preset['presetname']] = $preset['presetname'];
        }
        $form['fields']['advanced']['imagecache_icon'] = array(
          '#type' => 'select',
          '#title' => t('Imagecache preset for Icon'),
          '#options' => $imagecache_presets,
          '#default_value' => $this->options['fields']['advanced']['imagecache_icon'],
        );
      }
    }

    // Band/interval settings
    $form['bands'] = array('#type' => 'fieldset', '#title' => t('Band/interval settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    $intervals = array('second' => t('Second'), 'minute' => t('Minute'), 'hour' => t('Hour'), 'day' => t('Day'), 'week' => t('Week'), 'month' => t('Month'), 'year' => t('Year'), 'decade' => t('Decade'), 'century' => t('Century'), 'millennium' => t('Millennium'));
    $interval_widths = array(10 => 10, 25 => 25, 50 => 50, 75 => 75, 100 => 100, 150 => 150, 200 => 200, 300 => 300, 400 => 400, 500 => 500, 1000 => 1000);

    $form['bands']['band1_unit'] = array(
      '#type' => 'select',
      '#title' => t('Main band interval unit'),
      '#default_value' => $this->options['bands']['band1_unit'],
      '#options' => $intervals,
      '#description' => t(''),
    );
    $form['bands']['band1_unit_width'] = array(
      '#type' => 'select',
      '#title' => t('Main band interval unit width'),
      '#default_value' => $this->options['bands']['band1_unit_width'],
      '#options' => $interval_widths,
      '#description' => t('Select the width, in pixels, for the interval.'),
    );
    $form['bands']['band2_unit'] = array(
      '#type' => 'select',
      '#title' => t('Summary band interval unit'),
      '#default_value' => $this->options['bands']['band2_unit'],
      '#options' => array_merge(array('' => t('Not displayed')), $intervals),
      '#description' => t(''),
    );
    $form['bands']['band2_unit_width'] = array(
      '#type' => 'select',
      '#title' => t('Summary band interval unit width'),
      '#default_value' => $this->options['bands']['band2_unit_width'],
      '#options' => $interval_widths,
      '#description' => t('Select the width, in pixels, for the interval.'),
    );
    $form['bands']['focus'] = array(
      '#type' => 'select',
      '#title' => t('Initial date focus'),
      '#options' => array('today' => t('The current date'), 'first' => t('First event in timeline'), 'last' => t('Last event in timeline'), 'middle' => t('Middle Event'), 'median' => t('Median'), 'custom' => t('Custom')),
      '#default_value' => $this->options['bands']['focus'],
      '#description' => t('Determines which event or date the timeline view should initially be centered on after it has loaded. Leave blank to use default value.'),
    );
    $form['bands']['focus_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom initial date focus'),
      '#default_value' => isset($this->options['bands']['focus_custom']) ? $this->options['bands']['focus_custom'] : '',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-style-options-bands-focus' => array('custom')),
      '#description' => t('Please enter a custom initial date to focus the timeline on.') .' '. t('Format: %time.', array('%time' => format_date(time(), 'custom', 'Y-m-d H:i:s O'))),
    );
/* js is unstable within timeline. maybe next version the simile grup is going to fix this. - fd
    $form['limit'] = array('#type' => 'fieldset', '#title' => t('Limit timeline bands'), '#collapsible' => TRUE, '#collapsed' => TRUE);
    $form['limit']['start'] = array(
      '#type' => 'select',
      '#title' => t('Timeline start date'),
      '#default_value' => isset($this->options['limit']['start']) ? $this->options['limit']['start'] : '',
      '#options' => array('endless' => 'Endless', 'first' => t('First event in timeline'), 'last' => t('Last event in timeline'), 'today' => t('The current date'), 'custom' => t('Custom')),
      '#description' => t('Limit timeline bands to start at this defined date'),
    );

    $form['limit']['custom_start'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom start date'),
      '#default_value' => isset($this->options['limit']['custom_start']) ? $this->options['limit']['custom_start'] : '',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-style-options-limit-start' => array('custom')),
      '#description' => t('Please enter a custom end date for the timeline.') .' '. t('Format: %time.', array('%time' => format_date(time(), 'custom', 'Y-m-d H:i:s O'))),
    );

    $form['limit']['end'] = array(
      '#type' => 'select',
      '#title' => t('Timeline end date'),
      '#default_value' => isset($this->options['limit']['end']) ? $this->options['limit']['end'] : '',
      '#options' => array('endless' => 'Endless', 'first' => t('First event in timeline'), 'last' => t('Last event in timeline'), 'today' => t('The current date'), 'custom' => t('Custom')),
      '#description' => t('Limit timeline bands to end at this defined date'),
    );
    $form['limit']['custom_end'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom end date'),
      '#default_value' => isset($this->options['limit']['custom_end']) ? $this->options['limit']['custom_end'] : '',
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-style-options-limit-end' => array('custom')),
      '#description' => t('Please enter a custom end date for the timeline.') .' '. t('Format: %time.', array('%time' => format_date(time(), 'custom', 'Y-m-d H:i:s O'))),
    );

/* DEPRECATED: Controls -fd
    // Miscellanous settings
    $form['misc'] = array('#type' => 'fieldset', '#title' => t('TODO: Miscellaneous settings'), '#collapsible' => TRUE, '#collapsed' => TRUE);
    $form['misc']['controls'] = array(
      '#type' => 'radios',
      '#title' => t('Enable controls'),
      '#default_value' => $this->options['misc']['controls'],
      '#options' => array(FALSE => t('Disabled'), TRUE => t('Enabled')),
      '#description' => t('Whether to display filtering and highlighting controls under the timeline. Not recommended for block mode.'),
    );
    */
  }

  // Copied from views_plugin_row because not implemented in views_plugin_style to suport $base_table and $_base_field with relationships. -fd
  function query() {
    if (isset($this->base_table)) {
      if (isset($this->options['relationship']) && isset($this->view->relationship[$this->options['relationship']])) {
        $relationship = $this->view->relationship[$this->options['relationship']];
        $this->field_alias = $this->view->query->add_field($relationship->alias, $this->base_field);
      }
      else {
        $this->field_alias = $this->view->query->add_field($this->base_table, $this->base_field);
      }
    }
  }

  /**
   * Render the timeline style.
   * Data for the simile timeline is getting collected within the view
   */
  function render() {
    static $delta = 0;

    // Group the rows according to the grouping field, if specified.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Build data for timeline
    ctools_include('Timeline.class', 'timeline', 'includes');
    ctools_include('TimelineEvent.class', 'timeline', 'includes');
    $timeline = new Timeline($this->view, $sets);

    // Allow other modules to alter the data array.
    drupal_alter('timeline_data', $timeline->data);

    // Convert data to json and push them to drupal's js behaviors
    $data = array();
    $data['timeline'][$delta] = array($timeline->data['id'] . '_' . $delta => $timeline->data);
    drupal_add_js($data, 'setting');

    // Show debug textarea if enabled
    if (user_access('administer views') && variable_get('timeline_debug', FALSE)) {
      $this->options['debug'] = $timeline->debug();
    }

    // options that are used for template preprocessor
    $this->options['timeline'] = array(
      'class' => $this->view->name .'_'. $delta,
      'width' => _timeline_to_dim($this->view->style_options['display']['width']),
      'height' => _timeline_to_dim($this->view->style_options['display']['height']),
      'align' => '',
      'data_count' => count($timeline->data['events']['events']),
      'controls' => FALSE, // $this->view->style_options['misc']['controls'],
      'theme' => $this->options['display']['appearance']['theme'],
    );
    $delta++;
    return theme($this->theme_functions(), $this->view, $this->options);
  }
}
