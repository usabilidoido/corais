<?php
// $Id$

/**
 * A handler to grab the node teaser associated with an activity.
 *
 * Has bonus options to restrict inclusion based on site and group settings.
 */
class grayside_oa_feeds_handler_field_activity_description extends views_handler_field_markup {

  function option_definition() {
    $options = parent::option_definition();
    $options['restricted'] = array('default' => 1);
    return $options;
  }

  /**
   * Add the security options.
   */
  function options_form(&$form, $form_state) {
    parent::options_form($form, $form_state);
    $form['restricted'] = array(
      '#type' => 'select',
      '#title' => t('Restrict access'),
      '#description' => t('Access to the description is limited based on site and group settings.'),
      '#default_value' => $this->options['restricted'],
      '#options' => array(
        0 => t('No restriction'),
        1 => t('Public content only'),
      ),
    );
  }

  /**
   * Constructor. Add additional fields.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['node_nid'] = array('table' => 'node', 'field' => 'nid');
  }

  function render($values) {
    // Warning: This is a really bad replacement for an actual access check.
    if ($this->options['restricted']
      && variable_get('user_register', 0)
      && node_access('view', node_load($values->{$this->aliases['node_nid']}), drupal_anonymous_user())) {
        return parent::render($values);
    }
    elseif (!$this->options['restricted']) {
      return parent::render($values);
    }
    return;
  }
}