// $Id;

$(document).ready(function() {
  
  // initially hide some configuration options when the default settings should be used
  $('#sticky-notes-admin-visibility-form fieldset').each(function() {
    var fieldset = $(this);
    if ($(this).find('.use-default input[checked]').val() == 1) {
      $(fieldset).find('.fieldset-wrapper').children('.form-item').css('display', 'none');
    }
  });
  
  // attach behaviour for the use-default radio items
  $('.use-default .form-radios input').change(function() {
    if ($(this).val() == 1) {
      $(this).parents('fieldset').find('.fieldset-wrapper').children('.form-item').css('display', 'none');
    }
    else {
      $(this).parents('fieldset').find('.fieldset-wrapper').children('.form-item').css('display', 'block');
    }
  });
  
});