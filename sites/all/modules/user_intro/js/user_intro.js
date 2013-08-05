Drupal.behaviors.user_intro = function(context) {
  // prevent defaults
    $('.prevent-default').each(function(i, el){
      $(el).click(function(e){
        e.preventDefault();
      });
    });

  // setup hiding intro
    $('#hide-user-intro-trigger').click(function(e) {
      $('#user-intro').fadeOut();
      $('#user-intro-show').fadeIn();
    });

  // setup disabling intro
    $('#user-intro-disable-form #edit-disabled').change(function(e) {
      $.get(Drupal.settings.basePath + 'user_intro/disable', function(data){
        $('#user-intro').fadeOut();       
      });
    });

  // setup showing intro
    $('#show-user-intro-trigger').click(function(e) {
      $('#user-intro-show').fadeOut();
      $('#user-intro').fadeIn();
    });
}