<?php

/**
 * Spaces view filter handler.
 *
 * This filter delegates filtering to the 'views_filter' method on the calling space object.
 * Refer to the 'space_og' class in the Spaces OG module for an example implementation.  If
 * you are creating your own space type by implementing the 'space' interface, you can use
 * as-is or modify the 'views_filter' method for the purposes of your space type.
 */
class spaces_handler_filter_spaces_current extends views_handler_filter {
  /**
   * override can_expose()
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * Meta query handler which delegates query building to the implementing spaces.
   */
  function query() {
    $space = spaces_get_space();
    if ($space) {
      if ($this->relationship) {
        $base_table = $this->view->query->relationships[$this->relationship]['table'];
      }
      else {
        $base_table = $this->view->base_table;
      }
      $space->views_filter($this->view->query, $base_table, $this->relationship);
    }
  }

  /**
   * Provide a form for setting the operator.
   *
   * This may be overridden by child classes, and it must
   * define $form['operator'];
   */
  function operator_form(&$form, &$form_state) {
    $form['message'] = array(
      '#type' => 'item',
      '#value' => t('Items will be filtered to show only those that belong to the active space.'),
    );
  }

  /**
   * Override of admin_summary().
   */
  function admin_summary() {
    return t('True');
  }
}
