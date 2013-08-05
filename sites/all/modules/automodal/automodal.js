// $ID$

(function ($, Drupal, window, undefined) {

  /*
   * Add the automodal settings to the selectors.
   */
  Drupal.behaviors.automodal = function (context) {
    $.each(Drupal.settings.automodal, function(selector, settings) {
      $(selector +':not(.automodal-processed)', context)
        .addClass('automodal-processed')
        .bind('click', function() {
          settings.url = $(this).attr('href') || '#';
          if (settings.url.indexOf('?') >= 0) {
            settings.url += '&'
          }
          else {
            settings.url += '?'
          }
          settings.url += 'automodal=true';

          // Allow others to alter the settings as needed.
          settings = Drupal.automodal.settingsAlterCall(settings);

          settings.onSubmit = Drupal.automodal.onSubmitCall;
          Drupal.modalFrame.open(settings);
          return false;
        });
    });
  }

  Drupal.automodal = Drupal.automodal || {};

  /**
   * Functions under Drupal.automodal.onSubmitCall are iterated over onSubmit
   * for the modalFrames when the window is set to close.
   */
  Drupal.automodal.onSubmitCallback = {
    automodal: function(args, statusMessages) {
      if (args.redirect) {
        window.location = decodeURIComponent(args.redirect);
      }

      if (args.reload) {
        window.location.reload();
      }
    }
  }

  /**
   * Functions under Drupal.automodal.settingsAlter are iterated over when a
   * Modal Frame is created allowing the alteration of settings.
   */
  Drupal.automodal.settingsAlter = {
    automodal: function(settings) {
      if (settings.automodalClose) {
        settings.url += '&automodalClose=true';
      }

      if (settings.automodalReload) {
        settings.url += '&automodalReload=true';
      }

      if (settings.automodalRedirect != undefined) {
        settings.url += '&automodalRedirect=' + encodeURIComponent(settings.automodalRedirect);
      }
      return settings;
    }
  }

  /**
   * Allow the settings to be altered for Modal Frame.
   *
   * When a Modal Frame instance is created allow the the settings to be altered
   * by others. Other scripts can alter the settings by adding functions under
   * Drupal.automodal.settingsAlter. There is a default implementation at
   * Drupal.automodal.settingsAlter.automodal.
   */
  Drupal.automodal.settingsAlterCall = function(settings) {
    var newSettings = settings;
    for (var i in Drupal.automodal.settingsAlter) {
      newSettings = Drupal.automodal.settingsAlter[i](newSettings);
    }
    return newSettings;
  }

  /**
   * Iterate over multiple onSubmit functions when Modal Frame calls the onSubmit
   * function. There is a default implementation at
   * Drupal.automodal.onSubmitCallback.automodal. Other scripts can add their
   * own callbacks as functions under Drupal.automodal.onSubmitCallback (e.g.,
   * Drupal.automodal.onSubmitCallback.foo).
   */
  Drupal.automodal.onSubmitCall = function(args, statusMessages) {
    for (var i in Drupal.automodal.onSubmitCallback) {
      Drupal.automodal.onSubmitCallback[i](args, statusMessages);
    }
  }
})(jQuery, Drupal, window);