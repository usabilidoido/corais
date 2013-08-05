<?php

/**
 * @file course_outline.inc
 *
 * Functions to show and edit graphical course outline.
 */

/**
 * Form constructor for course outline form.
 *
 * @see course_menu()
 * @see _course_outline_object_form()
 * @see theme_course_outline_overview_form()
 */
function course_outline_overview_form(&$form_state) {
  $form = array();

  // Get around ahah_helper PHP notices.
  // @see http://drupal.org/node/1371792
  $form['#action'] = '';
  $form['#id'] = '';

  // Register the form with ahah_helper so we can use it. Also updates
  // $form_state['storage'] to ensure it contains the latest values that have
  // been entered, even when the form item has temporarily been removed from
  // the form. So if a form item *once* had a value, you *always* can retrieve
  // it.
  ahah_helper_register($form, $form_state);

  // Determine the default value of the 'usage' select. When nothing is stored
  // in $form_state['storage'] yet, it's the form hasn't been submitted yet,
  // thus it's the first time the form is being displayed.
  // Load the modal library and add the modal javascript.
  ctools_include('ajax');
  ctools_include('modal');
  ctools_include('dependent');
  ctools_include('object-cache');
  ctools_modal_add_js();

  // Wrapper for objects and more button.
  $form['#tree'] = TRUE;
  $form['#prefix'] = '<div class="clear-block" id="course-outline-wrapper">';
  $form['#suffix'] = '</div>';

  // Shortcut.
  $cform = &$form['course_outline'];

  if (isset($form_state['values']['nid'])) {
    $node = node_load($form_state['values']['nid']);
  }
  else {
    $node = node_load(arg(1));
  }

  $course = course_get_course($node);

  // Check if "Add another" was clicked.
  if (isset($form_state['values']['op']) && $form_state['values']['op'] == 'Add another' && !empty($form_state['values']['more']['object_type'])) {
    // Ensure that we cached the course.
    course_editing_start($course);

    // Create a new course object in the session, and let the rest of the form
    // builder handle it.
    $obj_uniqid = uniqid('course_object_');
    $_SESSION['course'][$node->nid]['editing'][$obj_uniqid] = array();

    // Populate temporary course object, save it in the session.
    $new = array();
    $new['weight'] = 0;

    // Get highest weight and add to it.
    if (isset($form_state['values']['course_outline']['objects'])) {
      foreach ($form_state['values']['course_outline']['objects'] as $key => $object) {
        if ($object['weight'] >= $new['weight']) {
          $new['weight'] = $object['weight'] + 1;
        }
      }
    }

    $new['nid'] = $node->nid;
    $new['coid'] = $obj_uniqid;
    list($new['module'], $new['object_type']) = explode('-', $form_state['values']['more']['object_type']);
    $_SESSION['course'][$node->nid]['editing'][$obj_uniqid] = $new;
  }

  $form['nid']['#type'] = 'hidden';
  $form['nid']['#value'] = $node->nid;

  // Grab initial list of objects from DB or session.
  if (!empty($_SESSION['course'][$node->nid]['editing'])) {
    $objects = $_SESSION['course'][$node->nid]['editing'];
  }
  else {
    $objects = $node->course['objects'];
  }

  // Sort list of objects we pulled from session or DB by weight for proper
  // display.
  uasort($objects, '_course_outline_overview_form_cmp_function');

  $cform['#title'] = 'Course objects';
  $form['#theme'] = 'course_outline_overview_form';

  if (!empty($_SESSION['course'][$node->nid]['editing'])) {
    drupal_set_message('Changes to this course have not yet been saved.', 'warning');
  }

  $handlers = course_get_handlers('object');

  // Wrapper for just the objects.
  $cform['objects']['#tree'] = TRUE;
  $object_counts = array();
  if (count($objects)) {
    foreach (array_keys($objects) as $uniqid) {
      $courseObject = course_get_course_object_by_id($uniqid);
      $rform = _course_outline_object_form($courseObject);

      // Keep track of how many of each course object we have.
      // @todo probably some simpler way to do this effectively
      if (!isset($object_counts[$courseObject->getModule()][$courseObject->getComponent()])) {
        $object_counts[$courseObject->getModule()][$courseObject->getComponent()] = 1;
      }
      else {
        $object_counts[$courseObject->getModule()][$courseObject->getComponent()]++;
      }

      // Don't allow user to change type of object.
      $rform['object_type'] = array(
        '#type' => 'hidden',
        '#value' => $courseObject->getOption('object_type'),
      );
      $rform['object_type_show'] = array(
        '#type' => 'markup',
        '#value' => filter_xss_admin($handlers[$courseObject->getOption('module')][$courseObject->getOption('object_type')]['name'] . '<br/><small><i>' . ucwords(str_replace('_', ' ', $courseObject->getOption('module'))) . '</i></small>'),
      );
      $cform['objects'][$uniqid] = $rform;
    }
  }

  // Add another button and select box for new objects.
  $object_types = array(
    '' => '- select object -',
  );
  foreach ($handlers as $module => $object_definitions) {
    if ($object_definitions) {
      foreach ($object_definitions as $key => $object_info) {
        $class = $object_info['class'];
		$max_object_count = 0; // hacked $class::getMaxOccurences();
        $under_limit = !$max_object_count || !(isset($object_counts[$module][$key]) && $object_counts[$module][$key] >= $max_object_count);
        if ($under_limit && empty($object_info['legacy'])) {
          $module_string = ucwords(str_replace('_', ' ', $module));
          $object_types[$module . '-' . $key] = $module_string . ': ' . $object_info['name'];
        }
      }
    }
  }

  $form['more'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $form['more']['add_another'] = array(
    '#type' => 'submit',
    '#value' => 'Add another',
    '#ahah' => array(
      'event' => 'click',
      'method' => 'replace',
      'path' => ahah_helper_path(AHAH_HELPER_PARENTS_ENTIRE_FORM),
      'wrapper' => 'course-outline-wrapper',
    ),
    '#submit' => array('ahah_helper_submit'),
  );
  $form['more']['object_type'] = array(
    '#type' => 'select',
    '#options' => $object_types,
  );

  // Submit and reset buttons.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
    '#submit' => array('course_outline_overview_form_submit'),
  );

  if (!empty($_SESSION['course'][$node->nid]['editing'])) {
    $form['reset'] = array(
      '#type' => 'submit',
      '#value' => 'Reset',
      '#submit' => array('course_outline_overview_form_reset'),
    );
  }

  $cform['objects']['#element_validate'] = array('_course_outline_overview_validate_objects');
  //return for form on tab
  return $form;
}

/**
 * Submit handler for resetting a Course back to stored defaults.
 */
function course_outline_overview_form_reset(&$form, &$form_state) {
  unset($_SESSION['course'][$form['nid']['#value']]['editing']);
}

/**
 * Form constructor for a course object.
 *
 * To be re-used in listing and creating new course objects.
 */
function _course_outline_object_form($courseObject = NULL) {
  $rform['#tree'] = TRUE;
  $uniqid = $courseObject->getId();

  foreach (array('coid', 'instance', 'module', 'object_type', 'nid') as $key) {
    $rform[$key] = array(
      '#type' => 'value',
      '#default_value' => $courseObject->getOption($key),
    );
  }

  // Do not use prefix/suffix because markup only renders with a value, and we
  // need the wrapper before the title is saved for ajax population after each
  // settings modal update.
  $title = $courseObject->getTitle();
  $rform['title'] = array(
    '#prefix' => '<div id="title-' . $uniqid . '">',
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => check_plain($title ? $title : ' '),
  );

  $summary = $courseObject->renderOptionsSummary();

  $rform['summary'] = array(
    '#prefix' => '<div id="summary-' . $uniqid . '">',
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => filter_xss_admin($summary ? $summary : ' '),
  );

  // Placeholder for the settings link, it gets added after this function runs
  // in course_outline_overview_form(). #value needs a space for the prefix and
  // suffix to render.
  // Settings link for saved objects.
  $text = t('Settings');
  $path = "node/{$courseObject->getCourseNid()}/course-object/nojs/{$uniqid}/options";
  $l_options = array(
    'query' => array(
      'destination' => "node/{$courseObject->getCourseNid()}/course-outline",
    ),
    'attributes' => array('class' => 'ctools-use-modal'),
  );
  $rform['options']['#value'] = l($text, $path, $l_options);

  $rform['weight'] = array(
    '#type' => 'weight',
    '#delta' => 50,
    '#default_value' => $courseObject->getOption('weight'),
    '#attributes' => array(
      'class' => 'course-object-weight',
    ),
  );

  $rform['dummy'] = array(
    '#type' => 'checkbox',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('dummy' => 1),
  );

  return $rform;
}

/**
 * Theme the course outline overview form as a table.
 *
 * @see course_outline_overview_form()
 *
 * @ingroup themeable
 */
function theme_course_outline_overview_form($form) {
  $objects = & $form['course_outline']['objects'];
  drupal_add_tabledrag('course-objects', 'order', 'sibling', 'course-object-weight');
  drupal_add_css(drupal_get_path('module', 'course') . '/css/admin.css');

  $rows = array();

  foreach (element_children($objects) as $key) {
    $object = & $objects[$key];
    unset($objects[$key]['title']['#title']);
    unset($objects[$key]['object_type']['#title']);
    $rows[$key]['data'][] = array('data' => NULL, 'width' => 1);
    $rows[$key]['data'][] = drupal_render($objects[$key]['title']) . drupal_render($objects[$key]['summary']);
    $rows[$key]['data'][] = drupal_render($objects[$key]['object_type']) . drupal_render($objects[$key]['object_type_show']);
    $rows[$key]['data'][] = drupal_render($objects[$key]['options']);

    // Add draggable settings to row.
    $rows[$key]['class'] = 'draggable';
    $rows[$key]['data'][] = drupal_render($objects[$key]['weight']);

    // Add id to row for per-row ajax handling.
    $rows[$key]['id'] = "row-{$key}";

    // Deletion handling.
    $nid = $object['nid']['#value'];
    // @todo should we unify this, so we always use $courseObject->getOptions()
    // instead of pulling from the session?
    if (!empty($_SESSION['course'][$nid]['editing'][$key]['delete'])) {
      $rows[$key]['class'] = 'deleted';
    }

    // Mark everything else as printed.
    drupal_render($objects[$key]);
  }
  $table = theme('table', array(), $rows, array('id' => 'course-objects'));
  $table .= drupal_render($form);
  return $table;
}

/**
 * Validation callback.
 */
function _course_outline_overview_validate_objects(&$form, &$form_state) {

}

/**
 * Comparator function for course outline weights.
 */
function _course_outline_overview_form_cmp_function($a, $b) {
  if (is_object($a)) {
    if (isset($a->weight) && isset($b->weight)) {
      return $a->weight - $b->weight;
    }
  }
  else {
    if (isset($a['weight']) && isset($b['weight'])) {
      return $a['weight'] - $b['weight'];
    }
  }
}

/**
 * Submit handler.
 */
function course_outline_overview_form_submit(&$form, &$form_state) {
  $node = node_load($form['nid']['#value']);

  // Get form state values for object elements on the course outline overview:
  // - An associative array of course objects, keyed by ID. The ID for already
  //   saved objects is {course_outline}.coid, but for AHAH created objects the
  //   key is a generated unique ID until save.
  //   - coid: The key loaded from the database. If empty, the object is new.
  //   - module: The implementing module name (course_quiz etc).
  //   - object_type: The course object key as defined by
  //     hook_course_handlers().
  $objects = $form_state['values']['course_outline']['objects'];

  // Sort by weight so we can renumber.
  uasort($objects, '_course_outline_overview_form_cmp_function');

  $i = -50;
  foreach ($objects as $object_key => $object) {
    // Get each course object settings saved form values, which are not on the
    // outline overview form.
    if (!$courseObject = course_get_course_object_by_id($object_key)) {
      // For non-JS, no settings have been saved. We have to construct a new
      // course object.
      $courseObject = course_get_course_object($object);
    }
    $options = $courseObject->getOptions();

    // Renumber weights to the way draggable table would do it in case of no JS.
    $courseObject->setOption('weight', $i++);

    // We only save overview objects with a selected component, whether loaded
    // from the database or AHAH created.
    if (isset($object['object_type'])) {
      // Delete database loaded objects that are on the chopping block.
      $is_loaded = $object['coid'];
      if ($options['delete'] && $is_loaded) {
        if ($options['delete_instance']) {
          // Also delete related object instance(s) if specified.
          $courseObject->delete();
        }
        course_outline_delete_object($object);
      }
      // If not slated for deletion, save the both db loaded and AHAH objects.
      elseif (!$options['delete']) {
        // If we get this far, save the object.
        $courseObject->save();
      }
    }
  }

  // Clear the editing session.
  unset($_SESSION['course'][$node->nid]['editing']);

  drupal_set_message('Updated course.');

  // @todo use form #redirect instead of drupal_goto()?
  drupal_goto("node/$node->nid/course-outline");
}

/**
 * Generate HTML of the course outline.
 *
 * @param object $node
 *
 * @return course outline list.
 */
function course_outline_list($node) {
  global $user;

  $course = course_get_course($node, $user);

  // Iterate over objects.
  $workflow = array();
  $prev_req = NULL;
  foreach ($course->getObjects() as $key => $courseObject) {
    $step = array();
    $step['style'] = 'not-started';
    $step['id'] = 'course-object-' . $courseObject->getId();
    $step['image'] = '';
    $step['status'] = '';

    if (!$courseObject->access('see')) {
      if ($status = $courseObject->getStatus()) {
        drupal_set_message(filter_xss($status), 'info');
      }
      continue;
    }
    if ($courseObject->access('take')) {
      // User can take this course object.
      $step['link'] = $courseObject->getUrl();

      if (!$prev_req || $prev_req->getFulfillment()->isComplete()) {
        $step['status'] = 'Begin';
      }

      // Step is complete.
      if ($courseObject->getFulfillment()->isComplete()) {
        $step['class'] = 'completed';
        $step['status'] = 'Complete';
        $step['image'] = 'misc/watchdog-ok.png';
      }
      elseif ($courseObject->getFulfillment()->getId()) {
        $step['status'] = 'Continue';
        $step['class'] = 'continue';
        $step['image'] = 'misc/menu-collapsed.png';
        if ($course->getActive() === $courseObject) {
          $step['status'] = $courseObject->getStatus();
          if (!$step['status'] && $courseObject->getCourse()->getActive()->getId() == $courseObject->getId()) {
            $step['status'] = 'In progress';
            $step['class'] = 'in-progress';
            $step['image'] = 'misc/watchdog-warning.png';
          }
        }
      }
    }
    else {
      // User cannot access this step yet.
      $step['style'] = 'not-started';
      $step['status'] = implode('<br/>', $courseObject->getAccessMessages());
    }

    $img = theme('image', $step['image']);

    if (empty($step['link'])) {
      $data = $courseObject->getTitle() . '<br/><span class="course-outline-status">' . $step['status'] . '</span>';
    }
    else {
      $data = $img . ' ' . l("{$courseObject->getTitle()}<br/>", $step['link'], array('html' => TRUE)) . '<span class="course-outline-status">' . $step['status'] . '</span>';
    }

    $item = array(
      'data' => $data,
      'id' => $step['id'],
      'class' => $step['style'],
    );

    // Allow other modules to modify this list item.
    $courseObject->overrideOutlineListItem($item);

    // Add this item to the list.
    $workflow[] = $item;

    // We have to clone it, otherwise $courseObject becomes the next object.
    $prev_req = clone $courseObject;
  }

  if ($prev_req && $prev_req->getFulfillment()->isComplete()) {
    $workflow[] = array(
      'data' => $img . ' ' . l(t('Complete'), "node/$node->nid/course-complete", array('html' => TRUE)),
      'id' => 'complete',
    );
  }

  $output = '';
  if ($workflow) {
    $output .= '<div class="course-outline">';
    $output .= '<span class="trigger"></span>';
    $output .= '<h4 class="course-title">' . $node->title . '</h4>';
    $output .= theme('item_list', $workflow);
    $output .= '</div>';
  }
  return $output;
}

/**
 * Render a landing page for course completion.
 *
 * @param stdClass $course_node A course node.
 *
 * @return string HTML for the landing page.
 *
 * @todo change the name of this function (since it's more than just completion
 * links).
 */
function course_outline_show_complete_links($course_node) {
  global $user;
  $account = $user;
  $report = course_report_load($course_node, $account);
  $txt_out = '';
  if ($report->complete) {
    $txt_out .= '<p>' . t('Thank you for participating in this activity.') . '</p>';
    $links = array(
      'course' => array('Return to course', "node/$course_node->nid", 'Return to the course to view course details and material.'),
    );
    // Allow modules to add links to the course completion landing page, such as
    // post-course actions.
    drupal_alter('course_outline_completion_links', $links, $course_node, $account);
    foreach ($links as $link) {
      // @todo change the way $links work. Replace the three element array with
      // something else. Maybe $element and $value from theme_form_element() if
      // we must be this controlling?
      // Example:
      // @code
      //   $element = t('A description');
      //   $value = l($text, $path);
      //   $links = array('element' => $element, 'value' => $value).
      // @endcode
      // But why not allow modules to alter the landing page however they want
      // (instead of requiring 'links' with this very specific output format)?
      $txt_out .= theme('form_element', array('#description' => $link[2]), l($link[0], $link[1]));
    }
  }
  else {
    $course = course_get_course($course_node, $account);
    $objects = $course->getObjects();
    $requirements_outstanding = array();
    foreach ($objects as $courseObject) {
      // Find required course objects the user has not yet completed.
      if ($courseObject->getOption('required') && !$courseObject->getOption('complete')) {
        $requirements_outstanding[$courseObject->getId()] = $courseObject->getFulfillment();
      }
    }

    $items = array();
    foreach ($requirements_outstanding as $req) {
      $status_css = $req->complete ? 'complete' : 'incomplete';
      $status_img = $req->complete ? 'ok' : ($req->required ? 'error' : 'warning');
      $status_optional = ($status_img == 'warning') ? ' (optional)' : '';
      $grade = $req->graded ? ' - Your grade: ' . $req->grade_result . '%, Pass grade: ' . $req->passing_grade . '%' : '';
      $items[] = array(
        array(
          'data' => theme('image', "misc/watchdog-{$status_img}.png", $status_css),
          'width' => 20,
        ),
        $req->title . $status_optional . $grade,
      );
    }
    $reqs_txt = theme('table', NULL, $items);

    $txt_out .= '<p>' . t('You must complete the remaining requirements to proceed.') . '</p>';
    $txt_out .= $reqs_txt . '<div class="homelink"><p>' . l($course_out_return_array['course_outline_link_text'], $course_out_return_array['course_outline_link']) . '</p></div>';

    $links = array(
      'course' => array('Return to course', "node/$course_node->nid/takecourse", 'Return to the course to view course details and material.'),
    );
    // Allow modules to alter remaining requirement links on the course
    // completion landing page.
    drupal_alter('course_outline_incomplete_links', $links, $course_node, $account);
    foreach ($links as $link) {
      $txt_out .= theme('form_element', array('#description' => $link[2]), l($link[0], $link[1]));
    }
  }

  return $txt_out;
}

/**
 * Delete a fulfillment.
 */
function course_outline_delete_fulfillment($mixed) {
  $sql = "DELETE FROM {course_outline_fulfillment} WHERE coid = %d AND uid = %d";
  db_query($sql, $mixed->coid, $mixed->uid);
  return db_affected_rows() > 0;
}
