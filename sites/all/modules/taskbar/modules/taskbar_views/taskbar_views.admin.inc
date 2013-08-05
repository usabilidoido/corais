<?php
/**
 * Settings form for the views provider.
 */
function taskbar_views_settings(&$form, &$form_state, $item) {
  $form['provider_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views settings'),
    '#collapsible' => TRUE,
  );
  
  if (empty($item->settings['view'])) {
    $item->settings['view'] = '';
  }
  if (empty($item->settings['display'])) {
    $item->settings['display'] = 'default';
  }
  if (empty($item->settings['tracking'])) {
    $item->settings['tracking'] = FALSE;
  }

  $views = drupal_map_assoc(array_keys(views_get_all_views()));
  $selected_view = !empty($form_state['values']['view']) ? $form_state['values']['view'] : $item->settings['view'];
  $form['provider_settings']['view'] = array(
    '#type' => 'select',
    '#title' => t('View'),
    '#description' => t('Select the view you would like to use for this item.'),
    '#required' => TRUE,
    '#default_value' => $selected_view,
    '#options' => $views,
    '#ahah' => array(
      'path' => 'taskbar/provider/views/ahah',
      'wrapper' => 'views-display-wrapper',
    ),
  );
  $form['provider_settings']['views_display_holder'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="views-display-wrapper">',
    '#suffix' => '</div>',
  );
  $form['provider_settings']['views_display_holder']['display'] = array(
    '#type' => 'select',
    '#title' => t('Display'),
    '#options' => _taskbar_views_get_display($selected_view),
    '#default_value' => $item->settings['display'],
  );

  $form['provider_settings']['tracking'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track Views result'),
    '#description' => t('Permanently monitor Views result to check and display number of new entries in the result. <strong>Enable this option might cause heavy resource usage on the server.</strong>'),
    '#default_value' => $item->settings['tracking'],
  );
}

function taskbar_views_settings_submit($form, &$form_state, &$item) {
  $item->settings['view'] = $form_state['values']['view'];
  $item->settings['display'] = $form_state['values']['views_display_holder']['display'];
  $item->settings['tracking'] = $form_state['values']['tracking'];
}

/**
 * The AHAH callback. It processes the form and then.
 */
function taskbar_views_ahah_callback() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  // Enable the submit/validate handlers to determine whether AHAH-submittted.
  $form_state['ahah_submission'] = TRUE;
  $form['#programmed'] = $form['#redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  $changed_elements = $form['provider_settings']['views_display_holder'];

  // Prevent duplicate wrappers.
  unset($changed_elements['#prefix'], $changed_elements['#suffix']);

  $output = theme('status_messages') . drupal_render($changed_elements);

  drupal_json(array(
    'status'   => TRUE,
    'data'     => $output,
  ));
}

function _taskbar_views_get_display($view_name) {
  $view = views_get_view($view_name);
  $displays = array();
  foreach (array_keys($view->display) as $id) {
    $displays[$id] = $id . ' (' . $view->display[$id]->display_title . ')';
  }
  return $displays;
}
