<?php
// $Id: views-view-views-jsgantt.tpl.php,v 1.4.2.7 2010/10/07 15:48:54 danepowell Exp $

/**
 * @file
 */

$jsgantt_dir = drupal_get_path('module', 'views_jsgantt');
drupal_add_js($jsgantt_dir . "/jsgantt/jsgantt.js");
drupal_add_css($jsgantt_dir . "/jsgantt/jsgantt.css");
drupal_add_css($jsgantt_dir . "/views_jsgantt.css");
?>
<div style="position:relative" class="gantt" id="GanttChartDIV"></div>
<script language="javascript">
var g = new JSGantt.GanttChart('g',document.getElementById('GanttChartDIV'), '<?php print($options['format']); ?>');
g.setShowRes(0); g.setShowDur(0); g.setShowComp(0); g.setCaptionType('Resource'); g.setShowStartDate(0); g.setShowEndDate(0);
<?php
for ($rid = 0; $rid < count($view->result); $rid++) {
  $row = $view->result[$rid];
  // for each remaining row in the result after the 'current' row
  for ($i = $rid+1; $i < count($view->result); $i++) {
    // if this row belongs to the same project as the 'current' row
    if ($view->result[$i]->$options['parentid'] == $row->$options['parentid']) {
      // move this row up
      array_splice($view->result, $rid+1, 0, array($view->result[$i]));
      array_splice($view->result, $i+1, 1);
      break;
    }
  }
}
$last_group = 0;
foreach ($view->result as $rid => $row) {
  $task = array();
  $task['pID'] = is_numeric($row->nid) ? $row->nid : 0;
  $task['pName'] = check_plain($row->$options['tasktitle']);
  $task['pStart'] = date("n/j/Y", strtotime($row->$options['startdate']));
  $task['pEnd'] = $row->$options['enddate'] ? date("n/j/Y", strtotime($row->$options['enddate'])) : '';
  $task['pComp'] = 0;
  if (!empty($options['progress'])) {
    if (is_numeric($row->$options['progress'])) {
      $task['pComp'] = $row->$options['progress'];
    }
  }
  $task['pRes'] = $view->render_field($options['resource'], $rid);
  if ($task['pRes'] == 'Anonymous') {
    $task['pRes'] = '';
  }
  // replace divs with spans to display caption with multiple values inline
  $task['pRes'] = str_replace('div', 'span', $task['pRes']);
  // wrap caption in a no-wrap span to prevent line breaks
  $task['pRes'] = '<span style="white-space: nowrap;">' . $task['pRes'] . '</span>';
  // separate multiple values with a comma
  $task['pRes'] = str_replace('/span><span', '/span><span>, </span><span', $task['pRes']);
  $task['pColor'] = 'f87217';
  if ($options['colorsource'] == 'progress') {
    $cutoff = ($task['pComp'] / 100) * (strtotime($task['pEnd']) - strtotime($task['pStart'])) + strtotime($task['pStart']);
    if ($cutoff > time()) {
      $task['pColor'] = '0000ff';
    }
    if ($task['pComp'] == 100) {
      $task['pColor'] = '817679';
    }
  }
  else if (!empty($options['colorby'])) {
    $list = array_filter(array_map('trim', explode("\n", $options['colorlist'])), 'strlen');
    foreach ($list as $opt) {
      // Sanitize the user input with a permissive filter.
      $opt = content_filter_xss($opt);
      if (strpos($opt, '|') !== FALSE) {
        list($key, $value) = explode('|', $opt);
        $colors[$key] = (isset($value) && $value !=='') ? $value : $key;
      }
      else {
        $colors[$opt] = $opt;
      }
    }
    $color_key = $row->$options['colorby'];
    if (is_array($colors) && array_key_exists($color_key, $colors)) {
      $task['pColor'] = $colors[$color_key];
    }
  }
  $task['pLink'] = url('node/' . $task['pID']);
  $task['pMile'] = $task['pEnd'] ? 0 : 1;
  $task['pParent'] = 0;
  if (!empty($options['parentid'])) {
    if (is_numeric($row->$options['parentid'])) {
      $task['pParent'] = $row->$options['parentid'];
    }
  }
  $task['pDepend'] = 0;
  if (!empty($options['dependson'])) {
    if (is_numeric($row->$options['dependson'])) {
      $task['pDepend'] = implode(',',$row->$options['dependson']);
    }
  }
  $format = "g.AddTaskItem(new JSGantt.TaskItem(%d, '%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d, 1, '%s'));\n";
  if ($task['pParent'] != $last_group) {
    // This task has a different parent from the last - add the parent group
    $task['pParentLink'] = url('node/' . $task['pParent']);
    $task['pParentTitle'] = '';
    if (!empty($options['parenttitle'])) {
      $task['pParentTitle'] = check_plain($row->$options['parenttitle']);
    }
    printf($format, $task['pParent'], $task['pParentTitle'], '', '', '', $task['pParentLink'], 0, '', 0, 1, 0, 0);
    $last_group = $task['pParent'];
  }
  printf($format, $task['pID'], $task['pName'], $task['pStart'], $task['pEnd'], $task['pColor'], $task['pLink'], $task['pMile'],$task['pRes'], $task['pComp'],0, $task['pParent'], $task['pDepend']);
}
?>
g.Draw();
g.DrawDependencies();
</script>
