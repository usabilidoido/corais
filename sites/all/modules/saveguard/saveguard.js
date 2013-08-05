// $Id: saveguard.js,v 1.1 2007/11/27 20:48:36 starbow Exp $

/*
 *  Called when there is unsaved data on the page
 *    to warn users leaving (or reloading) page that they would lose their changes.
 *  The onbeforeunload event works in FF, IE and Safari, but not in Opera (yet).
 *  The form of the dialog is unmodifiable:
 *  
 *  Are you sure you want to navigate away from this page?
 *  Your message here
 *  Press OK to continue, or Cancel to stay on the current page.
 */
Drupal.markPageUnsaved = function(msg) {
  if (!$('body').is('.has-unsaved-changes')) { // Only do it once.
    $('body').addClass('has-unsaved-changes');
    if (!msg) { // default message
      msg = "If you continue your unsaved changes will be lost.";
    }
    window.onbeforeunload = function() {return msg;};
    $(':submit, input:image').click(function() {
      window.onbeforeunload = null;  // Turn off the warning before submit triggers it.
    });
  }
};

if( Drupal.jsEnabled ) {
  $(document).ready(function(){ 
    msg = Drupal.settings.saveguard.msg;
    $('input, select').change(function() {Drupal.markPageUnsaved(msg)}); // checkboxes, radio buttons and selects
    $('input, textarea').keypress(function() {Drupal.markPageUnsaved(msg)}); // textfields and textareas
  });
}
