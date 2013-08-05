<?php

$view = new view;
$view->name = 'course_enrollment_vbo_admin';
$view->description = 'Per-node signup administration interface with checkboxes for bulk operations.';
$view->tag = 'Signup';
$view->base_table = 'course_enrolment';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'edit courses';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'bulk';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'timestamp' => 'timestamp',
  'enrol_end' => 'enrol_end',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'timestamp' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'enrol_end' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
$handler->display->display_options['style_options']['operations'] = array(
  'system_message_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'course_edit_enrollment_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_script_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'system_goto_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'course_remove_enrollment_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'label' => 'Remove enrollment',
  ),
  'system_send_email_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
);
$handler->display->display_options['style_options']['execution_type'] = '1';
$handler->display->display_options['style_options']['display_type'] = '1';
$handler->display->display_options['style_options']['display_result'] = 1;
$handler->display->display_options['style_options']['merge_single_action'] = 0;
$handler->display->display_options['style_options']['hide_selector'] = 0;
$handler->display->display_options['style_options']['preserve_selection'] = 1;
/* Relationship: Course enrolments: Course node */
$handler->display->display_options['relationships']['nid_1']['id'] = 'nid_1';
$handler->display->display_options['relationships']['nid_1']['table'] = 'course_enrolment';
$handler->display->display_options['relationships']['nid_1']['field'] = 'nid';
$handler->display->display_options['relationships']['nid_1']['required'] = 0;
/* Relationship: Course enrolments: User */
$handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
$handler->display->display_options['relationships']['uid_1']['table'] = 'course_enrolment';
$handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
$handler->display->display_options['relationships']['uid_1']['required'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid_1';
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
$handler->display->display_options['fields']['timestamp']['label'] = 'Enroll start';
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
/* Field: Course enrolments: End of enrolment */
$handler->display->display_options['fields']['enrol_end']['id'] = 'enrol_end';
$handler->display->display_options['fields']['enrol_end']['table'] = 'course_enrolment';
$handler->display->display_options['fields']['enrol_end']['field'] = 'enrol_end';
$handler->display->display_options['fields']['enrol_end']['relationship'] = 'course_enrollment';
$handler->display->display_options['fields']['enrol_end']['label'] = 'Enroll End';
$handler->display->display_options['fields']['enrol_end']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['external'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['enrol_end']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['enrol_end']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['trim'] = 0;
$handler->display->display_options['fields']['enrol_end']['alter']['html'] = 0;
$handler->display->display_options['fields']['enrol_end']['element_label_colon'] = 1;
$handler->display->display_options['fields']['enrol_end']['element_default_classes'] = 1;
$handler->display->display_options['fields']['enrol_end']['hide_empty'] = 0;
$handler->display->display_options['fields']['enrol_end']['empty_zero'] = 0;
$handler->display->display_options['fields']['enrol_end']['hide_alter_empty'] = 1;
/* Argument: Node: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'nid_1';
$handler->display->display_options['arguments']['nid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['nid']['validate_type'] = 'course';
$handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['nid']['not'] = 0;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'node/%/course-enrollments/list';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'List';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'Enrollments';
$handler->display->display_options['tab_options']['weight'] = '0';
