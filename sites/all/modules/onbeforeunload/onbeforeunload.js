// $Id: onbeforeunload.js,v 1.1.2.6 2009/03/30 19:57:22 markuspetrux Exp $

/**
 * @file
 * Provides an API to allow other modules use the onBeforeUnload event
 * of the browser window.
 *
 * Public methods of the Drupal.onBeforeUnload object:
 *
 * - addCallback(module, callback)
 *   Add an onBeforeUnload callback.
 *
 * - removeCallback(module)
 *   Remove an onBeforeUnload callback.
 *
 * - callbackExists(module)
 *   Check if a callback for a particular module exists.
 *
 * - enable()
 *   Enable the onBeforeUnload event handler.
 *
 * - disable()
 *   Disable the onBeforeUnload event handler.
 *
 * Note that any previously installed onBeforeUnload handler will still be
 * invoked by our window event handler. We should do this to prevent from
 * breaking the implementation of the one who installed its own before us.
 */

/**
 * Drupal behavior for onBeforeUnload API.
 */
Drupal.behaviors.onBeforeUnload = function(context) {
  var self = Drupal.onBeforeUnload;

  // Bind our window handler if not already bound.
  if (!self.processed) {
    // Ensure we do not repeat this process more than once.
    self.processed = true;

    // Save a reference to the previous onBeforeUnload handler.
    if (typeof window.onbeforeunload == 'function') {
      self._previousWindowHandler = window.onbeforeunload;
    }

    // Now, bind our window handler for the onBeforeUnload event.
    window.onbeforeunload = self._windowHandler;

    // Finally, enable our event handler.
    self.enable();
  }
};

/**
 * Global onBeforeUnload object.
 */
Drupal.onBeforeUnload = Drupal.onBeforeUnload || {
  processed: false,
  _previousWindowHandler: null,
  _enabled: false,
  _callbacks: {}
};

/**
 * Global window handler for the onBeforeUnload event.
 */
Drupal.onBeforeUnload._windowHandler = function(event) {
  var module, callback, result, results = [];
  var self = Drupal.onBeforeUnload;

  // Invoke any previous onBeforeUnload handler and save its result.
  if (typeof self._previousWindowHandler == 'function') {
    result = self._previousWindowHandler(event);
    if (typeof result == 'string') {
      results.push(result);
    }
  }

  // If enabled, invoke all our installed onBeforeUnload callbacks.
  if (self._enabled) {
    for (module in self._callbacks) {
      result = (self._callbacks[module])();
      if (typeof result == 'string') {
        results.push(result);
      }
    }
  }

  // If we got any results, then we'll return them concatenated.
  // This will fire up a confirmation alert to the user that's
  // implemented by the browser itself (it cannot be themed).
  if (results.length > 0) {
    return results.join('\n');
  }
};

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
Drupal.onBeforeUnload.addCallback = function(module, callback) {
  if (typeof module == 'string' && typeof callback == 'function') {
    this._callbacks[module] = callback;
    return true;
  }
  return false;
};

/**
 * Remove an onBeforeUnload callback.
 *
 * @param module
 *   The name of the module.
 * @return
 *   TRUE if the callback was successfully removed, FALSE otherwise.
 */
Drupal.onBeforeUnload.removeCallback = function(module) {
  if (typeof this._callbacks[module] == 'function') {
    delete this._callbacks[module];
    return true;
  }
  return false;
};

/**
 * Check if a callback for a particular module exists.
 *
 * @param module
 *   The name of the module.
 * @return
 *   TRUE if the callback exists, FALSE otherwise.
 */
Drupal.onBeforeUnload.callbackExists = function(module) {
  return (typeof this._callbacks[module] == 'function');
};

/**
 * Disable the onBeforeUnload event handler.
 */
Drupal.onBeforeUnload.disable = function() {
  this._enabled = false;
};

/**
 * Enable the onBeforeUnload event handler.
 */
Drupal.onBeforeUnload.enable = function() {
  this._enabled = true;
};
