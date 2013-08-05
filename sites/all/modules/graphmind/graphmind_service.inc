<?php
// $Id: graphmind_service.inc,v 1.1.2.2 2010/03/23 09:15:08 itarato Exp $


/**
 * Returns all the available views for GraphMind.
 * @return array
 */
function graphmind_service_get_views() {
  $data = array();

  $tags = variable_get('graphmind_views_tags', '');
  $tags = str_replace(' ', '', $tags);
  $tag_array = explode(',', $tags);

  $views = views_get_all_views();
  foreach ((array)$views as $view) {

    $views_tags = str_replace(' ', '', $view->tag);
    $views_tag_array = explode(',', $views_tags);
    $tag_is_match = strlen($tags) == 0;

    foreach ((array)$views_tag_array as $view_tag) {
      if (in_array($view_tag, $tag_array)) {
        $tag_is_match = TRUE;
      }
    }

    if (!$tag_is_match) continue;

    $data[] = array(
      'name' => $view->name,
      'baseTable' => $view->base_table,
      'tag' => $view->tag,
      'disabled' => $view->disabled,
    );
  }

  return $data;
}


/**
 * Save a GraphMind map. (FreeMind XML format.)
 * @param int $nid
 * @param string $mm
 * @return string
 */
function graphmind_service_save_graphmind($nid, $mm, $change_time) {
  $node = node_load($nid);
  $node->body = $mm;

  // @TODO multiedit interruption
  if (node_last_changed($node->nid) > $change_time) {
    return t('This content has been modified by another user, changes cannot be saved.');
  }

  node_save($node);
  return '1';
}


/**
 * Save commands from a client.
 *
 * @param array $commands
 */
function graphmind_service_save_commands($commands, $nid, $client_id) {
//  dd($commands);

  foreach ((array)$commands as $command) {
    $oCmd = new stdClass();
    $oCmd->command = serialize((array)$command);
    $oCmd->created_at = time();
    $oCmd->nid = $nid;
    $oCmd->client_id = $client_id;
//    dd($oCmd, 'object');
    drupal_write_record('graphmind_command', $oCmd);
  }

  return TRUE;
}


/**
 *
 * @param integer $timestamp
 * @return array
 */
function graphmind_service_update($nid, $client_id, $latest_cid, $commands) {
  // Saving commands
  graphmind_service_save_commands($commands, $nid, $client_id);

  if ($latest_cid == 0) {
    $result = db_fetch_object(db_query('SELECT changed FROM {node} WHERE nid = %d;', $nid));
    $result = db_fetch_object(db_query('SELECT MIN(cid) AS min FROM {graphmind_command} WHERE created_at > %d;', $result->changed));
    $latest_cid = $result->cid - 1;
  }

  $resource = db_query('
    SELECT cid, created_at, command
    FROM {graphmind_command}
    WHERE 
      nid = %d AND
      client_id != \'%s\' AND
      cid > %d
    ORDER BY created_at ASC;
  ', $nid, $client_id, $latest_cid);
  $commands = array();
  while ($row = db_fetch_object($resource)) {
    $row->command = unserialize($row->command);
    $commands[] = $row;
  }
  return $commands;
}


function graphmind_service_load_mindmap_features($nid) {
  return graphmind_service_features_get($nid);
}


/**
 * Services callback - get a preview of a node.
 *
 * @param integer $nid
 * @return string
 */
function graphmind_service_tooltip_get_view($nid) {
  $node = node_load($nid);
  if (!$node) {
    return '';
  }

  $out = $node->title;
  
  if (!empty($node->body)) {
    $view = $node->body;
    $view = preg_replace("/<[^>]*>/", '', $view);
    $view = preg_replace("/\s{1,}/", ' ', $view);
    $view = preg_replace("/\n{1,}/", "\n", $view);
    $out .= "\n\n" . $view;
  }
  
  return $out;
}
