// $Id: purl.admin.js,v 1.1.2.2 2009/11/22 15:03:23 jmiccolis Exp $

if (typeof(Drupal) == "undefined" || !Drupal.purl_admin) {
  Drupal.purl_admin = {};
}

Drupal.purl_admin.attach = function() {
  $('select[id^="edit-purl-"]:not(.purl-processed)').change(function(i) {
    Drupal.purl_admin.alter(this);
  }).each(function(i){
    Drupal.purl_admin.alter(this);
  }).addClass('purl-processed');
}

Drupal.purl_admin.alter = function(elem){
  // Fist, hide anything in the config column of this row.
  $(elem).parents('td').next().find('div.form-item').hide();
  // Then, make items visible that have an id based on the selected value.
  $(elem).parents('td').next().find('div[id^="edit-purl-method-'+elem.value+'"]').show();
}

Drupal.behaviors.purl = function() {
  if ($('form#purl-settings-form').size() > 0) {
    Drupal.purl_admin.attach();
  }
}
