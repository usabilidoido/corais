<?php

// $Id: webform_report.inc,v 1.1.2.27.2.18 2011/01/26 20:40:45 jimbullington Exp $

/**
 * @file
 * This file contains common functions and functions required
 * to ouput a report for the webform report module
 */

/**
 * Search for nodes of type webform and returns their nids and titles
 * in an associative array.
 */
function _webform_report_get_webforms() {
  $webforms = array();
  $webform_types = variable_get('webform_node_types', array('webform'));
  $placeholders = db_placeholders($webform_types, 'varchar');
  $result = db_query(db_rewrite_sql("SELECT n.nid, n.title FROM {node} n WHERE n.type in ($placeholders)"), $webform_types);

  while ($row = db_fetch_object($result)) {
    $webforms[$row->nid] = t($row->title);
  }
  return $webforms;
}

/**
 * Get webform components.
 *
 * @param nid
 *   the node id of a webform
 * @return 
 *   an associative array of webform components
 */
function _webform_report_get_components($nid) {
  
  $components = array();
  
  // include meta components
  $components[-1] = array('name' => t('Submitted By User'), 'type' => 'string');
  $components[-2] = array('name' => t('Submission Date'), 'type' => 'date');
  $components[-3] = array('name' => t('Submission Time'), 'type' => 'time');
  $components[-4] = array('name' => t('Submission IP Address'), 'type' => 'string');
  $components[-5] = array('name' => t('Edit Link'), 'type' => 'link');
  $components[-6] = array('name' => t('View Link'), 'type' => 'link');
  $components[-7] = array('name' => t('Delete Link'), 'type' => 'link');

  // get components from webform  
  $result = db_query("SELECT c.cid, c.name, c.type FROM {webform_component} c WHERE c.type <> 'fieldset' AND c.nid = %d ORDER BY c.weight", $nid);
  while ($row = db_fetch_object($result)) {
    $components[$row->cid] = array('name' => filter_xss($row->name), 'type' => $row->type);
  }
  
  return $components;
}

/**
 * return an associative array containing the webform report columns
 *
 * @param node
 *   the webform report node
 * @param components
 *   array of webform components
 * @return 
 *   an associative array of webform report columns
 */
function _webform_report_get_columns($node, $components) {

  // build list of columns on the report
  $columns = array();
  if (is_array($node->columns)) {
    for ($i=0; $i<count($node->columns); $i++) {
      $cid = $node->columns[$i]['cid'];
      if (array_key_exists($cid, $components)) {
        $columns[] = array(
          'name' => $components[$cid]['name'],
          'type' => $components[$cid]['type'],
          'format' => $node->columns[$i]['format'],
          'option' => isset($node->columns[$i]['option'])?$node->columns[$i]['option']:'',
          'cid' => $node->columns[$i]['cid'],
          'hidden' => $node->columns[$i]['hidden'],
        );
      }
    }
  }
  
  return $columns;
}

/**
 * return an associative array containing the webform report filters
 *
 * @param node
 *   the webform report node
 * @param components
 *   array of webform components
 * @return 
 *   an associative array of webform report filters
 */
function _webform_report_get_filters($node, $components) {

  // build list of filters
  $filters = array();
  if (is_array($node->filters)) {
    for ($i=0; $i<count($node->filters); $i++) {
      $cid = $node->filters[$i]['field'];
      if (array_key_exists($cid, $components)) {
        $filters[] = array(
          'name' => $components[$cid]['name'],
          'type' => $components[$cid]['type'],
          'ftype' => $node->filters[$i]['type'],
          'value' => $node->filters[$i]['value'],
          'cid' => $node->filters[$i]['field'],
        );
      }
    }
  }

  // check for search values - add filters for search
  $search = &$_SESSION['search_form'][$node->nid];
  if (is_array($search)) {
    $col = $search['column'];
    $val = $search['value'];
    if ($val != '') {
      $columns = _webform_report_get_columns($node, $components);
      // if all columns, add search filter for each column
      if ($col == -1) {
        foreach($columns as $index => $column) {
          // exclude special links (view, edit, etc)
          if ($column['cid'] > -5) {
            $filters[] = array(
              'name' => '',
              'type' => $column['type'],
              'ftype' => -1,
              'value' => $val,
              'cid' => $column['cid']
            );
          }
        }
      }
      // add search filter for specified column
      else {
        if (array_key_exists($col, $columns)) {
          $filters[] = array(
            'name' => '',
            'type' => $columns[$col]['type'],
            'ftype' => -1,
            'value' => $val,
            'cid' => $columns[$col]['cid']
          );
        }
      }
    }
  }
  return $filters;
}

/**
 * return an array containing the webform report filter options
 *
 * @return 
 *   an array of webform report filter options
 */
function _webform_report_get_filter_options($datetimeonly = FALSE) {

  $opt = array();
  
  $opt[0] = t('none');
  if (!$datetimeonly) {
    $opt[1] = t('begins with');
    $opt[2] = t('does not begin with');
    $opt[3] = t('contains');
    $opt[4] = t('does not contain');
  }
  $opt[5] = t('equals');
  $opt[6] = t('does not equal');
  $opt[7] = t('is empty');
  $opt[8] = t('is not empty');
  $opt[9] = t('greater than');
  $opt[10] = t('less than');
  $opt[11] = t('greater than or equal');
  $opt[12] = t('less than or equal');
  $opt[13] = t('between');
  
  return $opt;
}  

/**
 * return an associative array containing the webform report sort criteria
 *
 * @param node
 *   the webform report node
 * @param components
 *   array of webform components
 * @return 
 *   an associative array of webform report sort criteria
 */
function _webform_report_get_sorton($node, $components) {

  // build list of sort parameters
  $sorton = array();
  if (is_array($node->sorton)) {
    for ($i=0; $i<count($node->sorton); $i++) {
      $cid = $node->sorton[$i]['cid'];
      if (array_key_exists($cid, $components)) {
        $sorton[] = array(
          'name' => $components[$cid]['name'],
          'type' => $components[$cid]['type'],
          'order' => $node->sorton[$i]['order'],
          'cid' => $node->sorton[$i]['cid'],
        );
      }
    }
  }

  return $sorton;
}

/**
 * Get submission data for the specified webform.
 *
 * @param node 
 *   the current node object
 * @return 
 *   a database query result set
 */
function _webform_report_get_data($node) {

  if (isset($node->wnid)) {
    return db_query("
      SELECT w.nid, c.name, c.cid, d.nid, d.sid, d.data, s.uid, u.name as user, s.submitted, s.remote_addr
      FROM {webform} w
      LEFT JOIN {webform_submitted_data} d ON w.nid = d.nid
      LEFT JOIN {webform_component} c ON d.cid = c.cid
      LEFT JOIN {webform_submissions} s  ON d.sid = s.sid
      LEFT JOIN {users} u ON s.uid = u.uid
      WHERE d.nid = c.nid 
      AND c.nid = s.nid
      AND s.nid = %d
      ORDER BY d.sid, c.cid, d.no", $node->wnid);
  }
  else {
    return NULL;
  }
}

/**
 * Get node body content for the specified webform report.
 *
 * @param node 
 *   the current node object
 * @param formatcsv 
 *   if TRUE format the output as a CSV file
 * @return 
 *   a string of text or a themed table
 */
function _webform_report_get_body_content($node, $teaser=FALSE, $page=FALSE, $formatcsv = FALSE) {

  $output = '';
  
  // query submissions  
  $rs = _webform_report_get_data($node);  

  // if any submissions
  if (!empty($rs)) {

    // display search form if desired
    if ($node->options['search_form'] && $page) {
      $output .= drupal_get_form('webform_report_search_form', $node);
    }
    
    // get report criteria
    $components = _webform_report_get_components($node->wnid);
    $columns = _webform_report_get_columns($node, $components);
    
    // if any columns
    if (count($columns) > 0) {
    
      // get other report criteria
      $filters = _webform_report_get_filters($node, $components);
      $sorton = _webform_report_get_sorton($node, $components);
  
      // init values
      $headers = array();
      $rows = array();
      $fields = array();
      $csid = 0;
      
      // set column headers
      foreach ($columns as $index => $col) {
        if ($col['hidden'] != TRUE) {
          // set report header - also save type for later
          $headers[] = array(
            'data' => $col['name'],
            'field' => $col['cid'],
            'type' => $col['type']
          );
        }
        // fields by cid for quick lookup
        $fields[$col['cid']] = $col['name'];
        
        // Get mapping for select lists (Start)
        if ($col['type'] == 'select') {
          // get component info
          $result = db_query("SELECT c.extra FROM {webform_component} c WHERE c.nid = %d AND c.cid = %d", $node->wnid, $col['cid']);
          $r = db_fetch_object($result);
          $extra = unserialize($r->extra);
          // load the webform select component handler
          module_load_include('inc', 'webform', 'components/select');
          // detect webforms 3.x and handle 
          if (function_exists('_webform_select_options_callback')) {
            // make a webform "component"
            $wfcomp['extra'] = $extra;
            // get select options
            $columns[$index]['pairs'] = _webform_select_options($wfcomp);
          }
          // get options from webforms 2.x
          else {
            if (strpos($extra['items'],'|')){
              $columns[$index]['pairs'] = array();
              foreach (explode("\n",$extra['items']) as $pair){
                list ($k,$v) = explode ('|',$pair);
                $columns[$index]['pairs'][$k] = $v;
              }
            }
          }
        }
        // Get mapping for select lists (End)
      }
      // add filter fields to lookup
      foreach ($filters as $index => $filter) {
        // fields by cid for quick lookup
        $fields[$filter['cid']] = $filter['name'];
      }
      
      // get column data
      while (TRUE) {
        
        // get next submission data
        $row = db_fetch_object($rs);
      
        // check for end of data or end of submission
        if (!$row || $row->sid != $csid) {
          
          // process data for this submission, if any
          if ($csid != 0) {
          
            // test submission against filters
            if (_webform_report_test_filters($data, $filters, $columns)) {
              // output row if filters pass
              $rows[] = _webform_report_output($data, $columns);
            }
          }
          // if end of submission data, exit
          if (!$row) {
            break;
          }
          
          // set current submission id
          $csid = $row->sid;
          
          // empty raw data row
          $data = array();
          
          // save submitter uid and node nid
          $data['uid'] = $row->uid;
          $data['nid'] = $row->nid;
            
          // fill in meta fields
          if (array_key_exists(-1, $fields)) {
            $data[-1] = array(l($row->user, 'user/' . $row->uid));
          }
          if (array_key_exists(-2, $fields)) {
            $data[-2] = array($row->submitted);
          }
          if (array_key_exists(-3, $fields)) {
            $data[-3] = array($row->submitted);
          }
          if (array_key_exists(-4, $fields)) {
            $data[-4] = array($row->remote_addr);
          }
          if (array_key_exists(-5, $fields)) {
            $data[-5] = array(l('edit', 'node/'. $row->nid . '/submission/' . $row->sid . '/edit'));
          }
          if (array_key_exists(-6, $fields)) {
            $data[-6] = array(l('view', 'node/'. $row->nid . '/submission/' . $row->sid));
          }
          if (array_key_exists(-7, $fields)) {
            $data[-7] = array(l('delete', 'node/'. $row->nid . '/submission/' . $row->sid . '/delete'));
          }
        }
      
        // if component is on report
        if (array_key_exists($row->cid, $fields)) {
          // add raw data
          $data[$row->cid][] = $row->data;
        }
        
      }   // end - while (TRUE)...
      
      // see if any rows are available
      if (count($rows) > 0) {
        // sort
        _webform_report_sort($headers, $rows, $sorton, $columns);
        
        // execute php code, if given
        if (isset($node->options['php_code'])) {
          eval('?>' . $node->options['php_code']);
        }
        
        // output in requested format  
        if ($formatcsv) {
          // format as csv
          $output = _webform_report_output_csv($headers, $rows);
        }
        else {
          // Display number of rows after description.
          $output .= filter_xss_admin($node->description) . 
                     ' (' . count($rows) . ' ' . t('results') . ') ';
          if (!$node->options['hide_csv']) {
            $output .= l(t('Download as CSV'), 'node/' . arg(1) . '/csv') . '</p>';
          }
          // output current page
          $output .= _webform_report_pager($headers, $rows, $node);
        }
      }
      // no submissions met criteria
      else {
        $output .= t('There are no submissions that match the criteria for the selected webform.');
      }
      
    }   // end - if (count($columns) > 0)...
    
    // webform report does not have any criteria
    else {
      $output = t('It appears that no criteria have been specified for this report. 
        Please click on the Criteria tab to add webform data to your report.');
    }
    
  }   // end - if (!empty($rs))...
  
  // the selected webform has no submissions.
  else {
    $output = t('There are no submissions for the selected webform. Either the form
       has not yet been completed by anyone, or the results have been cleared. This will not
       prevent you from creating this report, but this message will be displayed on the report
       page until someone submits the selected webform.');
  }
  
  return $output;
}

/**
 * test data against filters
 *
 * @param data
 *   array of raw submission data
 * @param filters
 *   list of filters to test against data
 * @return 
 *   TRUE if filters passed, otherwise FALSE
 */
function _webform_report_test_filters($data, $filters, $columns) {

  // filter result, return true if no filters
  $ok = TRUE;

  // if any filters
  if (count($filters) > 0) {
  
    // search flag
    $hit = FALSE;

    // load submitter user and webform node for later
    $submitter = user_load($data['uid']);
    $wfnode = node_load($data['nid']);
    
    // loop through all filters  
    foreach($filters as $index => $filter) {

      // reset result for each filter  
      $ok = FALSE;
        
      // check that cid is in data
      if (array_key_exists($filter['cid'], $data)) {
    
        // get raw data
        $raw = $data[$filter['cid']];
        
        // format value
        foreach ($columns as $col) {
          if ($col['cid'] == $filter['cid']) {
            break;
          }
        }
        $value = _webform_report_format_data($raw, $col);
        
        // prepare filter values
        $filter_data = strip_tags(strtolower(trim($value['data'])));
        $filter_value = $filter['value'];
        
        // do token substitution, if installed
        if (module_exists('token')) {
          // replace globals
          $filter_value = token_replace($filter_value);
          // replace submitter user values
          $filter_value = token_replace($filter_value, 'user', $submitter);
          // replace webform node values
          $filter_value = token_replace($filter_value, 'node', $wfnode);
        }

        // normalize
        $filter_value = strtolower(trim($filter_value));

        // extract filter values for between
        if ($filter['ftype'] == 13) {
            $filter_value = explode('|', $filter_value);
            // should be two values
            if (count($filter_value) == 1) {
              $filter_value[] = '';
            }
        }        
        
        // flag eq, ne, gt, lt, ge, le, between compares
        $cmpflag = (
          $filter['ftype'] == 5 || $filter['ftype'] == 6 ||
          $filter['ftype'] == 9 || $filter['ftype'] == 10 ||
          $filter['ftype'] == 11 || $filter['ftype'] == 12 || 
          $filter['ftype'] == 13
        );
        
        // see if special date and time processing is required
        if ($filter['type'] == 'date' && $cmpflag) {
          // user entered date
          if ($filter['cid'] > 0) {
            // webform 2.x stored dates as array elements
            if (count($raw) > 1) {
              $filter_data = strtotime($raw[0] .'/'. $raw[1] .'/'. $raw[2]);
            }
            // handle webform 3.x dates (yyyy-mm-dd)
            else {
              $filter_data = strtotime($raw[0]);
            }
          }
          // submission date
          else {
            $filter_data = strtotime(date('m/d/Y', $raw[0]));
          }
        }
        if ($filter['type'] == 'time' && $cmpflag) {
          // user entered time
          if ($filter['cid'] > 0) {
            $filter_data = strtotime($raw[0] .':'. $raw[1]);
          }
          // submission time
          else {
            $filter_data = strtotime(date('H:m', $raw[0]));
          }
        }
        // convert filter value to timestamp if necessary
        if (($filter['type'] == 'date' || $filter['type'] == 'time') && $cmpflag) {
          if (is_array($filter_value)) {
            $filter_value[0] = strtotime($filter_value[0]); 
            $filter_value[1] = strtotime($filter_value[1]); 
          }
          else {
            $filter_value = strtotime($filter_value);
          }
        }
        
        // extract filename for file types
        if ($filter['type'] == 'file') {
          $filter_data = '';
          // new file handling in webforms 6.x-3.x - data is an id
          if (is_numeric($raw[0])) {
            // must load the webform file component handler
            module_load_include('inc', 'webform', 'components/file');
            $file = webform_get_file($raw[0]);
            if (!empty($file->fid)) {
              $filter_data = webform_file_name($file->filepath);
            }
          }
          // file handling for prior versions of webforms - data is serialized fileinfo
          else {
            $tmp = unserialize($raw[0]);
            if (!empty($tmp['filename'])) {
              $filter_data = $tmp['filename'];
            }
          }
        }
        
        // apply filter
        switch($filter['ftype']) {
    
          // search for x
          case -1:
            // convert submission date and time to strings
            if ($filter['cid'] == -2) {
              $filter_data = date('m/d/Y', $raw[0]);
            }
            if ($filter['cid'] == -3) {
              $filter_data = date('H:m', $raw[0]);
            }
            if ($filter_data != '' && strpos($filter_data, $filter_value) !== FALSE) {
              $hit = TRUE;
              $ok = TRUE;
            }
            break;
            
          // none
          case 0:
            $ok = TRUE;
            
          // Begins with x.
          case 1:
            if (substr($filter_data, 0, drupal_strlen($filter_value)) == $filter_value) {
              $ok = TRUE;
            }
            break;
        
          // Does not begin with x.
          case 2:
            if (substr($filter_data, 0, drupal_strlen($filter_value)) != $filter_value) {
              $ok = TRUE;
            }
            break;
            
          // Contains x.
          case 3:
            if (!$filter_value || strpos($filter_data, $filter_value) !== FALSE) {
              $ok = TRUE;
            }
            break;
            
          // Does not contain x.
          case 4:
            if (!$filter_value || strpos($filter_data, $filter_value) === FALSE) {
              $ok = TRUE;
            }
            break;
            
          // Equals x.
          case 5:
            if ($filter_data == $filter_value) {
              $ok = TRUE;
            }
            break;
            
          // Does not equal x.
          case 6:
            if ($filter_data != $filter_value) {
              $ok = TRUE;
            }
            break;
            
          // is empty
          case 7: 
            if (drupal_strlen($filter_data) == 0) {
              $ok = TRUE;
            }
            break;
            
          // is not empty        
          case 8: 
            if (drupal_strlen($filter_data) != 0) {
              $ok = TRUE;
            }
            break;
            
          // greater than
          case 9: 
            if ($filter_data > $filter_value) {
              $ok = TRUE;
            }
            break;
            
          // less than
          case 10: 
            if ($filter_data < $filter_value) {
              $ok = TRUE;
            }
            break;
            
          // greater than or equal
          case 11: 
            if ($filter_data >= $filter_value) {
              $ok = TRUE;
            }
            break;
            
          // less than or equal
          case 12: 
            if ($filter_data <= $filter_value) {
              $ok = TRUE;
            }
            break;
          
          // between
          case 13:
            // negate filter if both values are empty
            if ((!$filter_value[0] && !$filter_value[1]) ||       
                ($filter_data >= $filter_value[0] && $filter_data <= $filter_value[1])) {
              $ok = TRUE;
            }
            break;
          }   // end - switch($filter['type'])...
          
        }   // end - if (array_key_exists($filter['cid'], $data)...
        
        // if hit on search, quit - search filters are last,
        // so any report filters have already been applied
        if ($hit) {
          break;
        }
        
        // if last filter was search, keep going
        if ($filter['ftype'] == -1) {
          continue;
        }
        
        // if filter did not pass, don't check any further
        if (!$ok) {
          break;
        }
      
      }   // end - foreach($filters as $index => $filter)...
      
    }   // end - if (count($filters) > 0)...
  
  // return filter result
  return $ok;
}

/**
 * format output table row for report
 *
 * @param data 
 *   raw submission data - array with key of cid, value an array of raw data
 * @param columns
 *   list of report columns
 * @return 
 *   table row suitable for theme_table
 */
 function _webform_report_output($data, $columns) {

  $row = array();

  // add each column to output row, applying formatting
  foreach ($columns as $index => $col) {

    // check if data exists
    if (array_key_exists($col['cid'], $data)) {
    
      // get raw data
      $raw = $data[$col['cid']];

      // add column to output row
      $row[] = _webform_report_format_data($raw, $col);
    }
    else {
    
      // no data, add dummy data
      $row[] = array('data' => '', 'field' => $col['cid']);
    }
  }
  
  return $row;
}

/**
 * format raw submission data
 *
 * @param raw
 *   array of raw submission data
 * @param column
 *   the report column
 * @return 
 *   array of formatted table data
 */
function _webform_report_format_data($raw, $column) {

  $out = '';
  
  // handle file type
  if ($column['type'] == 'file') {
  
    // init variables
    $link = '';
    $text = '';
    $path = '';
    $text = '';
     
    // new file handling in webforms 6.x-3.x - data is an id
    if (is_numeric($raw[0])) {
      // must load the webform file component handler
      module_load_include('inc', 'webform', 'components/file');
      $file = webform_get_file($raw[0]);
      if (!empty($file->fid)) {
        $text = webform_file_name($file->filepath);
        $path = webform_file_url($file->filepath);
        $link = '<a href="' . $path . '">' . 
                $text . '</a>' . 
                ' (' . (int) ($file->filesize/1024) . ' KB)';
      }
    }
    // file handling for prior versions of webforms - data is serialized fileinfo
    else {
      $tmp = unserialize($raw[0]);
      if (!empty($tmp['filename'])) {
        $text = $tmp['filename'];
        $path = base_path() . $tmp['filepath'];
        $link = '<a href="' . $path . '">' . 
                $text . '</a>' . 
                ' (' . (int) ($tmp['filesize'] / 1024) .' KB)';
      }
    }
    // output file column - save path for later
    $out = array('data' => $link, 'field' => $column['cid'], 'sort' => $text, 'path' => $path);
  }
    
  // handle dates
  else if ($column['type'] == 'date') {
    $ts = 0;
    // if report date
    if ($column['cid'] > 0) {
      // handle webforms 6.x-3.x dates
      if (count($raw) == 1) {
        // date is stored as yyyy-mm-dd
        $ts = strtotime($raw[0]);
      }
      // handle prior webforms versions
      else {
        // if date was selected
        if ($raw[0] && $raw[1]) {
          // make timestamp, dates are stored month, day, year
          $ts = strtotime($raw[0] .'/'. $raw[1] .'/'. $raw[2]);
        }
      }
    }
    // otherwise get submission timestamp
    else {
      $ts = $raw[0];
    }
    // if valid timestamp
    if ($ts > 0) {
      $format = $column['format'];
      // set default format if none given
      if (empty($format)) {
        $format = 'Y-m-d';
      }
      // format the date
      $out = array('data' => date($format, $ts), 'field' => $column['cid'], 'sort' => $ts);
    }
  }
    
  // handle times
  else if ($column['type'] == 'time') {
    $ts = 0;
    if ($column['cid'] > 0) {
      // handle webforms 6.x-3.x time
      if (count($raw) == 1) {
        // date is stored as hh:mm:ss
        $ts = strtotime($raw[0]);
      }
      // handle prior webforms versions
      else {
      // if time was selected
        if ($raw[0] && $raw[1]) {
          // make timestamp, times are stored hh, mm, ampm
          $ts = strtotime($raw[0] . ':' . $raw[1] . $raw[2]);
        }
      }
    }
    else {
      // otherwise get submission timestamp
      $ts = $raw[0];
    }
    // if valid timestamp
    if ($ts > 0) {
      $format = $column['format'];
      // default format if none given
      if (empty($format)) {
        $format = 'H:i';
      }
      // format the time
      $out = array('data' => date($format, $ts), 'field' => $column['cid'], 'sort' => $ts);
    }
  }
  
  // handle select type
  else if ($column['type'] == 'select') {
    $data = '';
    $sort = '';
    // process each key in case of multi-selects
    for ($i=0; $i<count($raw); $i++) {
      if ($i > 0) {
        $data .= ',';
        $sort .= ',';
      }
      // save key value
      $key = $raw[$i];
      // display key if wanted or if no key/value pairs exist
      if ($column['format'] == 'KEY' || !array_key_exists('pairs', $column)) {
        $data .= $key;
      }
      else {
        // set select value to friendly format (default)
        $data .= isset($column['pairs'][$key]) ? $column['pairs'][$key] : '';
      }
      // set sort key - use key if selected
      if ($column['option'] == 'KEY') {
        $sort .= $key;
      }
      // otherwise use data
      else {
        $sort .= $column['pairs'][$key];
      }
    }
    // save the value
    $out = array('data' => $data, 'field' => $column['cid'], 'sort' => $sort);
  }
  
  // handle other column types
  else {
    // get raw data
    $tmp = filter_xss_admin(implode(',', $raw));
    // build sort key  
    $sort = strip_tags($tmp);
    // allow for accents if module is present
    if (function_exists('_accents_remove_accents')) {
      $sort = _accents_remove_accents($sort);
    }
    $out = array('data' => $tmp, 'field' => $column['cid'], 'sort' => $sort);

    // add links
    if (valid_email_address($out['data'])) {
      $out['data'] = l($out['data'], 'mailto:' . $out['data']);
    }
    else {
      if (valid_url($out['data'], TRUE)) {
          $out['data'] = l($out['data'], $out['data']);
      }
    }
  }
  
  // pass hidden attribute to output
  if ($column['hidden'] == TRUE) {
    $out['hidden'] = TRUE;
  }
    
  // return formatted data
  return $out;
}

/**
 * sort row data
 *
 * anonymous sort comparison function creation adapted from:
 * http://www.the-art-of-web.com/php/sortarray/
 *
 * @param headers
 *   array of report headers
 * @param rows
 *   array of report rows
 * @param sorton
 *   list of default sort parameters
 */
function _webform_report_sort(&$headers, &$rows, $sorton, $columns) {

  // will be empty if user has not clicked on a header
  $init = !isset($_GET['order']);
  
  // get current sort - 'name' key is column name, 'sql' is cid
  $ts = tablesort_get_order($headers);
  // get sort order
  $ts_order = tablesort_get_sort($headers);

  // if inital display (no column header has been clicked)
  if ($init) {
  
    // if sort parameters given
    if (count($sorton) > 0) {
      
      // init variables
      $a = array();
      $cid = 0;
      $ord = '';
      
      // loop through sorts      
      foreach ($sorton as $sort) {
        // find column to be sorted
        foreach($columns as $index => $col) {
          if ($sort['cid'] == $col['cid']) {
            // save first sort column
            if ($cid == 0) {
              $cid = $sort['cid'];
              if ($sort['order'] == SORT_ASC) {
                $ord = 'asc';
              }
              else {
                $ord = 'desc';
              }
            }
            // add the sort function
            $a[] = _webform_report_sort_column($index, ($sort['order'] == SORT_DESC), $sort['type']);
            break;
          }
        }
      }   // end - foreach ($sorton...
      
      // flag the header with the first sort column
      foreach ($headers as $index => $header) {
        // mark the first sort header
        if ($header['field'] == $cid) {
          $headers[$index]['sort'] = $ord;
          break;
        }  // end - if ($header['field'])...
      }   // end - foreach ($headers as... 
      
      if (count($a) == 0) {
        // probably no columns
        $code = 'return 0;';
      }
      else if (count($a) == 1) {
        // only one clause, return it
        $code = 'return ' . $a[0] . ';';
      }
      else {
        // multiple clauses, build descending compare function
        $code = '';
        for ($i=0; $i<count($a); $i++) {
          if ($i < count($a) - 1) {
            $code .= 'if (($tmp = ' . $a[$i] . ') == 0) { ';
          }
          else {
            $code .= '$tmp = ' . $a[$i] . '; ';
          }
        }
        for ($i=0; $i<count($a) - 1; $i++) {
          $code .= ' } ';
        }
        $code .= 'return $tmp;';
      }
    }
  }
  // user has clicked on a column header - must sort that column
  else {
    foreach ($headers as $index => $header) {
      if ($header['field'] == $ts['sql']) {
        $headers[$index]['sort'] = $ts_order;
        $code = 'return ' . _webform_report_sort_column($index, ($ts_order == 'desc'), $header['type']) . ';';
        break;
      }
    }
  }
  // sort the rows
  usort($rows, create_function('$a,$b', $code));
}

/**
 * Generate a sort function for a column
 *
 * @param index
 *   the index of the field to sort
 * @param desc 
 *   if true sort is descending
 * @param type
 *   the field type 
 * @return 
 *   the sort function for the given column
 */
function _webform_report_sort_column($index, $desc = FALSE, $type = '') {

  // create comparison based on type
  // for dates and times compare timestamps
  if ($type == 'date' || $type == 'time') {
      $code = "(\$a[$index]['sort']-\$b[$index]['sort'])";
  }
  // for all other types, handle as string
  else {
    $code = "strcasecmp(\$a[$index]['sort'], \$b[$index]['sort'])";
  }
  // invert output for descending sort
  if ($desc) {
    $code = $code . ' * -1';
  }
  $code = '(' . $code . ')';

  return $code;
}

/**
 * Display a list of webform reports in the admin area.
 *
 * @return 
 *   a themed table
 */
function _webform_report_page() {

  $output = NULL;
  if (user_access('access webform reports')) {
    $headers = array(t('View'), t('Edit'), t('Delete'));

    $result = db_query(db_rewrite_sql("SELECT n.nid, n.title FROM {node} n WHERE n.type='webform_report'"));
   
    while ($node = db_fetch_object($result)) {
      $rows[] = array(
        l($node->title,'node/' . $node->nid),
        l(t('edit'), 'node/'.$node->nid.'/edit'),
        l(t('delete'), 'node/'.$node->nid.'/delete')
      );
    }
  
    $output = theme_table($headers, $rows, array('class' => 'webform'));
  }
  return $output;
}

/**
 * Paginate the webform report.
 *
 * @return 
 *   a themed table with page links
 */
function _webform_report_pager($headers, $rows, $node) {

  global $pager_page_array, $pager_total;

  // get results per page setting - start with default
  $results_per_page = 20;
  // if value set, use that
  if (array_key_exists('results_per_page', $node->options)) { 
    $results_per_page = $node->options['results_per_page'];
  }
  // Add the css file for form display.
  drupal_add_css(drupal_get_path('module', 'webform_report') . '/webform_report.css');

  // Break the array into chunks for pagination.
  if ($results_per_page != 0) {
    $pages = array_chunk($rows, $results_per_page, TRUE); 
  }
  else {
    $pages[0] = $rows;
  }
  
  // Grab the 'page' query parameter.
  // Taken from pager_query() in pager.inc
  $page = isset($_GET['page']) ? $_GET['page'] : '';
  // Convert comma-separated $page to an array, used by other functions.
  $pager_page_array = explode(',', $page);
  if ($page == '') $page = 0;
  
  // filter out hidden columns
  $out = array();
  foreach ($pages[$page] as $row) {
    $outrow = array();
    foreach ($row as $cell) {
      $hide = isset($cell['hidden']) ? $cell['hidden'] : FALSE;
      if (!$hide) {
        $outrow[] = $cell;
      }
    }
    $out[] = $outrow;
  }
  
  // format the table with the current page
  $output = theme_table($headers, $out, array('class' => 'webform_report'));
    
  // Put some magic in the two global variables
  // Based on code in pager_query() in pager.inc
  $pager_total[0] = count($pages);
  $pager_page_array[0] =
    max(0, min(
      (int)$pager_page_array[0],
      ((int)$pager_total[0]) - 1)
    );
      
  // Add the pager to the output.
  $output .= theme('pager', NULL, $results_per_page, 0);

  return $output;
}

/**
 * Output a webform report in CSV format
 *
 * @param node
 *   the current node
 */
function webform_report_csv($node) {

  $output = _webform_report_get_body_content($node, FALSE, FALSE, TRUE);
      
  $fname = 'wfr_export.csv';
  header('Content-Type: text/plain');
  header('Content-Length: ' . strlen($output));
  header('Content-Disposition: attachment; filename="' . $fname . '"');
  echo $output;
}

/**
 * Format webform report data as a CSV
 *
 * @param headers
 *   report headers
 * @param rows
 *   report rows
 * @return 
 *   CSV output
 */
function _webform_report_output_csv($headers, $rows) {

  $output = '';
  
  // output headers
  $tmp = array();
  foreach($headers as $header) {
    $tmp[] .= _webform_report_format_csv_column($header['data']);
  }
  $output .= implode(',', $tmp) . "\n";

  // output rows
  foreach($rows as $row) {
    $tmp = array();
    foreach($row as $cell) {
      // output cell if not hidden
      if (!array_key_exists('hidden', $cell)) {
        // special handling for file types - use entire path
        if (array_key_exists('path', $cell)) {
          $data = $cell['path'];
        }
        else {
          $data = strip_tags($cell['data']);
        }
        $tmp[] = _webform_report_format_csv_column($data);
      }
    }
    $output .= implode(',', $tmp) . "\n";
  }

  return $output;
}

/**
 * Format a CSV column value
 *
 * @param value
 *   the value to format for CSV
 * @return 
 *   CSV column
 */
function _webform_report_format_csv_column($value) {

  // if value contains a comma, it should be delimited by quotes
  if (strpos($value, ',') !== FALSE) {
      // if value contains quotes, double the quotes
      if (strpos($value, '"')) {
        return '"' . str_replace('"', '""', $value) . '"';
      }
      else {
        return '"' . $value . '"';
      }
  }
  return $value;
}

