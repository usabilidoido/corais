<?php

class spaces_handler_field_node extends views_handler_field_node {
  /**
   * Override of option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['spaces'] = array('default' => array(
      'type' => NULL,
      'frontpage' => FALSE
    ));
    return $options;
  }

  /**
   * Override of options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // @TODO: atm this assumes a pattern of spaces_{$type} for the PURL
    // provider corresponding to each space type. Fixing this requires some
    // serious overhaul of the explicit connection (read: there is none
    // currently) between a space type and its PURL provider.
    $types = array();
    foreach (spaces_types() as $type => $info) {
      if (strpos('node/%node', $info['path']) === 0) {
        $types["spaces_{$type}"] = $info['title'];
      }
    }

    if (!empty($types)) {
      $form['spaces']['#tree'] = TRUE;
      $form['spaces']['frontpage'] = array(
        '#title' => t('Link to space frontpage'),
        '#description' => t('Link a group node to its frontpage instead of its node page.'),
        '#type' => 'checkbox',
        '#default_value' => !empty($this->options['spaces']['frontpage']),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-link-to-node' => array(1)
        ),
      );
      $form['spaces']['type'] = array(
        '#title' => t('Space type'),
        '#type' => 'select',
        '#options' => $types,
        '#default_value' => !empty($this->options['spaces']['type']),
        '#process' => array('views_process_dependency'),
        '#dependency' => array(
          'edit-options-spaces-frontpage' => array(1)
        ),
      );
    }
  }

  /**
   * Override of render_link().
   * Add purl options to link if 'frontpage' option is enabled. Note that
   * the PURL url is generated using 'absolute' to keep from overriding *all*
   * of render_as_link().
   */
  function render_link($data, $values) {
    parent::render_link($data, $values);
    if ($data !== NULL && $data !== '' && !empty($this->options['spaces']['frontpage']) && !empty($this->options['spaces']['type'])) {
      $this->options['alter']['path'] = url('<front>', array(
        'absolute' => TRUE,
        'purl' => array(
          'provider' => $this->options['spaces']['type'],
          'id' => $values->{$this->aliases['nid']},
        )
      ));
    }
    return $data;
  }
}
