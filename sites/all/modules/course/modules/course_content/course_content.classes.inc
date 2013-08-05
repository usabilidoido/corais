<?php

class CourseObjectContent extends CourseObjectNode {
  function getNodeTypes() {
    $handlers = course_content_course_handlers();
    return array_keys($handlers['object']);
  }

  /**
   * Set the default content type to be created.
   */
  function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);
    if (!$this->getInstanceId()) {
      $desc = $form['node_type']['#options'][$this->getComponent()];
      $form['node_type']['#options'] = array($this->getComponent() => $desc);
    }
  }

  /**
   * If course object is saved without configuration, make sure we have a
   * default node type.
   */
  function optionsDefinition() {
    $options = parent::optionsDefinition();
    $options['node_type'] = $this->config['object_type'];
    return $options;
  }
}
