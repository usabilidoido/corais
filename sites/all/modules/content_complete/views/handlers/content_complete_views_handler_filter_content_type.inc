<?php
// $Id: content_complete_views_handler_filter_content_type.inc,v 1.1.2.2 2009/09/11 15:24:36 pvhee Exp $

/**
 *  @file
 *  This defines a views handler to filter nodes by whether or not they're Content-Complete-enabled.
 */

class content_complete_views_handler_filter_content_type extends views_handler_filter_boolean_operator {
  function query() {
    $types = node_get_types();
    $enabled_types = array();
        
    foreach ($types as $type => $name) {
      if (content_complete_enabled_type($type)) {
        $enabled_types[] = '"'. $type .'"';
      }
    }
    
    $this->query->add_where(0, 'node.type '. (empty($this->value) ? 'NOT IN' : 'IN') .'('. implode(', ', $enabled_types) .')');
  }
}