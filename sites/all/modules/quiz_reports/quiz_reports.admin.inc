<?php


/*
 * @file
 * Administration file for Quiz stats Module
 *
 */

/**
 * Redirects the user to the latest revision for the selected quiz
 * 
 * @param object $quiz
 *  Quiz node object.
 * @return string
 *  Themed html 
 */
function quiz_reports_redirect($quiz) {

  if ( $quiz->type !== 'quiz' ) 
    return t('This is not a quiz, and we can not create statistics for this type.');

  
  // This query returns the latest revision of a quiz that has answers.
  // Returning revisions with no answers does not make sense for statistics.
  $sql = "SELECT
            max(nr.vid)
          FROM
            {node_revisions} nr
          INNER JOIN
            {quiz_node_results} qnr ON qnr.vid = nr.vid
          WHERE
            nr.nid = %d
          ORDER BY
            nr.vid DESC";
  
  $revision = db_result( db_query($sql, $quiz->nid) );
  
  if ( $revision !== NULL ) {
     $path = $_GET['q'] . '/' . $revision;
     drupal_goto( $path );
  } else {
    return t('An error has occurred: The quiz returned no revisions.');
  }
  
}

/**
 * Get stats for a single quiz. Maybe also for a single user.
 *
 * @param $vid
 *   quiz revision id
 *
 * @param $uid
 *   User id
 *
 * @return
 *   HTML page with charts/graphs
 */
function quiz_reports_get_report($vid) {
  $revision_selector = drupal_render_form('quiz-stats-revision', _guiz_reports_get_quiz_revisions( $vid ));
  
  // Accumulated for quiz and NOT questions
  $accumulated = array(
    'data' => _quiz_reports_get_quiz_accumulated_data($vid, $uid),
    'properties' => _quiz_reports_get_quiz_properties($vid),
  );
  
  drupal_add_css(drupal_get_path('module', 'quiz_reports') .'/theme/quiz_reports.css', 'module');
  drupal_add_js(drupal_get_path('module', 'quiz_reports') . '/theme/javascript/jquery.cycle.all.2.72.js', 'module');
  drupal_add_js(drupal_get_path('module', 'quiz_reports') . '/theme/javascript/jquery.easing.1.1.1.js', 'module');
  drupal_add_js(drupal_get_path('module', 'quiz_reports') . '/theme/javascript/quiz_reports.js', 'module');
   
  // Get stats for questions
  $questions = _quiz_reports_get_statistics_for_questions($vid, $uid);
  // Load themed output for each question
  foreach ( $questions as $question ) {
    $function = $question->type . '_get_themed_output';
    $question_output = function_exists($function) ? call_user_func($function, $question->statistics) : '<div class="no-statistics">' . t('No statistics is implemented for this question type') . '</div>';
    $question->output = theme('quiz_get_question', $question, $question_output);
  }
  $output = theme('quiz_reports_output', $revision_selector, $accumulated, $questions);
  return $output;
  
}

/**
 * Gets all revisions for a Quiz and calculates completed takes for each revision.
 *
 * @todo Setting the default value doesnt seem to work.
 * @todo Remove form element from form array. Is not needed.
 *
 * @param string $vid 
 * @return array
 *      A API form array. Used by drupal_render_form to create the HTML form output.
 */
function _guiz_reports_get_quiz_revisions( $vid ) {
  // Fetch node id for the given quiz revision id ($vid).
  $sql = "SELECT nid FROM {node_revisions} WHERE vid = %d";
  $nid = db_result( db_query( $sql, $vid ) );
  
  // Get all revisions of quiz
  $sql = "SELECT vid FROM {node_revisions} WHERE nid = %d";
  $result = db_query($sql, $nid);
  while ($row = db_fetch_array($result)) {
    $revisions[] = $row['vid'];
  }
  
  // Get all takes for each revision
  foreach( $revisions as $key=>$_vid ) {
    $sql = "SELECT COUNT(*) FROM {quiz_node_results} WHERE nid = %d AND vid = %d";
    $result = db_query($sql, $nid, $_vid);
    $takes = db_result($result);
    $revision_takes[$key+1] = array('revision' => $key+1, 'version' => $_vid, 'takes' => $takes);
  }
  
  $revision_takes = array_reverse($revision_takes, true);
  
  foreach ($revision_takes as $revision) {
    $options[$revision['version']] = t('Revision @revision (@takes)', array( '@revision' => $revision['revision'], '@takes' => format_plural($revision['takes'], '1 take', '@count takes')));
  }
  
  // Make form
  $form = array();
  
  $form['quizreports-form'] = array(
    '#type' => 'form',
    '#prefix' => '<div class="revision-wrapper">',
    '#suffix' => '</div>',
  );

  $form['quizreports-form']['revisions'] = array(
    '#type' => 'select',
    '#name' => 'ChangeRevision',
    '#id' => 'quizreports-revision-changer',
    '#title' => t('Choose revision'),
    '#value' => $vid,
    '#options' => $options,
    '#description' => t('Select from list to change revision to view statistics from.'),
  );
  
  // Add correct path to javascript settings, this is used in
  // theme/javascript/quiz_reports.js to change
  $path = explode("/", $_GET['q']);
  array_pop($path);
  $path = implode('/', $path);
  
  drupal_add_js(array('quiz_reports' => array('basePath' => $path)), 'setting');
  
  // Return form
  return $form;
  
}

function _quiz_reports_get_quiz_properties( $vid ) {
  
  $sql = "SELECT
            *
          FROM
            {quiz_node_properties}
          WHERE
            vid = %d";

  return db_fetch_array(db_query($sql, $vid));
  
}

/**
 * Loads all questions for given quiz revision ($vid) and generates statistics for each
 * question type.
 *
 * @param string $vid 
 * @param string $uid 
 * @return array
 */
function _quiz_reports_get_statistics_for_questions($vid, $uid) {
  $questions = array();

  $sql = "SELECT 
            n.nid,
            qnr.child_nid AS question_nid,
            qnr.child_vid AS question_vid,
            qnr.weight,
            nr.body AS title,
            n.type,
            nt.name AS type_name
          FROM 
            {quiz_node_relationship} qnr 
          INNER JOIN
            {node_revisions} nr ON nr.vid = qnr.child_vid
          INNER JOIN 
            {node} n ON n.nid = nr.nid
          INNER JOIN 
            {node_type} nt ON nt.type = n.type
          WHERE 
            qnr.parent_vid = %d
          ORDER BY 
            qnr.weight";
  
  $nodes = db_query($sql, $vid);
  
  while ( $node = db_fetch_object($nodes) ) {
    $questions[] = _quiz_reports_get_statistics_for_question_type($node, $vid);
  }
  return $questions;
  
}

/**
 * Calculates generic statistics for a question, filtered by the quiz revision ($vid). 
 * It then tries to locate and call a function specific for the question type that 
 * will return statistics for the question type.
 *
 * @param object $question 
 * @param string $vid
 *          Version ID of Quiz and NOT the question for the Quiz.
 * @return object
 */
function _quiz_reports_get_statistics_for_question_type($question, $vid) {
  $statistics = array();
  
  $sql = "SELECT
          	COUNT(*) AS count
          FROM
          	{quiz_node_results_answers}
          WHERE
          	question_nid = %d
            AND question_vid = %d";
  
  $count = db_result( db_query($sql, $question->question_vid, $question->question_vid) );
  
  // Generate statistics that are the same for each question type...
  $statistics['generic'] = _guiz_reports_get_generic_statistics_for_question($question, $vid);
  
  // Try to locate a function that will give question type
  // specific statistics.  
  $type = $question->type;
  
  $func = $question->type . '_report_get_statistics';
  $statistics['specific'] = function_exists($func) ? call_user_func($func, $question, $vid, $statistics['generic']) : array();
  
  // Apply question statistics to question object
  $question->statistics = $statistics;
  return $question;
}

function _guiz_reports_get_generic_statistics_for_question( $question, $vid ) {
    
    $generic = array();
    
    // Count all takes
    $sql = "SELECT 
            	count(*) AS count,
            	SUM(is_correct=0) AS incorrect,
	            SUM(is_correct=1) AS correct,
	            SUM(is_skipped=1) AS skipped
            FROM
            	{quiz_node_results_answers} qnra
            INNER JOIN
            	{quiz_node_results} qnr using(result_id)
            WHERE
            	qnra.question_vid = %d
            	AND qnr.vid = %d";
    
    $stats = db_fetch_array(db_query($sql, $question->question_vid, $vid));
    
    $count = $stats['count'];
    $generic['taken'] = $count;
    
    if ( $count > 0 ) {
      $correct = $stats['correct'];
      $generic['correct_percentage'] = round(($correct/$count)*100);
      $generic['correct'] = $correct;
      
      $correct = $stats['correct'];
      $generic['correct_percentage'] = round(($correct/$count)*100);
      $generic['correct'] = $correct;
      
      $incorrect = $stats['incorrect'];
      $generic['incorrect_percentage'] = round(($incorrect/$count)*100);
      $generic['incorrect'] = $incorrect;
      
      $skipped = $stats['skipped'];
      $generic['skipped_percentage'] = round(($skipped/$count)*100);
      $generic['skipped'] = $skipped;
      
    }
    
    $sql = "SELECT max_score FROM {quiz_question_properties} WHERE nid = %d AND vid = %d";
    $max_score = db_result(db_query($sql, $question->question_nid, $question->question_vid));
    $generic['max_score'] = $max_score;
    
    return $generic;
    
}

/**
 * Generates accumulated statistics for a given quiz
 *
 * @param string $vid 
 *                Version ID to lookup quiz
 * @param string $uid 
 *                Not in use, should probably be removed
 * @return array
 */
function _quiz_reports_get_quiz_accumulated_data($vid, $uid) {
  
  
  // Get quiz properties
  $sql = "SELECT 
            *
          FROM 
            {quiz_node_properties}
          WHERE vid = %d";
  
  $quiz_properties = db_fetch_object(db_query($sql, $vid));
  
  // Array to be returned
  $accumulated = array();
  
  $accumulated['takes'] = array();
  
  // Total count taken (does not take into account not completed takes).
  $sql = "SELECT 
            COUNT(vid) AS count
          FROM
            {quiz_node_results}
          WHERE
            vid = %d";
  
  $result = db_fetch_object(db_query($sql, $vid));
  $accumulated['takes']['started'] = (int) $result->count;
  
  // Total count completed takes
  $sql = "SELECT 
            COUNT(vid) AS count
          FROM
            {quiz_node_results}
          WHERE
            vid = %d 
            AND time_end > 0";
  
  $result = db_fetch_object(db_query($sql, $vid));
  $accumulated['takes']['completed'] = (int) $result->count;
  
  // Passed takes
  $sql = "SELECT 
             COUNT(vid) AS count
           FROM
             {quiz_node_results}
           WHERE
             vid = %d 
             AND score > %d
             AND time_end > 0";
  
  $result = db_fetch_object(db_query($sql, $vid, (int) $quiz_properties->pass_rate));
  $accumulated['takes']['passed'] = (int) $result->count;
  
  // Completed takes subtracted by passed takes
  $accumulated['takes']['failed'] = $accumulated['takes']['completed'] - $accumulated['takes']['passed'];
  
  // Started takes subtracted by completed takes
  $accumulated['takes']['in_progress'] = $accumulated['takes']['started'] - $accumulated['takes']['completed'];
  
  $accumulated['score'] = array();
  
  // Calculate average, high and low scores for quiz
  $sql = "SELECT
            MAX(score) AS high,
            MIN(score) AS low,
  	        AVG(score) AS average
          FROM 
  	        {quiz_node_results}
          WHERE
  	        vid = %d
  	        AND time_end > 0
          GROUP BY
  	        vid";
  
  $result = db_fetch_object(db_query($sql, $vid));
  $accumulated['score']['average'] = (int)$result->average;
  $accumulated['score']['high'] = (int) $result->high;
  $accumulated['score']['low'] = (int) $result->low;
  
  // Pass rate for quiz
  $accumulated['score']['pass_rate'] = (int) $quiz_properties->pass_rate;
    
  $accumulated['time'] = array();
  
  // Calculate average, fast and slowest time for quiz
  $sql = "SELECT
          	MAX(time_end - time_start) AS time_max,
          	MIN(time_end - time_start) AS time_min,
          	ROUND(AVG(time_end - time_start)) AS time_average
          FROM 
          	{quiz_node_results}
          WHERE 
          	time_end > 0 
          	AND vid = %d
          GROUP BY 
          	vid";
  
  $result = db_fetch_object(db_query($sql, $vid));
  
  $accumulated['time']['average'] = (int) $result->time_average;
  $accumulated['time']['fastest'] = (int) $result->time_min;
  $accumulated['time']['slowest'] = (int) $result->time_max;
  
  return $accumulated;
  
}

