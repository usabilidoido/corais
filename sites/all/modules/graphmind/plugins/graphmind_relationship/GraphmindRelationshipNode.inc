<?php
// $Id$


class GraphmindRelationshipNode {

  /**
   *
   * @var GraphmindRelationshipNode
   */
  public $parent;
  
  /**
   *
   * @var array
   */
  public $children = array();
  
  /**
   *
   * @var integer
   */
  public $nid;
  
  /**
   *
   * @var Object
   */
  public $node;
  
  
  /**
   * Constructor
   */
  public function __construct($nid, $depth, $parent = NULL) {
    $this->nid  = (int)$nid;
    $this->node = node_load($this->nid);
    $this->parent = $parent;
    
    if ($depth > 0 && $this->node) {
      $result = db_query('
        SELECT target_nid
        FROM {graphmind_relationship}
        WHERE source_nid = %d
      ', $this->nid);

      while ($row = db_fetch_object($result)) {
        if (!$this->hasNodeAlready($row->target_nid)) {
          $child = new GraphmindRelationshipNode($row->target_nid, $depth - 1, $this);
          $this->children[$row->target_nid] = $child;
        }
      }
    }
  }
  
  
  /**
   * Loop check.
   * 
   * @param integer $nid
   * @return boolean
   */
  public function hasNodeAlready($nid) {
    if ($this->nid == $nid) {
      return TRUE;
    }
    
    if ($this->parent) {
      return $this->parent->hasNodeAlready($nid);
    }
    
    return FALSE;
  }
  
  
  /**
   * Creates an service capable argument structure.
   * 
   * @return Array
   */
  public function exportToArray() {
    $data = array();
    $data['nid'] = $this->nid;
    $data['node'] = $this->node;
    $data['children'] = array();
    
    foreach((array)$this->children as $child) {
      $data['children'][] = $child->exportToArray();
    }
    
    return $data;
  }
  
  
  /**
   * Check if the map data is obsoleted.
   * 
   * @param array $mapSubtree
   * @return boolean TRUE if it's up to date. FALSE if obsoleted.
   */
  public function checkMapDataUpdate($mapSubtree) {
    $map_children_nids = array();
    $self_children_nids = array_keys($this->children);
    
    foreach ((array)$mapSubtree['children'] as $child) {
      $map_children_nids[] = $child['nid'];
    }
    
    if (array_diff($map_children_nids, $self_children_nids) || array_diff($self_children_nids, $map_children_nids)) {
      return FALSE;
    }
    
    if ($mapSubtree['node']['title'] != $this->node->title) {
      return FALSE;
    }
    
    foreach ((array)$mapSubtree['children'] as $mapChild) {
      if (!$this->children[$mapChild['nid']]->checkMapDataUpdate($mapChild)) {
        return FALSE;
      }
    }
    
    return TRUE;
  }
  
}
