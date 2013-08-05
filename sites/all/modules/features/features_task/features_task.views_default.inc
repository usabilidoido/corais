<?php

/**
 * Implementation of hook_views_default_views().
 */
function features_task_views_default_views() {
  $views = array();

  // Exported view: add_task_group
  $view = new view;
  $view->name = 'add_task_group';
  $view->description = 'Add referred task to group';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('fields', array(
    'phpcode' => array(
      'label' => 'Group Alias',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'value' => '<?php 
echo drupal_lookup_path(\'alias\', \'node/\'.$data->nid);
$modifier = purl_load(array(\'id\' => isset($data->nid) ? $data->nid : NULL, \'provider\' => \'spaces_og\'));
echo $modifier[\'value\'];
?>',
      'exclude' => 1,
      'id' => 'phpcode',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
    ),
    'field_image_fid' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'user-s_default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_image_fid',
      'table' => 'node_data_field_image',
      'field' => 'field_image_fid',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '[path]/node/add/task?edit[field_referenced_book_page][0][nid][nid]=%1[nid:!1]',
        'make_link' => 1,
        'path' => '[phpcode]/node/add/task?edit[field_referenced_book_page][0][nid][nid]=[%1][nid:!1]',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 0,
      'link_to_node' => 0,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'title' => array(
      'order' => 'ASC',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'php',
      'validate_fail' => 'ignore',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        6 => 0,
        1 => 0,
        3 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'mass_contact' => 0,
        'nodequeuenode' => 0,
        'patch' => 0,
        'sheetnode' => 0,
        'board' => 0,
        'page' => 0,
        'service_provider' => 0,
        'toolkit' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        6 => 0,
        1 => 0,
        3 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => 'TRUE',
    ),
  ));
  $handler->override_option('filters', array(
    'uid' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'uid',
      'table' => 'og_uid',
      'field' => 'uid',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Criar Tarefa à partir deste Conhecimento');
  $handler->override_option('css_class', 'addtask');
  $handler->override_option('header_format', '7');
  $handler->override_option('header_empty', 0);
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'summary' => '',
    'columns' => array(
      'phpcode' => 'phpcode',
      'field_image_fid' => 'field_image_fid',
      'title' => 'title',
    ),
    'info' => array(
      'phpcode' => array(
        'separator' => '',
      ),
      'field_image_fid' => array(
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(),
    'separator' => '',
    'hide_empty' => 0,
  ));
  $handler = $view->new_display('block', 'Bloco', 'block_1');
  $handler->override_option('block_description', 'Add referenced task in a group');
  $handler->override_option('block_caching', -1);
  $translatables['add_task_group'] = array(
    t('Add referenced task in a group'),
    t('Bloco'),
    t('Create linked task'),
    t('Padrões'),
  );

  $views[$view->name] = $view;

  // Exported view: oa_custom_task2
  $view = new view;
  $view->name = 'oa_custom_task2';
  $view->description = 'Tasks';
  $view->tag = 'oa_custom';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 0,
      'flag' => 'completed',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'phpcode' => array(
      'label' => 'Título da Etapa/Tarefa',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'value' => '<?php
//echo \'<pre>\'.print_r($data,1).\'</pre>\';

$iconpath = drupal_get_path(\'module\', \'features_task\');
if ($data->node_type == \'milestone\') {
  $targetdate = $data->node_data_field_ms_completiondate_field_ms_completiondate_value;
  if ($targetdate != ""){
    $targetdate = \'[\'.date("m-d-Y", strtotime($targetdate)).\']\';
  }
  echo \'<a href="node/\'.$data->nid.\'"><strong>&nbsp;\'.features_task_neat_trim($data->node_title,40)."</strong></a> <span style=\'font-style: italic;text-decoration: underline\'><a href=\'node/add/task?edit[field_parentid][0][value]=".$data->nid."&edit[space_og][gid][value]=".$data->og_ancestry_group_nid."\'>".t("add task")."</a></span>";
} elseif ($data->node_type == \'task\') {
  $class = "notcompleted";
  if ($data->flag_content_content_id != ""){
    $class ="completed";
  }
  
  if($data->node_comment_statistics_comment_count > 0){
    $comcount = "&nbsp;(".$data->node_comment_statistics_comment_count.")&nbsp;";
  }
  echo \'<a href="node/\'.$data->nid.\'"><span class="\'.$class.\'">\'.features_task_neat_trim($data->node_title,40).$comcount.\'</span></a>\';  
}
?>',
      'exclude' => 0,
      'id' => 'phpcode',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'field_task_assignedto_uid' => array(
      'label' => 'Assigned To',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => 'Aguardando voluntário',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'total_time' => array(
      'label' => 'Work hours',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 1,
      'hide_alter_empty' => 1,
      'set_precision' => 1,
      'precision' => '0',
      'decimal' => '.',
      'separator' => ',',
      'format_plural' => 0,
      'format_plural_singular' => '1',
      'format_plural_plural' => '@count',
      'prefix' => '',
      'suffix' => ' hs',
      'exclude' => 0,
      'id' => 'total_time',
      'table' => 'time_tracker_totals',
      'field' => 'total_time',
      'relationship' => 'none',
    ),
    'ops' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_type' => 'toggle',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
    ),
    'phpcode_1' => array(
      'label' => 'Prioridade',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<?php
//echo \'<pre>\'.print_r($data,1).\'</pre>\';

if ($data->node_type == \'task\') {
  /* dim out if this task is completed */
  if ($data->flag_content_content_id != ""){
    switch($data->node_data_field_task_duedate_field_task_priority_value){
      case 1:
        echo "<span class=\'completed\'>High Priority";
        break;
      case 2:
        echo "<span class=\'completed\'>Medium Priority";
        break;
      case 3:
        echo "<span class=\'completed\'>Low Priority";
        break;
      default:
        echo "<span>";
        break;
    }
  } else {
    switch($data->node_data_field_task_duedate_field_task_priority_value){
      case 1:
        echo "<span class=\'priority-high\'>High Priority";
        break;
      case 2:
        echo "<span class=\'priority-med\'>Medium Priority";
        break;
      case 3:
        echo "<span class=\'priority-low\'>Low Priority";
        break;
      default:
        echo "<span>";
        break;
    }
  }
  echo "</span>";
}
?>',
      'exclude' => 1,
      'id' => 'phpcode_1',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'value0' => array(
      'label' => 'Ordem',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'field_parentid_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'relationship' => 'none',
    ),
    'field_task_priority_value' => array(
      'label' => 'Priority',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_task_priority_value',
      'table' => 'node_data_field_task_priority',
      'field' => 'field_task_priority_value',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'edit_node' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'text' => '',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Title',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_ms_completiondate_value' => array(
      'label' => 'Milestone Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'both',
      ),
      'exclude' => 1,
      'id' => 'field_ms_completiondate_value',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value',
      'relationship' => 'none',
    ),
    'body' => array(
      'label' => 'Body',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
      'relationship' => 'none',
    ),
    'comment_count' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 1,
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 1,
      'id' => 'comment_count',
      'table' => 'node_comment_statistics',
      'field' => 'comment_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'value0' => array(
      'order' => 'ASC',
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'spaces_feature',
    'spaces_feature' => 'features_task',
    'perm' => 'access content',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Etapas e Tarefas');
  $handler->override_option('header_format', '7');
  $handler->override_option('header_empty', 0);
  $handler->override_option('footer_format', '4');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('empty', '<p>Ferramenta para gerenciar o que deve ser feito em um projeto. A etapa tem uma data definida e aparece no cronograma, enquanto as tarefas não tem data definida, mas tem responsáveis. Adicione uma nova Etapa ou Tarefa no menu Criar Conteúdo.</p>');
  $handler->override_option('empty_format', '4');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'draggabletable');
  $handler->override_option('style_options', array(
    'override' => 0,
    'sticky' => 0,
    'order' => 'asc',
    'summary' => '',
    'columns' => array(
      'phpcode' => 'phpcode',
      'field_task_assignedto_uid' => 'field_task_assignedto_uid',
      'ops' => 'ops',
      'phpcode_1' => 'phpcode_1',
      'value0' => 'value0',
      'field_parentid_value' => 'field_parentid_value',
      'field_task_priority_value' => 'field_task_priority_value',
      'edit_node' => 'edit_node',
      'title' => 'title',
      'field_ms_completiondate_value' => 'field_ms_completiondate_value',
      'body' => 'body',
      'comment_count' => 'comment_count',
    ),
    'info' => array(
      'phpcode' => array(
        'separator' => '',
      ),
      'field_task_assignedto_uid' => array(
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
      'phpcode_1' => array(
        'separator' => '',
      ),
      'value0' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_parentid_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_priority_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_ms_completiondate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'body' => array(
        'separator' => '',
      ),
      'comment_count' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => -1,
    'tabledrag_order' => array(
      'field' => 'value0',
      'handler' => 'native',
    ),
    'tabledrag_order_visible' => array(
      'visible' => 0,
    ),
    'tabledrag_hierarchy' => array(
      'field' => 'field_parentid_value',
      'handler' => 'cck',
    ),
    'tabledrag_hierarchy_visible' => array(
      'visible' => 0,
    ),
    'draggableviews_depth_limit' => '1',
    'draggableviews_repair' => array(
      'repair' => 0,
    ),
    'tabledrag_types' => array(
      0 => array(
        'node_type' => 'milestone',
        'type' => 'leaf',
        'tabledrag_type_del_button' => 'Apagar',
      ),
      1 => array(
        'node_type' => 'milestone',
        'type' => 'root',
        'tabledrag_type_del_button' => 'Apagar',
      ),
    ),
    'tabledrag_types_add' => 'Add type',
    'tabledrag_expand' => array(
      'expand_links' => 0,
      'collapsed' => 0,
      'by_uid' => 0,
    ),
    'draggableviews_extensions' => array(
      'extension_top' => '3',
      'extension_bottom' => '3',
    ),
    'tabledrag_lock' => array(
      'lock' => 0,
    ),
    'draggableviews_default_on_top' => '1',
    'draggableviews_button_text' => 'Salvar ordem',
    'draggableviews_arguments' => array(
      'use_args' => 0,
    ),
    'draggableviews_book_radios' => '0',
    'draggableviews_book_argument' => 'none',
    'draggableviews_book_filter' => 'status',
  ));
  $handler = $view->new_display('page', 'Tasks', 'page_1');
  $handler->override_option('title', 'Tarefas a fazer');
  $handler->override_option('path', 'tasks');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Tasks',
    'description' => '',
    'weight' => '-10',
    'name' => 'features',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => 'Tasks',
    'description' => '',
    'weight' => '0',
    'name' => 'features',
  ));
  $handler = $view->new_display('page', 'My Tasks', 'page_2');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 0,
      'flag' => 'completed',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
    'field_task_assignedto_uid' => array(
      'label' => 'Responsável',
      'required' => 0,
      'delta' => -1,
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'phpcode_2' => array(
      'label' => 'Milestone',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<?php
//echo \'<pre>\'.print_r($data,1).\'</pre>\';
if($data->node_data_field_parentid_field_parentid_value != ""){
  $milestone = node_load($data->node_data_field_parentid_field_parentid_value);
  //echo "gothere";
  //echo \'<pre>\'.print_r($milestone,1).\'</pre>\';
  echo $milestone->title;
}
?>',
      'exclude' => 0,
      'id' => 'phpcode_2',
      'table' => 'customfield',
      'field' => 'phpcode',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Title',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_task_duedate_value' => array(
      'id' => 'field_task_duedate_value',
      'table' => 'node_data_field_task_duedate',
      'field' => 'field_task_duedate_value',
    ),
    'field_task_assignedto_uid' => array(
      'label' => 'Assigned To',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
      'relationship' => 'none',
    ),
    'ops' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_type' => '',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Use default',
      ),
    ),
    'phpcode_1' => array(
      'label' => 'Priority',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<?php
//echo \'<pre>\'.print_r($data,1).\'</pre>\';

if ($data->node_type == \'task\') {
  switch($data->node_data_field_task_duedate_field_task_priority_value){
    case 1:
        echo "<span class=\'priority-high\'>High Priority";
        break;
      case 2:
        echo "<span class=\'priority-med\'>Medium Priority";
        break;
      case 3:
        echo "<span class=\'priority-low\'>Low Priority";
      break;
    default:
      echo "<span>";
      break;
  }
  echo "</span>";
}
?>',
      'exclude' => 0,
      'id' => 'phpcode_1',
      'table' => 'customfield',
      'field' => 'phpcode',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'phpcode' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<?php
$iconpath = drupal_get_path(\'module\', \'features_task\');
if($data->node_data_field_task_duedate_field_task_attachments_data != ""){
  echo "<a href=\'node/".$data->nid."\'><img title=\'Task has attachments\' src=\'/".$iconpath."/attachment.png\'/></a>";
}  
?>',
      'exclude' => 0,
      'id' => 'phpcode',
      'table' => 'customfield',
      'field' => 'phpcode',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'edit_node' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'text' => '',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'relationship' => 'none',
    ),
    'field_task_priority_value' => array(
      'label' => 'Priority',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 1,
      'id' => 'field_task_priority_value',
      'table' => 'node_data_field_task_priority',
      'field' => 'field_task_priority_value',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'field_task_attachments_fid' => array(
      'label' => 'Attachments',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 1,
      'id' => 'field_task_attachments_fid',
      'table' => 'node_data_field_task_attachments',
      'field' => 'field_task_attachments_fid',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'field_parentid_value' => array(
      'label' => 'parentid',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 1,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_task_duedate_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_task_duedate_value',
      'table' => 'node_data_field_task_duedate',
      'field' => 'field_task_duedate_value',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'field_task_priority_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_task_priority_value',
      'table' => 'node_data_field_task_priority',
      'field' => 'field_task_priority_value',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '0',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Use default',
      ),
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'uid_current' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'uid_current',
      'table' => 'users',
      'field' => 'uid_current',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'field_task_assignedto_uid',
    ),
  ));
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 1,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'field_task_duedate_value' => 'field_task_duedate_value',
      'field_task_assignedto_uid' => 'field_task_assignedto_uid',
      'ops' => 'ops',
      'phpcode_1' => 'phpcode_1',
      'phpcode' => 'phpcode',
      'edit_node' => 'edit_node',
      'field_task_priority_value' => 'field_task_priority_value',
      'field_task_attachments_fid' => 'field_task_attachments_fid',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_duedate_value' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'field_task_assignedto_uid' => array(
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
      'phpcode_1' => array(
        'separator' => '',
      ),
      'phpcode' => array(
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => '',
      ),
      'field_task_priority_value' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'field_task_attachments_fid' => array(
        'separator' => '',
      ),
    ),
    'default' => 'field_task_priority_value',
  ));
  $handler->override_option('path', 'tasks/my');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => 'My Tasks',
    'description' => '',
    'weight' => '10',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => 'My Tasks',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler = $view->new_display('block', 'Bloco: Progresso Etapas', 'block_1');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'field_ms_completiondate_value2' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 1,
      ),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'format_interval',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 1,
      'id' => 'field_ms_completiondate_value2',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_date_value2' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 1,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'format_interval',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_date_value2',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_ms_completiondate_value' => array(
      'id' => 'field_ms_completiondate_value',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value',
    ),
    'value0' => array(
      'order' => 'ASC',
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '0',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
  ));
  $handler->override_option('title', 'Progresso');
  $handler->override_option('empty', '');
  $handler->override_option('style_plugin', 'default');
  $handler->override_option('style_options', array(
    'grouping' => '',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'title' => 'title',
      'field_ms_completiondate_value2' => 'field_ms_completiondate_value2',
      'field_date_value2' => 'field_date_value2',
    ),
    'separator' => ' - ',
    'hide_empty' => 0,
  ));
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('attachment', 'Gantt Etapas', 'attachment_1');
  $handler->override_option('fields', array(
    'field_task_assignedto_uid' => array(
      'label' => 'Assigned To',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => 'Aguardando voluntário',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'field_parentid_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Título',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 1,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'field_ms_completiondate_value' => array(
      'label' => 'Milestone Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value',
      ),
      'exclude' => 0,
      'id' => 'field_ms_completiondate_value',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'field_ms_completiondate_value2' => array(
      'label' => 'Milestone Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_ms_completiondate_value2',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_date_value2' => array(
      'label' => 'Period',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_date_value2',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_date_value' => array(
      'label' => 'Period',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value',
      ),
      'exclude' => 0,
      'id' => 'field_date_value',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_date_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_date_value',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'value0' => array(
      'order' => 'ASC',
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => '>=',
      'value' => array(
        'min' => NULL,
        'max' => NULL,
        'value' => NULL,
        'default_date' => 'now -1 year',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node_data_field_date.field_date_value2' => 'node_data_field_date.field_date_value2',
        'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => 'now -1 year',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('empty', '');
  $handler->override_option('style_plugin', 'views_jsgantt');
  $handler->override_option('style_options', array(
    'tasktitle' => 'title',
    'startdate' => 'field_date_value',
    'enddate' => 'field_date_value2',
    'progress' => '',
    'resource' => 'title',
    'parentid' => 'field_parentid_value',
    'parenttitle' => '',
    'dependson' => '',
    'colorsource' => 'progress',
    'colorby' => '',
    'colorlist' => '',
    'format' => 'week',
  ));
  $handler->override_option('attachment_position', 'before');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', FALSE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
    'page_1' => 'page_1',
    'default' => 0,
    'page_2' => 0,
    'page_5' => 0,
    'page_3' => 0,
    'block_1' => 0,
    'block_2' => 0,
    'page_4' => 0,
  ));
  $handler = $view->new_display('block', 'Tarefas da etapa', 'block_2');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Título',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'ops' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_type' => 'toggle',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
    ),
  ));
  $handler->override_option('arguments', array(
    'field_parentid_value' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'ignore',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
      ),
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'milestone' => 'milestone',
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'sheetnode' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => 'OG_VIEWS_DO_NOT_VALIDATE_MEMBERSHIP',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Tarefas desta etapa');
  $handler->override_option('empty', '');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'phpcode' => 'phpcode',
      'field_task_assignedto_uid' => 'field_task_assignedto_uid',
      'ops' => 'ops',
      'phpcode_1' => 'phpcode_1',
      'value0' => 'value0',
      'field_parentid_value' => 'field_parentid_value',
      'field_task_priority_value' => 'field_task_priority_value',
      'edit_node' => 'edit_node',
      'title' => 'title',
      'field_ms_completiondate_value' => 'field_ms_completiondate_value',
      'body' => 'body',
      'comment_count' => 'comment_count',
    ),
    'info' => array(
      'phpcode' => array(
        'separator' => '',
      ),
      'field_task_assignedto_uid' => array(
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
      'phpcode_1' => array(
        'separator' => '',
      ),
      'value0' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_parentid_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_priority_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_ms_completiondate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'body' => array(
        'separator' => '',
      ),
      'comment_count' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('page', 'Gantt detalhado', 'page_4');
  $handler->override_option('fields', array(
    'field_task_assignedto_uid' => array(
      'label' => 'Assigned To',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => 'Aguardando voluntário',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'ops' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_type' => 'toggle',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
    ),
    'phpcode_1' => array(
      'label' => 'Prioridade',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<?php
//echo \'<pre>\'.print_r($data,1).\'</pre>\';

if ($data->node_type == \'task\') {
  /* dim out if this task is completed */
  if ($data->flag_content_content_id != ""){
    switch($data->node_data_field_task_duedate_field_task_priority_value){
      case 1:
        echo "<span class=\'completed\'>High Priority";
        break;
      case 2:
        echo "<span class=\'completed\'>Medium Priority";
        break;
      case 3:
        echo "<span class=\'completed\'>Low Priority";
        break;
      default:
        echo "<span>";
        break;
    }
  } else {
    switch($data->node_data_field_task_duedate_field_task_priority_value){
      case 1:
        echo "<span class=\'priority-high\'>High Priority";
        break;
      case 2:
        echo "<span class=\'priority-med\'>Medium Priority";
        break;
      case 3:
        echo "<span class=\'priority-low\'>Low Priority";
        break;
      default:
        echo "<span>";
        break;
    }
  }
  echo "</span>";
}
?>',
      'exclude' => 1,
      'id' => 'phpcode_1',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'field_parentid_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Title',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_ms_completiondate_value' => array(
      'label' => 'Milestone Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value',
      ),
      'exclude' => 1,
      'id' => 'field_ms_completiondate_value',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'field_ms_completiondate_value2' => array(
      'label' => 'Milestone Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_ms_completiondate_value2',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_date_value2' => array(
      'label' => 'Period',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_date_value2',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_date_value' => array(
      'label' => 'Period',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value',
      ),
      'exclude' => 0,
      'id' => 'field_date_value',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_date_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_date_value',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'value0' => array(
      'order' => 'ASC',
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => 'not empty',
      'value' => array(
        'min' => NULL,
        'max' => NULL,
        'value' => NULL,
        'default_date' => '',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => '',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('empty', '<p>Você precisa adicionar etapas com data de início e fim pra visualizar o cronograma.</p>');
  $handler->override_option('style_plugin', 'views_jsgantt');
  $handler->override_option('style_options', array(
    'tasktitle' => 'title',
    'startdate' => 'field_date_value',
    'enddate' => 'field_date_value2',
    'progress' => '',
    'resource' => 'title',
    'parentid' => 'field_parentid_value',
    'parenttitle' => '',
    'dependson' => '',
    'colorsource' => 'progress',
    'colorby' => '',
    'colorlist' => '',
    'format' => 'day',
  ));
  $handler->override_option('path', 'tasks/gantt');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Cronograma detalhado',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler = $view->new_display('block', 'Bloco: Cronograma', 'block_3');
  $handler->override_option('fields', array(
    'field_parentid_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Title',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_ms_completiondate_value' => array(
      'label' => 'Milestone Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value',
      ),
      'exclude' => 1,
      'id' => 'field_ms_completiondate_value',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'field_ms_completiondate_value2' => array(
      'label' => 'Milestone Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_ms_completiondate_value2',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_date_value2' => array(
      'label' => 'Period',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_date_value2',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_date_value' => array(
      'label' => 'Period',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value',
      ),
      'exclude' => 0,
      'id' => 'field_date_value',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'value0' => array(
      'label' => 'Ordem',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'exclude' => 0,
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_date_value' => array(
      'order' => 'ASC',
      'delta' => -1,
      'id' => 'field_date_value',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'value0' => array(
      'order' => 'ASC',
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => '>=',
      'value' => array(
        'min' => NULL,
        'max' => NULL,
        'value' => NULL,
        'default_date' => 'now -1 week',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node_data_field_date.field_date_value' => 'node_data_field_date.field_date_value',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => 'now -1 week',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Próximas etapas no cronograma');
  $handler->override_option('empty', '');
  $handler->override_option('style_plugin', 'views_jsgantt');
  $handler->override_option('style_options', array(
    'tasktitle' => 'title',
    'startdate' => 'field_date_value',
    'enddate' => 'field_date_value2',
    'progress' => '',
    'resource' => 'title',
    'parentid' => 'field_parentid_value',
    'parenttitle' => '',
    'dependson' => '',
    'colorsource' => 'progress',
    'colorby' => 'value0',
    'colorlist' => '',
    'format' => 'month',
  ));
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('page', 'To Do', 'page_3');
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '0',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
  ));
  $handler->override_option('title', 'Tarefas a fazer');
  $handler->override_option('path', 'tasks/todo');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'To Do',
    'description' => '',
    'weight' => '-10',
    'name' => 'features',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => 'To-do Tasks',
    'description' => '',
    'weight' => '0',
    'name' => 'features',
  ));
  $handler = $view->new_display('page', 'Knowledge Cards', 'page_5');
  $handler->override_option('relationships', array(
    'field_referenced_book_page_nid' => array(
      'id' => 'field_referenced_book_page_nid',
      'table' => 'node_data_field_referenced_book_page',
      'field' => 'field_referenced_book_page_nid',
    ),
    'field_task_assignedto_uid' => array(
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
    ),
  ));
  $handler->override_option('fields', array(
    'field_input_value' => array(
      'label' => 'Input',
      'alter' => array(
        'alter_text' => 1,
        'text' => '<div class="cardfrente">
<div class="inputs">[field_input_value]</div>',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_input_value',
      'table' => 'node_data_field_input',
      'field' => 'field_input_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'field_referenced_book_page_nid',
    ),
    'phpcode_2' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'value' => '<?php
//print_r ($data);
if (!$data->node_node_data_field_referenced_book_page_type) {
  return l($data->node_title, "/node/".$data->nid);
}
?>',
      'exclude' => 0,
      'id' => 'phpcode_2',
      'table' => 'customfield',
      'field' => 'phpcode',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'title_1' => array(
      'label' => 'Conhecimento relacionado',
      'alter' => array(
        'alter_text' => 0,
        'text' => '<div class="title">[title_1]</div>',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 0,
      'hide_alter_empty' => 0,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title_1',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'field_referenced_book_page_nid',
    ),
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 1,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'field_output_value' => array(
      'label' => 'Ouput',
      'alter' => array(
        'alter_text' => 1,
        'text' => '<div class="outputs">[field_output_value]</div>
</div>',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_output_value',
      'table' => 'node_data_field_output',
      'field' => 'field_output_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'field_referenced_book_page_nid',
    ),
    'field_parentid_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'relationship' => 'none',
    ),
    'picture' => array(
      'label' => 'Responsável',
      'alter' => array(
        'alter_text' => 1,
        'text' => '<div class="card_pessoa">
[picture]',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'imagecache_preset' => 'imageboard-thumb',
      'exclude' => 0,
      'id' => 'picture',
      'table' => 'users',
      'field' => 'picture',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'field_task_assignedto_uid',
    ),
    'name' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 1,
        'text' => '[name]
</div>',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_user' => 1,
      'overwrite_anonymous' => 1,
      'anonymous_text' => 'Aguardando voluntários',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'field_task_assignedto_uid',
    ),
    'value0' => array(
      'label' => 'Ordem',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'exclude' => 0,
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'total_time' => array(
      'label' => 'Total time',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'set_precision' => 0,
      'precision' => '0',
      'decimal' => '.',
      'separator' => ',',
      'format_plural' => 0,
      'format_plural_singular' => '1',
      'format_plural_plural' => '@count',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 1,
      'id' => 'total_time',
      'table' => 'time_tracker_totals',
      'field' => 'total_time',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'estimate' => array(
      'label' => 'Estimativa de horas',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'exclude' => 1,
      'id' => 'estimate',
      'table' => 'time_estimate_entry',
      'field' => 'estimate',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_date_value2' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'day',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'both',
      ),
      'exclude' => 0,
      'id' => 'field_date_value2',
      'table' => 'node_data_field_date',
      'field' => 'field_date_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'phpcode' => array(
      'label' => 'Prazo',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'value' => '<?php
$day_hours = 8;
$total = round($data->time_tracker_totals_total_time);
$estimate = $data->time_estimate_entry_estimate;
/*
$node = node_load($data->nid);
$estimate = $node->time_estimate_entry_estimate;
$total = round($node->time_tracker_totals_total_time);
*/
$cards_number = $total / $day_hours;
$extra_cards = $cards_number - $estimate / $day_hours;
$resto = $estimate % $day_hours;
echo \'<div class="card_time">\';
while ($cards_number > $i) {
  if ($extra_cards > 0) {
      echo \'<div class="cardfrente extra">\' ;
  } else {
      echo \'<div class="cardfrente">\';
  }
  echo \'<a href="/node/\' . $data->nid .\'">\';
  echo theme_image(drupal_get_path(\'module\', \'features_task\') . \'/icon_time_cards.png\', t(\'Time icon\'));
  echo \'<br />\';
  echo $total . \' \' . t(\'hours\') ;
/*  if ($cards_number - $i > 1) {
      echo $day_hours . \' \' . t(\'hours\'); 
  } elseif ($i > 0) {
      echo $i  . \' \'. t(\'days and\') . $resto . \' \' . t(\'hours\');
  } else {
      echo $resto . \' \' . t(\'hours\');
  }*/
  echo \'</a></div>\';
  $i++;
  $extra_cards--; 
} 
echo \'</div>\';
 ?>',
      'exclude' => 0,
      'id' => 'phpcode',
      'table' => 'customfield',
      'field' => 'phpcode',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'label' => 'Tipo',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'machine_name' => 0,
      'exclude' => 0,
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'view_node' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'text' => 'view',
      'exclude' => 0,
      'id' => 'view_node',
      'table' => 'node',
      'field' => 'view_node',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'edit_node' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'text' => '',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Cartões de Conhecimento');
  $handler->override_option('css_class', 'card');
  $handler->override_option('distinct', 1);
  $handler->override_option('style_options', array(
    'override' => 0,
    'sticky' => 0,
    'order' => 'asc',
    'summary' => '',
    'columns' => array(
      'field_input_value' => 'title_1',
      'phpcode_2' => 'title_1',
      'title_1' => 'title_1',
      'title' => 'title',
      'field_output_value' => 'title_1',
      'field_parentid_value' => 'field_parentid_value',
      'picture' => 'picture',
      'name' => 'picture',
      'value0' => 'value0',
      'total_time' => 'total_time',
      'estimate' => 'estimate',
      'field_date_value2' => 'phpcode',
      'phpcode' => 'phpcode',
      'view_node' => 'view_node',
      'edit_node' => 'view_node',
    ),
    'info' => array(
      'field_input_value' => array(
        'separator' => '',
      ),
      'phpcode_2' => array(
        'separator' => '',
      ),
      'title_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_output_value' => array(
        'separator' => '',
      ),
      'field_parentid_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'picture' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'value0' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'total_time' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'estimate' => array(
        'separator' => '',
      ),
      'field_date_value2' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'phpcode' => array(
        'separator' => '',
      ),
      'view_node' => array(
        'separator' => '<br />',
      ),
      'edit_node' => array(
        'separator' => '',
      ),
    ),
    'default' => -1,
    'tabledrag_order' => array(
      'field' => 'value0',
      'handler' => 'native',
    ),
    'tabledrag_order_visible' => array(
      'visible' => 0,
    ),
    'tabledrag_hierarchy' => array(
      'field' => 'field_parentid_value',
      'handler' => 'cck',
    ),
    'tabledrag_hierarchy_visible' => array(
      'visible' => 0,
    ),
    'draggableviews_depth_limit' => '1',
    'draggableviews_repair' => array(
      'repair' => 0,
    ),
    'tabledrag_types' => array(
      0 => array(
        'node_type' => 'task',
        'type' => 'leaf',
        'tabledrag_type_del_button' => 'Apagar',
      ),
      1 => array(
        'node_type' => 'milestone',
        'type' => 'root',
        'tabledrag_type_del_button' => 'Apagar',
      ),
    ),
    'tabledrag_types_add' => 'Add type',
    'tabledrag_expand' => array(
      'expand_links' => 0,
      'collapsed' => 0,
      'by_uid' => 0,
    ),
    'draggableviews_extensions' => array(
      'extension_top' => '3',
      'extension_bottom' => '3',
    ),
    'tabledrag_lock' => array(
      'lock' => 0,
    ),
    'draggableviews_default_on_top' => '1',
    'draggableviews_button_text' => 'Salvar ordem',
    'draggableviews_arguments' => array(
      'use_args' => 0,
    ),
    'draggableviews_book_radios' => '0',
    'draggableviews_book_argument' => 'none',
    'draggableviews_book_filter' => 'status',
  ));
  $handler->override_option('path', 'tasks/cards');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Knowledge Cards',
    'description' => '',
    'weight' => '10',
    'name' => 'features',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => 'Tasks',
    'description' => '',
    'weight' => '0',
    'name' => 'features',
  ));
  $handler = $view->new_display('block', 'My Tasks Taskbar', 'block_4');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 0,
      'flag' => 'completed',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
    'field_task_assignedto_uid' => array(
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
    ),
  ));
  $handler->override_option('fields', array(
    'group_nid' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => FALSE,
        'text' => '',
        'make_link' => FALSE,
        'path' => '',
        'absolute' => '',
        'alt' => '',
        'rel' => '',
        'link_class' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'trim' => FALSE,
        'max_length' => '',
        'word_boundary' => TRUE,
        'ellipsis' => TRUE,
        'strip_tags' => FALSE,
        'html' => FALSE,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'type' => 'separator',
      'separator' => ', ',
      'spaces_og_frontpage' => 1,
      'crayon' => array(
        'enabled' => 'enabled',
        'acronym' => 'acronym',
      ),
      'exclude' => 0,
      'id' => 'group_nid',
      'table' => 'og_ancestry',
      'field' => 'group_nid',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'ops' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_type' => 'toggle',
      'exclude' => 1,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '35',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'uid_current' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'uid_current',
      'table' => 'users',
      'field' => 'uid_current',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'field_task_assignedto_uid',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '0',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Tarefas Pendentes');
  $handler->override_option('empty', '');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => 'ul',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'group_nid' => 'group_nid',
      'ops' => 'ops',
      'title' => 'title',
    ),
    'separator' => '',
    'hide_empty' => 0,
  ));
  $handler->override_option('block_description', 'Minhas tarefas');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->override_option('pane_title', '');
  $handler->override_option('pane_description', '');
  $handler->override_option('pane_category', array(
    'name' => 'View panes',
    'weight' => 0,
  ));
  $handler->override_option('allow', array(
    'use_pager' => 0,
    'items_per_page' => 0,
    'offset' => 0,
    'link_to_view' => 0,
    'more_link' => 0,
    'path_override' => 0,
    'title_override' => 'title_override',
    'exposed_form' => 0,
    'fields_override' => 0,
  ));
  $handler->override_option('argument_input', array());
  $handler->override_option('link_to_view', 0);
  $handler->override_option('inherit_panels_path', 0);
  $translatables['oa_custom_task2'] = array(
    t('<p>Ferramenta para gerenciar o que deve ser feito em um projeto. A etapa tem uma data definida e aparece no cronograma, enquanto as tarefas não tem data definida, mas tem responsáveis. Adicione uma nova Etapa ou Tarefa no menu Criar Conteúdo.</p>'),
    t('<p>Você precisa adicionar etapas com data de início e fim pra visualizar o cronograma.</p>'),
    t('Bloco: Cronograma'),
    t('Bloco: Progresso Etapas'),
    t('Content pane'),
    t('Defaults'),
    t('Etapas e Tarefas'),
    t('Gantt Etapas'),
    t('Gantt detalhado'),
    t('Knowledge Cards'),
    t('My Tasks'),
    t('My Tasks Taskbar'),
    t('Pending Tasks'),
    t('Progresso'),
    t('Próximas etapas no cronograma'),
    t('Tarefas a fazer'),
    t('Tarefas da etapa'),
    t('Tarefas desta etapa'),
    t('Tasks'),
    t('To Do'),
  );

  $views[$view->name] = $view;

  // Exported view: oa_milestones
  $view = new view;
  $view->name = 'oa_milestones';
  $view->description = 'Milestones';
  $view->tag = 'oa_custom';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 0,
      'flag' => 'completed',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'phpcode' => array(
      'label' => 'Título da Etapa/Tarefa',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<?php
//echo \'<pre>\'.print_r($data,1).\'</pre>\';

$iconpath = drupal_get_path(\'module\', \'features_task\');
if ($data->node_type == \'milestone\') {
  $targetdate = $data->node_data_field_ms_completiondate_field_ms_completiondate_value;
  if ($targetdate != ""){
    $targetdate = \'[\'.date("m-d-Y", strtotime($targetdate)).\']\';
  }
  echo \'<a href="node/\'.$data->nid.\'"><img title="\'.$data->node_title." - ".$data->node_revisions_body.\'" style="padding-bottom: 4px; vertical-align: middle;" src="/\'.$iconpath.\'/milestone-small.png" /><strong>&nbsp;\'.features_task_neat_trim($data->node_title,40)."</strong></a> <span style=\'font-style: italic;text-decoration: underline\'><a href=\'node/add/task?edit[field_parentid][0][value]=".$data->nid."&edit[space_og][gid][value]=".$data->og_ancestry_group_nid."\'>".t("add task")."</a></span>";
} elseif ($data->node_type == \'task\') {
  $class = "notcompleted";
  if ($data->flag_content_content_id != ""){
    $class ="completed";
  }
  
  if($data->node_comment_statistics_comment_count > 0){
    $comcount = "&nbsp;(".$data->node_comment_statistics_comment_count.")&nbsp;";
  }
  echo \'<a href="node/\'.$data->nid.\'"><span class="\'.$class.\'">\'.features_task_neat_trim($data->node_title,40).$comcount.\'</span></a>\';  
}
?>',
      'exclude' => 0,
      'id' => 'phpcode',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'field_task_assignedto_uid' => array(
      'label' => 'Assigned To',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => 'Aguardando voluntário',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'ops' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_type' => 'toggle',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
    ),
    'phpcode_1' => array(
      'label' => 'Prioridade',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'value' => '<?php
//echo \'<pre>\'.print_r($data,1).\'</pre>\';

if ($data->node_type == \'task\') {
  /* dim out if this task is completed */
  if ($data->flag_content_content_id != ""){
    switch($data->node_data_field_task_duedate_field_task_priority_value){
      case 1:
        echo "<span class=\'completed\'>High Priority";
        break;
      case 2:
        echo "<span class=\'completed\'>Medium Priority";
        break;
      case 3:
        echo "<span class=\'completed\'>Low Priority";
        break;
      default:
        echo "<span>";
        break;
    }
  } else {
    switch($data->node_data_field_task_duedate_field_task_priority_value){
      case 1:
        echo "<span class=\'priority-high\'>High Priority";
        break;
      case 2:
        echo "<span class=\'priority-med\'>Medium Priority";
        break;
      case 3:
        echo "<span class=\'priority-low\'>Low Priority";
        break;
      default:
        echo "<span>";
        break;
    }
  }
  echo "</span>";
}
?>',
      'exclude' => 1,
      'id' => 'phpcode_1',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'value0' => array(
      'label' => 'Ordem',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'field_parentid_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'relationship' => 'none',
    ),
    'field_task_priority_value' => array(
      'label' => 'Priority',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_task_priority_value',
      'table' => 'node_data_field_task_priority',
      'field' => 'field_task_priority_value',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'edit_node' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'text' => '',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Title',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_ms_completiondate_value' => array(
      'label' => 'Milestone Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => TRUE,
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'both',
      ),
      'exclude' => 1,
      'id' => 'field_ms_completiondate_value',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value',
      'relationship' => 'none',
    ),
    'body' => array(
      'label' => 'Body',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
      'relationship' => 'none',
    ),
    'comment_count' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 1,
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 1,
      'id' => 'comment_count',
      'table' => 'node_comment_statistics',
      'field' => 'comment_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'value0' => array(
      'order' => 'ASC',
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '0',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'spaces_feature',
    'spaces_feature' => 'features_task',
    'perm' => 'access content',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Etapas e Tarefas');
  $handler->override_option('header_format', '7');
  $handler->override_option('header_empty', 0);
  $handler->override_option('footer_format', '4');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('empty_format', '4');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'draggabletable');
  $handler->override_option('style_options', array(
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'phpcode' => 'phpcode',
      'field_task_assignedto_uid' => 'field_task_assignedto_uid',
      'ops' => 'ops',
      'phpcode_1' => 'phpcode_1',
      'value0' => 'value0',
      'field_parentid_value' => 'field_parentid_value',
      'field_task_priority_value' => 'field_task_priority_value',
      'edit_node' => 'edit_node',
      'title' => 'title',
      'field_ms_completiondate_value' => 'field_ms_completiondate_value',
      'body' => 'body',
      'comment_count' => 'comment_count',
    ),
    'info' => array(
      'phpcode' => array(
        'separator' => '',
      ),
      'field_task_assignedto_uid' => array(
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
      'phpcode_1' => array(
        'separator' => '',
      ),
      'value0' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_parentid_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_priority_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_ms_completiondate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'body' => array(
        'separator' => '',
      ),
      'comment_count' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => -1,
    'tabledrag_order' => array(
      'field' => 'value0',
      'handler' => 'native',
    ),
    'tabledrag_order_visible' => array(
      'visible' => 0,
    ),
    'tabledrag_hierarchy' => array(
      'field' => 'field_parentid_value',
      'handler' => 'cck',
    ),
    'tabledrag_hierarchy_visible' => array(
      'visible' => 0,
    ),
    'draggableviews_depth_limit' => '1',
    'draggableviews_repair' => array(
      'repair' => 'repair',
    ),
    'tabledrag_types' => array(
      0 => array(
        'node_type' => 'milestone',
        'type' => 'leaf',
        'tabledrag_type_del_button' => 'Apagar',
      ),
      1 => array(
        'node_type' => 'milestone',
        'type' => 'root',
        'tabledrag_type_del_button' => 'Apagar',
      ),
    ),
    'tabledrag_types_add' => 'Add type',
    'tabledrag_expand' => array(
      'expand_links' => 'expand_links',
      'collapsed' => 0,
      'by_uid' => 0,
    ),
    'draggableviews_extensions' => array(
      'extension_top' => '3',
      'extension_bottom' => '3',
    ),
    'tabledrag_lock' => array(
      'lock' => 0,
    ),
    'draggableviews_default_on_top' => '1',
    'draggableviews_button_text' => 'Salvar ordem',
    'draggableviews_arguments' => array(
      'use_args' => 0,
    ),
    'draggableviews_book_radios' => '0',
    'draggableviews_book_argument' => 'none',
    'draggableviews_book_filter' => 'status',
  ));
  $handler = $view->new_display('block', 'Bloco', 'block_1');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'field_ms_completiondate_value2' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 1,
      ),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'format_interval',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_ms_completiondate_value2',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value2',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_ms_completiondate_value' => array(
      'id' => 'field_ms_completiondate_value',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value',
    ),
    'value0' => array(
      'order' => 'ASC',
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '0',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
  ));
  $handler->override_option('title', 'Progresso');
  $handler->override_option('style_plugin', 'default');
  $handler->override_option('style_options', array(
    'grouping' => '',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'title' => 'title',
      'field_ms_completiondate_value2' => 'field_ms_completiondate_value2',
    ),
    'separator' => ' - ',
    'hide_empty' => 0,
  ));
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('block', 'Tarefas da etapa', 'block_2');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Título',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
    'ops' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_type' => 'toggle',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
    ),
  ));
  $handler->override_option('arguments', array(
    'field_parentid_value' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'ignore',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
      ),
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'milestone' => 'milestone',
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'sheetnode' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => 'OG_VIEWS_DO_NOT_VALIDATE_MEMBERSHIP',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Tarefas desta etapa');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'phpcode' => 'phpcode',
      'field_task_assignedto_uid' => 'field_task_assignedto_uid',
      'ops' => 'ops',
      'phpcode_1' => 'phpcode_1',
      'value0' => 'value0',
      'field_parentid_value' => 'field_parentid_value',
      'field_task_priority_value' => 'field_task_priority_value',
      'edit_node' => 'edit_node',
      'title' => 'title',
      'field_ms_completiondate_value' => 'field_ms_completiondate_value',
      'body' => 'body',
      'comment_count' => 'comment_count',
    ),
    'info' => array(
      'phpcode' => array(
        'separator' => '',
      ),
      'field_task_assignedto_uid' => array(
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
      'phpcode_1' => array(
        'separator' => '',
      ),
      'value0' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_parentid_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_priority_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_ms_completiondate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'body' => array(
        'separator' => '',
      ),
      'comment_count' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $translatables['oa_milestones'] = array(
    t('Bloco'),
    t('Defaults'),
    t('Etapas e Tarefas'),
    t('Progresso'),
    t('Tarefas da etapa'),
    t('Tarefas desta etapa'),
  );

  $views[$view->name] = $view;

  // Exported view: task_complete
  $view = new view;
  $view->name = 'task_complete';
  $view->description = 'Display a progress bar of tasks completed.';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 0,
      'flag' => 'completed',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_content_rel',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'build_mode' => 'teaser',
    'links' => 1,
    'comments' => 0,
  ));
  $translatables['task_complete'] = array(
    t('Padrões'),
  );

  $views[$view->name] = $view;

  // Exported view: task_gant
  $view = new view;
  $view->name = 'task_gant';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 1,
        'max_length' => '20',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_ms_completiondate_value2' => array(
      'label' => 'Fim da etapa',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'custom',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value2',
      ),
      'exclude' => 0,
      'id' => 'field_ms_completiondate_value2',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value2',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'field_ms_completiondate_value' => array(
      'label' => 'Início da etapa',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'custom',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value',
      ),
      'exclude' => 0,
      'id' => 'field_ms_completiondate_value',
      'table' => 'node_data_field_ms_completiondate',
      'field' => 'field_ms_completiondate_value',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'field_parentid_value' => array(
      'id' => 'field_parentid_value',
      'table' => 'node_data_field_parentid',
      'field' => 'field_parentid_value',
    ),
    'field_task_assignedto_uid' => array(
      'label' => 'Assigned To',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'milestone' => 'milestone',
        'task' => 'task',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'current' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'relationship' => 'none',
    ),
    'date_filter' => array(
      'operator' => '>=',
      'value' => array(
        'value' => NULL,
        'min' => NULL,
        'max' => NULL,
        'default_date' => 'now -1 year',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'date_fields' => array(
        'node_data_field_ms_completiondate.field_ms_completiondate_value' => 'node_data_field_ms_completiondate.field_ms_completiondate_value',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => 'now -1 year',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'views_jsgantt');
  $handler->override_option('style_options', array(
    'tasktitle' => 'title',
    'startdate' => 'field_ms_completiondate_value',
    'enddate' => 'field_ms_completiondate_value2',
    'progress' => '',
    'resource' => 'field_task_assignedto_uid',
    'parentid' => 'field_parentid_value',
    'parenttitle' => '',
    'dependson' => '',
    'colorsource' => 'progress',
    'colorby' => '',
    'colorlist' => '',
    'format' => 'week',
  ));
  $handler = $view->new_display('block', 'Bloco', 'block_1');
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('page', 'Página', 'page_1');
  $handler->override_option('path', 'taskgantt');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $translatables['task_gant'] = array(
    t('Bloco'),
    t('Padrões'),
    t('Página'),
  );

  $views[$view->name] = $view;

  return $views;
}
