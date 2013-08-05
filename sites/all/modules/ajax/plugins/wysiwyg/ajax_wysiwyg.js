/**
 * Ajax wysiwyg Plugin
 *
 * @see http://drupal.org/project/ajax
 * @see irc://freenode.net/#drupy
 * @depends Drupal 6
 * @author brendoncrawford
 * @note This file uses a 79 character width limit.
 *
 */

/**
 * Ajax Forms plugin for wysiwyg API
 * 
 * @param {String} hook
 * @param {Object} args
 * @return {Bool}
 */
Drupal.Ajax.plugins.wysiwyg = function(hook, args) {
  var p, e;
  if (hook === 'submit') {
    if (Drupal.wysiwyg && Drupal.wysiwygDetach) {
      for (w in Drupal.wysiwyg.instances) {
        p = Drupal.wysiwyg.instances[w];
        e = $('#' + p.field);
        Drupal.wysiwygDetach(e[0], p);
      }
    }
  }
  return true;
}
