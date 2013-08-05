<?php

class CourseObjectCertificate extends CourseObject {

  public function take() {
    // Fulfill immediately.
    $this->getFulfillment()->setComplete(1)->save();
    return l('Download certificate', "node/" . $this->getCourseNid() . "/certificate");
  }

  public function optionsDefinition() {
    $options = parent::optionsDefinition();
    $options['required'] = 0;
    $options['certificate_node'] = 1;
    return $options;
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);
    $options = $this->getOptions();

    $form['certificate_node'] = array(
      '#title' => 'Use node settings from Certificate',
      '#type' => 'checkbox',
      '#default_value' => $options['certificate_node'],
      '#description' => 'This will direct the user to node/' . $this->getCourseNid() . '/certificate',
      '#disabled' => TRUE,
    );
  }

  public function getTakeType() {
    return 'content';
  }

  public function getCloneAbility() {
    return TRUE;
  }

  public static function getMaxOccurences() {
    return 1;
  }

}
