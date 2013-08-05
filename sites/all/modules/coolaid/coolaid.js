
function getViewportHeight() {
  var viewportheight;
  // the more standards compliant browsers (mozilla/netscape/opera/IE7) use window.innerWidth and window.innerHeight
  if (typeof window.innerWidth != 'undefined') {
    viewportheight = window.innerHeight;
  }
  else if (typeof document.documentElement != 'undefined'
   && typeof document.documentElement.clientWidth !=
   'undefined' && document.documentElement.clientWidth != 0) {
    viewportheight = document.documentElement.clientHeight;
  }
  // older versions of IE
  else {
    viewportheight = document.getElementsByTagName('body')[0].clientHeight;
  }
  return viewportheight;
}

Drupal.behaviors.coolaid = function(context) {

  $(function() {
    // Setup
    $('div.coolaid').first().not('coolaid-processed').each(function() {
      var launch = $('<a>').addClass('coolaid-launch').attr('href', '#').attr('title', Drupal.t('Help')).html('(?)');
      if ($('div.coolaid div.coolaid-message', context).length == 0) {
        $(launch).addClass('coolaid-add-message').html('(+)').attr('title', Drupal.t('Add help'));
      }
      $(this).before(launch);

      var coolaid_width = $(this).width();
      var coolaid_height = $(this).height();
      $(this).hide().addClass('coolaid-processed');

      var sup = $('<div>').addClass('coolaid-sup').html($(this).html()).hide();
      $('body', context).append(sup);

      var close = $('<a>').addClass('coolaid-close').attr('href', '#').attr('title', Drupal.t('Close')).html('(X)');
      $('div.coolaid-sup div.coolaid-content', context).prepend(close);

      var overlay = $('<div>').attr('id', 'coolaid-overlay').fadeTo(0, 0).hide();
      $('body', context).append(overlay);

    });

    // Show overlay
    $('a.coolaid-launch', context).click(function() {
      viewportheight = getViewportHeight();
      var max_height = viewportheight * 0.8;
      if (coolaid_height < max_height) {
        max_height = coolaid_height;
      }
      var top_offset = (viewportheight - max_height) / 2;
      $('div.coolaid-sup', context)
        .css('top', top_offset + 'px')
        .css('height', max_height + 'px')
        .css('position', 'absolute')
        .css('width', coolaid_width + 'px')
        .css('left', '50%')
        .css('margin-left', -(coolaid_width / 2));
      $('div#coolaid-overlay', context)
        .css('width', $('html').width())
        .css('height', $('html').height())
        .show()
        .fadeTo('fast', 0.66);
      $('div.coolaid-sup', context).show();
    });

    // Hide overlay
    $('div#coolaid-overlay, a.coolaid-close', context).click(function() {
      $('div.coolaid-sup', context).hide();
      $('div#coolaid-overlay', context).fadeTo('slow', 0).hide();
    });

  });

};