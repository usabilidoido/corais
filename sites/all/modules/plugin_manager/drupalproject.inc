<?php

/**
 * @file
 * Object representation of any project hosted on Drupal.org
 */

class DrupalProject implements Project {
  protected $xml;

  /**
   * Build a project out of a short_name.  This will pull in all of the
   * information the XML 
   */
  function __construct($short_name) {
    $version = constant("DRUPAL_CORE_COMPATIBILITY");

    // Look up the data for every project requested.
    $file = drupal_http_request("http://updates.drupal.org/release-history/$project/$version");
    $this->xml = simplexml_load_string($file->data);

    // If it can't find the info for this project, skip to the next one.
    if ($this->xml == FALSE){
      throw new Exception(t('Downloading the release history failed for @project.', array('@project' => $project)));
    }
  }
  
  /**
   * Get a listing of all taxonomy terms applied to a project.
   * 
   * @return
   *   An arrays that represents any terms associated with the project.
   */
  public function get_terms() {
    
  }
  
  /**
   * Get all available information about the project.
   * 
   * @return
   *   An associative array that contains the name, description, author,
   * and project link for the current project.
   */
  public function get_project_info() {
    
  }
  
  /**
   * Get a list of all of the available releases.
   * 
   * @return
   *   An array of all of the version names available for release, sorted
   * by order of priority.
   */
  public function get_release_list() {
    return array_keys(_parse_release_info());
  }
  
  /**
   * Get the release information for a specific release version 
   * 
   * @param $version
   *   The version to get release information for.
   * @return
   *   An array containing all available release information.
   */
  public function get_release_info($version) {

  }
  
  /**
   * Get the version number of the recommended release.
   * 
   * @return
   *   A string containing the recommended release version number.
   */
  public function recommended_release() {
    $releases = _parse_release_info();
    if (empty($releases)) {
      throw new Exception(t('No releases are defined for this project.'));
    }
    return key($releases);
  }
  
  /**
   * Download the selected release.
   * 
   * @return
   *   A string containing the path of the downloaded release.
   */
  public function download_release($version) {
  }

  protected function _parse_release_info() {
    static $release_list = array();
    if (!isset($release_list[$version])) {
      // Find out the recommended and supported versions so that we can
      // later prioritize.
      $rec = (string)$xml->recommended_major;
      $sup = explode(",", $xml->supported_majors);

      // Get the information about every release
      foreach ($xml->releases->release AS $release) {
        if ((string)$release->status == 'published') {
          $ver = (string)$release->version;
          $release_list[$ver] = array(
            'release_link'  => (string)$release->release_link,
            'download_link' => (string)$release->download_link,
            'version_major' => (string)$release->version_major,
            'version_patch' => (string)$release->version_patch,
            'version_extra' => (string)$release->version_extra,
            'recommended' => ($rec == (string)$release->version_major),
            'supported' => in_array((string)$release->version_major, $sup),
          );
        }
      }

      // Order them and then return the results.
      uasort($release_list, 'this->_sort_releases');
    }
    return $release_list;
  }

  /**
   * Helper function to sort projects by priority.
   */
  protected function _sort_releases($a, $b) {
    $hierarchy = array('recommended', 'supported', 'version_major', 'version_patch', 'version_extra');
    foreach($hierarchy AS $level) {
      if ($b[$level] != $a[$level]) {
        return $b[$level] - $a[$level];
      }  
    }
    return 0;
  }

}
