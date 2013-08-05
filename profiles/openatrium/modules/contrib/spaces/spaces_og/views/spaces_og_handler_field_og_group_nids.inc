<?php

class spaces_og_handler_field_og_group_nids extends og_views_handler_field_og_group_nids {
  /**
   * Override of init().
   * Omit/exclude this field if we're within a group space.
   */
  function init(&$view, $options) {
    parent::init($view, $options);
    $space = spaces_get_space();
    if ($space && $space->type === 'og') {
      $this->options['exclude'] = TRUE;
    }
  }

  /**
   * Override of option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['spaces_og_frontpage'] = array('default' => TRUE);
    return $options;
  }

  /**
   * Override of options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['spaces_og_frontpage'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link to group frontpage'),
      '#default_value' => $this->options['spaces_og_frontpage'],
    );
  }

  /**
   * Collect group title, purl here.
   */
  function pre_render(&$values) {
    if ($this->options['spaces_og_frontpage']) {
      $this->field_alias = $this->aliases['og_ancestry_nid'];
      foreach ($values as $value) {
        $result = og_get_node_groups_result($value->nid);
        while ($row = db_fetch_object($result)) {
          $this->items[$value->nid][$row->group_nid] = l($row->title, '<front>', array('purl' => array('provider' => 'spaces_og', 'id' => $row->group_nid)));
        }
      }
    }
    else {
      parent::pre_render($values);
    }
  }
}
