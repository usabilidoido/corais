<?php

$view = new view;
$view->name = 'course_enrollment_search_add';
$view->description = '';
$view->tag = '';
$view->base_table = 'users';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = '%1: Search and Enroll';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'edit courses';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'bulk';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'realname' => 'realname',
  'mail' => 'mail',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'realname' => array(
    'align' => '',
    'separator' => '',
  ),
  'mail' => array(
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
  'user_block_ip_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'user_block_user_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'user_user_operations_block' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_delete_user_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'system_message_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'course_add_enrollment_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'course_signup_add_enrollment_action' => array(
    'selected' => 0,
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
  'views_bulk_operations_user_roles_action' => array(
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
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'system_send_email_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'user_user_operations_unblock' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
);
$handler->display->display_options['style_options']['execution_type'] = '1';
$handler->display->display_options['style_options']['display_type'] = '0';
$handler->display->display_options['style_options']['display_result'] = 1;
$handler->display->display_options['style_options']['merge_single_action'] = 1;
$handler->display->display_options['style_options']['hide_selector'] = 0;
$handler->display->display_options['style_options']['preserve_selection'] = 1;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'Username';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 0;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: User: E-mail */
$handler->display->display_options['fields']['mail']['id'] = 'mail';
$handler->display->display_options['fields']['mail']['table'] = 'users';
$handler->display->display_options['fields']['mail']['field'] = 'mail';
$handler->display->display_options['fields']['mail']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['mail']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['mail']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['mail']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['mail']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['mail']['alter']['trim'] = 0;
$handler->display->display_options['fields']['mail']['alter']['html'] = 0;
$handler->display->display_options['fields']['mail']['hide_empty'] = 0;
$handler->display->display_options['fields']['mail']['empty_zero'] = 0;
$handler->display->display_options['fields']['mail']['link_to_user'] = '0';
/* Argument: Global: Null */
$handler->display->display_options['arguments']['null']['id'] = 'null';
$handler->display->display_options['arguments']['null']['table'] = 'views';
$handler->display->display_options['arguments']['null']['field'] = 'null';
$handler->display->display_options['arguments']['null']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['null']['validate_type'] = 'course';
$handler->display->display_options['arguments']['null']['must_not_be'] = 0;
/* Filter: User: Name */
$handler->display->display_options['filters']['uid']['id'] = 'uid';
$handler->display->display_options['filters']['uid']['table'] = 'users';
$handler->display->display_options['filters']['uid']['field'] = 'uid';
$handler->display->display_options['filters']['uid']['operator'] = 'not in';
$handler->display->display_options['filters']['uid']['value'] = array(
  0 => 0,
);
$handler->display->display_options['filters']['uid']['expose']['operator'] = FALSE;
/* Filter: User: Name (raw) */
$handler->display->display_options['filters']['name']['id'] = 'name';
$handler->display->display_options['filters']['name']['table'] = 'users';
$handler->display->display_options['filters']['name']['field'] = 'name';
$handler->display->display_options['filters']['name']['operator'] = 'contains';
$handler->display->display_options['filters']['name']['exposed'] = TRUE;
$handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['limit_operators'] = array();
$handler->display->display_options['filters']['name']['expose']['label'] = 'User: Name';
$handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
$handler->display->display_options['filters']['name']['expose']['optional'] = TRUE;
$handler->display->display_options['filters']['name']['expose']['single'] = TRUE;
$handler->display->display_options['filters']['name']['case'] = 0;
/* Filter: User: E-mail */
$handler->display->display_options['filters']['mail']['id'] = 'mail';
$handler->display->display_options['filters']['mail']['table'] = 'users';
$handler->display->display_options['filters']['mail']['field'] = 'mail';
$handler->display->display_options['filters']['mail']['operator'] = 'contains';
$handler->display->display_options['filters']['mail']['exposed'] = TRUE;
$handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
$handler->display->display_options['filters']['mail']['expose']['label'] = 'User: E-mail';
$handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
$handler->display->display_options['filters']['mail']['expose']['optional'] = 1;
$handler->display->display_options['filters']['mail']['case'] = 0;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'bulk';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'mail' => 'mail',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'mail' => array(
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
  'user_block_ip_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'user_block_user_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'user_user_operations_block' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_delete_user_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'system_message_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'course_add_enrollment_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'course_signup_add_enrollment_action' => array(
    'selected' => 0,
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
  'views_bulk_operations_profile_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_user_roles_action' => array(
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
  'system_send_email_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'user_user_operations_unblock' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
);
$handler->display->display_options['style_options']['execution_type'] = '1';
$handler->display->display_options['style_options']['display_type'] = '0';
$handler->display->display_options['style_options']['display_result'] = 1;
$handler->display->display_options['style_options']['merge_single_action'] = 1;
$handler->display->display_options['style_options']['hide_selector'] = 0;
$handler->display->display_options['style_options']['preserve_selection'] = 1;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['path'] = 'node/%/course-enrollments/search-add';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Search and Enroll';
$handler->display->display_options['menu']['weight'] = '0';
