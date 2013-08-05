<?php
// $Id: pageroute_test.inc,v 1.1.2.1 2009/12/05 11:45:44 sgilits Exp $

/**
 * @file
 * Contains a helper class for the tests
 */

/**
 * PagerouteTestCase is a little class that simplifies testing pageroute tests
 */
abstract class PagerouteTestCase extends DrupalWebTestCase {
  function setUp($module = '') {
    if (empty($module)) {
      parent::setUp('pageroute', 'pageroute_ui');
    }
    else {
      if (!module_exists($module)) {
        return;
      }
      else {
        parent::setUp('pageroute', 'pageroute_ui', $module);
      }
    }
    
  }

  function useAdmin() {
    $user = $this->drupalCreateUser(array('administer pageroutes', 'administer nodes', 'access administration pages'));
    $this->drupalLogin($user);
    $this->drupalGet('admin/build/pageroute');
    $this->assertText('Pageroute', "Make sure browser is on the Pageroute site.");
    return $user;
  }

  function importPageRoutes($route) {
    $route_prid = db_last_insert_id('pageroute_routes', 'prid');
    $this->drupalGet('admin/build/pageroute');
    $this->clicklink('Import');
    $edit = array('import' => $route);
    $this->drupalPost('admin/build/pageroute/import', $edit, 'Import');
    eval('$route_array = ' . $route . ';');
    $this->assertText($route_array['path'], 'Make sure the pageroute ' . $route_array['path'] . ' was imported.');
    return $route_prid;
  }
}