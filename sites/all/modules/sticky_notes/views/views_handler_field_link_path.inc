<?php
/**
 * @file
 * Contains a linkable 'link_path' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a page.
 */
class views_handler_field_link_path extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_path'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_page'] = array(
      '#title' => t('Link this field to the page it references'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_page']),
    );
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_page']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $data;
    }
    return $data;
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}
