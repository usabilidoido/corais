<?php
// $Id: convert_name2uid.php,v 1.1.2.2 2009/09/29 09:40:45 hutch Exp $
/**
 * @file
 * Contributed by RusRabbit, see http://drupal.org/comment/reply/568168
 *
 * Here's the code I used for nodes, teasers and comments conversion. The
 * following example is for nodes; for teasers and comments we have to put
 * correct table name (comments for comments) and columns names
 * respectively (tid, cid instead of nid and teaser, comment instead of
 * body). I put the code into the block on a certain page while site is
 * offline, just for ease of use. The code is written to handle non-latin
 * folder names, too (I had some folders named in Russian).
 */

$sql = "SELECT name, uid FROM {users}";
$result = db_query(db_rewrite_sql($sql));
while ($userinfo = db_fetch_object($result)) {
  $utfname=$userinfo->name;
  $utfname = rawurlencode($utfname);
  $username[$utfname] = $userinfo->uid;
}

$nodez = db_query('SELECT nid, body FROM {node_revisions}');
$i=0;
while ($mynode = db_fetch_object($nodez)) {
  $editnode = $mynode->comment;
  $currentnid = $mynode->nid;
  $search = '/(imagepicker\/)([a-z]{1,}\/)([a-zA-ZÐ-ï°-Ï0-9_.-\s\+\%]{1,})/e';
  $replace = '"$1".$username["$3"]';
  $result = preg_replace($search, $replace, $editnode);
  db_query("UPDATE {node_revisions} SET body = '$result' WHERE nid = $currentnid");
}

