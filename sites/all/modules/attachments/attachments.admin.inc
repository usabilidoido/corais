<?php
// $Id: attachments.admin.inc,v 1.4 2010/06/26 14:02:15 adaddinsane Exp $
/**
 * @file
 * Modify the user interface and the display of file attachments - settings
 */

/**
 * Settings for modifying the way the Attachments user interface works
 */
function attachments_settings_ui() {
  $form = array();

  $form[Attachments::VAR_JS_SWITCH] = array(
    '#type' => 'checkbox',
    '#title' => t('Modify the way the Attachments user interface works.'),
    '#default_value' => variable_get(Attachments::VAR_JS_SWITCH, TRUE),
    '#weight' => 0,
  );
  $form[Attachments::VAR_DESCRIPT] = array(
    '#type' => 'textarea',
    '#title' => t('Replacement text'),
    '#description' => t('Change the descriptive text for file attachments to match the new interface.'),
    '#default_value' => variable_get(Attachments::VAR_DESCRIPT, Attachments::DESCRIPT_DEFAULT),
    '#weight' => 0,
  );
  return system_settings_form($form);
}

/**
 * Settings for modifying the way the Attachments user interface works
 */
function attachments_settings_style() {
  $form = array();

  $options = array();
  $results = module_invoke_all('attachments', 'style');
  foreach ($results as $module => $data) {
    foreach ($data as $theme => $name) {
      $options[$theme] = t($name);
    }
  }

  $form[Attachments::VAR_STYLE] = array(
    '#type' => 'radios',
    '#title' => t('File attachment style'),
    '#options' => $options,
    '#default_value' => variable_get(Attachments::VAR_STYLE, Attachments::STYLE_DEFAULT),
    '#default' => 0,
  );

  $form[Attachments::VAR_TITLE] = array(
    '#type' => 'textfield',
    '#title' => t('File attachment title'),
    '#description' => t('Set the title for the list of attachments, this is not applied to the default Drupal attachments display. Use <em>&lt;none&gt;</em> for no title.'),
    '#default_value' => variable_get(Attachments::VAR_TITLE, Attachments::VAR_TITLE_DEFAULT),
    '#default' => 1,
  );

  return system_settings_form($form);
}

/**
 * Settings for maximum number of attachments per content type
 */
function attachments_settings_files() {
  $form = array();


  $limits = variable_get(Attachments::VAR_MAX_FILES, array());
  $form['limit'] = array(
    '#type' => 'item',
    '#description' => t('Enter the maximum number of attachments ' .
'that can be uploaded per node type, use 0 (zero) for unlimited. ' .
'Only node types which have attachments enabled are shown.'),
  );

  $displayed = 0;
  $node_types = node_get_types('names');
  $form['limit'][Attachments::VAR_MAX_FILES] = array(
    '#tree' => TRUE,
  );
  foreach ($node_types as $type => $name) {
    if ($attachments = variable_get('upload_' . $type, 0)) {
      $displayed++;
    }
    $form['limit'][Attachments::VAR_MAX_FILES][$type] = array(
      '#type' => 'textfield',
      '#title' => check_plain($name),
      '#access' => $attachments,
      '#default_value' => is_array($limits) ? (is_numeric($limits[$type])?$limits[$type]:0) : (is_numeric($limits)?$limits:0),
      '#element_validate' => array('_attachments_limit_validate'),
      '#size' => 5,
      '#maxlength' => 2,
      '#weight' => -15,
    );
  }
  if ($displayed==0) {
    drupal_set_message(t('No node types have attachments enabled.'), 'warning');
  }
  return system_settings_form($form);
}


/**
 * Settings for the 'attachments' module displays
 */
function attachments_settings_exts() {
  $form = array();

  $form[Attachments::VAR_EXTENSION_SWITCH] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the advanced File extensions display in every form that has file attachments.'),
    '#default_value' => variable_get(Attachments::VAR_EXTENSION_SWITCH, TRUE),
    '#weight' => -10,
  );

  $groups = variable_get(Attachments::VAR_EXTENSIONS, Attachments::exts());
  $groups[' '] = array();
  $form['ext_groups'] = array(
    '#tree' => TRUE,
    '#theme' => 'attachments_settings_table',
    '#weight' => -9,
  );
  $g = 0;
  $gvals = $values[Attachments::VAR_EXTENSIONS];
  foreach ($groups as $group => $exts) {
    if (!is_array($exts)) continue;

    $form['ext_groups'][$g] = array(
      '#element_validate' => array('_attachments_group_validate'),
      '#delta' => $g,
      '#delta_max' => count($groups)-1,
      'group' => array(
        '#type' => 'textfield',
        '#size' => 32,
        '#maxlength' => 32,
        '#default_value' => trim($group),
        '#weight' => 1,
      ),
      'exts' => array(
        '#type' => 'textfield',
        '#size' => 32,
        '#maxlength' => 64,
        '#default_value' => implode(' ', $exts),
        '#weight' => 2,
      ),
    );
    $g++;
  }

  $form[Attachments::VAR_OTHER_EXTS] = array(
    '#type' => 'textfield',
    '#title' => t('Default extension group'),
    '#description' => t('Name of the group to use for any unclaimed extensions.'),
    '#default_value' => variable_get(Attachments::VAR_OTHER_EXTS, Attachments::OTHER_EXTS),
    '#weight' => -7,
  );

  $form['#submit'] = array('attachments_settings_submit');
  return system_settings_form($form);
}

/**
 * Validate the file limit fields
 */
function _attachments_limit_validate($element, &$form_state) {
  // Are we using this element?
  if ($element['#access']) {
    // get the value regardless of its position in the form hierarchy
    $values =& $form_state['values'];
    foreach ($element['#parents'] as $key) $values =& $values[$key];

    if (!is_numeric($values)) {
      form_error($element, t('Limit value must be a positive integer.'));
    }
    else {
      $values = (int) $values;
      if ($values<0) {
        form_error($element, t('Limit value must be a positive integer.'));
      }
    }
  }
}

/**
 * Validate a group name/extensions field
 */
function _attachments_group_validate($element, &$form_state) {
  // get the value regardless of its position in the form hierarchy
  $values =& $form_state['values'];
  foreach ($element['#parents'] as $key) $values =& $values[$key];

  $group =& $values['group'];
  $exts =& trim($values['exts']);
  if (($group && !$exts) || (!$group && $exts)) {
    if ($element['#delta']<$element['#delta_max']) {
      form_error($element['group'], t('Both or neither fields must be filled in.'));
    }
  }
  if ($group) {
    $group = drupal_strtolower(trim($group));
  }
  if ($exts) {
    $exts = drupal_strtolower(implode(' ', array_filter(explode(' ', $exts))));
  }
}

/**
 * Implementation of hook_submit().
 */
function attachments_settings_submit($form, &$form_state) {
  $values =& $form_state['values']['ext_groups'];
  $groups = array();
  foreach ($values as $pair) {
    if ($pair['exts']) {
      $groups[$pair['group']] = explode(' ', $pair['exts']);
    }
  }
  variable_set(Attachments::VAR_EXTENSIONS, $groups);

  # remove this from $form_state to stop the default
  # system from saving it in the variable table
  unset($form_state['values']['ext_groups']);
}
