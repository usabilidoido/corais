<?php

$view = new view;
$view->name = 'transcript';
$view->description = '';
$view->tag = '';
$view->base_table = 'course_report';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Completed Activities';
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'timestamp' => 'timestamp',
  'date_completed' => 'date_completed',
  'section_name' => 'section_name',
);
$handler->display->display_options['style_options']['default'] = 'date_completed';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'timestamp' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'date_completed' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
  'section_name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['text']['id'] = 'area';
$handler->display->display_options['empty']['text']['table'] = 'views';
$handler->display->display_options['empty']['text']['field'] = 'area';
$handler->display->display_options['empty']['text']['label'] = '';
$handler->display->display_options['empty']['text']['empty'] = FALSE;
$handler->display->display_options['empty']['text']['content'] = 'You don\'t have any completed activities.';
$handler->display->display_options['empty']['text']['format'] = '2';
/* Relationship: Course report: Course node */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'course_report';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
$handler->display->display_options['relationships']['nid']['required'] = 1;
/* Relationship: Course report: Course user */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'course_report';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Relationship: Course report: Course enrollment */
$handler->display->display_options['relationships']['course_enrollment']['id'] = 'course_enrollment';
$handler->display->display_options['relationships']['course_enrollment']['table'] = 'course_report';
$handler->display->display_options['relationships']['course_enrollment']['field'] = 'course_enrollment';
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Course enrolments: Start of enrolment */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['relationship'] = 'course_enrollment';
$handler->display->display_options['fields']['timestamp']['label'] = 'Date enrolled';
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
$handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
$handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'm/d/Y';
/* Field: Course report: Course status */
$handler->display->display_options['fields']['section_name']['id'] = 'section_name';
$handler->display->display_options['fields']['section_name']['table'] = 'course_report';
$handler->display->display_options['fields']['section_name']['field'] = 'section_name';
$handler->display->display_options['fields']['section_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['section_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['section_name']['empty_zero'] = 0;
/* Field: Course report: Course date completed */
$handler->display->display_options['fields']['date_completed']['id'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['table'] = 'course_report';
$handler->display->display_options['fields']['date_completed']['field'] = 'date_completed';
$handler->display->display_options['fields']['date_completed']['label'] = 'Date completed';
$handler->display->display_options['fields']['date_completed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['external'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['date_completed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['date_completed']['alter']['html'] = 0;
$handler->display->display_options['fields']['date_completed']['element_label_colon'] = 1;
$handler->display->display_options['fields']['date_completed']['element_default_classes'] = 1;
$handler->display->display_options['fields']['date_completed']['hide_empty'] = 0;
$handler->display->display_options['fields']['date_completed']['empty_zero'] = 0;
$handler->display->display_options['fields']['date_completed']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['date_completed']['date_format'] = 'custom';
$handler->display->display_options['fields']['date_completed']['custom_date_format'] = 'm/d/Y';
/* Argument: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['uid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['uid']['validate_type'] = 'php';
$handler->display->display_options['arguments']['uid']['validate_options']['code'] = 'global $user;
if ($user->uid == $argument) {
  return TRUE;
}

if (user_access(\'access all course reports\')) {
  return TRUE;
}

return FALSE;';
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
/* Filter: Course report: Complete */
$handler->display->display_options['filters']['complete']['id'] = 'complete';
$handler->display->display_options['filters']['complete']['table'] = 'course_report';
$handler->display->display_options['filters']['complete']['field'] = 'complete';
$handler->display->display_options['filters']['complete']['value'] = '1';
$handler->display->display_options['filters']['complete']['expose']['operator'] = FALSE;

/* Display: Pending */
$handler = $view->new_display('page', 'Pending', 'pending');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Pending Activities';
$handler->display->display_options['defaults']['empty'] = FALSE;
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['text']['id'] = 'text';
$handler->display->display_options['empty']['text']['table'] = 'views';
$handler->display->display_options['empty']['text']['field'] = 'area';
$handler->display->display_options['empty']['text']['label'] = '';
$handler->display->display_options['empty']['text']['empty'] = FALSE;
$handler->display->display_options['empty']['text']['content'] = 'You don\'t have any pending activities.';
$handler->display->display_options['empty']['text']['format'] = '2';
$handler->display->display_options['empty']['text']['tokenize'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Course enrolments: Start of enrolment */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['relationship'] = 'course_enrollment';
$handler->display->display_options['fields']['timestamp']['label'] = 'Date enrolled';
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
$handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
$handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'd/m/Y';
/* Field: Course report: Course status */
$handler->display->display_options['fields']['section_name']['id'] = 'section_name';
$handler->display->display_options['fields']['section_name']['table'] = 'course_report';
$handler->display->display_options['fields']['section_name']['field'] = 'section_name';
$handler->display->display_options['fields']['section_name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['section_name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['section_name']['alter']['html'] = 0;
$handler->display->display_options['fields']['section_name']['hide_empty'] = 0;
$handler->display->display_options['fields']['section_name']['empty_zero'] = 0;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Course report: Complete */
$handler->display->display_options['filters']['complete']['id'] = 'complete';
$handler->display->display_options['filters']['complete']['table'] = 'course_report';
$handler->display->display_options['filters']['complete']['field'] = 'complete';
$handler->display->display_options['path'] = 'user/%/activities/pending';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Courses (in progress)';
$handler->display->display_options['menu']['weight'] = '0';

/* Display: Completed */
$handler = $view->new_display('page', 'Completed', 'completed');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Completed Activities';
$handler->display->display_options['defaults']['empty'] = FALSE;
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['text']['id'] = 'area';
$handler->display->display_options['empty']['text']['table'] = 'views';
$handler->display->display_options['empty']['text']['field'] = 'area';
$handler->display->display_options['empty']['text']['label'] = '';
$handler->display->display_options['empty']['text']['empty'] = FALSE;
$handler->display->display_options['empty']['text']['content'] = 'You don\'t have any completed activities.';
$handler->display->display_options['empty']['text']['format'] = '2';
$handler->display->display_options['path'] = 'user/%/activities/completed';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'Transcript';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'My Activities';
$handler->display->display_options['tab_options']['weight'] = '0';
$handler->display->display_options['tab_options']['name'] = 'primary-links';
