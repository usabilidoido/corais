
Drupal.behaviors.featured_comments_links = function() {
  $('a.featured-comments-ajax-toggle').click(function() {
    var a = this;
    href = $(this).attr('href');
    $(a).addClass('throbbing');
    $.ajax({
      type: 'POST',
      data: { js: 1 },
      url: href,
      global: true,
      success: function (data) {
        // Parse response
        $(a).removeClass('throbbing');
        data = Drupal.parseJson(data);
        // Change text on success
        if (data.status) {
          // Change label back
          $(a).attr('href', data.href);
          $(a).html(data.label);

          if (data.href.search('unfeature') > -1) {
            $('#featured-comments-admin-form #edit-status-' + data.cid).val('1');
            $(a).removeClass('toggle-add');
            $(a).addClass('toggle-remove');
          }
          else {
            $('#featured-comments-admin-form #edit-status-' + data.cid).val('0');
            $(a).removeClass('toggle-remove');
            $(a).addClass('toggle-add');
          }
          return;
        }
      },
      error: function(data) {
        $(a).removeClass('throbbing');
      }
    });
    return false;
  });
}