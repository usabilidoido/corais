/**
 * Automatic ajax validation
 *
 * @see http://drupal.org/project/ajax
 * @see irc://freenode.net/#drupy
 * @depends Drupal 6
 * @author brendoncrawford
 * @note This file uses a 79 character width limit.
 * 
 * @note
 *   When using an Drupal.Ajax form within a Lightbox/Thickbox which is loaded via
 *   AJAX, be sure to call Drupal.attachBehaviors(LightBoxContainer) where
 *   LightBoxContainer is the DOM element containing the Lightbox/Thickbox.
 * 
 * @see http://drupal.org/node/114774#javascript-behaviors
 *
 */

/**
 * Ajax Forms plugin for quicktabs module
 * 
 * @param {String} hook
 * @param {Object} args
 * @return {Bool}
 */
Drupal.Ajax.plugins.quicktabs = function(hook, args) {
  var quicktabs_original;
  if (hook === 'init') {
    if (Drupal.quicktabs) {
      quicktabs_original = Drupal.quicktabs.tab.prototype.success;
      Drupal.quicktabs.tab.prototype.success = function(data){
        this.quicktabs_original = quicktabs_original;
        this.quicktabs_original(data);
        Drupal.attachBehaviors(data.container);
      }
    }
  }
  return true;
}


