
Drupal.behaviors.reference_preview_hide_unhide = function(context) {
  var checkboxId = 'edit-reference-preview-enable';
  if ($('#' + checkboxId).is(':checked')) {
    $('#reference_preview_form_div').show("slow");
  }
  else {
    $('#reference_preview_form_div').css('display','none');
  }
}