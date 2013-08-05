<?php
// $Id: gb.inc,v 1.29 2009/11/04 06:32:05 gordon Exp $
/**
 * @file
 * Provide all git integration
 */

/**
 * Display Summary Page for the git repository
 */
function gb_summary($path, $repos, $hash = 'HEAD') {
  $repos = realpath($repos);

  $output = '';

  $output.= '<h3>'. t('shortlog') .'</h3>';
  $output.= gb_get_short_log($path, $repos, $hash);

  if ($tag_list = gb_get_tag_list($path, $repos, $hash, 16)) {
    $output.= '<h3>'. t('tags') .'</h3>';
    $output.= $tag_list;
  }
  
  return $output;
}

function gb_shortlog($path, $repos, $head = 'HEAD') {
  global $pager_page_array, $pager_total;
  $page = isset($_GET['page']) ? $_GET['page'] : '';
  $pager_page_array = explode(',', $page);
  $element = 0;
  
  if (empty($pager_page_array[$element])) {
    $pager_page_array[$element] = 0;
  }
  
  $refs = gb_rev_list($repos, NULL, $head);
  $chunks = array_chunk($refs, 50);
  
  $pager_total[$element] = count($chunks);
  
  $start = array_shift($chunks[$pager_page_array[$element]]);
  
  $output = gb_get_short_log($path, $repos, $start, 50);
  $output.= theme('pager', array(), 20, $element);
  
  return $output;
}

function gb_create_tag(&$form_state, $path, $repos, $hash) {
  if (isset($_SESSION['gb_success'])) {
    drupal_set_message($_SESSION['gb_success'] ? t('Tag successfully created') : t('Tag was not created, please check logs for the error.'));
    unset($_SESSION['gb_success']);
  }
  
  if (gb_check_status()) {
    drupal_set_message(t('The current enviroment has uncommitted changes. Any created tags will not be a true representation of the current system.'), 'error');
  }
  
  if ($hash == 'HEAD' && $info = gb_get_commit_info($repos, $hash)) {
    $hash = $info['hash'];
  }
  
  $form = array();
  
  $form['params'] = array(
    '#type' => 'value',
    '#value' => array(
      'path' => $path,
      'repos' => $repos,
      'hash' => $hash,
    ),
  );
  
  $form['tag_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Tag identifier'),
    '#description' => t('Enter a relevant identifier which resonably represents the reason for the tag being created. eg. release-1, hotfix-5. The can only contain alphanumeric characters and _-.'),
    '#required' => TRUE,
  );
  
  $form['anotation'] = array(
    '#type' => 'textarea',
    '#title' => t('Anotation'),
    '#description' => t('Add any additional information which will be recorded with the tag for later reference.'),
  );
  
  $form['push_to_origin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Push tag to upstream respository.'),
    '#description' => t('Checking this will send the tag that you are creating to the upstream repository. If the web user does not have write access to the remote repository then the push will fail, but the tag will have been created in the local repository.'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create Tag'),
    '#access' => !gb_check_status() || user_access('access create commit tag with changes'),
  );
  return $form;
}

function gb_create_tag_validate(&$form, &$form_state) {
  if (!preg_match('/^[a-z0-9\-\.\_]+$/', $form_state['values']['tag_id'])) {
    form_set_error('tag_id', t('Tag Identifier on only contain the characters a-z, A-Z and -_.'));
  }
  else if (gb_tag_exists($form_state['values']['params']['repos'], $form_state['values']['tag_id'])) {
    form_set_error('tag_id', t('Tag %tag already exists', array('%tag' => $form_state['values']['tag_id'])));
  }
}

function gb_create_tag_submit(&$form, &$form_state) {
  global $user;
  
  $params = $form_state['values']['params'];
  $i=0;
  do {
    $dst = file_directory_temp() .'/git_dir_'. $i++ .'.git';
  }
  while(file_exists($dst));
  
  $batch = array(
    'title' => t('Creating Tag %tag', array('%tag' => $form_state['values']['tag_id'])),
    'operations' => array(
      array('gb_batch_clone', array($params['repos'], $dst, TRUE)),
      array('gb_batch_set_user', array($user->name, $user->mail)),
      array('gb_batch_tag_commit', array($params['hash'], $form_state['values']['tag_id'], $form_state['values']['anotation'])),
      array('gb_batch_fetch_tags', array($params['repos'], $dst)),
    ),
    'finished' => 'gb_batch_complete',
    'file' => drupal_get_path('module', 'gb') .'/gb.batch.inc',
  );
  
  if ($form_state['values']['push_to_origin']) {
    $batch['operations'][] = array('gb_batch_push_tags', array($params['repos']));
  }
  
  $batch['operations'][] = array('gb_batch_rm_repository', array($dst));
  
  batch_set($batch);
}

function gb_tools($path, $repos, $hash) {
  $elements = array();
  
  $elements['fetch'] = array(
    '#type' => 'item',
    '#title' => l(t('Fetch updates'), $path .'/fetch/'. $hash, array('query' => drupal_get_destination())),
    '#description' => t('Fetching the commits from the origin will not update the current system but allow the viewing of changes that have not been put into the current repository.'),
    '#access' => user_access('fetch commits'),
  );
  
  return drupal_render($elements);
}

function gb_checkout(&$form_state, $repos, $head) {
  if (gb_check_status($repos)) {
    drupal_set_message(t('There is currently changes in the system which have not been commited. This can cause issues which checking out new versions of the repository.'), 'error');
    drupal_goto('');
  }
  
  drupal_set_title(t('Checkout %head', array('%head' => $head)));
  $form = array();
  
  $form['repos'] = array(
    '#type' => 'value',
    '#value' => $repos,
  );
  $form['head'] = array(
    '#type' => 'value',
    '#value' => $head,
  );
  
  return confirm_form(
    $form,
    t('Are you sure you want checkout a new version of your repository.'),
    (isset($_GET['destination']) ? $_GET['destination'] : ''),
    t('This operation is going to change your system. If the system has been changed without the system knowing about the change, this may cause loss of code.')
  );
}

function gb_checkout_submit(&$form, &$form_state) {
  $cmd = 'checkout '. $form_state['values']['head'];
  if (gb_real_git($cmd, $output, $form_state['values']['repos'])) {
    watchdog('git', 'Problem checking out %head.<br />%output', array('%head' => $form_state['values']['head'],'%output' => $output), WATCHDOG_ERROR);
    drupal_set_message(t('There was a problem with checkout of %head. Check the logs and correct any issues.', array('%head' => $form_state['values']['head'])),'error');
  }
  else {
    drupal_set_message(t('The checkout was successful. Please check the !status to make sure there are no problems.', array('!status' => l(t('status report'), 'admin/reports/status'))));
  }
}

function gb_commit_page($path, $repos, $hash) {
  $info = gb_get_commit_info($repos, $hash, TRUE);
      
  return theme('commit_info', $info, $path, $repos);
}

function gb_commit_diff($path, $repos, $hash, $parent = 0) {
  $info = gb_get_commit_info($repos, $hash, TRUE);
  
  return theme('commit_diff', $info, $path, $repos, $info['parent'][$parent]);
}

function gb_tag_page($path, $repos, $hash) {
  $info = gb_get_tag_info($repos, $hash);
      
  return theme('tag_info', $info, $path, $repos);
}

function gb_download($repos, $hash, $format = 'tar') {
  $cmd = 'archive --format='. $format .' '. $hash;
  if ($format == 'tar') {
    $ext = 'tar.gz';
    $format = 'tar-gz';
    $cmd.= '|gzip';
  }
  else {
    $ext = $format;
    $encoding = $format;
  }
  
  $filename = $hash .'.'. $ext;
  
  header('Content-Type: application/x-'. $encoding);
	header('Content-Transfer-Encoding: binary');
	header('Content-Disposition: attachment; filename="'. $filename .'";');
	
	gb_git_passthru($repos, $cmd);
	exit();
}

function gb_fetch($repos, $head) {
  if ($head == 'HEAD') {
    $head = gb_get_current_branch($repos);
  }
  if ($upstream = gb_get_upstream($repos, $head)) {
    $cmd = 'fetch '. $upstream;
    if (!gb_real_git($cmd, $output, $repos)) {
      drupal_set_message(t('Upstream changed have been successfully fetched'));
    }
    else {
      watchdog('git', 'Problem fetching changes upstream commits.<br />%output', array('%output' => $output), WATCHDOG_ERROR);
      drupal_set_message(t('Problem fetching upstream changes.'), 'error');
    }
  }
  else {
    drupal_set_message(t('Unable to determine upstream branch for %branch', array('%branch' => $head)), 'error');
  }
  drupal_goto();
  return '';
}

/**
 *
 */
function gb_get_short_log($path, $repos, $start = 'HEAD', $count = 16) {
  $defaults = array('path' => $path, 'repos' => $repos, 'start' => $start, 'count' => $count);
  $last_date = '';
  drupal_add_css(drupal_get_path('module', 'gb') .'/gb.css');
  
  _gb_format_refs($path, TRUE);
  
  $revs = gb_rev_list($repos, $count, $start);
  $rows = array();
  foreach ($revs as $rev) {
    $info = gb_get_commit_info($repos, $rev);
    $info+= $defaults;
    
    $this_date = format_date($info['author']['date'], 'custom', 'j M Y');
    if ($this_date != $last_date) {
      $rows[] = array(
        array('data' => $this_date, 'colspan' => 3, 'class' => 'date-break'),
      );
      $last_date = $this_date;
    }
    
    $rows[] = array(
      'data' => array(
        format_date($info['author']['date'], 'custom', 'H:i:s'),
        '<div class="author">'. $info['author']['name'] .'</div>' .
        '<div class="description">'. $info['description'][0] . implode('', array_map('_gb_format_refs', $info['refs'])) .'</div>',
        theme('links', module_invoke_all('link', 'commit', $info)),
      ),
      'class' => 'commit',
    );
  }

  $output.= theme('table', array(), $rows, array('class' => 'gb-short-log'));
  
  return $output;
}

/**
 *
 */
function gb_get_tag_list($path, $repos, $start = 'HEAD', $count = 8) {
  $defaults = array('path' => $path, 'repos' => $repos, 'start' => $start, 'count' => $count);
  drupal_add_css(drupal_get_path('module', 'gb') .'/gb.css');
  
  $tags = array_slice(gb_get_tags($repos), 0, $count);
  
  $rows = array();
  foreach ($tags as $tag) {
    $defaults['tag'] = $tag['name'];
    $info = gb_get_tag_info($repos, $tag['hash']);
    $info+= $defaults;
    
    $rows[] = array(
      format_date($info['tagger']['date'], 'small'),
      $info['tagger']['name'],
      l($info['tag'], _gb_format_commit_url($path, $info['hash'])),
      implode("<br />\n", $info['description']),
      theme('links', module_invoke_all('link', 'tag', $info)),
    );
  }
  
  if ($rows) {
    $output = theme('table', array(), $rows);
  }
  return $output;
}

function _gb_format_refs($a, $init = FALSE) {
  static $path;
    
  if ($init) {
    $path = $a;
    return;
  }

  return '<span class="'. $a['type'] .'">'. l($a['name'], _gb_format_head_url($path, $a['path'])) .'</span>';
}

function _gb_format_person($a) {
  return theme('format_person', $a);
}

function _gb_format_hash($a, $init = FALSE) {
  static $path = NULL;
  
  if ($init) {
    $path = $a;
    return;
  }
  
  return l($a, _gb_format_commit_url($path, $a));
}

function _gb_format_head_url($path, $name) {
  static $head = NULL;
  
  if (!$head) {
    $info = gb_get_commit_info($params['repos'], 'HEAD');
    $head = $info['refs'][0]['path'];
  }
  return $name == $head ? $path : $path .'/'. $name;
}

function _gb_format_commit_url($path, $hash) {
  return $path .'/commit/'. urlencode($hash);
}

function _gb_format_tag_url($path, $hash) {
  return $path .'/tag/'. urlencode($hash);
}

function gb_get_heads($repos) {
  return gb_show_ref($repos, '--heads');
}

function gb_get_tags($repos) {
  $tags = gb_show_ref($repos, '--tags');
  foreach ($tags as $key => $tag) {
    $info = gb_get_tag_info($repos, $tag['hash']);
    $tags[$key]['created'] = $info['tagger']['date'];
  }
    
  uasort($tags, '_gb_sort_tags');
  return $tags;
}

function _gb_sort_tags($a, $b) {
  return $a['created'] < $b['created'];
}

/**
 * Get commit info
 */
function gb_get_commit_info($repos, $hash, $changes = FALSE) {
  $info = array(
    'hash' => $hash,
  );

  $cmd = 'rev-list --header --max-count=1 '. $hash;
  $output = gb_git($repos, $cmd);
  
  while (!empty($output)) {
    $line = array_shift($output);
    
    if (empty($line)) {
      $info['description'] = $output;
      $output = array();
      break;
    }
    else if (preg_match('/^(\w+)\s(.*)$/i', $line, $match)) {
      switch ($match[1]) {
        case 'committer':
        case 'author':
          $person = $match[2];
          if (preg_match('/^(.*) [<](.*)[>] ([0-9]+) (\+[0-9]{4})/i', $person, $details)) {
            $info[$match[1]][] = array(
              'name' => $details[1],
              'mail' => $details[2],
              'date' => $details[3],
              'tz' => $details[4],
            );
          }
          break;
        
        default:
          $info[$match[1]][] = $match[2];
      }
    }
    else if (preg_match('/^[0-9a-f]{40}$/i', $line)) {
      $info['hash'] = $line;
    }
  }
  
  $info['author'] = $info['author'][0];
  
  $info['refs'] = gb_get_refs($repos, $info['hash']);
  if ($changes) {
    $info['files'] = gb_get_file_changes($repos, $info['hash']);
  }
  $info['repos'] = $repos;
  
  return $info;
}

function gb_get_tag_info($repos, $hash) {
  $info = array(
    'hash' => $hash,
    'description' => array(),
  );
  
  gb_real_git('cat-file -t '. $hash, $type, $repos);
  if ($type[0] == 'tag') {
    $cmd = 'cat-file tag '. $hash;
    $output = gb_git($repos, $cmd);
  
    while (!empty($output)) {
      $line = array_shift($output);
    
      if (empty($line)) {
        $info['description'] = $output;
        $output = array();
        break;
      }
      else if (preg_match('/^(\w+)\s(.*)$/i', $line, $match)) {
        switch ($match[1]) {
          case 'tagger':
            $person = $match[2];
            if (preg_match('/^(.*) [<](.*)[>] ([0-9]+) (\+[0-9]{4})/i', $person, $details)) {
              $info[$match[1]][] = array(
                'name' => $details[1],
                'mail' => $details[2],
                'date' => $details[3],
                'tz' => $details[4],
              );
            }
            break;
        
          case 'tag':
          case 'type':
          case 'object':
            $info[$match[1]] = $match[2];
            break;
        
          default:
            $info[$match[1]][] = $match[2];
        }
      }
    }
    $info['tagger'] = $info['tagger'][0];
  }
  else {
    $commit_info = gb_get_commit_info($repos, $hash);
    $info['tagger'] = $commit_info['committer'][0];
  }
  
  return $info;
}

function gb_get_file_changes($repos, $hash) {
  $return = gb_git($repos, "diff-tree -r -M --no-commit-id {$hash}");
  
  return array_filter(array_map('_gb_parse_difftree', $return));
}

function _gb_parse_difftree($a) {
  if (preg_match('/^:(\d+)\s(\d+)\s([0-9a-f]{40})\s([0-9a-f]{40})\s(.)([\d]{0,3})\s+(.*)$/i', $a, $match)) {
    if ($match[5] == 'R' || $match[5] == 'C') {
      list($match[8], $match[7]) = split("\t", $match[7]);
    }
    else {
      $match[8] = '';
    }
    return array(
      'filename' => $match[7],
      'from_id' => $match[3],
      'to_id' => $match[4],
      'status' => $match[5],
      'percent' => $match[6],
      'old_file' => $match[8],
    );
  }
  return;
}

function _gb_get_change_description($a) {
  switch ($a['status']) {
    case 'A':
      return t('New file');
      
    case 'D':
      return t('Delete file');

    case 'R':
      return t('Renamed from %file (%percent% similarity)', array('%file' => $a['old_file'], '%percent' => $a['percent']*1));

    case 'C':
      return t('Copied from %file (%percent% similarity)', array('%file' => $a['old_file'], '%percent' => $a['percent']*1));
  }
}

function gb_diff($repos, $from, $to) {
  $cmd = 'diff -u --src-prefix="" --dst-prefix="" '. $from .'..'. $to;
    
  return gb_git($repos, $cmd);
}

function gb_get_refs($repos, $hash) {
  static $all_refs = array();
  
  if (empty($all_refs[$repos])) {
    $refs = gb_show_ref($repos, '--dereference');
    
    foreach ($refs as $ref) {
      $rhash = $ref['hash'];
      if ($ref['type'] == 'tags') {
        if (isset($refs[$ref['fullname'] .'^{}'])) {
          $rhash = $refs[$ref['fullname'] .'^{}']['hash'];
        }
        else {
          unset($ref);
        }
      }
      if (isset($ref)) {
        $all_refs[$repos][$rhash][] = $ref;
      }
    }
  }
  
  return isset($all_refs[$repos][$hash]) ? $all_refs[$repos][$hash] : array();
}

function gb_show_ref($repos, $options = '', $hash = NULL) {
  
  gb_real_git('show-ref '. $options, $return, $repos);
  
  $refs = array();
  
  foreach ($return as $line) {
    if (preg_match('/^([0-9a-f]{40}) refs\/(.*?)\/(.*)$/i', $line, $match)) {
      if (!isset($hash) || $hash == $match[1]) {
        $refs['refs/'. $match[2] .'/'. $match[3]] = array(
          'hash' => $match[1],
          'type' => $match[2],
          'fullname' => 'refs/'. $match[2] .'/'. $match[3],
          'name' => $match[3],
          'path' => $match[2] .'/'. $match[3],
        );
      }
    }
  }
  
  return $refs;
}

/**
 * Get rev list
 */
function gb_rev_list($repos, $max_count = NULL, $start = 'HEAD') {
  $cmd = 'rev-list '. $start;

  if (isset($max_count)) {
    $cmd = 'rev-list --max-count='. $max_count .' '. $start;
  }

  return gb_git($repos, $cmd);
}

function gb_tag_exists($repos, $tag) {
  $refs = gb_show_ref($repos, '--tags');
  
  return isset($refs['refs/tags/'. $tag]);
}

function gb_check_status($repos = NULL) {
  static $status;
  
  if (!isset($status)) {
    gb_real_git('status', $output, $repos);
    $status = in_array('# Changed but not updated:', $output) || in_array('# Changes to be committed:', $output);
  }
  
  return $status;
}

function gb_get_current_branch($repos, $hash = 'HEAD') {
  $cmd = 'branch -v -a --contains '. $hash;
  gb_real_git($cmd, $output, $repos);
  
  $output = preg_grep('/HEAD|\(no branch\)/', $output, PREG_GREP_INVERT);
  $line = reset($output);
  
  if (isset($line) && preg_match('/^.\s(.*?)\s+/i', $line, $matches)) {
    return basename($matches[1]);
  }
  
  return FALSE;
}

function gb_branch_exists($repos, $head, $type = 'heads', $remote = NULL) {
  if ($heads = gb_show_ref($repos, '--head')) {
    if ($remote) {
      return isset($heads['refs/'. $type .'/'. $remote .'/'. $head]);
    }
    else {
      return isset($heads['refs/'. $type .'/'. $head]);
    }
  }
  return FALSE;
}

function gb_get_upstream($repos, $head) {
  $cmd = 'config branch.'. $head .'.remote';
  if (gb_real_git($cmd, $remote, $repos)) {
    return FALSE;
  }
  
  return $remote[0];
}