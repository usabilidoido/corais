// $Id: urlfill.js,v 1.2 2009/09/17 17:37:09 phayes Exp $ 

function urlfillHideUnhide(fieldName) {
  var checkboxId = 'edit-urlfill-onoff-' + fieldName.replace(/_/g,'-');
  if ($('#' + checkboxId).is(':checked')) {
    $('#urlfill_form_div_' + fieldName).show("slow");
  }
  else {
    $('#urlfill_form_div_' + fieldName).css('display','none');
  }
}

Drupal.behaviors.urlfillForm = function (context) {
  // Hide the auto-fill settings if nessisary
  for(i=0; i < Drupal.settings.urlfill_fields.length; i++) {
    urlfillHideUnhide(Drupal.settings.urlfill_fields[i]);
  }
  
};