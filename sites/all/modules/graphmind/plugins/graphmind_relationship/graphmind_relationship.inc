<?php
// $Id$


/**
 * Service callback - add a new relationship.
 *
 * @param integer $source_nid
 * @param integer $target_nid
 * @param string $relationship_rid
 * @return boolean
 */
function graphmind_relationship_on_add_relationship($source_nid, $target_nid, $relationship_rid = NULL) {
  return graphmind_relationship_add_relationship($source_nid, $target_nid, $relationship_rid);
}


/**
 * Service callback - delete a relationship.
 *
 * @param integer $source_nid
 * @param integer $target_nid
 * @param string $relationship_rid
 * @return boolean
 */
function graphmind_relationship_on_delete_relationship($source_nid, $target_nid, $relationship_rid = NULL) {
  return graphmind_relationship_delete_relationship($source_nid, $target_nid, $relationship_rid);
}


/**
 * Service callback - get a nodes relationships.
 *
 * @param integer $source_nid
 * @param string $relationship_rid
 * @return array
 */
function graphmind_relationship_on_get_relationships($source_nid, $relationship_rid = NULL) {
  return graphmind_relationship_get_relationships($source_nid, $relationship_rid);
}


/**
 * Services callback - get a nodes subtree.
 *
 * @param integer $nid
 * @param integer $depth
 * @return array
 */
function graphmind_relationship_get_subtree($nid, $depth = null) {
  if ($depth == null) {
    $depth = GRAPHMIND_RELATIONSHIP_DEFAULT_DEPTH;
  }

  return _graphmind_relationship_get_subtree($nid, $depth);
}


/**
 * Build a subtree array to a node by it's relationships.
 *
 * @param integer $nid
 * @param integer $depth
 * @return array
 */
function _graphmind_relationship_get_subtree($nid, $depth) {
  if ($depth <= 0) return array();

  module_load_include('inc', 'graphmind_relationship', 'GraphmindRelationshipNode');
  
  $node = new GraphmindRelationshipNode($nid, $depth);
  return $node->exportToArray();
}


/**
 * Services callback - check if update needs.
 *
 * @param array $tree
 * @return boolean
 */
function graphmind_relationship_check_update($tree, $depth) {
  return _graphmind_relationship_check_subtree_update($tree, $depth);
}


/**
 * Check 1 level of a subtree if any update happened.
 *
 * @param array $subtree
 * @return boolean
 */
function _graphmind_relationship_check_subtree_update($mapTree, $depth) {
  module_load_include('inc', 'graphmind_relationship', 'GraphmindRelationshipNode');
  $rootNnode = new GraphmindRelationshipNode($mapTree['nid'], $depth);
  $isUptodate = $rootNnode->checkMapDataUpdate($mapTree);
  return $isUptodate;
}


/**
 * Service callback - get user color.
 *
 * @param integer $uid
 */
function graphmind_relationship_get_user_color($uid) {
  if (!isset($_SESSION['GraphmindRelationship']['UserColor'][$uid])) {
    $digits = array('8', '9', 'A', 'B', 'C', 'D', 'E', 'F');
    $length = count($digits);
    $r = $digits[rand(0, $length - 1)];
    $g = $digits[rand(0, $length - 1)];
    $b = $digits[rand(0, $length - 1)];
    $_SESSION['GraphmindRelationship']['UserColor'][$uid] = '0x' . $r . $r . $g . $g . $b . $b;
  }

  return $_SESSION['GraphmindRelationship']['UserColor'][$uid];
}
