<?php

class grayside_oa_feeds_handler_field_activity_id extends atrium_activity_handler_field_activity {

  function option_definition() {
    $options = parent::option_definition();
    $options['absolute'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Add the time_horizon element.
   */
  function options_form(&$form, $form_state) {
    parent::options_form($form, $form_state);
    unset($form['time_horizon']);
    $form['guid'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use as GUID for feeds'),
      '#description' => t("GUID's for feeds need to be globally unique. This toggle appends 'at %url' to the identifier.", array('%url' => $GLOBALS['base_url'])),
      '#default_value' => $this->options['guid'],
    );
  }

  /**
   * Contructor. Add additional fields.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['node_revision'] = array('table' => 'node_revisions', 'field' => 'vid');
    $this->additional_fields['comment_cid'] = array('table' => 'comments', 'field' => 'cid');
  }

  /**
   * Render this field as a link to group space.
   */
  function render($value) {
    // Has comments
    if (!empty($value->{$this->field_alias})) {
      $id = $value->{$this->aliases['nid']} . ':c' . $value->{$this->aliases['comment_cid']};
    }
    // No comments
    else {
      $id = $value->{$this->aliases['nid']} . ':r' . $value->{$this->aliases['node_revision']};
    }
    if ($this->options['guid']) {
      $id .= ' at ' . $GLOBALS['base_url'];
    }
    return $id;
  }
}