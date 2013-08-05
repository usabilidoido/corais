Drupal.behaviors.spaces_dashboard_admin = function(){
  $('#dashboard-form-content table:not(.sticky-header):gt(0)').hide();
  $('ul#dashboard-blocks-links li a:eq(0)').addClass('active');

  $('#dashboard-blocks-links li a').click(function(){
    var elem = $(this);
    if (elem.hasClass('active')) {
      return false;
    }
    $('#dashboard-form-content table:not(.sticky-header)').hide();
    $.each(elem.attr('class').split(' '), function(){
      if (this.match(/dashboard-blocks/)) {
	$('#dashboard-form-content table#'+this).show();
	return false;
      }
    });
    $('ul#dashboard-blocks-links li a').removeClass('active');
    elem.toggleClass('active');
    return false;
  });
}
