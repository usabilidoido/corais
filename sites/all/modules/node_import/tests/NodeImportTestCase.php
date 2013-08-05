<?php

/**
 * @file
 * DrupalWebTestCase wrapper with some extra functions to
 * ease the development of node_import tests.
 */

class NodeImportTestCase extends DrupalWebTestCase {
  public function setUp() {
    $modules = func_get_args();
    $modules[] = 'node_import';
    $modules[] = 'date_api';
    call_user_func_array(array('parent', 'setUp'), $modules);
  }

  public function tearDown() {
    parent::tearDown();
  }

  /**
   * Output variable in test results.
   *
   * @param ...
   *   Variables to dump.
   *
   * @return
   *   Nothing.
   */
  public function debug() {
    $vars = func_get_args();
    foreach ($vars as $var) {
      $this->fail("<pre>" . print_r($var, TRUE) . "</pre>", 'Debug');
    }
  }

  /**
   * Create the user that will do the import.
   *
   * @param $perms
   *   Array of permissions. The 'import content' permission
   *   will be added.
   *
   * @return
   *   User object.
   */
  public function nodeImportCreateUser($perms = array()) {
    if (!is_array($perms)) {
      $perms = func_get_args();
    }
    $perms[] = 'import content';
    $user = $this->drupalCreateUser($perms);
    return $user;
  }

  /**
   * Create a file with the specified data.
   *
   * @param $data
   *   Twodimensional array.
   *
   * @param $file_options
   *   Array with record separator, etc.
   *
   * @return
   *   String. Path to file (in temporary directory).
   */
  public function nodeImportCreateFile($data, $file_options = array()) {
    module_load_include('inc', 'node_import');
    $path = file_create_filename($this->randomName() . '.csv', file_directory_temp());

    $fp = fopen($path, 'w');
    foreach ($data as $row) {
      $s = node_import_write_to_string($row, $file_options);
      fputs($fp, $s);
    }
    fclose($fp);

    return $path;
  }

  /**
   * Create an import task by going through the form.
   *
   * Note that the user needs to be logged in. Also note that this
   * will not start the import. You will need to run
   * $this->nodeImportDoAllTasks() for this which will try to finish
   * all import tasks.
   *
   * $data['file'] needs to be a full real path to a file.
   *
   * @param $data
   *   Array of data to submit.
   *
   * @return
   *   Import task id or FALSE if it fails.
   */
  public function nodeImportCreateTask($data) {
    $defaults = array(
      'type' => '',
      'file' => '',
      'file_options' => array(),
      'map' => array(),
      'options' => array(),
      'defaults' => array(),
    );
    $data = array_merge($defaults, $data);

    $step = 1;
    $steps = 8;

    $this->drupalPost('admin/content/node_import/add', array(), t('Next'));
    $title = t('Select content type');
    $expect = t('@title (step @step of @steps)', array('@title' => $title, '@step' => $step, '@steps' => $steps));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;
    $step++;

    $this->drupalPost(NULL, array('type' => $data['type']), t('Next'));
    $title = t('Select file');
    $expect = t('@title (step @step of @steps)', array('@title' => $title, '@step' => $step, '@steps' => $steps));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;
    $step++;

    $this->drupalPost(NULL, array('files[file_upload]' => realpath($data['file'])), t('Upload file'));
    $title = t('New file uploaded');
    $expect = t('New file %name uploaded to %path.', array('%name' => basename($data['file']), '%path' => node_import_directory() . basename($data['file'])));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;

    $this->drupalPost(NULL, array(), t('Next'));
    $title = t('Set file options');
    $expect = t('@title (step @step of @steps)', array('@title' => $title, '@step' => $step, '@steps' => $steps));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;
    $step++;

    $this->drupalPost(NULL, $data['file_options'], t('Next'));
    $title = t('Map file columns');
    $expect = t('@title (step @step of @steps)', array('@title' => $title, '@step' => $step, '@steps' => $steps));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;
    $step++;

    $this->drupalPost(NULL, $data['map'], t('Next'));
    $title = t('Set import options');
    $expect = t('@title (step @step of @steps)', array('@title' => $title, '@step' => $step, '@steps' => $steps));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;
    $step++;

    $this->drupalPost(NULL, $data['options'], t('Next'));
    $title = t('Set default values');
    $expect = t('@title (step @step of @steps)', array('@title' => $title, '@step' => $step, '@steps' => $steps));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;
    $step++;

    $this->drupalPost(NULL, $data['defaults'], t('Next'));
    $title = t('Preview import');
    $expect = t('@title (step @step of @steps)', array('@title' => $title, '@step' => $step, '@steps' => $steps));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;
    $step++;

    $this->drupalPost(NULL, array(), t('Next'));
    $title = t('Start import');
    $expect = t('@title (step @step of @steps)', array('@title' => $title, '@step' => $step, '@steps' => $steps));
    if (!$this->assertRaw($expect, t('%title found.', array('%title' => $title)), 'Node import')) return FALSE;
    $step++;

    $this->drupalPost(NULL, array(), t('Start import'));
    $url = url('admin/content/node_import/', array('absolute' => TRUE));
    if (!$this->assertTrue(strpos($this->getUrl(), $url) === 0, t('Redirected to import progress page.'), 'Node import')) return FALSE;
    $taskid = substr($this->getUrl(), strlen($url));
    $this->pass(t('Import task %taskid created.', array('%taskid' => $taskid)), 'Node import');

    return $taskid;
  }

  /**
   * Finish all pending import tasks.
   *
   * @param $max_runs
   *   Integer. Maximum number of iterations.
   *
   * @return
   *   Number of cron runs needed.
   */
  public function nodeImportDoAllTasks($max_runs = 100) {
    module_load_include('inc', 'node_import');

    $runs = 0;
    while ($runs <= $max_runs) {
      $runs++;
      node_import_do_all_tasks('ms', '1000');
      $result = db_result(db_query("SELECT count(*) FROM {node_import_tasks} WHERE status <> %d", NODE_IMPORT_STATUS_DONE));
      if ($result > 0) continue;
      break;
    }

    $this->assertTrue($runs <= $max_runs, t('Needed %runs iterations (of %max_runs allowed).', array('%runs' => $runs, '%max_runs' => $max_runs)), 'Node import');

    return $runs;
  }

  /**
   * Check to see if the row status is as specified.
   *
   * @param $result
   *   Record from {node_import_status}.
   *
   * @param $status
   *   What the status should be.
   *
   * @param $message
   *   Message to display along with the assertion.
   *
   * @param $group
   *   The type of assertion - examples are "Browser", "PHP".
   *
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  public function assertRowStatus($result, $status, $message = '', $group = 'Node import') {
    return $this->assert(is_array($result) && isset($result['status']) && $result['status'] == $status, $message ? $message : t('Row status set to %d', array('%d' => $status)), $group);
  }

  /**
   * Check to see if the row status is NODE_IMPORT_STATUS_DONE.
   *
   * @param $result
   *   Record from {node_import_status}.
   *
   * @param $message
   *   Message to display along with the assertion.
   *
   * @param $group
   *   The type of assertion - examples are "Browser", "PHP".
   *
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  public function assertRowStatusDONE($result, $message = '', $group = 'Node import') {
    return $this->assertRowStatus($result, NODE_IMPORT_STATUS_DONE, $message ? $message : t('Row status set to DONE.'), $group);
  }

  /**
   * Check to see if the row status is NODE_IMPORT_STATUS_ERROR.
   *
   * @param $result
   *   Record from {node_import_status}.
   *
   * @param $message
   *   Message to display along with the assertion.
   *
   * @param $group
   *   The type of assertion - examples are "Browser", "PHP".
   *
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  public function assertRowStatusERROR($result, $message = '', $group = 'Node import') {
    return $this->assertRowStatus($result, NODE_IMPORT_STATUS_ERROR, $message ? $message : t('Row status set to ERROR.'), $group);
  }

  /**
   * Check to see if the row status is NODE_IMPORT_STATUS_PENDING.
   *
   * @param $result
   *   Record from {node_import_status}.
   *
   * @param $message
   *   Message to display along with the assertion.
   *
   * @param $group
   *   The type of assertion - examples are "Browser", "PHP".
   *
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  public function assertRowStatusPENDING($result, $message = '', $group = 'Node import') {
    return $this->assertRowStatus($result, NODE_IMPORT_STATUS_PENDING, $message ? $message : t('Row status set to PENDING.'), $group);
  }

  public function assertRowErrorContains($result, $error, $message = '', $group = 'Node import') {
    $errors = is_array($result['errors']) ? $result['errors'] : unserialize($result['errors']);
    return $this->assert(in_array($error, $errors, TRUE), $message ? $message : t('Row errors contains error message.'), $group);
  }
}

