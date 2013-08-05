<?php

/**
 * Helper to implementation of hook_views_default_views().
 */
function _features_task_views_default_views() {
  $views = array();

  // Exported view: oa_custom_members
  $view = new view;
  $view->name = 'oa_custom_members';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'users';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'name' => array(
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
      'link_to_user' => 0,
      'overwrite_anonymous' => 0,
      'anonymous_text' => '',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'users_current' => array(
      'operator' => 'all',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'users_current',
      'table' => 'spaces',
      'field' => 'users_current',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));

  $views[$view->name] = $view;

  // Exported view: oa_custom_task2
  $view = new view;
  $view->name = 'oa_custom_task2';
  $view->description = '';
  $view->tag = 'oa_custom';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 0,
      'flag' => 'task_status',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'phpcode' => array(
      'label' => 'Task/Milestone Title',
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

$iconpath = drupal_get_path(\'module\', \'features_task\');
if ($data->node_type == \'milestone\') {
  $targetdate = $data->node_data_field_ms_completiondate_field_ms_completiondate_value;
  if ($targetdate != ""){
    $targetdate = \'[\'.date("m-d-Y", strtotime($targetdate)).\']\';
  }
  echo \'<img title="\'.$data->node_title." - ".$data->node_revisions_body.\'" style="padding-bottom: 4px; vertical-align: middle;" src="/\'.$iconpath.\'/milestone-small.png" /><strong>&nbsp;\'.features_task_neat_trim($data->node_title,40).\'</strong>&nbsp;\'.$targetdate.\'&nbsp;&nbsp;\' ."<span style=\'font-style: italic;text-decoration: underline\'><a href=\'node/add/task?edit[field_parentid][0][value]=".$data->nid."&edit[space_og][gid][value]=".$data->og_ancestry_group_nid."\'>(add task)</a></span>";
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
        'button' => 'Override',
      ),
    ),
    'field_task_duedate_value' => array(
      'label' => 'Due Date',
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
      'format' => 'short',
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
      'id' => 'field_task_duedate_value',
      'table' => 'node_data_field_task_duedate',
      'field' => 'field_task_duedate_value',
      'relationship' => 'none',
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
      'link_type' => 'toggle',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
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
      'exclude' => 0,
      'id' => 'phpcode_1',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
    ),
    'phpcode_2' => array(
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
      'id' => 'phpcode_2',
      'table' => 'customfield',
      'field' => 'phpcode',
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
    'value0' => array(
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
      'exclude' => 0,
      'id' => 'value0',
      'table' => 'draggableviews_structure_node0',
      'field' => 'value0',
      'override' => array(
        'button' => 'Override',
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
      'format' => 'user-s_imagelink',
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
      'relationship' => 'none',
    ),
    'field_task_datecompleted_value' => array(
      'label' => 'Date Completed',
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
      'id' => 'field_task_datecompleted_value',
      'table' => 'node_data_field_task_datecompleted',
      'field' => 'field_task_datecompleted_value',
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
        'task' => 'task',
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
        'button' => 'Override',
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
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Milestones and Tasks');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'draggabletable');
  $handler->override_option('style_options', array(
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'phpcode' => 'phpcode',
      'field_ms_completiondate_value' => 'field_ms_completiondate_value',
      'field_task_duedate_value' => 'field_task_duedate_value',
      'field_task_assignedto_uid' => 'field_task_assignedto_uid',
      'field_task_attachments_fid' => 'field_task_attachments_fid',
      'field_task_priority_value' => 'field_task_priority_value',
      'field_task_status_value' => 'field_task_status_value',
      'timestamp' => 'timestamp',
      'edit_node' => 'edit_node',
      'value0' => 'value0',
      'field_parentid_value' => 'field_parentid_value',
      'title' => 'title',
    ),
    'info' => array(
      'phpcode' => array(
        'separator' => '',
      ),
      'field_ms_completiondate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_duedate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_assignedto_uid' => array(
        'separator' => '',
      ),
      'field_task_attachments_fid' => array(
        'separator' => '',
      ),
      'field_task_priority_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_status_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'timestamp' => array(
        'separator' => '',
      ),
      'edit_node' => array(
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
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
    'tabledrag_hierarchy' => array(
      'field' => 'field_parentid_value',
      'handler' => 'cck',
    ),
    'tabledrag_order' => array(
      'field' => 'value0',
      'handler' => 'native',
    ),
    'tabledrag_types' => array(
      '0' => array(
        'node_type' => 'task',
        'type' => 'leaf',
        'tabledrag_type_del_button' => 'Delete',
      ),
    ),
    'draggableviews_extensions' => array(
      'extension_top' => '3',
      'extension_bottom' => '3',
    ),
    'tabledrag_order_visible' => array(
      'visible' => 0,
    ),
    'tabledrag_hierarchy_visible' => array(
      'visible' => 0,
    ),
    'draggableviews_depth_limit' => '-1',
    'draggableviews_repair' => array(
      'repair' => 'repair',
    ),
    'tabledrag_types_add' => 'Add type',
    'tabledrag_expand' => array(
      'expand_links' => 'expand_links',
      'collapsed' => 0,
      'by_uid' => 0,
    ),
    'tabledrag_lock' => array(
      'lock' => 0,
    ),
    'draggableviews_default_on_top' => '1',
    'draggableviews_button_text' => 'Save order',
    'draggableviews_arguments' => array(
      'use_args' => 0,
    ),
  ));
  $handler = $view->new_display('page', 'Parent Page', 'page_1');
  $handler->override_option('path', 'tasks');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Tasks',
    'description' => '',
    'weight' => '-10',
    'name' => 'features',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));
  $handler = $view->new_display('page', 'My Tasks', 'page_2');
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
    'field_task_assignedto_uid' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_task_assignedto_uid_op',
        'identifier' => 'field_task_assignedto_uid',
        'label' => 'Filter by Assigned User: ',
        'optional' => 1,
        'single' => 1,
        'remember' => 1,
        'reduce' => 0,
      ),
      'id' => 'field_task_assignedto_uid',
      'table' => 'node_data_field_task_assignedto',
      'field' => 'field_task_assignedto_uid',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
      'reduce_duplicates' => 0,
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
    'type' => 'tab',
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
  ));
  $handler = $view->new_display('page', 'All Tasks', 'page_5');
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
        'button' => 'Override',
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
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_task_status_value_many_to_one' => array(
      'operator' => 'not',
      'value' => array(
        '7' => '7',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'field_task_status_value_many_to_one',
      'table' => 'node_data_field_task_status',
      'field' => 'field_task_status_value_many_to_one',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
  ));
  $handler->override_option('path', 'tasks/all-current');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'All Tasks',
    'description' => '',
    'weight' => '-20',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => '0',
  ));
  $handler = $view->new_display('page', 'All Completed', 'page_3');
  $handler->override_option('fields', array(
    'title_1' => array(
      'label' => 'Task Name',
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
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title_1',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'field_task_duedate_value' => array(
      'label' => 'Due Date',
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
      'format' => 'short',
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
      'id' => 'field_task_duedate_value',
      'table' => 'node_data_field_task_duedate',
      'field' => 'field_task_duedate_value',
      'relationship' => 'none',
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
      'relationship' => 'none',
      'override' => array(
        'button' => 'Use default',
      ),
    ),
    'phpcode_2' => array(
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
      'exclude' => 1,
      'id' => 'phpcode_2',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Use default',
      ),
    ),
    'field_task_whocompleted_uid' => array(
      'label' => 'Who Competed',
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
      'exclude' => 0,
      'id' => 'field_task_whocompleted_uid',
      'table' => 'node_data_field_task_whocompleted',
      'field' => 'field_task_whocompleted_uid',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'field_task_datecompleted_value_1' => array(
      'label' => 'Date Completed',
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
      'exclude' => 0,
      'id' => 'field_task_datecompleted_value_1',
      'table' => 'node_data_field_task_datecompleted',
      'field' => 'field_task_datecompleted_value',
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
  ));
  $handler->override_option('sorts', array(
    'field_task_datecompleted_value' => array(
      'order' => 'DESC',
      'delta' => -1,
      'id' => 'field_task_datecompleted_value',
      'table' => 'node_data_field_task_datecompleted',
      'field' => 'field_task_datecompleted_value',
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
        'button' => 'Override',
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
      'override' => array(
        'button' => 'Use default',
      ),
    ),
  ));
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'phpcode' => 'phpcode',
      'title_1' => 'title_1',
      'field_task_duedate_value' => 'field_task_duedate_value',
      'field_task_assignedto_uid' => 'field_task_assignedto_uid',
      'phpcode_1' => 'phpcode_1',
      'phpcode_2' => 'phpcode_2',
      'field_task_whocompleted_uid' => 'field_task_whocompleted_uid',
      'field_task_datecompleted_value_1' => 'field_task_datecompleted_value_1',
      'edit_node' => 'edit_node',
      'value0' => 'value0',
      'field_parentid_value' => 'field_parentid_value',
      'field_task_priority_value' => 'field_task_priority_value',
      'title' => 'title',
      'field_ms_completiondate_value' => 'field_ms_completiondate_value',
      'body' => 'body',
      'comment_count' => 'comment_count',
      'field_task_attachments_fid' => 'field_task_attachments_fid',
      'field_task_datecompleted_value' => 'field_task_datecompleted_value',
      'ops' => 'ops',
    ),
    'info' => array(
      'phpcode' => array(
        'separator' => '',
      ),
      'title_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_duedate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_task_assignedto_uid' => array(
        'separator' => '',
      ),
      'phpcode_1' => array(
        'separator' => '',
      ),
      'phpcode_2' => array(
        'separator' => '',
      ),
      'field_task_whocompleted_uid' => array(
        'separator' => '',
      ),
      'field_task_datecompleted_value_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'edit_node' => array(
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
      'field_task_attachments_fid' => array(
        'separator' => '',
      ),
      'field_task_datecompleted_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler->override_option('path', 'tasks/all-completed');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'All Completed',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));

  $views[$view->name] = $view;

  return $views;
}
