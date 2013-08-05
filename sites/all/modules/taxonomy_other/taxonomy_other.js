// $Id: taxonomy_other.js,v 1.1.2.2 2008/06/18 02:04:31 rellis Exp $
Drupal.behaviors.taxonomy_other = function (context) {
  $(".form-item[id^='taxonomy-other-'] label").remove();
  $(".form-item[id^='edit-taxonomy-'] :input").change(function(){
    var id = '#taxonomy-other-' + $(this).attr('id').substr('edit-taxonomy-'.length) + '-wrapper';
    if ($(this).val() == 'taxonomy_other') {
      $(id).show();
      $(id + ' :input').focus();
    } else {
      $(id + ' :input').val('');
      $(id).hide();
    }
  }).trigger("change");
};
