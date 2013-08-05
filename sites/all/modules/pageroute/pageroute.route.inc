<?php
// $Id: pageroute.route.inc,v 1.1.2.14 2009/12/12 16:46:26 sgilits Exp $
/**
 * @file
 * Route
 */

/**
 * PagerouteRoute that implements a who Pageroute
 */
class PagerouteRoute {
  public $args = array();
  public $prid;
  public $options;
  public $pages;
  public $pageIndex;
  public $pageAccess;
  public $path;
  public $pageDataCurrent;

  public function PagerouteRoute($route_data) {
    $this->prid = $route_data->prid;
    $this->path = $route_data->path;
    $this->options = $route_data->options;
    $this->pages = $route_data->pages;
    $this->pageAccess = array();

    foreach ($this->pages as $page) {
      $page->route = &$this;
    }

    $this->pageIndex = $route_data->pageIndex;
    $this->pageAccess = $route_data->pageAccess;

    $this->pageDataCurrent = NULL;
    $this->pageDataPrevious = NULL;
  }

  /**
   * Load a route from the pageroute cache
   */
  public static function load($prid, $reset = FALSE) {
    static $allroutes;

    if (!isset($allroutes) || $reset || !isset($allroutes[$prid])) {
      $cache = cache_get('pageroute');
      if (!$reset && !empty($cache->data) && isset($cache->data[$prid])) {
        $allroutes = $cache->data;
      }
      else {
        $route_data = PagerouteRoute::loadFromDatabase($prid);
  
        if (!$route_data) {
          return FALSE;
        }
        $allroutes[$prid] = $route_data;

        cache_set('pageroute', $allroutes);
      }
    }
    
    $route = new PagerouteRoute($allroutes[$prid]);
    drupal_alter('pageroute', $route);

    return $route;
  }

  /**
   * Load a route from the database
   */
  private static function loadFromDatabase($prid) {

    $result = db_query("SELECT * FROM {pageroute_routes} WHERE prid = %d", $prid);

    $route_data = db_fetch_object($result);

    if (!$route_data) {
      return FALSE;
    }

    $route_data->options = unserialize($route_data->options);

    $result = db_query("SELECT * FROM {pageroute_pages} WHERE prid = %d ORDER BY weight, name", $route_data->prid);

    $index = 0;
    $route_data->pages = array();
    $route_data->pageIndex = array();
    $route_data->pageAccess = array();

    while ($page = db_fetch_object($result)) {
      $page->options = unserialize($page->options);

      if (isset($page->options['activated']) && !$page->options['activated']) {
        //add deactivated pages to the pageIndex, but don't give them an index
        //so this pages are reachable, but we won't route to them
        $route_data->pageIndex[$page->name] = FALSE;
        continue;
      }

      $route_data->pages[$index] = $page;
      if ($route_data->options['tabs'] && isset($page->options['no_tab']) && $page->options['no_tab']) {
        $route_data->pages[$index]->no_tab = TRUE;
      }
      $route_data->pageIndex[$page->name] = $index;
      $index++;
    }

    return $route_data;
  }

  /**
   * Set suggested page:
   * Default page if no page was specified or access denied to specified page
   * else specified page
   */
  public function setSuggestedPage($target = PAGEROUTE_CURRENT, $page_name = NULL) {
    $page = &$this->pageDataCurrent;

    if ($page) {
      if (!isset($page_name)) {
        $page_name = $page->name;
      }
      $i = 0;
      do {
        $this->pageDataPrevious = &$this->pageDataCurrent;
        $this->pageDataCurrent = &$this->getPage($page_name, $target);
        $name = $this->pageDataCurrent;
        $page_name = $this->pageDataCurrent->name;
        $i++;
        if ($i>2) {
          break;
        }
      } while (isset($this->pageAccess[$page->name]) && $this->pageAccess[$this->pageDataCurrent->name]==FALSE);
    }
    else {
      $this->pageDataPrevious = &$this->pageDataCurrent;
      $this->pageDataCurrent = &$this->getDefaultPage();
    }

    return $this->pageDataCurrent;
  }

  /**
   * Get default page:
   * First page that is accessable
   */
  private function getDefaultPage() {
    foreach ($this->pages as $page) {
      if (empty($this->pageAccess) || $this->pageAccess[$page->name]!=FALSE || !isset($this->pageAccess[$page->name])) {
        return $page;
      }
    }
  }

  /**
   * Checks if this is a valid page and if access to the page shall be granted
   */
  public function checkPageAccess($page_name, $target = PAGEROUTE_CURRENT) {
    if ($target == PAGEROUTE_CURRENT) {
      return !isset($this->pageAccess[$page_name]) || $this->pageAccess[$page_name];
    }

    $access = FALSE;

    do {
      $page = $this->getPage($page_name, $target);

      if (!$page) {
        break;
      }

      $page_name = $page->name;

      $access = !isset($this->pageAccess[$page_name]) || $this->pageAccess[$page_name];

    } while ($access == FALSE);

    if ($access) {
      return $page;
    }

    return NULL;
  }

  /**
   * Build and return the form for the current page
   */
  public function getForm(&$form, &$page_data, &$page, &$form_state, &$args) {
    if (!$page) {
      return FALSE;
    }

    $page->getForm($form, $form_state, $args);
  }

  public function start($page_name) {
    if ($page_name) {
      $this->pageDataCurrent = &$this->getPage($page_name);
    }
    else {
      $this->pageDataCurrent = &$this->getDefaultPage();
    }
  }

  protected function getPage($page_name, $target = PAGEROUTE_CURRENT) {

    if (!isset($this->pageIndex[$page_name])) {
      return FALSE;
    }

    $index = $this->pageIndex[$page_name];

    switch ($target) {
      case PAGEROUTE_BACK:
        return $this->pages[$index - 1];
      case PAGEROUTE_FORWARD:
        return $this->pages[$index + 1];
      case PAGEROUTE_CURRENT:
      default:
        return $this->pages[$index];
    }
  }
}

/**
 * @file
 * Page base type
 */

/**
 * PageroutePage class that defines, how a page class should look like
 */
abstract class PageroutePage {
  public $title;
  public $options;
  public $route;
  public $name;
  public $weight;
  public $module;
  public $type;

  /**
   * Returns the form to display the page type.
   * @param $args
   *    Can be used to customize the form.
   * @return
   *    The form to display.
   */
  abstract public function getForm(&$form, &$form_state, &$args);

  /**
   *  Include stuff your page type needs.
   */
  abstract public function setUp();

  public function PageroutePage($page) {
    $this->load($page);
  }

  /**
   * Init with data of other page object/array
   */
  public function load($page) {
    $this->title = $page->title;
    $this->options = $page->options;
    $this->name = $page->name;
    $this->type = $page->type;
    $this->weight = $page->weight;
    $this->module = $page->module;

    $this->route = &$page->route;
  }

  /**
   *  Return the default action, if a cancel button is clicked.
   *  @return
   *    PAGEROUTE_CURRENT: stay on current page
   *    PAGEROUTE_FORWARD: go to next page
   *    PAGEROUTE_BACK: go to previous page
   */
  public function getCancelTarget() {
    return PAGEROUTE_CURRENT;
  }

  /**
   * Create a page object from a page data array
   * @return
   *   New page object
   */
  public static function getObject($page_data, $module = '') {
    $type = $page_data->type;
    $bases = pageroute_get_types('base');
    pageroute_include_page_type($module, $type);
    $page_class = pageroute_get_page_class($page_data->module, $page_data->type);
    $page = new $page_class($page_data);
    $page->setUp();

    return $page;
  }

  /**
   * These form fields are quite common for node forms, so page
   * types, which are also displaying node forms can use it.
   * @param $delete
   *    Wheter the "delete checkbox" should be added.
   */
  protected function nodeUI($page, &$form, $delete = TRUE) {
    $form['options']['preview'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display preview button'),
      '#default_value' => isset($page->options['preview']) ? $page->options['preview'] : 0,
      '#weight' => 5,
    );
    $form['options']['submit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display submit button'),
      '#default_value' => isset($page->options['submit']) ? $page->options['submit'] : 1,
      '#weight' => 5,
    );
    if ($delete) {
      $form['options']['nodelete'] = array(
        '#type' => 'checkbox',
        '#title' => t('Never display the delete button'),
        '#default_value' => isset($page->options['nodelete']) ? $page->options['nodelete'] : 1,
        '#weight' => 5,
      );
    }
  }

  /**
   * Remove any unnecessary buttons.
   */
  public function unsetForm(&$form, $group = '') {
    if ($group) {
      $options = $this->options[$group];
    }
    else {
      $options = $this->options;
    }
    if (isset($options['preview']) && !$options['preview']) {
      unset($form['buttons']['preview']);
    }

    if (isset($options['submit']) && !$options['submit']) {
      unset($form['buttons']['submit']);
    }

    if (isset($options['nodelete']) && $options['nodelete']) {
      unset($form['buttons']['delete']);
    }
  }

  /**
   * Handles all button clicks from your specific form.
   *  Set $args['target'] ( = $form_state['storage']['args']['target']) to specify the what to do:
   *    PAGEROUTE_CURRENT: stay on current page
   *    PAGEROUTE_FORWARD: go to next page
   *    PAGEROUTE_BACK: go to previous page
   *  To customize get_form(...) set $args['todo'] ( = $form_state['storage']['args']['todo']).
   */
  public function formSubmitted(&$form_state) { }

  /**
   * Describes the page type.
   *  @return
   *    array('name' => t('Human readable page type name'));
   */
  public abstract static function info();

  /**
   * Provides a help text for the pageroute user interface.
   *  @return
   *    t('Help text.')
   */
  public abstract static function help();

  /**
   * Gets the default submit handler from drupal
   */
  public static function getDefaultSubmitHandler($form) { }
  /**
   * Gets the default submit handler from drupal
   */
  public static function getDefaultValidateHandler($form) { }
}

