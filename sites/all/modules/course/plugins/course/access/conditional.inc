<?php

$plugin = array(
  'title' => t('Condition-based access'),
  'description' => t('Controls access by conditional dates'),
  'handler' => array(
    'class' => 'CourseObjectAccessConditional',
  ),
);

class CourseObjectAccessConditional extends CourseObjectAccess {

  function view() {
    return $this->take();
  }

  function see() {
    if ($this->getOption('conditional_hidden')) {
      return $this->view();
    }
  }

  function take() {
    $type = $this->getOption('conditional_type');
    $offset = $this->getOption('conditional_time');
    $time = time();
    $access = TRUE;
    if ($type) {
      foreach ($this->getCourseObject()->getCourse()->getObjects() as $courseObject) {
        if ($courseObject->getId() == $this->getOption('conditional_object')) {
          switch ($type) {
            case 'completed':
              $completed = $courseObject->getFulfillment()->getOption('date_completed');
              if (!$completed) {
                $access = FALSE;
              }
              else {
                $access = $time >= ($completed + $offset);
              }
              break;
            case 'started':
              $started = $courseObject->getFulfillment()->getOption('date_started');
              if (!$started) {
                $access = FALSE;
              }
              else {
                $access = $time >= ($started + $offset);
              }
              break;
          }
        }
      }
    }

    return $access;
  }

  function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults += array(
      'conditional_type' => NULL,
      'conditional_time' => NULL,
      'conditional_object' => NULL,
      'conditional_hidden' => NULL,
    );

    return $defaults;
  }

  function optionsValidate(&$form, &$values) {
    if (module_exists('duration_element')) {
      $values['conditional_time'] = $values['conditional_time']->to_single_metric('seconds');
    }
  }

  function optionsForm() {
    $form = array();
    $config = $this->getOptions();
    ctools_include('dependent');

    $form['conditional_type'] = array(
      '#title' => t('Event'),
      '#type' => 'select',
      '#options' => array(
        '',
        'started' => t('User started course object.'),
        'completed' => t('User completed course object.'),
      ),
      '#description' => t('This sets the conditional behavior.'),
    );

    $options = array('');
    foreach ($this->getCourseObject()->getCourse()->getObjects() as $courseObject) {
      $options[$courseObject->getId()] = $courseObject->getTitle();
    }

    if (module_exists('duration_element')) {
      $conditional_time = duration_create();
      $conditional_time->set_seconds($config['conditional_time']);
      $conditional_time->normalize();
      $form['conditional_time'] = array(
        '#title' => t('Time'),
        '#type' => 'duration_combo',
        '#description' => t('Length of time after the event happens when this course object should be accessible.'),
        '#largest_metric' => 'days',
        '#smallest_metric' => 'minutes',
        '#default_value' => $conditional_time,
      );
    }
    else {
      $form['conditional_time'] = array(
        '#title' => t('Time'),
        '#type' => 'textfield',
        '#size' => 8,
        '#description' => t('Length of time in seconds after the event happens when this course object should be accessible.'),
        '#default_value' => $config['conditional_time'],
      );
    }

    $form['conditional_object'] = array(
      '#title' => t('Course object'),
      '#type' => 'select',
      '#options' => $options,
      '#description' => t('The course object to check for the type and time of conditional display.'),
    );

    $form['conditional_hidden'] = array(
      '#title' => t('Hide object until ready'),
      '#type' => 'checkbox',
      '#description' => t('This will prevent the course object from appearing in the course outline until it is ready.'),
    );

    $conditional_dependent = array(
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-plugins-access-conditional-conditional-type' => array('completed', 'started')),
    );

    foreach (element_children($form) as $key) {
      if ($key != 'conditional_time') {
        $form[$key]['#default_value'] = $config[$key];
      }

      if ($key != 'conditional_type' && $key != 'conditional_time') {
        $form[$key] += $conditional_dependent;
      }
    }

    return $form;
  }

}
