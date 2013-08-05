<?php

class CourseObjectQuiz extends CourseObjectNode {

  /**
   * Create the quiz node and set it as this object's instance.
   */
  function create() {
    $quiz = new stdClass;
    $quiz->auto_created = TRUE;
    $quiz->type = 'quiz';
    $quiz->title = $this->getTitle();
    $quiz->uid = $this->user->uid;
    $quiz->quiz_always = TRUE;
    $quiz->quiz_open = $quiz->quiz_close = array(
      'month' => date('m'),
      'day' => date('d'),
      'year' => date('Y'),
    );
    $quiz = (object) array_merge(_quiz_get_node_defaults(), (array) $quiz);
    node_save($quiz);
    $this->setNode($quiz);
  }

  /**
   * The take URL of the quiz is /take.
   */
  function getTakeUrl() {
    return url("node/{$this->node->nid}/take");
  }

  /**
   * Marks a user's fulfillment record for this object complete if the user
   * passed the quiz.
   */
  function grade($user, $rid) {
    $nid = (int) $this->getInstanceId();
    $fulfillment = $this->getFulfillment();

    $result_ids = (array) $fulfillment->getOption('quiz_result_ids');
    $result_ids[] = $rid;
    $fulfillment->setOption('quiz_result_ids', $result_ids);

    $result = reset(quiz_get_score_data(array($nid), $user->uid));
    if ($result && ($result->percent_score >= $this->getOption('passing_grade'))) {
      $fulfillment->setGrade($result->percent_score)->setComplete()->save();
    }
    else {
      $fulfillment->setGrade($result->percent_score)->save();
    }
  }

  /**
   * Course quiz options.
   */
  public function optionsDefinition() {
    $options = parent::optionsDefinition();

    $options['passing_grade'] = 75;

    return $options;
  }

  /**
   * Add an option only pertinent to quiz?
   */
  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);
    $defaults = $this->getOptions();

    $form['grading']['passing_grade'] = array(
      '#title' => t('Passing grade'),
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $defaults['passing_grade'],
      '#description' => t('The user will not be able to proceed past this object unless this grade is met.'),
    );
  }

  /**
   * Let the user know if they have a Quiz without questions.
   */
  public function getWarnings() {
    $warnings = parent::getWarnings();

    if ($this->getInstanceId()) {
      if (!quiz_get_number_of_questions($this->node->vid)) {
        $warnings[] = t('This Quiz does not have any questions. Please !link.', array('!link' => l('add questions', "node/{$this->getInstanceId()}/questions")));
      }
    }

    return $warnings;
  }

  public function getReports() {
    return array(
      'results' => array(
        'title' => 'Results',
      ),
    );
  }

  public function getReport($key) {
    module_load_include('inc', 'quiz', 'quiz.admin');
    switch ($key) {
      case 'results':
        return array(
          'title' => t('Quiz results'),
          'content' => drupal_get_form('quiz_results_manage_results_form', $this->node),
        );
    }
  }

  /**
   * Remove all quiz attempts associated with this fulfillment.
   */
  public function unenroll() {
    parent::unenroll();
    $fulfillment = $this->getFulfillment();
    quiz_delete_results((array) $fulfillment->getOption('quiz_result_ids'));
  }

  function getNodeTypes() {
    return array('quiz');
  }

  function isGraded() {
    return TRUE;
  }

  function getCloneAbility() {
    return t('%object can only be partially cloned. It will be created with the same settings, but the without the questions', array('%object' => $this->getTitle()));
  }

  /**
   * Exception for quiz: we need to set auto_created.
   */
  function thaw($ice) {
    $this->node = $ice->node;
    unset($this->node->nid);
    $this->node->auto_created = TRUE;
    node_save($this->node);
    return $this->node->nid;
  }

  function getOptionsSummary() {
    $summary = parent::getOptionsSummary();
    if ($this->getInstanceId()) {
      $summary['questions'] = l('Edit questions', "node/{$this->getInstanceId()}/questions");
    }
    return $summary;
  }

}
