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
 * Ajax Forms plugin for thickbox
 * 
 * @param {String} hook
 * @param {Object} args
 * @return {Bool}
 */
Drupal.Ajax.plugins.thickbox = function(hook, args) {
  var tb_init_original;
  if (hook === 'scrollFind') {
    if (args.container.id === 'TB_window') {
      return false;
    }
    else {
      return true;
    }
  }
  else if (hook === 'init') {
    tb_init_original = window.tb_init;
    window.tb_init = function(domChunk){
      tb_init_original(domChunk);
      Drupal.attachBehaviors($('#TB_window'));
    }
  }
  return true;
}


