/**
 * Automatic ajax validation
 *
 * @see http://drupal.org/project/ajax
 * @see irc://freenode.net/#drupy
 * @depends Drupal 6
 * @author brendoncrawford
 * @note This file uses a 79 character width limit.
 */

/**
 * Ajax Forms plugin for scroller
 * 
 * @param {String} hook
 * @param {Object} args
 * @return {Bool}
 */
Drupal.Ajax.plugins.scroller = function(hook, args) {
  var scroll_weight, box, found, timer;
  if (hook === 'afterMessage') {
    if (args.options.scroller === true) {
      scroll_weight = 100;
      timer = window.setInterval(function(){
        box = args.local.submitter[0];
        found = false;
        // Watch for thickbox
        while (box.parentNode !== null &&
        Drupal.Ajax.invoke('scrollFind', {
          container: box
        })) {
          box = box.parentNode;
          // Document
          if (box === document) {
            if (box.documentElement.scrollTop &&
            box.documentElement.scrollTop > 0) {
              box.documentElement.scrollTop -= scroll_weight;
              found = true;
            }
          }
          // Body
          else 
            if (box === document.body) {
              if (box.scrollTop &&
              box.scrollTop > 0) {
                box.scrollTop -= scroll_weight;
                found = true;
              }
            }
            // Window
            else 
              if (box === window) {
                if ((window.pageYOffset && window.pageYOffset > 0) ||
                (window.scrollY && window.scrollY > 0)) {
                  window.scrollBy(0, -scroll_weight);
                  found = true;
                }
              }
              // Any other element
              else {
                if (box.scrollTop && box.scrollTop > 0) {
                  box.scrollTop -= scroll_weight;
                  found = true;
                }
              }
        }
        // Check if completed
        if (!found) {
          window.clearInterval(timer);
        }
        return true;
      }, 100);
      return true;
    }
  }
}


