<?php
// $Id$

/**
 * @file
 * Administration page callbacks for the protonotes module.
 */

/**
 * Form builder. Configure protonotes.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function protonotes_admin_settings() {
  // Get an array of node types with internal names as keys and
  // "friendly names" as values. E.g.,
  // array('page' => 'Page', 'story' => 'Story')
  $options = node_get_types('names');
  
  $form['protonotes_enabled'] = array( 
    '#type' => 'radio', 
    '#title' => t('Do you want to enable protonotes?'), 
    '#options' => $options, 
    '#default_value' => variable_get('protonotes_enabled', array('page')), 
    '#description' => t('A site wide toggle for using protonotes.'), 
  ); 

  $form['protonotes_groupnumber'] = array( 
    '#type' => 'text', 
    '#title' => t('Group Number'), 
    '#options' => $options, 
    '#default_value' => variable_get('protonotes_groupnumber', array('page')), 
    '#description' => t('What is the group number given by your Protonotes registration?'), 
  ); 

  $form['protonotes_notes_default'] = array( 
    '#type' => 'radio', 
    '#title' => t('Show Notes Default'), 
    '#options' => array('Show', 'Hide'), 
    '#default_value' => variable_get('protonotes_notes_default', array('page')), 
    '#description' => t('Do you want the notes showing or hidding by default?'), 
  ); 

  $form['protonotes_menubar_default'] = array( 
    '#type' => 'radio', 
    '#title' => t('Menubar default'), 
    '#options' => array('Show', 'Hide'), 
    '#default_value' => variable_get('protonotes_menubar_default', array('page')), 
    '#description' => t('Do you want the menubar showing or hidden by default?'), 
  ); 

  $form['protonotes_database_key'] = array( 
    '#type' => 'text', 
    '#title' => t('Database Key'), 
    '#options' => array('Show', 'Hide'), 
    '#default_value' => variable_get('protonotes_database_key', array('page')), 
    '#description' => t('If you are using a private database, include the key here'), 
  ); 
  return system_settings_form($form); 

}