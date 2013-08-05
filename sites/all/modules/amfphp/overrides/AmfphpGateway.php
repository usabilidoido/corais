<?php
// $Id: AmfphpGateway.php,v 1.2 2007/01/16 19:43:11 snelson Exp $

require_once drupal_get_path('module', 'amfphp') . "/amfphp/core/amf/app/Gateway.php";

class AmfphpGateway extends Gateway {
  
  /**
	 * Override the Gateway() method 
	 */
	function AmfphpGateway() {
		if(AMFPHP_PHP5) {
			include_once(drupal_get_path('module', 'amfphp') . "/overrides/php5Executive.php");
			include_once(AMFPHP_BASE . "shared/exception/php5Exception.php");
		}
		else {
			include_once(drupal_get_path('module', 'amfphp') . "/overrides/php4Executive.php");
			include_once(AMFPHP_BASE . "shared/exception/php4Exception.php");
		}
		
		$this->exec = new Executive();
		$this->filters = array();
		$this->actions = array();
		$this->registerFilterChain();
		$this->registerActionChain();
	}
	
	/**
	 * remove redundant actions
	 */
	function registerActionChain()
	{
		$this->actions['adapter'] = 'adapterAction';
		
		// we are not using class files, and security checking is done in drupal
		
		//$this->actions['class'] = 'classLoaderAction';
		//$this->actions['security'] = 'securityAction';
		
		$this->actions['exec'] = 'executionAction';
		
		//$this->actions['ws'] = 'webServiceAction';
	}
}