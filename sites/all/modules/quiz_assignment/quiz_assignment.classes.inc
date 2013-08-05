<?php
// $Id: short_answer.classes.inc,v 1.2.2.81 2010/11/05 16:14:48 falcon Exp $

/**
 * The main classes for the quiz assignment question type.
 *
 * These inherit or implement code found in quiz_question.classes.inc.
 *
 * If you are developing your own question type, the easiest place to start is with
 * multichoice.classes.inc. short_answer and long_answer are good for understanding
 * question types that involve textual answers.
 *
 * @file
 */

/**
 * Extension of QuizQuestion.
 */
class QuizAssignmentQuestion extends QuizQuestion {
  
  /**
   * Pattern used to validate file extension.
   *
   * A file extension must start with a period (.) follow by minimum 2 and 
   * maximum 4 characters. Characters can be lowercase a-z or 0-9.
   *
   * @static
   */
  public static $PATTERN_VALID_EXETENSION = '/^[.][a-z0-9]{2,4}$/';
  
  /**
   * Globally set string of valid extensions. 
   *
   * @var string
   */
  public $global_valid_extensions = NULL;
  
  /**
   * Local string of valid extensions. 
   *
   * Each question can override global list of valid extensions.
   *
   * @var string
   */
  public $valid_extensions = NULL;
  
  /**
   * An array of valid extensions.
   *
   * @var array
   */
  public $valid_extensions_array = array();
  
  /**
   * By default, use drupals local storage
   *
   * @var string
   */
  public $storage_path = NULL;
  
  /**
   * The subpath to store the uploaded file.
   *
   * @var string
   */
  public $storage_subpath = NULL;
  
  /**
   * @param $node
   *  
   */
  public function __construct(stdClass &$node) {
    parent::__construct($node);
    $this->getNodeProperties(); 
  }
  
  /**
   * Implementation of getCreationForm
   *
   * @see QuizQuestion#getCreationForm($form_state)
   *
   * @param
   *  FAPI form state
   * @return
   *  Must return a FAPI array.
   */
  public function getCreationForm(array $form_state = NULL) {
  	
    $form['file_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Valid file extensions'),
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE,
      '#description' => t('Use this field to limit the types of files that can be uploaded. Leave it empty (NOT RECOMMENDED) if you wish to allow all file types. Currently the site has a max file upload size of !max_size. By default, the list is populated from the global !url.', array('!max_size' => l(format_size(file_upload_max_size()), 'http://drupal.org/node/97193', array('attributes' => array('target' => '_blank'))), '!url' => l('quiz assignment question configuration', 'admin/quiz/settings/questions_settings'))),
      '#weight' => -4,
    );
    
    $form['file_fieldset']['valid_extensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Extensions'),
      '#description' => t('A comma-separated list of valid file extensions. Every extension must start with a period (.). Every quiz can override this list on an individual basis.'),
      '#value' => $this->valid_extensions,
    );
    
    return $form;
  }
  
  /**
   * Saving properties for question.
   *
   * We only allow the user to change the list of valid file extensions. If the
   * list of extensions is not the same as the global list, store them as 
   * question specific properties. 
   *
   * @param boolean $is_new
   *  Indicates if we are storing for the first time or if we are updating 
   *  existing properties.
   */
  public function saveNodeProperties($is_new = FALSE) {
    
    // Get valid file extensions from form
    $submitted_extensions = trim($_POST['valid_extensions'], ",");
    
    if ( $is_new ) {
      $sql = "INSERT INTO {quiz_assignment_properties} (nid, vid, valid_extensions) VALUES (%d, %d, '%s')";
      db_query($sql, $this->node->nid, $this->node->vid, $submitted_extensions); 
    }
    else {
      $sql = "UPDATE {quiz_assignment_properties} p SET p.valid_extensions = '%s' WHERE p.nid = %d AND p.vid = %d";
      db_query($sql, $submitted_extensions, $this->node->nid, $this->node->vid);
    }
     
  }
  
  /**
   * Validates the question.
   */
  public function validateNode(array &$form) {
  	
    // Get all extensions and validate them
    $extensions = split(',', trim($form['file_fieldset']['valid_extensions']['#post']['valid_extensions'], ","));
    $form_error = false;
    
    foreach ($extensions as $key=>$val) {
      if (preg_match(QuizAssignmentQuestion::$PATTERN_VALID_EXETENSION, trim($val)) == 0) {
        $form_error = true;
      }
    }
    
    if($form_error)
      form_set_error('valid_extensions', 'There is an error in your extension list');
    
  }
  
  public function delete($only_this_version = FALSE) {
    
    if ($only_this_version) {
      // Delete only this version nid
    }
    else {
      // Delete all versions for nid
    }
    
  }
  
  /**
   * Load question type properties for this question type.
   */
  public function getNodeProperties() {
  	
    if (isset($this->nodeProperties)) return $this->nodeProperties;
    
    $this->storage_path = trim(variable_get('quiz_assignment_local_storage_path', FALSE));
    $this->storage_subpath =  $this->node->type 
                              . DIRECTORY_SEPARATOR 
                              . 'nid' . $this->node->nid 
                              . '_vid' . $this->node->vid 
                              . DIRECTORY_SEPARATOR;
    $this->global_valid_extensions = $this->valid_extensions = variable_get('quiz_assignment_valid_file_extensions', '.doc, .docx, .odt, .pdf');
       
    // Now check if the quiz owner has overriden global valid extensions for this quiz
    $sql = "SELECT valid_extensions FROM {quiz_assignment_properties} WHERE nid = %d AND vid = %d";
    $override_extensions = db_result(db_query($sql, $this->node->nid, $this->node->vid));
    
    if ($override_extensions) {
      $this->valid_extensions = $override_extensions;
    }
    
    $valid_extensions_array = split(',', $this->valid_extensions);
    foreach ($valid_extensions_array as $ext) {
      $this->valid_extensions_array[] = trim($ext);
    }
    
    $props = parent::getNodeProperties();
        
    $sql = "SELECT valid_extensions FROM {quiz_assignment_properties} WHERE nid = %d AND vid = %d";
    $res = db_query($sql, $this->node->nid, $this->node->vid);
    $res_a = db_fetch_array($res);
    
    
    if (is_array($res_a) && !empty($res_a['valid_extensions'])) {
      $this->valid_extensions = $res_a['valid_extensions'];
    }
    
    $merge_props = array(
      'valid_extensions' => $this->valid_extensions, 
      'valid_extensions_array' => $this->valid_extensions_array,
      'storage_path' => $this->storage_path,
      'storage_subpath' => $this->storage_subpath,
    );
    
    $props = array_merge($props, $merge_props);
    $this->nodeProperties = $props;
    return $props;
  }
  
  public function getNodeView() {
  	
    $content = parent::getNodeView();
    if ($this->viewCanRevealCorrect()) {
      
    }
    else {
      
    }
    
    return $content;
  }
  
  /**
   *
   * @todo:
   *  - Check to see if we have an answer, if so append #markup FAPI to the form
   *    that displays info about the previously uploaded file.
   */
  public function getAnsweringForm(array $form_state = NULL, $rid) {
  	
  
  	$form = parent::getAnsweringForm($form_state, $rid);
    
    // Must set enctype on <form>-element to multipart/form-data to allow form
    // to handle file uploads.
    $form['#attributes'] = array('enctype' => "multipart/form-data");
    
    // Check if the current session has a result id.
    if( isset($_SESSION['quiz_' . arg(1)]))
    {
      $result_id = $_SESSION['quiz_' . arg(1)]['result_id'];    
    }
    
    if( $result_id ) {
      $sql = "SELECT answer.original_filename, answer.filepath FROM {quiz_assignment_user_answers} answer WHERE question_nid = %d AND question_vid = %d AND result_id = %d";
      $res = db_query($sql, $this->node->nid, $this->node->vid, $result_id);
      
      $file = NULL;
      while( $row = db_fetch_array($res)) {
        $file = $row;
        break;
      }
      
      // If we have a file uploaded, present it to the user
      if ($file) {
      
        $table_headers = array(
          array( 'data' => 'File', 'style' =>  'width:80%' ),
          array( 'data' => 'Format' ),
          array( 'data' => 'Size' ),
        );
        
        $table_rows = array(
          array(
            array( 'data' => l($file['original_filename'], $file['filepath']), 'style' => 'white-space:nowrap' ),
            array( 'data' => file_get_mimetype($file['original_filename']), 'style' => 'white-space:nowrap' ),
            array( 'data' => format_size(filesize($file['filepath'])), 'style' => 'white-space:nowrap' ),
          ),
        );
        
        $form['uploaded_file_group'] = array(
          '#type' => 'fieldset',
          '#title' => t('Uploaded file'),
          '#collapsible' => TRUE, 
          '#collapsed' => FALSE,
        );
        
        $form['uploaded_file_group']['uploaded_file'] = array(
          "#type" => 'markup',
          "#value" => '<p><i>' . t('Table list the file you have uploaded. Upload new file to replace.') . '</i></p>' . theme('table', $table_headers, $table_rows),
        );
      }
    }
    
    // Add max file size allowed to upload
    #$max_file_size = file_upload_max_size();
    #$form['max_file_size'] = array(
    #  '#type' => 'hidden',
    #  '#value' => $max_file_size,
    #);
    
    // Append the input type file element to the answering form    
    $form['file'] = array(
      '#type' => 'file',
      '#title' => t('Upload file as answer to this question'),
      '#description' => t('Max file size: ') . format_size(file_upload_max_size()) . '. ' . t('Valid file extensions: ') . $this->valid_extensions,
    );
    
    // It seems the Quiz core/Quiz question module must have at least one input
    // element with the name "tries", if not QuizQuestionResponse#save will not
    // be called.
    $form['tries[dummy]'] = array(
      '#type' => 'hidden',
      '#value' => 0,
    );
    return $form;
  }
  
  public function getBodyFieldTitle() {
    return t('Assignment instructions');
  }
  
  
  public function getMaximumScore() {
  	
    return 1;
  }

  
}

/**
 * This class handles the answering of questions. Response == answer in Quiz.
 *
 * Since this question type only handles uploading of files, the internal
 * workings of Quiz does not work for us. It will only try to send $_POST on
 * submit. What we need is stored in the global $_FILES array. 
 */

/**
 * Extension of QuizQuestionResponse
 */
class QuizAssignmentResponse extends QuizQuestionResponse {

  
  public $answer = NULL;
  
  /**
   *
   * @param int $result_id
   *  The current result id for the quiz take. It will be the same for all 
   *  answers for the given take on a quiz.
   *
   * @param stdClass $question_node
   *  The question node.
   *
   * @param array $answer
   *  An array with the result of the form created in 
   *  QuizQuestion::getAnsweringForm(). 
   *  
   */
  public function __construct($result_id, stdClass $question_node, $answer = NULL) {
    parent::__construct($result_id, $question_node, $answer);
    
    // The user has clicked "Back" or "Skip"   
    if (!isset($answer)) {
      // Since a quiz owner may allow skipping questions, we may get back to 
      // or leave a question without an actual answer. In that case we only have
      // a result id and question node, but not the answer. We can then use this
      // id with quesiton nid and vid to load answer from database.
          
      $this->answer = $this->getResponse();
      #$this->evaluated = false;
    }
    // The user has clicked "Next" or "Finish" (as in answered a question)
    else {
    
      // -----------------------------------------------------------------------
      // We have a new answer, but the data we want are not stored in $answer
      // passed as a parameter into this constructor. We need to load our info
      // from the global $_FILES array.
      //
      // But we also need to handle the scenario where the user has clicked on
      // the "Back" or "Skip" button, and then clicked "Next" or "Finish"
      // without attaching a new file. Since the question will list the 
      // previously uploaded file in a table, the user will believe that 
      // attaching a new file is not needed before submitting the question. In 
      // those cases, we need to treat the answer as "Back" or "Skip" event.
      // -----------------------------------------------------------------------
      
      // If true, the user doesn't want to upload a new file, but keep the old one
      if( $_FILES['files']['tmp_name']['file'] == "") { // Is there a better way to check for this?
        $this->answer = $this->getResponse();
      }
      else {
        $this->answer = $answer;
        $this->answer['source_file'] = $_FILES['files']['tmp_name']['file'];
        $this->answer['original_filename'] = $_FILES['files']['name']['file'];
        $this->answer['original_file_extension'] = $this->getFileExtension($this->answer['original_filename']);
        $this->answer['mimetype'] = file_get_mimetype($this->answer['original_filename']);
        $this->answer['delete_file_from_storage'] = TRUE;
        #$this->evaluated = false;
      }
    }
  }
  
  /**
   * Returns a file extension from a string. The returned string is always
   * lower cased.
   *
   * @param string $file
   *
   * @return string
   */
  private function getFileExtension($file) {
    return '.' . strtolower(end(preg_split("/\./", $file)));
  }
  
  /**
   * Checks to see if a file really is submitted
   *
   * @return boolean
   */
  private function isFileSubmitted() {
    $source = $_FILES['files']['tmp_name']['file'];
    
    if ( trim($source) == "" && $this->answer['delete_file_from_storage'] )
      return FALSE;
    
    return TRUE;
  }
  
  /**
   * Checks to see if the submitted file is of valid file type.
   *
   * @return boolean
   */
  private function isFileSubmittedValid() {
    if ( $this->answer['delete_file_from_storage'] ) {
      $filename = $_FILES['files']['name']['file'];
      $ext = $this->getFileExtension($filename);
      $valid_extensions = $this->question->valid_extensions_array;
      
      if ( !in_array($ext, $valid_extensions) ) {
        return FALSE;
      }
    }
    
    return TRUE;
  }
  
  /**
   * Implementation of isValid
   *
   * Note: This function is called after delete() and save(). This function will
   * only prevent the user from navigating away from the question. 
   *
   * @see QuizQuestionResponse#isValid()
   *
   * @return mixed
   *  A string if there is an error. Boolean TRUE if all ok.
   */
  public function isValid() {
  
    if( !$this->isFileSubmitted() ) {
      return t('You must upload a file');
    }
    
    if ( !$this->isFileSubmittedValid() ) {
      return t('Uploaded file is not valid. Valid file extensions: ') . $this->question->valid_extensions;
    }
    
    return TRUE;
  }
  
  /**
   * Implementation of save
   *
   * Note: This funciton is called after delete() but BEFORE isValid(). Any 
   * validation on the submit before saving must be done here.
   *
   * @see QuizQuestionResponse#save()
   */
  public function save() {
    
    // Store file in local storage
    $source_file = $_FILES['files']['tmp_name']['file'];
    
    // Move uploaded file from tmp to permanent directory.
    //
    // Check whether the file was uploaded via HTTP POST.
    // Returns TRUE if the file named by filename was uploaded via HTTP POST.
    // This is useful to help ensure that a malicious user hasn't tried to trick
    // the script into working on files upon which it should not be working--for
    // instance, /etc/passwd. 
    // See: http://no.php.net/manual/en/function.is-uploaded-file.php
    if ( is_uploaded_file($source_file) && $this->isFileSubmittedValid() ) {
      $original_filename = $_FILES['files']['name']['file'];
      $original_file_extension = $this->getFileExtension($original_filename);
      
      // Check if quiz owner wants to store uploaded files outside of drupals
      // storage system, if not use drupals storage system.
      $_storage_path = DIRECTORY_SEPARATOR . trim($this->question->storage_path, "/\\");

      if ( !empty($_storage_path) ) {
        // Check that the given path is valid, if not fall back to drupals
        // storage system.
        if ( file_check_directory($_storage_path) ) {
          $storage_path = $_storage_path;
        }
        else
        {
          $storage_path = file_directory_path();
        }
      } 
      else {
        $storage_path = file_directory_path();
      }
      
      $storage_subpath = DIRECTORY_SEPARATOR . trim($this->question->storage_subpath, "/\\");
      $storage_path_full = $storage_path . $storage_subpath . DIRECTORY_SEPARATOR;
      
      // Check if each directory in subpath exists, is writable or needs to be created
      $subpath_arr = split('/', trim($storage_subpath, "/\\"));
      $tmp_path = $storage_path;
      
      foreach ( $subpath_arr as $dir ) {
        $tmp_path .= DIRECTORY_SEPARATOR . $dir;
        
        if ( file_check_directory($tmp_path, TRUE) ) {
          continue;
        }
        else {
          $dir_check_error = true;
          break;
        }
        
      }
      
      if ( $dir_check_error ) {
        drupal_set_message( t('An error occured when storing the uploaded file (COULD NOT CREATE DIRECTORY).'), 'error' );
        return;
      }
    
      $destination_file = $storage_path_full . uniqid($this->question->nid . $this->question->vid . $this->rid . '_') . $original_file_extension;
      
      if ( move_uploaded_file($source_file, $destination_file) ) {
        $sql = "INSERT INTO {quiz_assignment_user_answers} (question_nid, question_vid, result_id, original_filename, filepath) VALUES (%d, %d, %d, '%s', '%s')";
        db_query($sql, $this->question->nid, $this->question->vid, $this->rid, $original_filename, $destination_file);
      }
      else {
        drupal_set_message(t('An error occured when storing the uploaded file (COULD NOT MOVE SOURCE TO DESTINATION).'), 'error');
      } 
    } // END: if ( is_uploaded_file($source_file) && $this->isFileSubmittedValid() )
   
  }
  
  /**
   * Implementation of delete
   *
   * Note: This function is called before save() and isValid()
   *
   * @see QuizQuestionResponse#delete()
   */
  public function delete() {
    
    // Do not delete file from storage if question is skipped    
    if ( $this->answer['delete_file_from_storage'] && $this->isFileSubmittedValid() ) {
      
      // $this->answers contains newly uploaded info, we need to load info
      // about what was upoloaded previously and delete that.
      $sql = "SELECT answer.original_filename, answer.filepath FROM {quiz_assignment_user_answers} answer WHERE question_nid = %d AND question_vid = %d AND result_id = %d";
      $res = db_query($sql, $this->question->nid, $this->question->vid, $this->rid);
      
      $file = NULL;
      while( $row = db_fetch_array($res) ) {
        $file = $row;
        break;
      }
      
      // Check that file actually exists before deleting
      if ( file_check_location($file['filepath']) ) {
        file_delete($file['filepath']);
      }
      
      // Remove old answer from database
      $sql = "DELETE FROM {quiz_assignment_user_answers} WHERE question_nid = %d AND question_vid = %d AND result_id = %d";
      db_query($sql, $this->question->nid, $this->question->vid, $this->rid);
      
    }
  }
  
  /**
   * Implementation of score
   *
   * @see QuizQuestionResponse#score()
   */
  public function score() {
    return 0;
  }
  
  /**
   * Implementation of getResponse
   *
   * @see QuizQuestionResponse#getResponse()
   */
  public function getResponse() {
    $this->answer = NULL;
    
    // Question has been answered allready. We fetch the answer data from the database.
    $sql = "SELECT original_filename, filepath, score, is_evaluated FROM {quiz_assignment_user_answers} WHERE question_nid = %d AND question_vid = %d and result_id = %d";
    $result = db_query($sql, $this->question->nid, $this->question->vid, $this->rid);
    
    while ( $row = db_fetch_array($result) ) {
      $this->answer['original_filename'] = $row['original_filename'];
      $this->answer['original_file_extension'] = $this->getFileExtension($this->answer['original_filename']);
      $this->answer['filepath'] = $row['filepath'];
      $this->answer['delete_file_from_storage'] = FALSE;
      $this->evaluated = $row['is_evaluated'];
      $this->score = $row['score'];
    }
    
    return $this->answer;
  }
  
  
  /**
   * Implementation of getReportFormResponse
   *
   * @see QuizQuestionResponse#getReportFormResponse($showpoints, $showfeedback, $allow_scoring)
   */
  public function getReportFormResponse($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = TRUE) {
    
    $form = array();
    
    $table_headers = array(
      array( 'data' => 'File', 'style' =>  'width:80%' ),
      array( 'data' => 'Size' ),
    );
    
    $file = $this->answer;
    
    $_storage_path = DIRECTORY_SEPARATOR . trim($this->question->storage_path, "/\\");
    
    // Check if file is stored outside of Drupal
    /*if ( !empty($_storage_path) ) {
      if ( file_check_directory($_storage_path) ) {
        $link_options = array(
          'query' => array('path' => $file['filepath'] ),
        );
        $link = l($file['original_filename'], 'node/' . $this->question->nid . '/quiz_assignment/download/' . $this->rid );
      }
    }
    
    // File is stored inside Drupal, just serve the file directly
    if (!$link) {
      $link = l($file['original_filename'], $file['filepath']);
    }*/
    
    
    $link = l($file['original_filename'], 'node/' . $this->question->nid . '/quiz_assignment/download/' . $this->rid );
    
    $table_rows = array(
      array(
        array( 'data' => $link, 'style' => 'white-space:nowrap' ),
        array( 'data' => format_size(filesize($file['filepath'])), 'style' => 'white-space:nowrap' ),
      ),
    );
    
    // If file is stored outside Drupal. 
    // http://<root>/node/%node/quiz_assignment/download/?path=/sites/default/files/quiz_assignment/nid24_vid58/245873_4d676765aec2b.zip
    $form['result'] = array(
      '#type' => 'markup',
      '#value' => theme('table', $table_headers, $table_rows),
    );
    
    return $form;
  }
  
  /**
   * Implementation of getReportFormScore
   *
   * @see QuizQuestionResponse#getReportFormScore($showpoints, $showfeedback, $allow_scoring)
   */
  public function getReportFormScore($showfeedback = TRUE, $showpoints = TRUE, $allow_scoring = FALSE) {
    
    // The score will be shown as a questionmark if the question haven't been scored already
    $score = ($this->isEvaluated()) ? $this->getScore() : '?';
    // We show a textfield if the quiz shall be scored. Markup otherwise
    if (quiz_access_to_score() && $allow_scoring) {
      return array(
        '#type' => 'textfield',
        '#default_value' => $score,
        '#size' => 3,
        '#maxlength' => 3,
        '#attributes' => array('class' => 'quiz-report-score'),
      );
    }
    else {
      return array(
        '#type' => 'markup',
        '#value' => $score,
      );
    }
  }
  
  /**
   * Implementation of getReportFormSubmit
   *
   * @see QuizQuestionResponse#getReportFormSubmit($showfeedback, $showpoints, $allow_scoring)
   */
  public function getReportFormSubmit($showfeedback = TRUE, $showpoints = TRUE, $allow_scoring = FALSE) {
    return $allow_scoring ? 'quiz_assignment_report_submit' : FALSE;
  }
}
