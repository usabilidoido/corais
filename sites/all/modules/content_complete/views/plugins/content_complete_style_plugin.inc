<?php
// $Id: content_complete_style_plugin.inc,v 1.1.2.2 2010/01/22 13:30:14 pvhee Exp $

/**
 * @file
 *Provide the views Complete Complete plugin.
 */

/**
  * Implementation of views_plugin_style().
  */
class content_complete_style_plugin extends views_plugin_style {
  function render($values) {
    // Make calculations here (always on the fly but using cached values to compute with...)
    $data = $this->compute_sum($values);
    return theme('content_complete_profile_percent_complete', $data);
  }
  
   /**
    * Style validation.
    */
   function validate() {
     $errors = parent::validate();
     
     if (empty($this->display->display_options['style_plugin'])) {
       return $errors;
     }
     
     // Make sure a completeness data field is provided.
     $fields = $this->display->handler->get_option('fields');
     if (!isset($fields['data'])) {
       $errors[] = t("You need to provide a completeness data field in the display '@display_title'.", array('@display_title' => $this->display->display_title));
     }
          
     return $errors;
   }
  
  /**
   * Compute the sum of a series of complete values.
   * 
   * @return 
   *   An array of fields such as percentage complete (summed).
   */
  function compute_sum(&$rows) {
    $data = array();
    
    // Sum percentages + find the next-to-complete action
    $first = 0;
    foreach ($rows as &$row) {
      $complete_data = unserialize($row->content_complete_data);
      
      $data['complete'] += $complete_data['completed'];
      $data['incomplete'] += $complete_data['incomplete'];
                        
      if (!$first && $complete_data['percent'] != 100) 
        $first = $complete_data['nid'];
    }
    $data['total'] = $data['complete'] + $data['incomplete'];
    
    // Average and compute percentages
    if ($data['total']) {
      $data['percent'] = round($data['complete'] / $data['total'] * 100);
      $next_completion = round(1 / $data['total'] * 100);
      $data['nextpercent'] = round($data['percent'] + $next_completion);     
    }
    
    // Fill complete data for the next-to-complete action
    if ($first) {
      // we need to recalculate this data on the fly to get e.g.
      // access to edit (cannot be cached, depending on who is viewing)
      $first_node = node_load($first);
      $first_data = content_complete_get_data($first_node);
      // TODO merge this array into $data, but keeping $data values
      $data['nid'] = $first_data['nid'];
      $data['nextfield'] = $first_data['nextfield'];
      $data['nextname'] = $first_data['nextname'];
      $data['type'] = $first_data['type'];
      $data['name'] = $first_data['name'];
      $data['edit'] = $first_data['edit'];
    }

    return $data;
  }
  
  
}