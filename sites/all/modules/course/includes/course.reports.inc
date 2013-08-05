<?php

/**
 * @file course.reports.inc
 * Reporting framework for course objects.
 */

/**
 * Page handler for course object reports.
 */
function course_object_reports_page($node) {
  $coid = arg(4);
  $report_key = arg(5);
  $report_subkey = arg(6);
  // Report navigation.
  $nav = '';
  // Report header.
  $header = '';
  // Report content.
  $content = '';


  $course = course_get_course($node);
  foreach ($course->getObjects() as $courseObject) {
    $lis = array();
    if (method_exists($courseObject, 'getReports')) {
      foreach ($courseObject->getReports() as $key => $report) {
        $report_li = array(
          'data' => course_object_single_report_link($courseObject, $key, $report),
        );
        $lis[] = $report_li;
      }
    }
    if ($lis) {
      $nav .= theme('item_list', $lis, $courseObject->getTitle());
    }
  }

  if ($coid) {
    $courseObject = course_get_course_object_by_id($coid);
    $report = $courseObject->getReport($report_key, $report_subkey);
    if ($report) {
      if (isset($report['url'])) {
        $content = course_iframe($report['url']);
      }
      if (isset($report['content'])) {
        $content = $report['content'];
      }
      $header = '<h3>' . $courseObject->getTitle() . ': ' . $report['title'] . '</h3>';
    }
    else {
      $content = 'Sorry, no report is available for this type.';
    }
  }


  return theme('course_report', $nav, $header, $content);
}

/**
 * Return a link to a report as HTML.
 */
function course_object_single_report_link($courseObject, $key, $report) {
  return l($report['title'], "node/{$courseObject->getCourseNid()}/course-reports/objects/{$courseObject->getId()}/$key");
}

/**
 * Theme the course objects report page.
 */
function theme_course_report($nav, $header, $body) {
  if (!$nav) {
    $body = t('None of the objects in your course have reports associated with them.');
  }

  $body = $header . $body;

  $row = array(
    array(
      'data' => $nav,
      'width' => 200,
      'id' => 'course-object-reports-nav',
      'valign' => 'top',
    ),
    array(
      'data' => $body,
      'id' => 'course-object-reports-content',
      'valign' => 'top',
    ),
  );

  $rows[] = $row;

  $out = theme('table', NULL, $rows, array(
    'id' => 'course-object-reports',
    ));

  return $out;
}
