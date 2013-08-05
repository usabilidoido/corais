;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; onBeforeUnload API
;; $Id: README.txt,v 1.1.2.6 2009/03/31 08:21:47 markuspetrux Exp $
;;
;; Project homepage: http://drupal.org/project/onbeforeunload
;; Module Author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CONTENTS OF THIS FILE
=====================
* OVERVIEW
  * Modules using the onBeforeUnload API
  * Web Standards
* INSTALLATION
* BASIC EXAMPLE
* API DOCUMENTATION


OVERVIEW
========

The onBeforeUnload module provides an API to allow other modules use the
onBeforeUnload event of the browser window.

This module does not provide any direct functionally, it rather provides
a centralized API that other modules can use to take advantage of the
onBeforeUnload event of the browser window.

Modules using the onBeforeUnload API
------------------------------------

- http://drupal.org/project/dirtyforms (Dirty Forms).

Web Standards
-------------

There is no public specification on the onBeforeUnload event. It was introduced
by Microsoft IE 4 and has subsequently been copied by other browsers.

References:

- onBeforeUnload event (Microsoft Developer Network).
  http://msdn.microsoft.com/en-us/library/ms536907(VS.85).aspx

- window.onbeforeunload (Mozilla Developer Center).
  https://developer.mozilla.org/en/DOM/window.onbeforeunload

Please, let me know if there's any other browser that supports this event.
Thanks! :)


INSTALLATION
============

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Goto Administer > Site building > Modules to install this module.


BASIC BASIC EXAMPLE
===================

Let's say your module needs to perform some check when the user leaves the page.
Ok, this module should add the onBeforeUnload API and its own javascript file
to the page with a code similar to this:

<?php
  // Add the onBeforeUnload API.
  onbeforeunload_add_js();

  // Add mymodule behavior.
  drupal_add_js(drupal_get_path('module', 'mymodule') .'/mymodule.js');
?>

<code>
/**
 * Contents of mymodule.js.
 */
Drupal.behaviors.mymodule = function(context) {
  if (!Drupal.onBeforeUnload.callbackExists('mymodule')) {
    Drupal.onBeforeUnload.addCallback('mymodule', function() {
      return 'Hello world!';
    });
  }
};
</code>

Please, check out the onBeforeUnload Example module, shipped with this package.


API DOCUMENTATION
=================

/**
 * Add an onBeforeUnload callback.
 *
 * Note that it is only possible to add one callback per module.
 * It is up to the module implementing the callback itself to
 * perform any additional tasks it may need.
 *
 * @param module
 *   The name of the module that adds the onBeforeUnload callback.
 * @param callback
 *   A function that will be called without arguments by our
 *   global onBeforeUnload handler.
 * @return
 *   TRUE if the callback was successfully added, FALSE otherwise.
 */
Drupal.onBeforeUnload.addCallback(module, callback);

/**
 * Remove an onBeforeUnload callback.
 *
 * @param module
 *   The name of the module.
 * @return
 *   TRUE if the callback was successfully removed, FALSE otherwise.
 */
Drupal.onBeforeUnload.removeCallback(module);

/**
 * Check if a callback for a particular module exists.
 *
 * @param module
 *   The name of the module.
 * @return
 *   TRUE if the callback exists, FALSE otherwise.
 */
Drupal.onBeforeUnload.callbackExists(module);

/**
 * Disable the onBeforeUnload event handler.
 */
Drupal.onBeforeUnload.disable();

/**
 * Enable the onBeforeUnload event handler.
 */
Drupal.onBeforeUnload.enable();
