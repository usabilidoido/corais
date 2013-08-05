<?php
// $Id: update_advanced.compare.inc,v 1.1 2009/09/26 07:55:26 davereid Exp $

/**
 * @file
 * Code required only when comparing available updates to existing data.
 */

// Make sure we always have /modules/update/update.compare.inc loaded.
module_load_include('inc', 'update', 'update.compare');

/**
 * Populate an array of disabled project data.
 *
 * @see _update_process_info_list()
 */
function _update_advanced_process_disabled_info_list(&$projects, $list, $project_type) {
  foreach ($list as $file) {
    if (!empty($file->status)) {
      // Skip enabled modules or themes.
      continue;
    }

    // Skip if the .info file is broken.
    if (empty($file->info)) {
      continue;
    }

    // Skip if it's a hidden module.
    if (!empty($file->info['hidden'])) {
      continue;
    }

    // If the .info doesn't define the 'project', try to figure it out.
    if (!isset($file->info['project'])) {
      $file->info['project'] = update_get_project_name($file);
    }

    // If we still don't know the 'project', give up.
    if (empty($file->info['project'])) {
      continue;
    }

    // If we don't already know it, grab the change time on the .info file
    // itself. Note: we need to use the ctime, not the mtime (modification
    // time) since many (all?) tar implementations will go out of their way to
    // set the mtime on the files it creates to the timestamps recorded in the
    // tarball. We want to see the last time the file was changed on disk,
    // which is left alone by tar and correctly set to the time the .info file
    // was unpacked.
    if (!isset($file->info['_info_file_ctime'])) {
      $info_filename = dirname($file->filename) .'/'. $file->name .'.info';
      $file->info['_info_file_ctime'] = filectime($info_filename);
    }

    if (!isset($file->info['datestamp'])) {
      $file->info['datestamp'] = 0;
    }

    $project_name = $file->info['project'];
    if (!isset($projects[$project_name])) {
      // Only process this if we haven't done this project, since a single
      // project can have multiple modules or themes.
      $projects[$project_name] = array(
        'name' => $project_name,
        'info' => $file->info,
        'datestamp' => $file->info['datestamp'],
        'includes' => array($file->name => $file->info['name']),
        // Core can't be disabled, there's no reason to special-case it here.
        'project_type' => $project_type,
      );
    }
    // Only record include data for disabled projects.
    elseif ($projects[$project_name]['project_type'] == $project_type) {
      $projects[$project_name]['includes'][$file->name] = $file->info['name'];
      $projects[$project_name]['info']['_info_file_ctime'] = max($projects[$project_name]['info']['_info_file_ctime'], $file->info['_info_file_ctime']);
      $projects[$project_name]['datestamp'] = max($projects[$project_name]['datestamp'], $file->info['datestamp']);
    }
  }
}
