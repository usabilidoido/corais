<?php

/**
 * Parent class for webform course tracking.
 */
class CourseObjectWebform extends CourseObjectNode {

  public function create() {
    $webform = new stdClass;
    $webform->type = 'webform';
    $webform->title = $this->getTitle();
    $webform->uid = $this->user->uid;
    node_save($webform);
    $this->setNode($webform);
  }

  public function getWarnings() {
    $warnings = parent::getWarnings();

    if ($this->getInstanceId()) {
      if (!$this->node->webform['components']) {
        $warnings[] = t('The Webform has no questions. Please !link.', array('!link' => l('add questions', "node/{$this->getInstanceId()}/webform")));
      }
    }

    return $warnings;
  }

  public function getReports() {
    return array(
      'submissions' => array(
        'title' => 'Submissions',
      ),
      'analysis' => array(
        'title' => 'Analysis',
      ),
      'download' => array(
        'title' => 'Download',
      ),
    );
  }

  public function getReport($key) {
    module_load_include('inc', 'webform', 'includes/webform.report');
    switch ($key) {
      case 'submissions':
        return array(
          'title' => t('Webform results'),
          'content' => webform_results_submissions($this->node, FALSE, 50),
        );
      case 'analysis':
        return array(
          'title' => t('Webform results'),
          'content' => webform_results_analysis($this->node),
        );
      case 'download':
        return array(
          'title' => t('Webform results'),
          'content' => drupal_get_form('webform_results_download_form', $this->node),
        );
    }
  }

  function getNodeTypes() {
    return array('webform');
  }

  function getCloneAbility() {
    return TRUE;
  }

  function getOptionsSummary() {
    $summary = parent::getOptionsSummary();
    if ($this->getInstanceId()) {
      $summary['questions'] = l('Edit questions', "node/{$this->getInstanceId()}/webform");
    }
    return $summary;
  }

}
