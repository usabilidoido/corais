<?php

/**
 * @file
 * Handles the admin page for the configuration of the sticky_notes module.
 */

/**
 * Formbuilder for the sticky notes admin form
 *
 * @param array $form_state
 *
 * @return array
 *  Form array
 *
 * @author berliner
 * @see sticky_notes_admin_global_form_validate()
 * @see sticky_notes_admin_global_form_submit()
 * @ingroup forms
 */

function sticky_notes_admin_global_form($form_state) {
  
  // add admin specific javascript
  drupal_add_js(drupal_get_path('module', 'sticky_notes') . '/js/sticky_notes.admin.js');
  
  $form = array();

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
  );

  $form['basic']['default_container_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Default container selector'),
    '#description' => t('If you have a centered layout you should change this to your content containers CSS ID. A possible value could be "#wrapper". If you are not sure, look in the HTML source of your frontpage and identify the element that contains your content area. All notes will be positioned relative to this element. Changing this after having added notes will change the former position of those notes on the document, so handle with care! This selector is used when no other selector has been specified in the <a href="@theme-settings">theme configuration</a>. If you use multiple themes for your site you should set this setting for each theme individually.', array('@theme-settings' => url('admin/build/themes/settings/' . sticky_notes_get_current_theme()))),
    '#default_value' => variable_get('sticky_notes_default_container_selector', STICKY_NOTES_DEFAULT_CONTAINER_SELECTOR),
  );

  $form['basic']['minimal_zindex'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimal z-index for sticky notes'),
    '#description' => t('If you encounter problems with the display of sticky notes, like partially hidden notes, you can try to increase this value.'),
    '#default_value' => variable_get('sticky_notes_minimal_zindex', STICKY_NOTES_DEFAULT_ZINDEX),
    '#size' => 5,
  );
  
  $form['basic']['hide_on_page_load'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always hide notes on page load'),
    '#description' => t('Check this option if you want sticky notes to be hidden when a page is loaded. Note that for users with sufficient permissions there will be a link in the info box that permits to show the notes, so this can not be used to generally always hide all notes.'),
    '#default_value' => variable_get('sticky_notes_page_visibility_hide_on_load', STICKY_NOTES_DEFAULT_PAGE_VISIBILITY_HIDE_ON_LOAD),
  );
  
  $form['basic']['auto_size'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic size adjustment in exposé view'),
    '#description' => t('Check this if you want that the sticky notes automatically change size to fit on your screen when you go into exposé mode.'),
    '#default_value' => variable_get('sticky_notes_auto_size', STICKY_NOTES_DEFAULT_AUTO_SIZE),
  );

  $form['basic']['allow_node_operations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow default Drupal node operations'),
    '#description' => t('Check this option if you want to allow access to the default drupal node edit forms (e.g. node/add/sticky_notes).'),
    '#default_value' => variable_get('sticky_notes_allow_node_operations', STICKY_NOTES_DEFAULT_ALLOW_NODE_OPERATIONS),
  );
  
  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#description' => t('Here you can change the way sticky notes will be displayed.')
  );
  
  $form['appearance']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => variable_get('sticky_notes_width', STICKY_NOTES_DEFAULT_WIDTH),
    '#size' => 5,
    '#description' => t('The width in pixels. Leave empty to use default value (154px).'),
  );
  
  $form['appearance']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => variable_get('sticky_notes_height', STICKY_NOTES_DEFAULT_HEIGHT),
    '#size' => 5,
    '#description' => t('The height in pixels. Leave empty to use default value (154px).'),
  );
  
  $form['appearance']['note_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Note color'),
    '#default_value' => variable_get('sticky_notes_note_color', STICKY_NOTES_DEFAULT_NOTE_COLOR),
    '#size' => 8,
    '#description' => t('A color code as you would set it in a stylesheet (e.g. #FDFB8C, red, ...). Leave empty to use system default.'),
  );
  
  $form['appearance']['text_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color'),
    '#default_value' => variable_get('sticky_notes_text_color', STICKY_NOTES_DEFAULT_TEXT_COLOR),
    '#size' => 8,
    '#description' => t('A color code as you would set it in a stylesheet (e.g. #FDFB8C, red, ...). Leave empty to use system default.'),
  );
  
  $form['appearance']['character_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Character limit'),
    '#default_value' => variable_get('sticky_notes_character_limit', STICKY_NOTES_DEFAULT_CHARACTER_LIMIT),
    '#size' => 5,
    '#description' => t('The maximum number of characters that a user can enter into a sticky note. Set to 0 for no limitation. Note that changing this setting after notes have already been created might prevent users from saving existing notes if the original note contains less that the specified amount of characters.'),
  );
    
  $form['appearance']['resizable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Resizable notes'),
    '#description' => t('Check this, if you want that users can change the size of individual notes. Note that deactivating this option does not reset the size of already modified notes to the default size.'),
    '#default_value' => variable_get('sticky_notes_resizable', STICKY_NOTES_DEFAULT_RESIZABLE),
  );
  
  $form['appearance']['reset_size'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply the given dimensions to all notes immediately.'),
    '#description' => t('ATTENTION: This action can not be undone!'),
    '#default_value' => FALSE,
  );
  
  $form['info_box'] = array(
    '#type' => 'fieldset',
    '#title' => t('Info box'),
    '#description' => t('The info box provides a quick way to access sticky notes. It will be displayed in one of the four corners of your screen and contains links to add and hide the sticky notes or to show an exposé of all notes on the current page.')
  );

  $form['info_box']['info_box_corner'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#options' => array(
      STICKY_NOTES_INFO_BOX_CORNER_TOP_LEFT => t('top left'),
      STICKY_NOTES_INFO_BOX_CORNER_TOP_RIGHT => t('top right'),
      STICKY_NOTES_INFO_BOX_CORNER_BOTTOM_LEFT => t('bottom left'),
      STICKY_NOTES_INFO_BOX_CORNER_BOTTOM_RIGHT => t('bottom right'),
    ),
    '#description' => t('Where to show the info box.'),
    '#default_value' => variable_get('sticky_notes_info_box_corner', STICKY_NOTES_DEFAULT_INFO_BOX_CORNER),
  );

  $form['info_box']['info_box_hide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically hide the info box'),
    '#description' => t('Check this, if you want that the info box will be hidden until you move your mouse in the selected corner.'),
    '#default_value' => variable_get('sticky_notes_info_box_hide', STICKY_NOTES_DEFAULT_INFO_BOX_HIDE),
  );
  
  $form['info_box']['toggle_visibility_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide hide / show buttons'),
    '#description' => t('Check this if you want that users can hide / show all notes at once.'),
    '#default_value' => variable_get('sticky_notes_toggle_visibility_state', STICKY_NOTES_DEFAULT_TOGGLE_VISIBILITY_STATE),
  );
  
  $form['info_box']['visibility_state_memory'] = array(
    '#type' => 'textfield',
    '#title' => t('Timespan of visibility memory in days'),
    '#description' => t('The number of days that the visibility state should be remembered for each user so that notes remain hidden / visible while the user visits your site.'),
    '#default_value' => variable_get('sticky_notes_visibility_state_memory', STICKY_NOTES_DEFAULT_VISIBILITY_STATE_MEMORY_DAYS),
    '#size' => 5,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;

}

/**
 * Implementation of hook_validate().
 *
 * @author berliner
 * @see sticky_notes_admin_global_form()
 * @see sticky_notes_admin_global_form_submit()
 */

function sticky_notes_admin_global_form_validate($form, &$form_state) {

  $values = $form_state['values'];
  if (!is_numeric($values['minimal_zindex'])) {
    form_set_error('minimal_zindex', t('You must specify an integer value.'));
  }
  
  if (!empty($values['width']) && (!is_numeric($values['width']) || $values['width'] < 0)) {
    form_set_error('width', t('You must specify a positive integer value or nothing for default width.'));
  }
  
  if (!empty($values['height']) && (!is_numeric($values['height']) || $values['height'] < 0)) {
    form_set_error('height', t('You must specify a positive integer value or nothing for default height.'));
  }
  
  if (!is_numeric($values['character_limit']) || $values['character_limit'] < 0) {
    form_set_error('character_limit', t('You must specify a positive integer value or 0 for unlimited character length.'));
  }
  
  if (!is_numeric($values['visibility_state_memory']) || $values['visibility_state_memory'] <= 0) {
    form_set_error('visibility_state_memory', t('You must specify a positive integer value for the visibility state memory.'));
  }

}

/**
 * Implementation of hook_submit().
 *
 * @author berliner
 * @see sticky_notes_admin_global_form()
 * @see sticky_notes_admin_global_form_validate()
 */

function sticky_notes_admin_global_form_submit($form, &$form_state) {

  $values = $form_state['values'];
  variable_set('sticky_notes_default_container_selector', $values['default_container_selector']);
  variable_set('sticky_notes_minimal_zindex', $values['minimal_zindex']);
  variable_set('sticky_notes_auto_size', $values['auto_size']);
  variable_set('sticky_notes_allow_node_operations', $values['allow_node_operations']);
  variable_set('sticky_notes_page_visibility_hide_on_load', $values['hide_on_page_load']);
  variable_set('sticky_notes_width', $values['width']);
  variable_set('sticky_notes_height', $values['height']);
  variable_set('sticky_notes_note_color', $values['note_color']);
  variable_set('sticky_notes_text_color', $values['text_color']);
  variable_set('sticky_notes_character_limit', $values['character_limit']);
  variable_set('sticky_notes_resizable', $values['resizable']);
  variable_set('sticky_notes_info_box_corner', $values['info_box_corner']);
  variable_set('sticky_notes_info_box_hide', $values['info_box_hide']);
  variable_set('sticky_notes_toggle_visibility_state', $values['toggle_visibility_state']);
  variable_set('sticky_notes_visibility_state_memory', $values['visibility_state_memory']);
  
  drupal_set_message(t('Your settings have been saved.'));
  
  if ($values['reset_size']) {
    $result = db_query("UPDATE {node_sticky_note} SET width = 0, height = 0 WHERE 1 = 1");
    if ($result) {
      drupal_set_message(t('All notes have been resized to the default settings.'));
    }
  }
  
}

/**
 * Implementation of hook_form().
 *
 * @param array $form_state 
 * @return array
 * @author berliner
 */

function sticky_notes_admin_priorities_form($form_state) {
  
  // add admin specific javascript
  drupal_add_js(drupal_get_path('module', 'sticky_notes') . '/js/sticky_notes.admin.js');
  
  if (isset($form_state['post']['priority'])) {
    $post = $form_state['post']['priority'];
  }
  
  $form = array();
  $form['#tree'] = TRUE;
  
  $form['priority'] = array(
    '#type' => 'fieldset',
    '#title' => t('Priority level'),
    '#description' => t('You can define up to !max different priority levels for sticky notes. For each level you can chose a distinct colored pin. Note that only priority levels with a name will be accessible for notes. Levels with an empty name won\'t show up in the priority select boxes on the sticky note forms.', array('!max' => STICKY_NOTES_PRIORITY_LEVEL_MAX_COUNT)),
  );
  
  $colors = array(0 => t('None')) + sticky_notes_get_priority_colors();
  $current_priority = (int) variable_get('sticky_notes_default_priority_level', STICKY_NOTES_DEFAULT_PRIORITY);
  
  for ((int) $i = 0; $i < STICKY_NOTES_PRIORITY_LEVEL_MAX_COUNT; $i++) {
    
    $form['priority']['default'][$i] = array(
      '#type' => 'radio',
      '#return_value' => $i,
      '#name' => 'priority[default]',
      '#parent' => array('priority'),
      '#default_value' => isset($post['default']) ? $post['default'] : $current_priority,
    );
    
    $form['priority']['name'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#size' => 20,
      '#default_value' => isset($post['name'][$i]) ? $post['name'][$i] : variable_get('sticky_notes_priority_name_' . $i, sticky_notes_get_default_priority_name($i)),
      '#description' => t('The name as it appears in the select box.')
    );
    
    $form['priority']['color'][$i] = array(
      '#type' => 'select',
      '#title' => t('Pin color'),
      '#options' => $colors,
      '#default_value' => isset($post['color'][$i]) ? $post['color'][$i] : variable_get('sticky_notes_priority_color_' . $i, sticky_notes_get_default_priority_color($i)),
      '#description' => t('The color of the pin. If you set this to none then no pin is displayed.')
    );
    
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
  
}

/**
 * Implementation of hook_form_submit().
 *
 * @param array $form 
 * @param array $form_state 
 * @return void
 * @author berliner
 */

function sticky_notes_admin_priorities_form_validate($form, $form_state) {
  
  $values = $form_state['values'];
  if (empty($values['priority']['name'][$values['priority']['default'][0]])) {
    // check if at least one priority has a name
    for ((int) $i = 0; $i < STICKY_NOTES_PRIORITY_LEVEL_MAX_COUNT; $i++) {
      if (strlen($values['priority']['name'][$i]) > 0) {
        form_set_error('priority][name][' . $values['priority']['default'][0], t('The default priority must have a name.'));
      }
    }
  }
  
}

/**
 * Implementation of hook_form_submit().
 *
 * @param array $form 
 * @param array $form_state 
 * @return void
 * @author berliner
 */

function sticky_notes_admin_priorities_form_submit($form, $form_state) {
  
  $values = $form_state['values'];
  
  for ($i = 0; $i < STICKY_NOTES_PRIORITY_LEVEL_MAX_COUNT; $i++) {
    variable_set('sticky_notes_priority_name_' . $i, $values['priority']['name'][$i]);
    variable_set('sticky_notes_priority_color_' . $i, $values['priority']['color'][$i]);
  }
  variable_set('sticky_notes_default_priority_level', $values['priority']['default'][0]);
  drupal_set_message(t('Your settings have been saved.'));
  
}

/**
 * Implementation of hook_form().
 *
 * @param array $form_state 
 * @return array
 * @author berliner
 */

function sticky_notes_admin_visibility_form($form_state) {
  
  // add admin specific javascript
  drupal_add_js(drupal_get_path('module', 'sticky_notes') . '/js/sticky_notes.admin.js');
  
  $form = array();
  $form['#tree'] = TRUE;
  
  $form['default'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default'),
    '#description' => t('This section allows you to control the default visibility behaviour of sticky notes. These default settings can be overridden for each role.')
  );
  
  $options = array(
    t('Show on every page except the listed pages.'),
    t('Show on only the listed pages.')
  );
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
  $access = user_access('use PHP for block visibility');
  if ($access) {
    $options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
    $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  $form['default']['page_visibility_type'] = array(
    '#type' => 'radios',
    '#title' => t('Show sticky notes on specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('sticky_notes_page_visibility_type', STICKY_NOTES_DEFAULT_PAGE_VISIBILITY_TYPE),
    '#description' => t('This concerns both notes and info box.'),
  );
  $form['default']['page_visibility_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('sticky_notes_page_visibility_pages', STICKY_NOTES_DEFAULT_PAGE_VISIBILITY_PAGES),
    '#description' => $description,
  );
  
  $form['default']['page_visibility_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show sticky notes from specific roles'),
    '#options' => user_roles(),
    '#default_value' => variable_get('sticky_notes_page_visibility_roles', array()),
    '#description' => t('Show sticky notes only for the selected role(s). If you select no roles, sticky notes will be visible to all users.'),
  );
  
  $form['default']['default_accessibility'] = array(
    '#type' => 'radios',
    '#title' => t('Default accessibility setting'),
    '#options' => array(0 => t('Private'), 1 => t('Public')),
    '#default_value' => variable_get('sticky_notes_default_accessibility', STICKY_NOTES_DEFAULT_DEFAULT_ACCESSIBILITY),
    '#description' => t('Wheather new sticky notes should be private or public by default.')
  );
  
  $form['default']['override_default_accessibility'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow user to change private / public'),
    '#default_value' => variable_get('sticky_notes_override_default_accessibility', STICKY_NOTES_DEFAULT_OVERRIDE_DEFAULT_ACCESSIBILITY),
    '#description' => t('If checked, users can override the default private / public setting above.'),
  );
  
  $roles = user_roles();
  
  foreach ($roles as $rid => $role) {
    
    $form[$rid] = array(
      '#type' => 'fieldset',
      '#title' => $role,
      '#description' => t('This section allows you to control sticky notes visibility for !role.', array('!role' => $role)),
      '#collapsible' => TRUE,
      '#collapsed' => variable_get('sticky_notes_role_' . $rid . '_use_default', 1) == 1,
    );

    $form[$rid]['use_default'] = array(
      '#type' => 'radios',
      '#title' => t('Use default'),
      '#options' => array(1 => t('Use global configuration.'), 0 => t('Use role specific configuration.')),
      '#default_value' => variable_get('sticky_notes_role_' . $rid . '_use_default', 1),
      '#description' => t('The following seetings will only apply if the role specific configuration should be used.'),
      '#prefix' => '<div class="use-default">',
      '#suffix' => '</div>',
    );
    
    $form[$rid]['page_visibility_type'] = array(
      '#type' => 'radios',
      '#title' => t('Show sticky notes on specific pages'),
      '#options' => $options,
      '#default_value' => variable_get('sticky_notes_role_' . $rid . '_page_visibility_type', 0),
      '#description' => t('This concerns both notes and info box.'),
    );
    
    $form[$rid]['page_visibility_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => variable_get('sticky_notes_role_' . $rid . '_page_visibility_pages', ''),
      '#description' => $description,
    );
    
    $form[$rid]['page_visibility_roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Show sticky notes from specific roles'),
      '#options' => $roles,
      '#default_value' => variable_get('sticky_notes_role_' . $rid . '_page_visibility_roles', array()),
      '#description' => t('If left empty than all users with this role will be able to see the notes of all other users.'),
    );
    
    $form[$rid]['default_accessibility'] = array(
      '#type' => 'radios',
      '#title' => t('Default accessibility setting'),
      '#options' => array(0 => t('Private'), 1 => t('Public')),
      '#default_value' => variable_get('sticky_notes_role_' . $rid . '_default_accessibility', STICKY_NOTES_DEFAULT_DEFAULT_ACCESSIBILITY),
      '#description' => t('Wheather new sticky notes should be private or public by default.')
    );

    $form[$rid]['override_default_accessibility'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow user to change private / public'),
      '#default_value' => variable_get('sticky_notes_role_' . $rid . '_override_default_accessibility', STICKY_NOTES_DEFAULT_OVERRIDE_DEFAULT_ACCESSIBILITY),
      '#description' => t('If checked, users can override the default private / public setting above.'),
    );
    
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
  
}

/**
 * Implementation of hook_form_submit().
 *
 * @param array $form 
 * @param array $form_state 
 * @return void
 * @author berliner
 */

function sticky_notes_admin_visibility_form_submit($form, $form_state) {
  
  $values = $form_state['values'];
  
  variable_set('sticky_notes_page_visibility_type', $values['default']['page_visibility_type']);
  variable_set('sticky_notes_page_visibility_pages', trim($values['default']['page_visibility_pages']));
  variable_set('sticky_notes_page_visibility_roles', $values['default']['page_visibility_roles']);
  variable_set('sticky_notes_default_accessibility', $values['default']['default_accessibility']);
  variable_set('sticky_notes_override_default_accessibility', $values['default']['override_default_accessibility']);
  
  $roles = user_roles();
  foreach ($roles as $rid => $role) {
    
    if (isset($values[$rid])) {
      variable_set('sticky_notes_role_' . $rid . '_use_default', $values[$rid]['use_default']);
      variable_set('sticky_notes_role_' . $rid . '_page_visibility_type', $values[$rid]['page_visibility_type']);
      variable_set('sticky_notes_role_' . $rid . '_page_visibility_pages', $values[$rid]['page_visibility_pages']);
      variable_set('sticky_notes_role_' . $rid . '_page_visibility_roles', $values[$rid]['page_visibility_roles']);
      variable_set('sticky_notes_role_' . $rid . '_default_accessibility', $values[$rid]['default_accessibility']);
      variable_set('sticky_notes_role_' . $rid . '_override_default_accessibility', $values[$rid]['override_default_accessibility']);
    }
    
  }
  
}

/**
 * Theme function the priority table
 *
 * @param array $items 
 * @return string
 * @author berliner
 */

function theme_sticky_notes_admin_priorities_form($form) {
  
  // drupal_add_tabledrag('active-trails-rule-configuration', 'order', 'sibling', 'active-trails-rule-weight', null, null, false);
  
  $header = array(
    array('data' => t('Default')),
    array('data' => t('Name')),
    array('data' => t('Pin color')),
  );
  
  $rows = array();
  $element = &$form['priority'];
  for ($i = 0; $i < STICKY_NOTES_PRIORITY_LEVEL_MAX_COUNT; $i++) {
    
    $row = array();
    $row[] = array('data' => drupal_render($element['default'][$i]));
    $row[] = array('data' => drupal_render($element['name'][$i]));
    $row[] = array('data' => drupal_render($element['color'][$i]));
    $rows[] = $row;
  }
  
  $output = '';
  
  if ($rows)
    $table= theme('table', $header, $rows);
  
  $form['priority']['#value'] = $table;
  
  $output.= drupal_render($form);
  return $output;
}