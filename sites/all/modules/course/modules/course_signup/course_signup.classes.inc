<?php

class CourseObjectSignup extends CourseObjectNode {

  /**
   * Specify that this object needs a navigation listener.
   */
  public function hasPolling() {
    return TRUE;
  }

  public function create() {
    if (!$this->getOption('instance')) {
      $this->setOption('instance', $this->getCourseNid());
    }
  }

  public function getTakeType() {
    return 'content';
  }

  public function take() {
    if ($this->getFulfillment()->isComplete()) {
      return "You have been marked attended by an administrator.";
    }
    else {
      return "You must be marked attended by an administrator.";
    }
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $form['node']['instance']['#title'] = t('Signup to check');
    $form['node']['instance']['#description'] .= ' ' . t('Node must be signup-enabled. Defaults to this course.');
  }

  /**
   * Set a specialized instance ID.
   *
   * Either the course node ID, or another signup node ID.
   */
  public function optionsSubmit(&$form, &$form_state) {
    if (!$form_state['values']['instance']) {
      $form_state['values']['instance'] = $this->getCourseNid();
    }
    parent::optionsSubmit($form, $form_state);
  }

  public function getReports() {
    return array(
      'attendance' => array(
        'title' => 'Attendance',
      ),
    );
  }

  public function getReport($key) {
    module_load_include('inc', 'signup', 'includes/node_output');
    switch ($key) {
      case 'attendance':
        return array(
          'title' => t('Attendance'),
          'content' => t('These are the users who have enrolled in this course object.<br/>You may administer attendance on the !signups tab.', array(
            '!signups' => l('Signups', "node/{$this->node->nid}/signups"),
          )) . signup_user_list_output($this->node),
        );
    }
  }

  function getNodeTypes() {
    return signup_content_types();
  }

  function getCloneAbility() {
    return TRUE;
  }

  function thaw($ice) {
    // Not creating a new signup. Using this course.
    return $this->getCourseNid();
  }

}
