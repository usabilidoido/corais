
$(document).ready(function() {
  
  /**
  * Disable the resize function for the sticky notes textarea
  */
  $("#sticky-notes-form textarea#edit-body").removeClass("resizable");
  $("#sticky-notes-form .resizable-textarea").removeClass("resizable-textarea");
  
  // attach click handler to the radio buttons
  $("#sticky-notes-form #edit-link_scope-wrapper input[type=radio]").click(function() {
    StickyNotes.getVisibilitySummary();
  });
  
  // attach change handler to the public checkbox
  $("#sticky-notes-form #edit-public").change(function() {
    StickyNotes.getVisibilitySummary();
  });
  
});

var StickyNotes = StickyNotes || { positions: [], zIndex : 0 };

/**
* Build a text summary of the visibility options set in the form
*/
StickyNotes.getVisibilitySummary = function() {
  
  // find the current path setting
  var where
  $("#sticky-notes-form #edit-link_scope-wrapper input[type=radio]").each(function() {
    if ($(this).attr('checked')) {
      var value = $(this).attr('value');
      if (value == 'all') {
        where = Drupal.t('all pages');
      }
      else if (value == 'path') {
        where = Drupal.settings.sticky_notes.current_path;
      }
      else if (value == 'pattern') {
        where = Drupal.settings.sticky_notes.current_pattern;
      }
      else if (value == -1) {
        where = Drupal.settings.sticky_notes['current_' + Drupal.settings.sticky_notes.original_scope];
      }
    }
  });
  
  // find the current public setting
  var who = $("#sticky-notes-form #edit-public").attr('checked') ? Drupal.t('everybody') : Drupal.t('me');
  
  // set the new summary
  $('#sticky-notes-visibility-summary-wrapper').html(Drupal.t('Visible on @where for @who', { '@where': where, '@who': who }));
};