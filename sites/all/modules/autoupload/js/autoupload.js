/*
 * Behavior for the automatic file upload
 */

Drupal.behaviors.autoUpload = function (context) {
  $('.filefield-element input.form-submit[value=Upload]', context).hide();
  $('.filefield-element input.form-file', context).change(function() {
    $parent = $(this).parents('.filefield-element');

    //setTimeout to allow for validation
    //would prefer an event, but there isn't one
    setTimeout(function() {
      if(!$('.error', $parent).length) {
        $('input.form-submit[value=Upload]', $parent).mousedown();
      }
    }, 100);
  });
}
