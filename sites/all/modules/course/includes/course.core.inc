<?php

/**
 * @file course.core.inc
 * File for main Course class.
 */
class CourseHandler {

  // Could be 'outline', 'course_object', 'settings'...
  public $handlerType = NULL;
  // For objects that store things in the database, this is the field where all
  // non-schema fields will be serialized to.
  public $table = NULL;
  public $primaryKey = NULL;
  public $serializedField = NULL;
  // Configuration for this handler.
  protected $config = array();
  private $accessMessages = array();

  function __construct($config = array()) {
    foreach ($config as $key => $value) {
      if ($key === $this->serializedField && !is_array($value)) {
        $data = unserialize($value);
        if (is_array($data)) {
          foreach ($data as $key2 => $value2) {
            $this->setOption($key2, $value2);
          }
        }
      }
      else {
        $this->setOptions((array) $config);
      }
    }
  }

  // Handlers must have an ID.
  function getId() {
    return $this->getOption($this->primaryKey);
  }

  /**
   * Stub. Get the summary of an object's options.
   *
   * @return array
   *   An associative array of summary keys and values.
   */
  public function getOptionsSummary() {
    $summary = array();

    foreach ($this->getWarnings() as $warning) {
      $warning = '<span class="error">' . $warning . '</span>';
      $summary['warnings'] = filter_xss_admin($warning);
    }

    return $summary;
  }

  /**
   * Get an object's configuration.
   *
   * This can be overridden. For example, values stored in courseobject sessions
   * need to have priority over those in the database.
   *
   * @return array
   */
  public function getOptions() {
    return array_merge($this->optionsDefinition(), (array) $this->config);
  }

  /**
   * Get an option stored in this CourseObject.
   *
   * @return mixed
   */
  public final function getOption($key) {
    $config = $this->getOptions();
    if (isset($config[$key])) {
      return $config[$key];
    }
    else {
      return NULL;
    }
  }

  /**
   * Set an option for this handler.
   *
   * @param string $option
   *   An option key.
   * @param mixed $value
   *   The option value.
   *
   * @return CourseHandler
   */
  public final function setOption($option, $value) {
    $this->config[$option] = $value;
    return $this;
  }

  /**
   * Set this entire handler's options.
   *
   * Deserialize the serialized column if necessary.
   *
   * @param array $options
   *   An array of options.
   *
   * @return CourseHandler
   */
  public final function setOptions($options) {
    $config = (array) $options;
    // Make sure the serialized field is not already extracted.
    if (isset($config[$this->serializedField]) && is_string($config[$this->serializedField])) {
      $data = unserialize($config[$this->serializedField]);
      if (is_array($data)) {
        // Merge serialized data onto options. Schema fields take precedence.
        $config = array_merge($data, $config);
      }
    }
    $this->config = $config;
    return $this;
  }

  /**
   * Merge an array of options onto the existing options.
   *
   * @param array $options
   *
   * @return CourseHandler
   *   Some type of CourseHandler (probably CourseObject or
   *   CourseObjectFulfillment)
   */
  public final function addOptions(array $options) {
    $this->config = $this->optionsMerge($this->config, $options);
    return $this;
  }

  /**
   * Merge arrays with replace, not append.
   *
   * @see http://www.php.net/manual/en/function.array-merge-recursive.php#102379
   */
  private function optionsMerge($Arr1, $Arr2) {
    foreach ($Arr2 as $key => $Value) {
      if (array_key_exists($key, $Arr1) && is_array($Value))
        $Arr1[$key] = $this->optionsMerge($Arr1[$key], $Arr2[$key]);

      else
        $Arr1[$key] = $Value;
    }

    return $Arr1;
  }

  /**
   * Handlers need to declare their defaults if they have a configuration form.
   */
  protected function optionsDefinition() {
    $options = array();

    return $options;
  }

  /**
   * Handlers can declare a form.
   */
  public function optionsForm(&$form, &$form_state) {

  }

  /**
   * Validate?
   */
  public function optionsValidate(&$form, &$form_state) {

  }

  /**
   * Save data somewhere.
   *
   * This can be overridden. For example, values stored in courseobject sessions
   * need to have priority over those in the database.
   */
  public function optionsSubmit(&$form, &$form_state) {

  }

  /**
   * Return an array of database fields. This determines what fields should be
   * serialized instead of stored.
   */
  protected function getDatabaseFields() {
    $schema = drupal_get_schema($this->table);
    return array_keys($schema['fields']);
  }

  /**
   * Return a list of warning strings about this handler.
   *
   * For example, if a user adds a quiz to a course with no questions, trigger a
   * message.
   *
   * @see CourseObjectQuiz
   * @see CourseObjectWebform
   */
  public function getWarnings() {
    return array();
  }

  /**
   * Set an access message to be displayed along with the course object when it
   * is in the outline. For example, "This activity will open on XYZ" or "Please
   * complete Step 1 to take this activity."
   *
   * @param string $key
   *   Message key.
   * @param string $message
   *   Message text.
   */
  public function setAccessMessage($key = NULL, $message = NULL) {
    if ($key == NULL) {
      return $this->accessMessages;
    }
    elseif ($message != NULL) {
      $this->accessMessages[$key] = $message;
    }
  }

  /**
   * Get an array of access messages.
   *
   * @return array
   */
  public function getAccessMessages() {
    return $this->setAccessMessage();
  }

  public function save() {
    $options = $this->getOptions();
    if (!isset($options['uuid'])) {
      $options['uuid'] = uuid_uuid();
    }

    // Set up serialized field for non-schema fields.
    $options[$this->serializedField] = array();

    $dbfields = $this->getDatabaseFields();
    foreach ($options as $key => $value) {
      if (array_search($key, $dbfields) === FALSE) {
        $options[$this->serializedField][$key] = $value;
      }
    }

    $keys = $this->getId() ? array($this->primaryKey) : array();
    drupal_write_record($this->table, $options, $keys);
    $this->setOptions($options);
    return $this;
  }

}

/**
 * Holds a user's total progress through a course and functionality to check
 * for completion of required objects.
 */
class CourseReport extends CourseHandler {

  private $course;

  /**
   * @param Course $course
   */
  public function __construct($course) {
    $this->primaryKey = 'crid';
    $this->handlerType = 'course_report';
    $this->serializedField = 'data';
    $this->table = 'course_report';

    $this->course = $course;

    $sql = "SELECT * FROM {course_report} WHERE nid = %d AND uid = %d";
    $result = db_query($sql, $this->course->getNode()->nid, $this->course->getUser()->uid);
    if ($config = db_fetch_array($result)) {
      parent::__construct($config);
    }
    else {
      parent::__construct(array(
        'nid' => $this->course->getNode()->nid,
        'uid' => $this->course->getUser()->uid,
      ));
    }
  }

  /**
   * Get the course of this tracker.
   *
   * @return Course
   */
  public function getCourse() {
    return $this->course;
  }

  /**
   * Track the course (scan required objects, update progress, completion, etc).
   */
  public function track() {
    $required = 0;
    $required_complete = 0;
    $prev = NULL;
    foreach ($this->course->getObjects() as $courseObject) {
      if (!$courseObject->getOption('enabled')) {
        continue;
      }

      if (!$prev) {
        $this->setOption('section_name', $courseObject->getTitle());
      }

      // Count required objects.
      $required += $courseObject->getOption('required');

      // Count completed required objects.
      $required_complete += ($courseObject->getOption('required') && $courseObject->getFulfillment()->isComplete());

      // Log last grade.
      if ($courseObject->isGraded() && $courseObject->getOption('grade_include')) {
        $this->setOption('grade_result', $courseObject->getFulfillment()->getOption('grade_result'));
      }

      if (!$courseObject->getFulfillment()->isComplete() && $prev && $prev->getFulfillment()->isComplete()) {
        $this->setOption('section_name', $courseObject->getTitle());
      }

      $prev = clone $courseObject;
    }

    if ($required_complete >= $required) {
      // Course requirements have been met.
      $this->setOption('section', 'complete');
      $this->setOption('section_name', 'Complete');
      $this->setOption('complete', 1);
      if (!$this->getOption('date_completed')) {
        $this->setOption('date_completed', time());
      }
    }

    $this->setOption('nid', $this->course->getNode()->nid);
    $this->setOption('uid', $this->course->getUser()->uid);
    module_invoke_all('course_report_presave', $this);
    $op = $this->getOption($this->primaryKey) ? 'update' : 'insert';
    $this->save();
    module_invoke_all("course_report_$op", $this);
  }

}

/**
 * An object that holds CourseObjects and tracker functions?
 */
class Course extends CourseHandler {

  // Node of course.
  private $node;
  // User in course.
  private $user;
  // Ordered list of course objects.
  private $courseObjects;
  // Course report tracker
  private $tracker;
  // The active course object.
  private $active = NULL;
  // The next course object.
  private $next;
  // The previous course object.
  private $prev;

  /**
   * @param stdClass $node
   * @param stdClass $user
   */
  public function __construct($node, $user = NULL) {
    $this->primaryKey = 'nid';
    $this->handlerType = 'course';
    $this->table = 'course_node';

    if (is_object($node)) {
      $this->node = $node;
    }
    else {
      $this->node = node_load($node);
    }

    if (is_object($user)) {
      $this->user = $user;
    }
    else {
      $this->user = user_load($user);
    }

    $sql = "SELECT * FROM {course_node} WHERE nid = %d";
    $result = db_query($sql, $this->node->nid);
    if ($config = db_fetch_array($result)) {
      parent::__construct($config);
    }
    else {
      parent::__construct(array(
        'nid' => $this->node->nid,
        'uid' => $this->user->uid,
      ));
    }

    $this->tracker = new CourseReport($this);
  }

  /**
   * Get the course tracker for this course/user.
   *
   * @return CourseReport
   */
  public function getTracker() {
    return $this->tracker;
  }

  /**
   * The Drupal path to take this course.
   *
   * @return string
   */
  public function getUrl() {
    return "node/{$this->node->nid}/takecourse";
  }

  /**
   * Set the active CourseObject in this Course.
   *
   * @param int $id
   *   A numeric course object ID.
   */
  public function setActive($id = NULL) {
    if (!$id && isset($_SESSION['course'][$this->node->nid]['taking']['active'])) {
      $id = $_SESSION['course'][$this->node->nid]['taking']['active'];
    }

    $old = NULL;
    $storeNext = FALSE;
    foreach ($this->getObjects() as $courseObject) {
      if ($id == $courseObject->getId()) {
        // Active - save old, store next.
        if ($old) {
          $this->prev = $old;
        }

        $storeNext = TRUE;
        $this->active = $courseObject;
      }
      elseif ($storeNext) {
        $this->next = clone $courseObject;
        $storeNext = FALSE;
      }

      $old = clone $courseObject;
    }
  }

  /**
   * Get the active CourseObject.
   *
   * @return CourseObject
   */
  public function getActive() {
    if (!$this->active) {
      $this->setActive();
    }

    return $this->active;
  }

  /**
   * Get the next course object, from the active course object.
   *
   * @return CourseObject
   */
  public function getNext() {
    if (!$this->active) {
      $this->setActive();
    }

    return $this->next;
  }

  /**
   * Get the previous course object, from the active course object.
   *
   * @return CourseObject
   */
  public function getPrev() {
    if (!$this->active) {
      $this->setActive();
    }

    return $this->prev;
  }

  /**
   * Generate navigation links.
   */
  public function getNavigation() {
    // Initialize the active Course.
    $this->setActive();

    $prev = $this->getPrev();
    $next = $this->getNext();

    $links = array();

    if ($prev) {
      $links['prev'] = l('Previous', $prev->getUrl(), array('html' => TRUE));
    }

    $links['back'] = l('Back to course', $this->getUrl());

    if ($next && $next->access('take')) {
      $links['next'] = l('Next', $next->getUrl(), array('html' => TRUE));
    }

    // Ask course objects if they want to override the navigation.
    if ($active = $this->getActive()) {
      foreach ($active->overrideNavigation() as $key => $link) {
        $links[$key] = $link;
      }
    }

    return $links;
  }

  /**
   * Track the course (scan required objects, update progress, completion, etc).
   */
  public function track() {
    $this->tracker->track();
  }

  /**
   * Get the course objects in this course.
   *
   * @return array
   *   An array of course objects.
   */
  public function getObjects($flush = FALSE) {
    if (!$this->courseObjects || $flush) {
      $this->courseObjects = array();
      $sql = 'SELECT * FROM {course_outline} co
        WHERE nid = %d
        ORDER BY weight ASC';
      $result = db_query($sql, $this->node->nid);

      while ($row = db_fetch_object($result)) {
        if ($courseObject = course_get_course_object($row, NULL, NULL, $this->user, $this)) {
          $this->courseObjects[] = $courseObject;
        }
      }
    }

    return $this->courseObjects;
  }

  public function getNode() {
    return $this->node;
  }

  public function getUser() {
    return $this->user;
  }

  /**
   * Un-enroll the user from all course objects and revoke access.
   *
   * Course object should clean up
   * and delete records related to this Course and user.
   *
   * NOT a top level class of CourseObject::unEnroll.
   *
   * @see CourseObjectNode::revoke()
   */
  public function unEnroll() {
    foreach ($this->getObjects() as $courseObject) {
      // Remove access.
      $courseObject->revoke();
      $courseObject->unenroll();
    }
  }

}

/**
 * Access handler for CourseObjects.
 *
 * Subtypes must define take(), see(), and view().
 */
abstract class CourseObjectAccess extends CourseHandler {

  private $courseObject;

  function __construct($config = array()) {
    $this->handlerType = 'course_access';
    parent::__construct($config);
  }

  public function setCourseObject($courseObject) {
    $this->courseObject = $courseObject;
  }

  public function getCourseObject() {
    return $this->courseObject;
  }

  abstract public function take();

  abstract public function see();

  abstract public function view();
}
