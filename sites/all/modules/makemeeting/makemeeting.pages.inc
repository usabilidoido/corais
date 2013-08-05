<?php

/**
 * Show the poll pages - scheduler and regular poll
 */
function makemeeting_show_pollpage($url) {
  $node_id = db_result(db_query("SELECT nid FROM {makemeeting_poll_heads} WHERE url = '%s'", $url));

  // poll url - showing the poll panel

  if (is_numeric($node_id) && $node_id > 0) {
    $node = node_load($node_id);

    $author = $node->uid == 0 ? ($node->anonym_name == "" ? t('Anonymous') : $node->anonym_name) : $node->name;
    if ($author != $node->name){
      $node->name = $author;
    }

    drupal_set_title(t("@name's question: @question", array("@name" => $author, "@question" => $node->title)));

    // Breadcrumb navigation
    drupal_set_breadcrumb(array(
                               l(t('Home'), NULL),
                               l(t('Make Meetings'), 'makemeeting'),
                               l(t("Question: @question", array('@question' => $node->title)), 'makemeeting/' . $node->poll_url)
                          ));

    return node_view($node);
  }

  // admin url - showing the admin panel

  $node_id = db_result(db_query("SELECT nid FROM {makemeeting_poll_heads} WHERE admin_url = '%s'", $url));
  if (is_numeric($node_id) && $node_id > 0) {
    $node = node_load($node_id);
    drupal_set_title(t("Admin page for: @question", array("@question" => $node->title)));
    drupal_set_breadcrumb(array(
                               l(t('Home'), NULL),
                               l(t('Make Meetings'), 'makemeeting'),
                               l(t("Admin page: @question", array('@question' => $node->title)), 'makemeeting/' . $node->poll_admin_url)
                          ));

    if ($node->poll_type == 1){
      return theme("makemeeting_poll_admin", $node, drupal_get_form("makemeeting_update_poll", $node));
    }
    elseif ($node->poll_type == 2) {
      return theme("makemeeting_poll_admin", $node, drupal_get_form("makemeeting_simplepoll_update_poll", $node));
    }
  }
}

/**
 * Showing voters logpage.
 *
 * @param mixed $admin_url
 * @return
 */
function makemeeting_logpage($admin_url) {
  // loading the node
  $node_id = db_result(db_query("SELECT nid FROM {makemeeting_poll_heads} WHERE admin_url = '%s'", $admin_url));
  $node = node_load($node_id);

  drupal_set_breadcrumb(array(
                             l(t('Home'), NULL),
                             l(t('Make Meetings'), 'makemeeting'),
                             l(t("Admin page: @question", array('@question' => $node->title)), 'makemeeting/' . $node->poll_admin_url),
                             l(t("View logs"), 'makemeeting/' . $node->poll_admin_url . "/log")
                        ));

  $result = db_query("SELECT * FROM {makemeeting_poll_logs} WHERE nid = %d ORDER BY dt DESC", $node_id);

  $rows = array();
  while ($row = db_fetch_array($result)){
    $rows[] = $row;
  }

  return theme('makemeeting_logpage', $rows);
}

/**
 * The send link forward page.
 */
function makemeeting_sendfw($poll_url) {
  // loading the node
  $node_id = db_result(db_query("SELECT nid FROM {makemeeting_poll_heads} WHERE url = '%s'", $poll_url));
  $node = node_load($node_id);

  // Breadcrumb navigation
  drupal_set_breadcrumb(array(
                             l(t('Home'), NULL),
                             l(t('Make Meetings'), 'makemeeting'),
                             l(t("Question: @question", array('@question' => $node->title)), 'makemeeting/' . $node->poll_url),
                             l(t("Send forward"), 'makemeeting/' . $node->poll_url . "/sendfw")
                        ));

  if (variable_get('makemeeting_send_email_enabled', '1') == 0) {
    return t('Send forward function is disabled by admin.');
  }

  return drupal_get_form("makemeeting_sendfw_form", $node);
}

/**
 * The default page for /makemeeting url, now showing some basic
 * information.
 */
function makemeeting_infopage() {
  // Breadcrumb navigation
  drupal_set_breadcrumb(array(
                             l(t('Home'), NULL),
                             l(t('Make Meetings'), 'makemeeting')
                        ));

  return t("Use the navigation menu to create polls.");
}

/**
 * Show own polls page
 * Only authenticated users can access this page
 */
function makemeeting_mypage() {
  GLOBAL $user;

  // get my polls from the database
  $res = db_query("SELECT n.title, p.url, p.admin_url FROM {makemeeting_poll_heads} as p JOIN {node} as n ON p.nid = n.nid WHERE (n.type = 'makemeeting' OR n.type = 'makemeeting_simplepoll') AND n.uid = %d ORDER BY n.created DESC", $user->uid);
  $data = array();
  while ($row = db_fetch_array($res)) {
    $data[] = $row;
  }

  // set the navigation line
  drupal_set_breadcrumb(array(
                             l(t('Home'), NULL),
                             l(t('Make Meetings'), 'makemeeting'),
                             l(t('List of my polls'), 'makemeeting/mypage'),
                        ));

  // returning information using makemeeting_mypage theme
  return theme("makemeeting_mypage", $data);
}
