<?php

$plugin = array(
  'title' => t('Date-based access'),
  'description' => t('Controls access by release and expiration dates'),
  'handler' => array(
    'class' => 'CourseObjectAccessTiming',
  ),
);

class CourseObjectAccessTiming extends CourseObjectAccess {

  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults += array(
      'duration' => NULL,
      'release' => NULL,
      'expiration' => NULL,
      'release_hidden' => NULL,
      'expiration_hidden' => NULL,
    );

    return $defaults;
  }

  function take() {
    $time = time();

    if ($this->getOption('duration')) {
      if ($this->getCourseObject()->getFulfillment()->getOption('date_started')) {
        $duration_end = $this->getCourseObject()->getFulfillment()->getOption('date_started') + ($this->getOption('duration'));
        if ($time > $duration_end) {
          $duration_end_h = date('Y-m-d H:i:s', $duration_end);
          $this->getCourseObject()->setAccessMessage('duration-expired', 'Your enrollment in this activity expired on ' . $duration_end_h);
          return FALSE;
        }
      }
    }

    $released = $this->isReleased();

    $expired = $this->isExpired();

    return $released && !$expired;
  }

  function see() {
    if (!$this->isReleased() && $this->getOption('release_hidden')) {
      return FALSE;
    }
    if ($this->isExpired() && $this->getOption('expiration_hidden')) {
      return FALSE;
    }
  }

  function view() {
    return $this->take();
  }

  function optionsValidate(&$form, &$values) {
    if (module_exists('duration_element')) {
      $values['duration'] = $values['duration']->to_single_metric('seconds');
    }
  }

  function optionsForm() {
    $form = array();
    $config = $this->getOptions();

    if (module_exists('duration_element')) {
      $duration = duration_create();
      $duration->set_seconds($config['duration']);
      $duration->normalize();
      $form['duration'] = array(
        '#title' => t('Duration'),
        '#description' => 'Length of time a user can remain in this object before it is closed.',
        '#type' => 'duration_combo',
        '#largest_metric' => 'days',
        '#smallest_metric' => 'minutes',
        '#default_value' => $duration,
      );
    }
    else {
      $form['duration'] = array(
        '#title' => t('Duration'),
        '#description' => 'Length of time in seconds a user can remain in this object before it is closed.',
        '#type' => 'textfield',
        '#size' => 8,
        '#default_value' => $config['duration'],
      );
    }

    $form['release'] = array(
      '#title' => t('Release date'),
      '#description' => t('When this object can be accessed. If this object is required, users will not be able to proceed until after this date.'),
      '#type' => 'date_popup',
      '#default_value' => $config['release'],
    );

    $form['expiration'] = array(
      '#title' => t('Expiration date'),
      '#description' => t('When this object will close. If this object is required, users will not be able to proceed to the next activity after this date.'),
      '#type' => 'date_popup',
      '#default_value' => $config['expiration'],
    );

    $form['release_hidden'] = array(
      '#title' => 'Hide until release date',
      '#type' => 'checkbox',
      '#default_value' => $config['release_hidden'],
    );

    $form['expiration_hidden'] = array(
      '#title' => 'Hide after expiration date',
      '#type' => 'checkbox',
      '#description' => 'Hide the object after the expiration. For example, an optional pre-test that expires.',
      '#default_value' => $config['expiration_hidden'],
    );

    return $form;
  }

  function isReleased() {
    if (time() <= strtotime($this->getOption('release'))) {
      $this->getCourseObject()->setAccessMessage('not-open', t('This activity will be available on @release.', array('@release' => $this->getOption('release'))));
      return FALSE;
    }
    else {
      return TRUE;
    }
  }

  function isExpired() {
    if ($this->getOption('expiration') && time() > strtotime($this->getOption('expiration'))) {
      $this->getCourseObject()->setAccessMessage('closed', t('This activity closed on @expiration.', array('@expiration' => $this->getOption('expiration'))));
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

}
