<?php

class CourseObjectScorm extends CourseObjectNode {

  public function create() {
    if ($this->getInstanceId()) {
      $scorm_node = node_load($this->getInstanceId());
    }
    else {
      $scorm_node = new stdClass;
    }
    $scorm_node->type = $this->getComponent();
    $scorm_node->title = $this->getTitle();
    $scorm_node->uid = $this->user->uid;
    node_save($scorm_node);
    $this->setNode($scorm_node);
  }

  function getNodeTypes() {
    return array();
  }

}
