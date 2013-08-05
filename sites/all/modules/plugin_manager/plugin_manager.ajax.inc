<?php

function plugin_manager_ajax_queue() {
  $queue = array();
  $result = db_query("SELECT pmt.short_name, title, tag, links FROM {plugin_manager_queue} pmq LEFT JOIN {plugin_manager_repository} pmr ON pmq.short_name = pmr.short_name LEFT JOIN {plugin_manager_taxonomy} pmt ON pmr.short_name = pmt.short_name ORDER BY title, tag");
  while ($row = db_fetch_array($result)) {
    if (!$row['short_name'])continue;
    $queue[$row['short_name']]['title'] = $row['title'];
    $queue[$row['short_name']]['link'] = l(t("Project Page"), $row['links']);
    $queue[$row['short_name']]['tag'] .= (isset($queue[$row['short_name']]['tag']) ? ', ' : '') . $row['tag'];
  }
  print drupal_to_js($queue);
  exit(0);
}

function plugin_manager_ajax_queue_add() {
  // Set the name of the plugin from post.  Otherwise die.
  if (isset($_POST['plugin'])) {
    $plugin = $_POST['plugin'];
  }
  else {
    exit(0);
  }

  plugin_manager_add_to_queue($plugin);
  plugin_manager_ajax_queue();
  exit(0);
}

function plugin_manager_ajax_queue_remove() {
  // Set the name of the plugin from post.  Otherwise die.
  if (isset($_POST['plugin'])) {
    $plugin = $_POST['plugin'];
  }
  else {
    exit(0);
  }

  plugin_manager_remove_from_queue($plugin);
  $queue = plugin_manager_get_queue();
  echo count($queue);
  exit(0);
}

function plugin_manager_ajax_queue_clear() {
  // If it wasn't confirmed, then don't do it.
  if (!isset($_POST['confirm'])) {
    exit(0);
  }

  plugin_manager_clear_queue();
  print drupal_to_js(array());
  exit(0);
}

function plugin_manager_ajax_search_term($type = '', $term = '') {
  // Make sure input was given.
  if (empty($type) OR empty($term)) {
    print drupal_to_js(array());
    exit(0);
  }

  if (drupal_strtolower($type) == 'themes') {
    $type = "AND tag = 'Themes'";
  }
  elseif (drupal_strtolower($type) == 'modules') {
    $type = "AND tag != 'Themes'";
  }
  else {
    $type = "";
  }

  $queue = array();
  $result = db_query("SELECT pmr.short_name, title, tag, links FROM {plugin_manager_repository} pmr LEFT JOIN {plugin_manager_taxonomy} pmt ON pmr.short_name = pmt.short_name WHERE (pmr.short_name LIKE '%%%s%%' OR title LIKE '%%%s%%') $type ORDER BY title, tag", $term, $term);
  while ($row = db_fetch_array($result)) {
    $queue[$row['short_name']]['title'] = $row['title'];
    $queue[$row['short_name']]['link'] = l(t("Project Page"), $row['links'], array('attributes'=>array('target'=>'_blank')));
    if ($queue[$row['short_name']]['tag']!=$row['tag'])
        $queue[$row['short_name']]['tag'] .= (isset($queue[$row['short_name']]['tag']) ? ', ' : '') . $row['tag'];
  }
  print drupal_to_js($queue);
  exit(0);
}

function plugin_manager_ajax_search_tag($tag) {
  // Make sure input was given.
  if (empty($tag)) {
    print drupal_to_js(array());
    exit(0);
  }

  $queue = array();
  $result = db_query("SELECT pmr.short_name, title, tag, links FROM {plugin_manager_repository} pmr LEFT JOIN {plugin_manager_taxonomy} pmt ON pmr.short_name = pmt.short_name WHERE tag LIKE '%s%%' ORDER BY title, tag", $tag);
  while ($row = db_fetch_array($result)) {
    $queue[$row['short_name']]['title'] = $row['title'];
    $queue[$row['short_name']]['link'] = l(t("Project Page"), $row['links'], array('attributes'=>array('target'=>'_blank')));
    $queue[$row['short_name']]['tag'] .= (isset($queue[$row['short_name']]['tag']) ? ', ' : '') . $row['tag'];
  }
  print drupal_to_js($queue);
  exit(0);
}

function plugin_manager_ajax_search_letter($letter) {
  // Make sure input was given.
  if (empty($letter)) {
    print drupal_to_js(array());
    exit(0);
  }

  $queue = array();
  $result = db_query("SELECT pmr.short_name, title, tag, links FROM {plugin_manager_repository} pmr LEFT JOIN {plugin_manager_taxonomy} pmt ON pmr.short_name = pmt.short_name WHERE title LIKE '%s%%' ORDER BY title, tag", $letter);
  while ($row = db_fetch_array($result)) {
    $queue[$row['short_name']]['title'] = $row['title'];
    $queue[$row['short_name']]['link'] = l(t("Project Page"), $row['links'], array('attributes'=>array('target'=>'_blank')));
    $queue[$row['short_name']]['tag'] .= (isset($queue[$row['short_name']]['tag']) ? ', ' : '') . $row['tag'];
  }
  print drupal_to_js($queue);
  exit(0);
}
