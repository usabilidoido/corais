<?php

$view = new view;
$view->name = 'course_report';
$view->description = '';
$view->tag = '';
$view->base_table = 'course_report';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Course Report';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'access all course reports';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = 50;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'realname' => 'realname',
  'timestamp' => 'timestamp',
  'date_completed' => 'date_completed',
  'section_name' => 'section_name',
);
$handler->display->display_options['style_options']['default'] = 'date_completed';
$handler->display->display_options['style_options']['info'] = array(
  'realname' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'timestamp' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'date_completed' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'section_name' => array(
    'sortable' => 1,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['order'] = 'desc';
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'Sorry, there is no course data to report on.';
$handler->display->display_options['empty']['area']['format'] = '1';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Relationship: Course report: Course node */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'course_report';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
$handler->display->display_options['relationships']['nid']['required'] = 1;
/* Relationship: Course report: Course user */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'course_report';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['label'] = 'Course User';
$handler->display->display_options['relationships']['uid']['required'] = 1;
/* Relationship: Course report: Course enrollment */
$handler->display->display_options['relationships']['course_enrollment']['id'] = 'course_enrollment';
$handler->display->display_options['relationships']['course_enrollment']['table'] = 'course_report';
$handler->display->display_options['relationships']['course_enrollment']['field'] = 'course_enrollment';
$handler->display->display_options['relationships']['course_enrollment']['required'] = 0;
/* Field: Course: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'course_node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['relationship'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = 'Node ID';
$handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['nid']['alter']['external'] = 0;
$handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['nid']['alter']['html'] = 0;
$handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nid']['hide_empty'] = 0;
$handler->display->display_options['fields']['nid']['empty_zero'] = 0;
$handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['nid']['format_plural'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['label'] = 'Course';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['external'] = 0;
$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = 1;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: User: Uid */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'users';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['relationship'] = 'uid';
$handler->display->display_options['fields']['uid']['label'] = 'User ID';
$handler->display->display_options['fields']['uid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['uid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['uid']['alter']['external'] = 0;
$handler->display->display_options['fields']['uid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['uid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['uid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['uid']['alter']['html'] = 0;
$handler->display->display_options['fields']['uid']['element_label_colon'] = 1;
$handler->display->display_options['fields']['uid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['uid']['hide_empty'] = 0;
$handler->display->display_options['fields']['uid']['empty_zero'] = 0;
$handler->display->display_options['fields']['uid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['uid']['link_to_user'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['external'] = 0;
$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Course enrolments: Start of enrolment */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['relationship'] = 'course_enrollment';
$handler->display->display_options['fields']['timestamp']['label'] = 'Date Started';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
$handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
$handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
/* Field: Course report: Course date completed */
$handler->display->display_options['fields']['date_completed']['id'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['table'] = 'course_report';
$handler->display->display_options['fields']['date_completed']['field'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['label'] = 'Date Completed';
$handler->display->display_options['fields']['date_completed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['html'] = 0;
$handler->display->display_options['fields']['date_completed']['hide_empty'] = 0;
$handler->display->display_options['fields']['date_completed']['empty_zero'] = 0;
/* Field: Course report: Course status */
$handler->display->display_options['fields']['section_name']['id'] = 'section_name';
$handler->display->display_options['fields']['section_name']['table'] = 'course_report';
$handler->display->display_options['fields']['section_name']['field'] = 'section_name';
$handler->display->display_options['fields']['section_name']['label'] = 'Status';
$handler->display->display_options['fields']['section_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['section_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['section_name']['empty_zero'] = 0;
/* Argument: Node: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'nid';
$handler->display->display_options['arguments']['nid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['title'] = '%1';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['nid']['validate_type'] = 'course';
$handler->display->display_options['arguments']['nid']['break_phrase'] = 1;
$handler->display->display_options['arguments']['nid']['not'] = 0;
/* Filter: Course report: Course date completed */
$handler->display->display_options['filters']['date_completed']['id'] = 'date_completed';
$handler->display->display_options['filters']['date_completed']['table'] = 'course_report';
$handler->display->display_options['filters']['date_completed']['field'] = 'date_completed';
$handler->display->display_options['filters']['date_completed']['operator'] = 'between';
$handler->display->display_options['filters']['date_completed']['exposed'] = TRUE;
$handler->display->display_options['filters']['date_completed']['expose']['operator'] = 'date_completed_op';
$handler->display->display_options['filters']['date_completed']['expose']['label'] = 'Completed between';
$handler->display->display_options['filters']['date_completed']['expose']['identifier'] = 'date_completed';

/* Display: Course report */
$handler = $view->new_display('page', 'Course report', 'page_1');
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Argument: Node: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'nid';
$handler->display->display_options['arguments']['nid']['default_action'] = 'default';
$handler->display->display_options['arguments']['nid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['title'] = '%1';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
$handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['nid']['validate_type'] = 'course';
$handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['nid']['not'] = 0;
$handler->display->display_options['path'] = 'node/%/course-reports/overview';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'Overview';
$handler->display->display_options['menu']['weight'] = '-1';
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'Course reports';
$handler->display->display_options['tab_options']['weight'] = '0';

/* Display: Global course report */
$handler = $view->new_display('page', 'Global course report', 'page_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'All courses';
$handler->display->display_options['path'] = 'admin/reports/course/overview/view';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'All courses';
$handler->display->display_options['menu']['description'] = 'View or download a report of all courses.';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['tab_options']['type'] = 'normal';
$handler->display->display_options['tab_options']['title'] = 'Course reports';
$handler->display->display_options['tab_options']['description'] = 'View and download course information.';
$handler->display->display_options['tab_options']['weight'] = '0';

/* Display: Global feed */
$handler = $view->new_display('feed', 'Global feed', 'feed_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Global feed';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'views_csv';
$handler->display->display_options['style_options']['provide_file'] = 1;
$handler->display->display_options['style_options']['filename'] = '%view.csv';
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['style_options']['quote'] = 1;
$handler->display->display_options['style_options']['header'] = 1;
$handler->display->display_options['path'] = 'admin/reports/course/overview/csv';
$handler->display->display_options['displays'] = array(
  'page_2' => 'page_2',
  'default' => 0,
  'page_1' => 0,
);
$handler->display->display_options['sitename_title'] = 0;

/* Display: Feed */
$handler = $view->new_display('feed', 'Feed', 'feed_2');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'views_csv';
$handler->display->display_options['style_options']['provide_file'] = 1;
$handler->display->display_options['style_options']['filename'] = 'course-report-!1.csv';
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['style_options']['quote'] = 1;
$handler->display->display_options['style_options']['header'] = 1;
$handler->display->display_options['path'] = 'node/%/report/csv';
$handler->display->display_options['displays'] = array(
  'page_1' => 'page_1',
  'default' => 0,
  'page_2' => 0,
);
