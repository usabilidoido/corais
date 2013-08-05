/**
 * @file
 *   Taskbar JavaScript tools
 */
(function ($) {
Drupal.taskbar = Drupal.taskbar || {timers:{}};
Drupal.settings.taskbar = Drupal.settings.taskbar || {};

Drupal.behaviors.taskbar = function (context) {
  if (Drupal.settings.taskbar.suppress) {
    // Sometimes taskbar_suppress(TRUE) is called too late.
    return;
  }

  if (context == document) {
    $('body').addClass('with-taskbar');
    if (Drupal.settings.taskbar.update) {
      if (Drupal.settings.taskbar.update_method == 'ajax') {
        setInterval('Drupal.taskbar.refresh()', Drupal.settings.taskbar.delay);
      }
      else {
        Drupal.taskbar.polling();
      }
    }
  }

  $('.taskbar-item').each(function() {
    var T = $(this);
    if (T.hasClass('taskbar-item-options-ajax')) {
      if (T.find('.taskbar-item-extra').length == 0) {
        T.append('<div class="taskbar-item-extra"></div>');
      }
      T.find('.taskbar-item-content:not(.taskbar-processed)').addClass('taskbar-processed').click(function() {
        T.toggleClass('taskbar-item-active');
        Drupal.taskbar.closeAll(T);
        var name = T.attr('id').replace(/^taskbar-item-/, '');
        if (T.find('.taskbar-item-extra').is(':visible')) {
          // If this item is auto refresh, cancel it then it is closed
          if (Drupal.settings.taskbar.ajax_refresh[name]) {
            if (Drupal.settings.taskbar.update_method == 'ajax') {
              clearInterval(Drupal.taskbar.timers[name]);
            }
            else {
              Drupal.taskbar.timers[name].abort();
            }
          }
          T.find('.taskbar-item-extra').slideToggle('fast');
        }
        else {
          T.find('.taskbar-item-extra').html('<div class="taskbar-item-loading"></div>');
          T.find('.taskbar-item-extra').slideToggle('fast');
          Drupal.taskbar.showItem(name);
          // If this item is auto refresh, set a refresh period for its extra content
          if (Drupal.settings.taskbar.ajax_refresh[name]) {
            if (Drupal.settings.taskbar.update_method == 'ajax') {
              Drupal.taskbar.timers[name] = setInterval(function() {Drupal.taskbar.showItem(name);}, Drupal.settings.taskbar.ajax_refresh[name]);
            }
            else {
              Drupal.taskbar.pollingItem(name);
            }
          }
        }
      });
    }
    else if (T.find('.taskbar-item-extra').length > 0) {
      T.find('.taskbar-item-content:not(.taskbar-processed)').addClass('taskbar-processed').click(function() {
        T.toggleClass('taskbar-item-active').find('.taskbar-item-extra').slideToggle('fast');
        Drupal.taskbar.closeAll(T);
      });
    }
    T.find('.taskbar-item-titlebar:not(.taskbar-processed)').addClass('taskbar-processed').click(function() {
      T.find('.taskbar-item-content').click();
    });
  });
}

Drupal.taskbar.closeAll = function(item) {
  if (Drupal.settings.taskbar.auto_close) {
    $('#taskbar .taskbar-item').not('#' + item.attr('id')).removeClass('taskbar-item-active').find('.taskbar-item-extra').hide();
  }
}

Drupal.taskbar.polling = function() {
  $.ajax({
    url: Drupal.settings.basePath + 'taskbar/polling/status',
    dataType: 'json',
    error: function () {
      //don't flood the servers on error, wait 10 seconds before retrying
      setTimeout(Drupal.taskbar.polling, 10000);
    },
    success: function (data) {
      Drupal.taskbar.AJAXrespond(data);
      Drupal.taskbar.polling();
    }
  });
}

Drupal.taskbar.refresh = function() {
  $.ajax({
    url: Drupal.settings.basePath + 'taskbar/status',
    dataType: 'json',
    success: Drupal.taskbar.AJAXrespond
  });
}

Drupal.taskbar.showItem = function(name) {
  $.ajax({
    url: Drupal.settings.basePath + 'taskbar/display/' + name,
    dataType: 'json',
    success: Drupal.taskbar.AJAXrespond
  });
}

Drupal.taskbar.pollingItem = function(name) {
  Drupal.taskbar.timers[name] = $.ajax({
    url: Drupal.settings.basePath + 'taskbar/polling/display/' + name,
    dataType: 'json',
    error: function () {
      //don't flood the servers on error, wait 10 seconds before retrying
      setTimeout(Drupal.taskbar.pollingItem, 10000);
    },
    success: function (data) {
      Drupal.taskbar.AJAXrespond(data);
      Drupal.taskbar.pollingItem(name);
    }
  });
}

// This is a subset of Ctools Ajax Responder commands
Drupal.taskbar.AJAXrespond = function(data) {
  $.each(data, function(i, item) {
    if (item.command == 'html') {
      $(item.selector).html(item.data);
    }
    else if (item.command == 'replace') {
      $(item.selector).replaceWith(item.data);
    }
    Drupal.attachBehaviors($(item.selector));
  });
}
})(jQuery);

