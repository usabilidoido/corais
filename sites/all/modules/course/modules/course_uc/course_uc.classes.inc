<?php

class CourseObjectUbercart extends CourseObjectNode {

  /**
   * Specify that this object needs a navigation listener.
   */
  public function hasPolling() {
    return TRUE;
  }

  function getTakeType() {
    return 'content';
  }

  function take() {
    if ($this->getFulfillment()->isComplete()) {
      return "You've already purchased this activity.";
    }
    else {
      $atc = drupal_get_form('uc_product_add_to_cart_form', $this->node);
      if (!$atc) {
        // The add to cart form was hidden?
        return "Something isn't right...";
      }
      else {
        return $atc;
      }
    }
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $form['node']['instance']['#title'] = t('Product to check');
    $form['node']['instance']['#description'] .= '</br>' . t('Use an existing ubercart-enabled product for checking payment. Defaults to this course.');
  }

  /**
   * Set a specialized instance ID.
   *
   * Either the course node ID, or another ubercart node ID.
   */
  public function optionsSubmit(&$form, &$form_state) {
    if (!$form_state['values']['instance']) {
      $form_state['values']['instance'] = $this->getCourseNid();
    }
    parent::optionsSubmit($form, $form_state);
  }

  /**
   * Don't create new products.
   */
  public function create() {
    if (!$this->getInstanceId()) {
      $this->setOption('instance', $this->getCourseNid());
    }
  }

  function getNodeTypes() {
    return uc_product_types();
  }

  function getCloneAbility() {
    return TRUE;
  }

  function thaw($ice) {
    // Not creating a new product. Using this course.
    return $this->getCourseNid();
  }
}
