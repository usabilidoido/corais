<?php
// $Id: content_complete_views_handler_field_data.inc,v 1.1.2.1 2010/01/22 13:30:14 pvhee Exp $

/**
 *  @file
 *  This defines a view that outputs the completeness data of a node.
 */

class content_complete_views_handler_field_data extends views_handler_field_custom {
	
  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->ensure_my_table();
    $this->field_alias = $this->query->add_field($this->table_alias, $this->real_field);
    $this->add_additional_fields();    
  }
  
}
