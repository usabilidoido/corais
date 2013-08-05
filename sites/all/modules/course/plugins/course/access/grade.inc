<?php

$plugin = array(
  'title' => t('Graded-based access'),
  'description' => t('Controls access by grade'),
  'handler' => array(
    'class' => 'CourseObjectAccessGrade',
  ),
);

class CourseObjectAccessGrade extends CourseObjectAccess {

  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults += array(
      'course_grade' => NULL,
      'course_grade_hidden' => NULL,
    );

    return $defaults;
  }

  function take() {
    $config = $this->getOptions();
    if ($this->getCourseObject()->getCourse()->getTracker()->getOption('grade_result') >= $config['course_grade']) {
      return TRUE;
    }
    else {
      $this->getCourseObject()->setAccessMessage('grade', t('You must have a grade of at least @grade% to take this activity.', array('@grade' => $this->getOption('course_grade'))));
      return FALSE;
    }
  }

  function see() {
    if (!$this->take() && $this->getOption('course_grade_hidden')) {
      return FALSE;
    }
  }

  function view() {
    return $this->take();
  }

  function optionsForm() {
    $form = array();
    $config = $this->getOptions();

    $form['course_grade'] = array(
      '#title' => t('Course grade required'),
      '#type' => 'textfield',
      '#size' => 4,
      '#description' => t('The user will not be able to access this object until this course grade is met.'),
      '#default_value' => $config['course_grade'],
    );

    $form['course_grade_hidden'] = array(
      '#title' => t('Hide until grade met'),
      '#description' => t('The user will not see this object until the course grade is met.'),
      '#type' => 'checkbox',
      '#default_value' => $config['course_grade_hidden'],
    );

    return $form;
  }

}
