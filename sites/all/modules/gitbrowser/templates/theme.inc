<?php
// $Id: theme.inc,v 1.8 2009/07/12 04:18:29 gordon Exp $

function template_preprocess_commit_info(&$variables) {
  drupal_add_css(drupal_get_path('module', 'gb') .'/gb.css');
    
  $info = $variables['info'];
      
  _gb_format_hash($variables['path'], TRUE);
  _gb_format_refs($variables['path'], TRUE);
  
  $variables['parents'] = isset($info['parent']) ? implode("<br />\n", array_map('_gb_format_hash', $info['parent'])) : '';
  $variables['tree'] = isset($info['tree']) ? implode("<br />\n", array_map('_gb_format_hash', $info['tree'])) : '';
  
  $variables['description'] = isset($info['description']) ? preg_replace('/#(\d+)/i', '<a href="http://drupal.org/node/\1">#\1</a>', implode("<br />\n", $info['description'])) : '';
  $variables['author'] =  isset($info['author']) ? theme('format_person', $info['author']) : '';
  
  $variables['committers'] = isset($info['committer']) ? implode("<br />\n", array_map('_gb_format_person', $info['committer'])) : '';
  $variables['refs'] = implode('', array_map('_gb_format_refs', $info['refs']));
  
  $variables['changes'] = theme('format_file_list', $info, $variables['path'], $variables['repos']);
  
  $variables+= $info;
}

function template_preprocess_commit_diff(&$variables) {
  drupal_add_css(drupal_get_path('module', 'gb') .'/gb.css');
    
  $info = $variables['info'];
  
  $variables['files'] = theme('format_file_list', $info, $variables['path'], $variables['repos']);
  
  $diff = implode("\n", gb_diff($variables['repos'], $variables['parent'], $info['hash']));
  $diff = str_replace(' ', '&nbsp;', htmlspecialchars($diff));
  
  $diff = preg_replace('/^diff\&nbsp;\-\-git\&nbsp;.*$/im', '<div class="diff-command">\0</div>', $diff);
  $diff = preg_replace('/^new\&nbsp;.*$/im', '<div>\0</div>', $diff);
  $diff = preg_replace('/^index\&nbsp;.*$/im', '<div class="diff-index">\0</div>', $diff);
  $diff = preg_replace('/^\+\+\+\&nbsp;.*$/im', '<div class="diff-new">\0</div>', $diff);
  $diff = preg_replace('/^\-\-\-\&nbsp;.*$/im', '<div class="diff-old">\0</div>', $diff);
  $diff = preg_replace('/^\+.*$/im', '<div class="diff-add">\0</div>', $diff);
  $diff = preg_replace('/^\-.*$/im', '<div class="diff-delete">\0</div>', $diff);
  $diff = preg_replace('/^\@.*$/im', '<div>\0</div>', $diff);
  $diff = preg_replace('/^\&nbsp;.*$/im', '<div>\0</div>', $diff);
    
  $variables['diff'] =& $diff;
  
  $variables+= $info;
}

function template_preprocess_tag_info(&$variables) {
  drupal_add_css(drupal_get_path('module', 'gb') .'/gb.css');
    
  $info = $variables['info'];
      
  _gb_format_hash($variables['path'], TRUE);
  _gb_format_refs($variables['path'], TRUE);
    
  $variables['description'] = isset($info['description']) ? preg_replace('/#(\d+)/i', '<a href="http://drupal.org/node/\1">#\1</a>', implode("<br />\n", $info['description'])) : '';
  $variables['tagger'] =  isset($info['tagger']) ? theme('format_person', $info['tagger']) : '';
  
  $variables+= $info;
}

function template_preprocess_format_file_list(&$variables) {
  $info =& $variables['info'];
  
  if (!empty($info['files'])) {
    $head = array(t('Files'), '');
    $rows = array_map('_gb_theme_format_files', $info['files']);
    
    $variables['files'] = theme('table', $head, $rows);
  }
  else {
    $variables['template files'][] = 'format-files-list-no-files';
  }
}

function _gb_theme_format_files($a) {
  return array(
    'data' => array(
      array(
        'data' => $a['filename'],
        'class' => 'filename',
      ),
      array(
        'data' => _gb_get_change_description($a),
        'class' => ($a['status'] == 'A' ? 'new-file' : ($a['status'] == 'D' ? 'delete-file' : 'change')),
      ),
    )
  );
}

function template_preprocess_format_person(&$variables) {
  $variables+= $variables['person'];
  
  $info['name'] .' &lt;'. $info['mail'] .'&gt; - '. 
  $variables['date'] = format_date($variables['date']);
}