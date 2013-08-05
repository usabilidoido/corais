
(function ($) {

Drupal.Nodejs.callbacks.nodejsNotify = {
  callback: function (message) {
    $.jGrowl(message.data.body, {header: message.data.subject, life:(Drupal.settings.nodejs_notify.notification_time*1000)});
  }
};

})(jQuery);

