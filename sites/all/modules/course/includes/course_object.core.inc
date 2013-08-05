<?php

/**
 * Parent abstract base class of all course objects.
 *
 * Represents a course object in the database.
 *
 * Also holds a fulfillment record if a user is given.
 */
abstract class CourseObject extends CourseHandler {

  // Hold course.
  private $course = NULL;
  // Hold course object from database and the fulfillment object.
  private $courseObjectFulfillment = NULL;
  // Hold a user.
  protected $user = NULL;

  /**
   * Construct a course object from a database record.
   *
   * Use course_get_course_object to load an object when parameters are not
   * already known.
   *
   * @param array $config
   *   An object with object_type, module, and instance
   * @param object $account
   *   (optional) A user account object.
   * @param Course $course
   *   (optional) An instantiated Course object.
   */
  function __construct($config = array(), $account = NULL, Course $course = NULL) {
    $config = (array) $config;

    $this->serializedField = 'data';
    $this->handlerType = 'course_object';
    $this->primaryKey = 'coid';
    $this->table = 'course_outline';


    // Pass configuration to parent.
    parent::__construct($config);

    if (!$account) {
      global $user;
      $account = $user;
    }
    $this->user = $account;

    if ($course) {
      $this->course = $course;
    }

    $this->courseObjectFulfillment = new CourseObjectFulfillment($this, $account);
  }

  /**
   * Override navigation links.
   *
   * @return array
   *   An array of navigation links. Keyed values will override matching items
   *   in Course::getNavigation().
   */
  public function overrideNavigation() {
    return array();
  }

  /**
   * Specify whether fulfillment uses asynchronous polling.
   *
   * @return bool
   *   Whether this object uses polling. Defaults to FALSE.
   */
  public function hasPolling() {
    return FALSE;
  }

  /**
   * Overrides a course outline list item.
   *
   * @param array $item
   *   A course outline list item. The structure mirrors an array element from
   *   the $items param from theme_item_list().
   */
  public function overrideOutlineListItem(&$item) {

  }

  /**
   * Access functionality for course objects.
   *
   * Possible values for $op are 'see', 'view', 'take'.
   *
   * "see" means see it in a course outline. For example, a conditional survey
   * should not be seen in the course outline. A quiz at the end of the course,
   * should show up, but the user must not be able to take it.
   *
   * "view" means view and interact with the object, but nothing would be
   * recorded. For example, accessing a quiz but not being able to submit
   * responses.
   *
   * "take" means interact with the object in a way that records data.
   *
   * Subclasses may override this functionality.
   */
  public function access($op = 'view', $account = NULL) {
    ctools_include('plugins');
    $access = FALSE;

    if (!$account) {
      global $user;
      $account = $user;
    }

    // Stock access: check for completion of previous object.
    switch ($op) {
      case 'see':
        $access = !$this->getOption('hidden') && $this->getOption('enabled');
        break;
      case 'take':
        if (!$account->uid) {
          return FALSE;
        }
      case 'view':
        // Get a copy of the course, so we can run setActive() without changing
        // the global course.
        $course = clone $this->getCourse();
        $course->setActive($this->getId());


        $courseObjects = $course->getObjects();
        if ($courseObjects && $courseObjects[0]->getId() == $this->getId()) {
          // This is the first course object. The learner should be able to
          // access it. Also, check for enrollment. Since there are no other
          // checks (as in previous object completions) we do have to check this.
          $access = course_enrolment_check($this->getCourseNid(), $account->uid);
        }

        if ($course->getPrev() && !$course->getPrev()->isRequired()) {
          // Previous step was not required.
          $access = TRUE;

          // But we need to see if at least one required step was completed (or the start of the course).
          $objects = array_reverse($course->getObjects());
          $check = FALSE;
          foreach ($objects as $object) {
            if ($check) {
              if ($object->isRequired()) {
                // Object is required.
                if (!$object->getFulfillment()->isComplete()) {
                  // Found a required object that was not complete.
                  $access = FALSE;
                  break;
                }
                else {
                  // The last required object was completed.
                  $access = TRUE;
                  break;
                }
              }
            }
            if ($object->getId() == $this->getId()) {
              // We found the object we are trying to check access on.
              // Now we want to go backwards.
              $check = 1;
            }
          }
        }

        if ($course->getPrev() && $course->getPrev()->getFulfillment()->isComplete()) {
          // If last object was complete, and we are on the current object,
          // grant access.
          $access = TRUE;
        }
    }

    // Plugin access.

    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      $class = ctools_plugin_get_class($plugin, 'handler');
      $accessPluginDefaults = array();
      if (isset($this->config['plugins']['access'][$key])) {
        $accessPluginDefaults = (array) $this->config['plugins']['access'][$key];
      }
      $accessPlugin = new $class();
      if ($accessPluginDefaults) {
        $accessPlugin->setOptions($accessPluginDefaults);
      }
      $accessPlugin->setCourseObject($this);

      // Run access check.
      $ret = $accessPlugin->$op();

      if ($ret === FALSE) {
        // If previous access was granted, revoke it.
        $access = $ret;
      }
    }

    return $access;
  }

  public function isActive() {
    return $this->getCourse()->current()->getId() == $this->getId();
  }

  /**
   * Define configuration elements and their defaults.
   *
   * Extended classes should call parent::optionsDefinition first to get the
   * parent's configuration.
   */
  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults += array(
      'uniqid' => NULL,
      'nid' => NULL,
      'title' => NULL,
      'enabled' => 1,
      'hidden' => 0,
      'required' => 1,
      'delete' => 0,
      'delete_instance' => 0,
      'grade_include' => 0,
      'instance' => NULL,
    );

    return $defaults;
  }

  /**
   * Default options form for all course objects.
   */
  public function optionsForm(&$form, &$form_state) {
    ctools_include('dependent');
    ctools_include('plugins');
    parent::optionsForm($form, $form_state);

    $config = $this->getOptions();

    $form['header']['#value'] = t("<h2>Settings for %t</h2>", array('%t' => $this->getTitle()));

    $form['uniqid'] = array(
      '#type' => 'hidden',
      '#value' => arg(4),
    );

    $form['nid'] = array(
      '#type' => 'hidden',
      '#value' => arg(1),
    );

    $form['title'] = array(
      '#title' => 'Title',
      '#type' => 'textfield',
      '#size' => 100,
      '#default_value' => check_plain($config['title']),
    );

    $form['enabled'] = array(
      '#title' => 'Enabled',
      '#type' => 'checkbox',
      '#default_value' => $config['enabled'],
    );

    $form['hidden'] = array(
      '#title' => t('Visible in outline'),
      '#type' => 'checkbox',
      //'#description' => t('Hide this course object in the course outline. If this object is required, progress through the course will be blocked until this object is complete.'),
      '#default_value' => !$config['hidden'],
    );

    $form['required'] = array(
      '#title' => t('Completion required'),
      '#type' => 'checkbox',
      '#default_value' => $config['required'],
    );


    $form['delete'] = array(
      '#title' => t('Delete'),
      '#type' => 'checkbox',
      '#default_value' => $config['delete'],
    );

    // Only allow deletion of existing instances.
    if (!empty($config['instance'])) {
      $form['delete_instance'] = array(
        '#title' => t('Also delete related object instance(s)'),
        '#type' => 'checkbox',
        '#default_value' => $config['delete_instance'],
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array('edit-delete' => array(1)),
      );

      // Check for multiple instances.
      $sql = "SELECT count(coid) FROM {course_outline} WHERE module = '%s' AND object_type = '%s' AND instance = '%s'";
      if (db_result(db_query($sql, $config['module'], $config['object_type'], $config['instance'])) > 1) {
        $form['delete_instance']['#description'] = t('<span class="error"><strong>WARNING</strong></span>: multiple course objects link to this instance. Deleting the instance might break the other course objects that use it.');
      }
    }

    if ($this->isGraded()) {
      $form['grading'] = array(
        '#title' => 'Grading',
        '#type' => 'fieldset',
        '#description' => t('Settings for graded objects.'),
      );

      $form['grading']['grade_include'] = array(
        '#title' => 'Include in final course grade',
        '#description' => 'Include this grade result for calculation of the final course grade.<br/>Currently, only the last grade result per Course will be used.',
        '#default_value' => $config['grade_include'],
        '#type' => 'checkbox',
      );
    }

    // Bring in access plugin configuration.
    $form['plugins']['#tree'] = TRUE;
    $form['plugins']['#weight'] = 998;
    $form['plugins']['access']['#title'] = 'Access plugins';
    $form['plugins']['access']['#type'] = 'fieldset';
    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      $form['plugins']['access']['#tree'] = TRUE;
      $form['plugins']['access'][$key] = array(
        '#title' => $plugin['title'],
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#collapsible' => TRUE,
      );

      // Initialize access class.
      $class = ctools_plugin_get_class($plugin, 'handler');
      $courseAccess = new $class();
      if (!empty($config['plugins']['access'][$key])) {
        $courseAccess->setOptions($config['plugins']['access'][$key]);
      }
      $courseAccess->setCourseObject($this);

      // Add access plugin form to our form.
      $form['plugins']['access'][$key] += $courseAccess->optionsForm();
    }

    // Update settings
    $form['update'] = array(
      '#value' => t('Update'),
      '#weight' => 999,
      '#type' => 'submit',
    );
  }

  public function optionsValidate(&$form, &$form_state) {
    // Pass validation to plugins.
    ctools_include('plugins');
    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      $values = & $form_state['values']['plugins']['access'][$key];
      $class = ctools_plugin_get_class($plugin, 'handler');
      $instance = new $class($values);
      $instance->optionsValidate($form['plugins']['access'][$key], $form_state['values']['plugins']['access'][$key]);
    }
  }

  /**
   * Save object configs to cache.
   */
  public function optionsSubmit(&$form, &$form_state) {
    ctools_include('plugins');

    $uniqid = $this->getId();
    $nid = $this->getCourseNid();

    // Flip 'visible' so it behaves like 'hidden'.
    $form_state['values']['hidden'] = ($form_state['values']['hidden'] != 1);

    // Object-specific settings
    foreach (array_keys($this->getOptions()) as $key) {
      if (!is_null($form_state['values'][$key])) {
        $_SESSION['course'][$nid]['editing'][$uniqid][$key] = $form_state['values'][$key];
      }
    }

    // Save plugin info.
    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      $_SESSION['course'][$nid]['editing'][$uniqid]['plugins']['access'][$key] = $form_state['values']['plugins']['access'][$key];
    }

    // Update the options.
    $this->setOptions($_SESSION['course'][$nid]['editing'][$uniqid]);

    // Perform ajax operations on the overview form, before overview submission.
    if ($form_state['ajax']) {
      $commands = array();

      // Set title.
      $title = '<div id="title-' . $uniqid . '">' . $form_state['values']['title'] . '</div>';
      $commands[] = ctools_ajax_command_replace("#title-{$uniqid}", $title);

      // Set summary.
      $html = '<div id="summary-' . $uniqid . '">' . $this->renderOptionsSummary() . '</div>';
      $commands[] = ctools_ajax_command_replace("#summary-{$uniqid}", $html);

      // Deletion handling.
      if ($form_state['values']['delete']) {
        // Add `deleted` class to the row. An admin stylesheet is loaded from
        // theme_course_outline_overview_form().
        $commands[] = ctools_ajax_command_attr("#row-{$uniqid}", 'class', 'deleted');
      }

      $commands[] = ctools_modal_command_dismiss();

      ctools_ajax_render($commands);
    }
  }

  /**
   * Get core options summary.
   *
   * @return array
   *   An associative array of summary keys and values.
   */
  public function getOptionsSummary() {
    $summary = parent::getOptionsSummary();

    // Get options.
    $options = $this->getOptions();

    // Get form for titles.
    $form = array();
    $this->optionsForm($form, $form_state);

    // Add course object core options to summary individually, because there are
    // options we don't want to display, and others that require special logic.
    $uniqid = $options['uniqid'];

    // Enabled.
    if ($options['enabled']) {
      $summary['enabled'] = $form['enabled']['#title'];
    }
    else {
      $summary['enabled'] = '<span class="warning">' . t('Not enabled') . '</span>';
    }

    // Hidden.
    if (!$options['hidden']) {
      $summary['hidden'] = $form['hidden']['#title'];
    }
    else {
      $summary['hidden'] = '<span class="warning">' . t('Not visible in outline') . '</span>';
    }

    // Required.
    if ($options['required']) {
      $summary['required'] = $form['required']['#title'];
    }
    else {
      $summary['required'] = '<span class="warning">' . t('Completion not required') . '</span>';
    }

    // Instance edit link.
    $editUrl = $this->getEditUrl();
    if (!empty($editUrl)) {
      $text = t('Edit instance');
      $summary['instance'] = l($text, $editUrl, array('external' => TRUE, 'query' => drupal_get_destination()));
    }
    elseif ($this->isTemporary()) {
      $summary['instance'] = '<span class="warning">' . t('Save course to edit object') . '</span>';
    }

    // Required.
    if (!empty($options['delete'])) {
      $dest = "node/{$options['nid']}/course-object/nojs/{$uniqid}/restore";
      $text = t('Object will be removed from outline');
      $restore_text = t('Restore this object to the course outline.');
      if ($options['delete_instance']) {
        $text = t('Object will be removed from outline, and related instance(s) will be deleted');
        $restore_text = t('Restore this object and related instance(s) to the course outline.');
      }
      $restore = ctools_ajax_text_button(t('Restore'), $dest, $restore_text);
      $summary['delete'] = '<span class="error">' . $text . '</span>' . ' ' . $restore;
    }

    return $summary;
  }

  /**
   * Get all course object implementations of getOptionsSummary().
   */
  public function renderOptionsSummary() {
    ctools_include('plugins');
    $summary = $this->getOptionsSummary();

    // Get summaries from plugins, and merge into the summary.
    foreach (ctools_get_plugins('course', 'access') as $key => $plugin) {
      // @todo how do we get these?
    }

    // @todo make this a themeable function.
    //return theme('course_object_summary');
    $rendered_summary = '';
    if (!empty($summary)) {
      $rendered_summary = $html = '<div class="description">' . theme('item_list', $summary) . '</div>';
    }

    return $rendered_summary;
  }

  /**
   * Get options, with session options having precedence.
   */
  public function getOptions() {
    $options = parent::getOptions();
    $sessionDefaults = array();
    if (isset($options['nid']) && isset($options['coid']) && isset($_SESSION['course'][$options['nid']]['editing'][$options['coid']])) {
      $sessionDefaults += array_filter((array) $_SESSION['course'][$options['nid']]['editing'][$options['coid']], array($this, 'optionFilter'));
    }
    return array_merge($options, (array) $sessionDefaults);
  }

  private function optionFilter($a) {
    return !is_null($a);
  }

  /**
   * Take a course object.
   *
   * - Set the session of this course object being taken. This allows for
   *   non-node objects to be tracked.
   * - Delegate the course object take functionality
   *
   * @return mixed
   *   HTML content or a redirect.
   */
  public final function takeCourseObject() {
    $_SESSION['course']['active'] = $this->getCourseNid();
    $_SESSION['course'][$this->getCourseNid()]['taking']['active'] = $this->getId();

    // Run access checks.
    if ($this->access('take')) {
      // Grant access to external course object.
      $this->grant();

      // Record start date.
      if (!$this->getFulfillment()->getOption('date_started')) {
        $this->getFulfillment()->setOption('date_started', time());
      }
    }
    else {
      // User can't access this object, revoke access.
      $this->revoke();
      return FALSE;
    }

    // Save fulfillment record.
    $this->getFulfillment()->save();

    // If we're not displaying any content but we want to fire take() anyway, to
    // let the course object know we sent the user.
    $out = $this->take();

    $url = $this->getTakeUrl();
    switch ($this->getTakeType()) {
      case 'iframe':
        return course_iframe($url);
      case 'popup':
        return "will popup $url";
      case 'content':
        return $out;
      case 'redirect':
      default:
        // This URL should have already been url()'d (it might be external).
        session_write_close();
        header("Location: $url");
        exit;
    }
  }

  /**
   * How should this course object be executed?
   *
   * - iframe: display an iframe with getTakeUrl() in it
   * - popup: launch getTakeUrl() in a popup
   * - modal: launch getTakeUrl() in a modal
   * - content: print the value from take() (or do whatever the module wants to
   *   do)
   */
  public function getTakeType() {
    return 'content';
  }

  /**
   *
   */
  public function take() {
    return 'This should be overridden by the module to return course content.';
  }

  /**
   * Return the URL to the course object router.
   */
  public function getUrl() {
    return 'node/' . $this->getCourseNid() . '/course-object/' . $this->getId();
  }

  /**
   * Get the URL to take this course object, if any.
   *
   * Outline handlers or subclasses should use getUrl().
   *
   * @return string
   */
  protected function getTakeUrl() {

  }

  /**
   * Get the URL to edit this course object, if any.
   *
   * @return string
   */
  public function getEditUrl() {

  }

  /**
   * Is this course object required for course completion?
   *
   * @return bool
   */
  public function isRequired() {
    return (bool) $this->getOption('required');
  }

  /**
   * Is this object graded?
   *
   * Returning TRUE here will cause some more configurations to show on the
   * object's form.
   *
   * @return bool
   */
  function isGraded() {
    return FALSE;
  }

  /**
   * Get the user's status in this course object.
   *
   * This is how an object would notify the user why they cannot proceed to the
   * next step from the course outline. For example, if this was a quiz and
   * they failed, this should let them know.
   */
  public function getStatus() {

  }

  /**
   * Get this course object's fulfillment object.
   *
   * @return CourseObjectFulfillment
   */
  public function getFulfillment() {
    return $this->courseObjectFulfillment;
  }

  /**
   * Get the instance ID. This could be the external component ID, a Node ID...
   *
   * @return int
   */
  function getInstanceId() {
    return $this->getOption('instance');
  }

  /**
   * Set this object's instance ID.
   *
   * @param mixed $id The external ID of this course object.
   */
  function setInstanceId($id) {
    return $this->setOption('instance', $id);
  }

  /**
   * Get the course node ID this CourseObject belongs to.
   *
   * @return int
   */
  function getCourseNid() {
    return intval($this->getOption('nid'));
  }

  /**
   * Set the Course for this CourseObject.
   *
   * @param Course|int $course
   *   A Course or node ID.
   *
   * @return CourseObject
   */
  public function setCourse($course) {
    if (is_numeric($course)) {
      $this->setOption('nid', $course);
      $courseNode = node_load($course);
      $this->course = course_get_course($courseNode);
    }
    else {
      $this->course = $course;
      $this->setOption('nid', $course->getNode()->nid);
    }

    return $this;
  }

  /**
   * Get the Course that contains this CourseObject.
   *
   * @return Course
   */
  function getCourse() {
    if (!$this->course) {
      $nid = $this->getCourseNid();
      if ($nid) {
        $this->course = new Course(node_load($nid), $this->user);
      }
    }

    return $this->course;
  }

  /**
   * Get the module that provides this course object.
   *
   * @return string
   */
  function getModule() {
    return $this->getOption('module');
  }

  /**
   * Get the object component for this course object.
   *
   * @return string
   */
  function getComponent() {
    return $this->getOption('object_type');
  }

  /**
   * Set the module that provides this course object.
   *
   * @return CourseObject
   */
  function setModule($module) {
    return $this->setOption('module', $module);
  }

  /**
   * Set the object component for this course object.
   *
   * @return CourseObject
   */
  function setComponent($component) {
    return $this->setOption('object_type', $component);
  }

  /**
   * Set the internal course object ID.
   *
   * @param int $coid
   *   ID of the course object.
   */
  function setId($coid) {
    return $this->setOption('coid', $coid);
  }

  /**
   * Set the user fulfilling/creating this course object.
   */
  function setUser($user) {
    $this->user = $user;
  }

  /**
   * Creates a course object.
   *
   * For example, this would create the new node and return the node ID if this
   * was a CourseObjectNode.
   *
   * Do not confuse this with save(), which saves the course outline record for
   * tracking.
   *
   * Course objects should call setInstanceId() if this is a course object
   * that creates external resources.
   */
  public function create() {
    //$this->setInstanceId($id);
  }

  /**
   * Deletes a course object's external resources.
   *
   * For example, this would delete the associated node (if this was a
   * CourseObjectNode) and delete all other associated data.
   */
  public function delete() {
    //something_delete($this->getInstanceId());
  }

  function getTitle() {
    $object_info = course_get_handlers('object');

    // If title is not specified, set title from component.
    if (!$this->getOption('title')) {
      // Get the component name from object info.
      $title = $object_info[$this->getOption('module')][$this->getOption('object_type')]['name'];
      $this->setOption('title', $title);
    }

    return $this->getOption('title');
  }

  /**
   * Give the course object a chance do asynchronous polling and set completion
   * on demand.
   *
   * Useful for external objects.
   */
  function poll() {

  }

  /**
   * Grant access to the external course object.
   *
   * For example, adding a user to an access control list.
   *
   * @see CourseObjectNode::grant()
   */
  function grant() {

  }

  /**
   * Revoke access to the external course object.
   *
   * For example, removing a user to an access control list.
   *
   * @todo This isn't called anywhere useful, yet.
   * @see CourseObjectNode::revoke()
   */
  function revoke() {

  }

  /**
   * Let the course object provide its own reports.
   *
   * @return array
   *   An array indexed by report key, containing 'title' which is the menu link
   *   in the course object reports.
   *
   * @see CourseObjectQuiz::getReports()
   */
  function getReports() {
    return array();
  }

  /**
   * Let the course object provide its own reports.
   *
   * @return array
   *   An array containing:
   *     - title: The title of this report as show on the page
   *     - content: Content to be displayed.
   *     - url: URL to be loaded in an iframe.
   *   Reports should return either 'content' or 'url'.
   *
   * @see CourseObjectQuiz::getReports()
   */
  function getReport($key) {
    return array();
  }

  function freeze() {

  }

  function thaw() {

  }

  /**
   * Returns an translated error message if this object has issues with cloning.
   *
   * @return mixed
   *   TRUE if cloning is supported, FALSE if cloning is not supported. A string
   *   if the object can clone but with caveats.
   */
  function getCloneAbility() {
    return t('The course object %object cannot be cloned. A new instance will be created.', array('%object' => $this->getTitle()));
  }

  /**
   * Let objects create their instances before saving the course object.
   */
  public function save() {
    // If there is no title, set it.
    $this->getTitle();

    if ($ice = $this->getOption('freeze')) {
      // Found frozen data.
      $this->setInstanceId($this->thaw($ice));
      $this->setOption('freeze', NULL);
    }

    // If there is no instance ID, create one.
    if (!$this->getInstanceId()) {
      $this->create();
    }

    // Set the ID to NULL because this is a temporary course object being
    // created for the first time.
    if (strpos($this->getId(), 'course_object_') !== FALSE) {
      $this->setId(NULL);
    }

    return parent::save();
  }

  /**
   * Remove any records associated with this course object for the user.
   *
   * For example, deleting quiz results when a user is removed from the course.
   */
  function unEnroll() {

  }

  /**
   * Checks the temporary status of a course object.
   */
  function isTemporary() {
    return strpos($this->getId(), 'course_object_') === 0;
  }

  /**
   * Return the number of occurances that can be in a course at the same time.
   * For example, the design of the Certificate module can only have 1 set of
   * mappings per node. The same goes for Course Credit. We may also want a
   * course object that can only be added twice (for example, a before/after
   * comparison).
   *
   * This method is static because we might have to call it without an object
   * being instantiated.
   */
  public static function getMaxOccurences() {
    return FALSE;
  }

}

/**
 * Parent class for course object fulfillment.
 *
 * Represents the fulfillment record in the database.
 *
 */
class CourseObjectFulfillment extends CourseHandler {

  private $courseObject;

  /**
   * Construct the fulfillment object.
   *
   * A CourseObject and user are required to construct a fulfillment object.
   *
   * @param CourseObject $courseObject
   * @param Object $user
   */
  function __construct(CourseObject $courseObject, $user) {
    // Set storage.
    $this->handlerType = 'course_object_fulfillment';
    $this->table = 'course_outline_fulfillment';
    $this->primaryKey = 'cofid';
    $this->serializedField = 'info';

    $this->courseObject = $courseObject;
    $sql = "SELECT * FROM {course_outline_fulfillment} WHERE coid = %d AND uid = %d";
    $fulfillment = db_fetch_array(db_query($sql, $this->courseObject->getId(), $user->uid));

    $this->setOptions($fulfillment);

    // Set course object ID.
    $this->setOption('coid', $this->courseObject->getId());
    $this->setOption('uid', $user->uid);
  }

  /**
   * Is this fulfillment complete?
   *
   * @return bool
   */
  function isComplete() {
    return (bool) $this->getOption('complete');
  }

  /**
   * Set this fulfillment complete.
   *
   * @param bool $complete
   *   Set to 0 to un-complete, 1 or omit to complete.
   */
  function setComplete($complete = 1) {
    if (!$this->getOption('date_completed')) {
      $this->setOption('date_completed', time());
    }

    return $this->setOption('complete', $complete);
  }

  /**
   * Set this fulfillment's grade.
   *
   * @param float $grade
   */
  function setGrade($grade) {
    return $this->setOption('grade_result', $grade);
  }

  /**
   * Get this fulfillment's grade.
   *
   * @return float
   *   A float value of the user's grade for this fulfillment.
   */
  function getGrade() {
    return $this->getOption('grade_result');
  }

  /**
   * Get this fulfillment's course object.
   */
  function getCourseObject() {
    return $this->courseObject;
  }

  public function delete() {
    $sql = "DELETE FROM {course_outline_fulfillment} WHERE cofid = %d";
    db_query($sql, $this->getId());
  }

  /**
   * Track course after saving fulfillment.
   */
  public function save() {
    parent::save();
    $this->getCourseObject()->getCourse()->track();
  }

}

/**
 * A course object that uses a node as a base.
 */
abstract class CourseObjectNode extends CourseObject {

  protected $node;

  public function __construct($object, $user = NULL, $course = NULL) {
    // Pass configuration to parent.
    parent::__construct($object, $user, $course);
    $this->node = node_load($this->getInstanceId());
  }

  public function hasNodePrivacySupport() {
    return module_exists('content_access') && module_exists('acl');
  }

  /**
   * Return a list of valid node types.
   *
   * @return array
   *   An array keyed by machine name of a node type.
   *   return array(
   *     'my_event' => 'My event content type',
   *   );
   */
  public abstract function getNodeTypes();

  /**
   * Simple node course object behavior is to just redirect to the node.
   */
  public function getTakeType() {
    return 'redirect';
  }

  public function getTakeUrl() {
    if ($this->getInstanceId()) {
      return url("node/{$this->node->nid}");
    }
  }

  public function getEditUrl() {
    if ($this->getInstanceId()) {
      return url("node/{$this->node->nid}/edit");
    }
  }

  public function create() {
    $node = new stdClass;
    $node->type = $this->getOption('node_type');
    $node->title = $this->getTitle();
    $node->uid = $this->user->uid;
    node_save($node);
    $this->setNode($node);
  }

  /**
   * Set the node and instance ID (node ID) of this CourseObjectNode.
   *
   * @param mixed $node
   *   A node or node ID.
   */
  public function setNode($node) {
    $this->node = $node;
    $this->setInstanceId($this->node->nid);
  }

  /**
   * Destroy the node instance.
   */
  public function delete() {
    node_delete($this->getInstanceId());
  }

  public function optionsDefinition() {
    $defaults = parent::optionsDefinition();

    $defaults['private'] = 0;

    $options = array_intersect_key(node_get_types('names'), $this->getNodeTypes());
    $defaults['node_type'] = key($options);

    return $defaults;
  }

  public function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    $config = $this->getOptions();

    if (!$this->getInstanceId()) {
      $types = drupal_map_assoc($this->getNodeTypes());
      $options = array_intersect_key(node_get_types('names'), $types);
      if (count($options) > 1) {
        array_unshift($options, '- select -');
      }
      $form['node_type'] = array(
        '#title' => 'Create node',
        '#type' => 'select',
        '#options' => $options,
        '#description' => 'Selecting a node type will automatically create this node and link it to this course object.',
        '#default_value' => $config['node_type'],
      );
      if (count($options) > 1) {
        $form['node_type']['#required'] = TRUE;
      }
    }

    $form['node'] = array(
      '#type' => 'fieldset',
      '#title' => 'Node',
      '#description' => ('Settings for node-based objects.'),
    );

    $form['node']['instance'] = array(
      '#title' => 'Existing node',
      '#autocomplete_path' => 'course/autocomplete/node/' . implode(',', $this->getNodeTypes()),
      '#type' => 'textfield',
      '#description' => t('Use an existing node instead of creating a new one.'),
    );

    $form['node']['private'] = array(
      '#title' => t('Private'),
      '#description' => $this->hasNodePrivacySupport() ? t('This content will not be available to users who are not enrolled in this course.') : t('You must enable content_access and acl in order to restrict course content to users who are enrolled in this course.'),
      '#type' => 'checkbox',
      '#default_value' => $config['private'],
      '#disabled' => !($this->hasNodePrivacySupport()),
    );

    $nid = $this->getInstanceId();
    if ($nid) {
      $node = node_load($nid);
      $link = l(t("'%title' [node id %nid]", array('%title' => $node->title, '%nid' => $node->nid)), "node/$node->nid", array('attributes' => array('target' => '_blank', 'title' => t('Open in new window')), 'html' => TRUE));
      $form['node']['instance']['#description'] = t('Currently set to !link', array('!link' => $link));
    }
  }

  /**
   * Validate the options form. Check the node type.
   */
  public function optionsValidate(&$form, &$form_state) {
    if (isset($form_state['values']['node_type']) && empty($form_state['values']['node_type'])) {
      form_set_error('node_type', 'Please select a node type.');
    }
  }

  public function optionsSubmit(&$form, &$form_state) {
    $nid = $form_state['values']['instance'];

    if (!is_numeric($nid)) {
      preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $nid, $matches);
      $nid = $matches[2];
    }

    if ($nid) {
      $form_state['values']['instance'] = $nid;
    }
    else {
      // Unset it, or we'll erase the relationship (since the textfield is
      // actually blank).
      unset($form_state['values']['instance']);
    }

    parent::optionsSubmit($form, $form_state);
  }

  public function getWarnings() {
    $warnings = parent::getWarnings();
    if ($this->getOption('private')) {
      $settings = variable_get('content_access_settings', array());

      if (!$settings['per_node'][$this->getComponent()]) {
        $warnings[] = t('%t is set to Private, but the content type %c does not have access control lists enabled. Users will not be able to acces this content. Please visit !l to set up content access settings.', array(
          '%t' => $this->getTitle(),
          '%c' => $this->getComponent(),
          '!l' => l('Access control', "admin/content/node-type/{$this->getComponent()}/access"),
          ));
      }
    }
    return $warnings;
  }

  /**
   * Grant access to course content before going to it.
   */
  function grant() {
    if ($this->hasNodePrivacySupport()) {
      if ($this->getOption('private')) {
        $uid = $this->user->uid;
        module_load_include('inc', 'content_access', 'content_access.admin');
        $acl_id = content_access_get_acl_id($this->node, 'view');
        acl_add_user($acl_id, $uid);
        acl_node_add_acl($this->node->nid, $acl_id, 1, 0, 0, content_access_get_settings('priority', $this->node->type));
        node_save($this->node);
      }
    }
  }

  /**
   * Duration expired (or something) - CourseObject is telling us so.
   */
  function revoke() {
    if ($this->hasNodePrivacySupport()) {
      if ($this->getOption('private')) {
        $uid = $this->user->uid;
        module_load_include('inc', 'content_access', 'content_access.admin');
        $acl_id = content_access_get_acl_id($this->node, 'view');
        acl_remove_user($acl_id, $uid);
        node_save($this->node);
      }
    }
  }

  /**
   * On object write, set privacy on this node.
   */
  function save() {
    parent::save();
    if ($this->hasNodePrivacySupport() && $this->getOption('private')) {
      // Ensure that per-node access is enabled.
      $global_settings = content_access_get_settings();
      $global_settings['per_node'][$this->node->type] = 1;
      content_access_set_settings($global_settings);

      // Remove "view" permissions to everyone on this node.
      $settings = content_access_get_per_node_settings($this->node);
      $settings['view'] = array();
      content_access_save_per_node_settings($this->node, $settings);

      // Resave node to update access.
      node_save($this->node);
    }
  }

  /**
   * Freeze data to persist over cloning/exporting.
   * @return array
   *   An array of data to be frozen.
   */
  function freeze() {
    if ($this->node->nid != $this->getCourse()->getNode()->nid) {
      // Don't freeze the course, if this course is part of the objects.
      $ice = new stdClass;
      $ice->node = $this->node;
      return $ice;
    }
  }

  /**
   * Thaw data frozen from an earlier export/clone.
   *
   * @param array $data
   *   Unfrozen data.
   *
   * @return int
   *   The new instance ID.
   */
  function thaw($ice) {
    $this->node = $ice->node;
    unset($this->node->nid);
    node_save($this->node);
    return $this->node->nid;
  }

  function getCloneAbility() {
    return t('%object will be cloned as a node. Results may vary.', array('%object' => $this->getTitle()));
  }

}
