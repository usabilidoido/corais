// $Id: onbeforeunload_example.js,v 1.1.2.3 2009/03/28 12:36:39 markuspetrux Exp $

/**
 * Install the Drupal behavior.
 *
 * This function will be called by Drupal.attachBehaviors() in misc/drupal.js.
 */
Drupal.behaviors.onBeforeUnloadExample = function(context) {
  if (!Drupal.onBeforeUnload.callbackExists('onbeforeunload_example')) {
    Drupal.onBeforeUnload.addCallback('onbeforeunload_example', Drupal.onBeforeUnloadExample);
  }
};

/**
 * onBeforeUnload Example callback.
 *
 * This function will be called by onBeforeUnload API when the user leaves the
 * page.
 *
 * The string returned here will be prompted to the user, so do NOT return
 * anything if you do not need to.
 */
Drupal.onBeforeUnloadExample = function() {
  if (Drupal.settings.onBeforeUnloadExample.showWarning) {
    return 'Hello world!\n\nThis warning has been provided by the onBeforeUnload Example module.';
  }
};
