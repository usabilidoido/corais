// $Id$

Drupal.behaviors.hide_submit = function(context) {
  // Hide button and siblings
  function hide_submit_button(obj, message, context) {
    $(obj, context).hide().siblings('input:submit').hide().end().after(message);
  }
  // Disable button and siblings
  function disable_submit_button(obj, context) {
    var $obj = $(obj, context);
    // Workaround for comment-form and node-form
    // inject missing "op" for preview or delete etc.
    $("#edit-hide-submit-fake-op", context)
      .attr("name", "op")
      .attr("value", $obj.attr("value"));
    $obj
      .attr("disabled", "disabled")
      .siblings('input:submit').attr("disabled", "disabled")
      .parents("form").submit();
  }
  var settings = Drupal.settings.hide_submit;
  if (settings.dbg) {
    // For debugging, this addtion to the script will paint included and excluded buttons
    $('input:submit', context).css({border:'6px red solid'});
    $(settings.selector, context).css({border:'6px green solid'});
  }
  // Hide buttons and inject message
  if (settings.mode == 'hide') {
        if (settings.image) {
            $("<img>").attr("src",settings.image);
        }
    $(settings.selector, context).click(function() {
      hide_submit_button(this, settings.message, context);
    })
    // Submit when ENTER is pressed
    if (settings.keypress) {
      $(settings.selector, context).keypress(function() {
        $(this).parents("form").submit();
        hide_submit_button(this, settings.message, context);
      })
    }
  }
  else { // mode == 'disable'
    $(settings.selector, context).click(function() {
      disable_submit_button(this, context);
    });
    // Submit when ENTER is pressed
    if (settings.keypress) {
      $(settings.selector, context).keypress(function() {
        disable_submit_button(this, context);
      });
    }
  }
}