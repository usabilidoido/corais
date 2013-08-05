<?php
// $Id: gb.batch.inc,v 1.2 2009/07/12 02:59:45 gordon Exp $

/**
 * @file
 * provide batch functions for processing git repositories. These function a written to be run
 * from the BatchAPI.
 */

/**
 * Create a clone of a git repository.
 *
 * This will create a clone of the specified repository and then set the newly cloned repository as the
 * default batch repository for all following batch commands.
 */
function gb_batch_clone($src, $dst, $bare, &$context) {
  if (isset($context['results']['abort']) && $context['results']['abort']) {
    return;
  }
  
  $cmd = 'clone -q ';
  if ($bare) {
    $cmd.= '--bare ';
  }
  $cmd.= $src .' '. $dst;
  
  if ($ret = gb_real_git($cmd, $output)) {
    drupal_set_message(t('Problem running git "%cmd"', array('%cmd' => $cmd)), 'error');
    $context['results']['abort'] = TRUE;
  }
  
  $context['results']['git'] = array(
    'repos' => $dst .($bare ? '' : '/.git'),
    'path' => $dst,
  );
  $context['finished'] = 1;
  $context['message'] = t('Clone repository');
}

/**
 * Set User Name
 */
function gb_batch_set_user($name, $mail, &$context) {
  if (isset($context['results']['abort']) && $context['results']['abort']) {
    return;
  }
  
  foreach (array('user.name '. escapeshellarg($name), 'user.email '. escapeshellarg($mail)) as $setting) {
    $cmd = 'config '. $setting;
    if (gb_real_git($cmd, $output, $context['results']['git']['repos'])) {
      drupal_set_message(t('Problem running git "%cmd"', array('%cmd' => $cmd)), 'error');
      $context['results']['abort'] = TRUE;
    }
  }
  
  $context['finished'] = 1;
  $context['message'] = t('Setting user identification');
}

/**
 * Create tag on the current batch repository
 */
function gb_batch_tag_commit($hash, $tag, $anotation, &$context) {
  if (isset($context['results']['abort']) && $context['results']['abort']) {
    return;
  }
  
  $anotation = $anotation ? escapeshellarg($anotation) : '\'\'';
  
  $cmd = 'tag -a '. escapeshellarg($tag) .' -m '. $anotation .' '. $hash;
  if (gb_real_git($cmd, $output, $context['results']['git']['repos'])) {
    drupal_set_message(t('Problem running git "%cmd"', array('%cmd' => $cmd)), 'error');
    $context['results']['abort'] = TRUE;
  }
  
  $context['finished'] = 1;
  $context['message'] = t('Create tag on repository');
}

/**
 * Add remote to other repository
 */
function gb_batch_remote_add($src, $dst, $name, &$context) {
  if (isset($context['results']['abort']) && $context['results']['abort']) {
    return;
  }
  
  // Remove any old versions of the gb_tmp_remote
  if (gb_real_git('remote show', $output, $src)) {
    // If this fails we are not going to worry so much.
  }
  elseif (in_array($name, $output)) {
    gb_real_git('remote rm '. $name, $output, $src);
  }
  
  $cmd = 'remote add '. escapeshellarg($name) .' '. escapeshellarg($dst);
  if (gb_real_git($cmd, $output, $src)) {
    drupal_set_message(t('Problem running git "%cmd"', array('%cmd' => $cmd)), 'error');
    $context['results']['abort'] = TRUE;
  }
  
  $context['finished'] = 1;
  $context['message'] = t('Added remote entry to pull in tag.');
}

/**
 * Fetch tags from a remote repository
 */
function gb_batch_fetch_tags($repos, $remote, &$context) {
  if (isset($context['results']['abort']) && $context['results']['abort']) {
    return;
  }
  
  $cmd = 'fetch --tags '. escapeshellarg($remote);
  if (gb_real_git($cmd, $output, $src)) {
    drupal_set_message(t('Problem running git "%cmd"', array('%cmd' => $cmd)), 'error');
    $context['results']['abort'] = TRUE;
  }
  
  $context['finished'] = 1;
  $context['message'] = t('Fetch new tag');
}

/**
 * Push tags to the origin of the specified repository
 */
function gb_batch_push_tags($repos, &$context) {
  if (isset($context['results']['abort']) && $context['results']['abort']) {
    return;
  }
  
  $cmd = 'push --tags';
  if (gb_real_git($cmd, $output, $repos)) {
    drupal_set_message(t('Problem running git "%cmd"', array('%cmd' => $cmd)), 'error');
    $context['results']['abort'] = TRUE;
  }
  
  $context['finished'] = 1;
  $context['message'] = t('Pushing the tags to the current origin.');
}

/**
 * 
 */
function gb_batch_rm_repository($repos, &$context) {
  if (is_dir($repos) && !gb_batch_rmdirr($repos)) {
    drupal_set_message(t('Unable to remove temporary repository "%path"', array('%path' => $repos)));
    $context['results']['abort'] = TRUE;
  }
  
  if ($context['results']['git']['repos'] == $repos) {
    unset($context['results']['git']);
  }
  
  $context['finished'] = 1;
  $context['message'] = t('Remove temporary repository.');
}

function gb_batch_rmdirr($dir) {
  if (!file_exists($dir)) {
    return true;
  }
  if (!is_dir($dir)){
    return unlink($dir);
  }
  foreach (scandir($dir) as $item) {
    if ($item == '.' || $item == '..') continue;
    if (!gb_batch_rmdirr($dir . DIRECTORY_SEPARATOR . $item)) return false;
  }
  return rmdir($dir);
}
/**
 * Complete batch processing
 */
function gb_batch_complete($success, $results, $operations) {
  $_SESSION['gb_success'] = $success;
}