<?php
// $Id$

/**
 * Provides access control for reader tabs.
 */
class atrium_reader_plugin_access extends views_plugin_access {
  function access($account) {
    return atrium_reader_access($this->options['feedtype']);
  }

  function get_access_callback() {
    return array('atrium_reader_access', array($this->options['feedtype']));
  }

  function summary_title() {
    return $this->options['feedtype'];
  }

  function option_defaults(&$options) {
    $options['type'] = 'atrium_reader'; // Wtf is this necessary? See views plugin stack : (
    $options['feedtype'] = 'news';
  }

  function options_form(&$form, &$form_state) {
    $types = atrium_reader_get_types();
    $form['feedtype'] = array(
      '#type' => 'select',
      '#options' => $types,
      '#title' => t('Type'),
      '#default_value' => $this->options['feedtype'],
      '#description' => t('Access will be granted if the Reader feature is active and there exist items of the selected type.'),
    );
  }
}
