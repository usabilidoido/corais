/**
 * Ajax FCKEditor Plugin
 *
 * @see http://drupal.org/project/ajax
 * @see irc://freenode.net/#drupy
 * @depends Drupal 6
 * @author brendoncrawford
 * @note This file uses a 79 character width limit.
 *
 */

/**
 * Ajax Forms plugin for FCKEditor
 * 
 * @param {String} hook
 * @param {Object} args
 * @return {Bool}
 */
Drupal.Ajax.plugins.FCKEditor = function(hook, args) {
  if (hook === 'submit') {
    if (window.doFCKeditorSave) {
      doFCKeditorSave();
    }
  }
  return true;
}
