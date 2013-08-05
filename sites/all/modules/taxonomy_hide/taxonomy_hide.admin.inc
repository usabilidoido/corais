<?php
// $Id: taxonomy_hide.admin.inc,v 1.1.2.3 2008/10/27 15:07:50 brmassa Exp $

/**
 * @file
 */

/**
 * Menu callback for administration settings.
 */
function _taxonomy_hide_admin_settings() {
  $form['taxonomy_hide_group_by_vocabulary'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Group by vocabulary'),
    '#default_value'  => variable_get('taxonomy_hide_group_by_vocabulary', 0),
    '#description'    => t('Check this box to group terms by vocabulary in node views.'),
  );
  $form['taxonomy_hide'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('Vocabulary settings'),
  );

  // Note that the settings will be saved as an array whose keys are the
  // vocabulary ids ($vid) and whose values are 0 (disabled) or $vid (enabled).
  //
  // The default_value should be an array whose values are the enabled
  // vocabulary ids.
  //
  // The options should be an array whose keys are the vocabulary ids, and whose
  // values are the names of the vocabularies.
  $select = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    $select[$vocabulary->vid] = $vocabulary->name;
  }
  $form['taxonomy_hide']['taxonomy_hide_vocabularies'] = array(
    '#type'           => 'checkboxes',
    '#title'          => t('Hide vocabularies'),
    '#default_value'  => array_keys(array_filter(variable_get('taxonomy_hide_vocabularies', array()))),
    '#options'        => $select,
    '#description'    => t('Choose the vocabularies whose terms should disappear from node views'),
    '#multiple'       => TRUE,
  );


  // Fieldset holding checkboxes for node type settings
  $form['node_types'] = array(
    '#description'  => t('Filters for node types. Node type settings will override the default filters. If none are checked for a type, the default filter will be used.'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
    '#title'        => t('Vocabulary filters for each node type'),
    '#type'         => 'fieldset',
  );

  $node_types = node_get_types();
  foreach ($node_types as $type) {
    $select = array();
    $vocabularies = taxonomy_get_vocabularies($type->type);
    if ($vocabularies) {
      foreach ($vocabularies as $vocabulary) {
        $select[$vocabulary->vid] = $vocabulary->name;
      }
      $form['node_types']['taxonomy_hide_vocabularies_'. $type->type] = array(
        '#default_value'  => array_keys(array_filter(variable_get('taxonomy_hide_vocabularies_'. $type->type, array()))),
        '#options'        => $select,
        '#type'           => 'checkboxes',
        '#title'          => t('Hide vocabularies for @type', array('@type' => $type->name)),
      );
    }
  }

  return system_settings_form($form);
}
