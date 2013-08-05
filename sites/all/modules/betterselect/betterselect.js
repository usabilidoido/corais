
Drupal.behaviors.initBetterSelect = function(context) {
  $('.better-select .form-checkboxes input[type="checkbox"]').click(function(){
    this.checked ? $(this).parent().parent().addClass('hilight') : $(this).parent().parent().removeClass('hilight');
  }).filter(":checked").parent().parent().addClass('hilight');
}

