<?php

/**
 * @file
 * Object representation of any project hosted on Drupal.org
 */

interface Project {
 
  /**
   * Build a project out of a short_name.  This will pull in all of the
   * information the XML 
   */
  function __construct($short_name);
  
  /**
   * Get a listing of all taxonomy terms applied to a project.
   * 
   * @return
   *   An arrays that represents any terms associated with the project.
   */
  public function get_terms();
  
  /**
   * Get all available information about the project.
   * 
   * @return
   *   An associative array that contains the name, description, author,
   * and project link for the current project.
   */
  public function get_project_info();
  
  /**
   * Get a list of all of the available releases.
   * 
   * @return
   *   An array of all of the version names available for release, sorted
   * by order of priority.
   */
  public function get_release_list();
  
  /**
   * Get the release information for a specific release version 
   * 
   * @param $version
   *   The version to get release information for.
   * @return
   *   An array containing all available release information.
   */
  public function get_release_info($version);
  
  /**
   * Get the version number of the recommended release.
   * 
   * @return
   *   A string containing the recommended release version number.
   */
  public function recommended_release();
  
  /**
   * Download the selected release.
   * 
   * @return
   *   A string containing the path of the downloaded release.
   */
  public function download_release($version);
}
