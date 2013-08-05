<?php

class CourseObjectManual extends CourseObject {

  /**
   * Display status message as course content.
   */
  public function take() {
    return $this->getStatus();
  }

  /**
   * Return a message about the user's status in this object, for when this
   * object is hidden.
   */
  public function getStatus() {
    $grade = $this->getFulfillment()->getGrade();
    $config = $this->getOptions();
    if (!$this->getFulfillment()->getId()) {
      // User has not been given a status yet.
      return $config['incomplete_msg'];
    }

    if ($this->getFulfillment()->isComplete()) {
      // Complete. User given passed status.
      return $config['complete_msg'];
    }
    else {
      // User given a status but it wasn't complete. This means they failed.
      return $config['failed_msg'];
    }
  }

  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults['complete_msg'] = 'Your instructor has marked you as passed.';
    $defaults['incomplete_msg'] = 'Your instructor has not given you a pass/fail grade yet.';
    $defaults['failed_msg'] = 'Your instructor has marked you as failed.';

    return $defaults;
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $config = $this->getOptions();

    $form['complete_msg'] = array(
      '#type' => 'textfield',
      '#title' => 'Complete message',
      '#default_value' => $config['complete_msg'],
    );

    $form['failed_msg'] = array(
      '#type' => 'textfield',
      '#title' => 'Failed message',
      '#default_value' => $config['failed_msg'],
    );

    $form['incomplete_msg'] = array(
      '#type' => 'textfield',
      '#title' => 'Incomplete message',
      '#default_value' => $config['incomplete_msg'],
    );
  }

}
