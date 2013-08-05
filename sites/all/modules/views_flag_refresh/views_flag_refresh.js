// $Id: views_flag_refresh.js,v 1.8 2010/09/08 03:59:02 cpliakas Exp $

/**
 * Event handler that listens for flags selected by the user.
 */
$(document).bind('flagGlobalAfterLinkUpdate', function(event, data) {
  var refresh = new viewsFlagRefresh(data.flagName);
  $.each(Drupal.settings.views.ajaxViews, function(index, settings) {
    refresh.ajax(settings);
  });
});

/**
 * Constructor for the viewsFlagRefresh object, sets the name of the flag that
 * was selected.
 * 
 * @param flagName
 *   The name of the flag selected by the user.
 */
function viewsFlagRefresh(flagName) {
  this.flagName = flagName;
}

/**
 * Class method that returns the Views module's AJAX path.
 * 
 * @reutrn
 *   The Views module's AJAX path.
 */
viewsFlagRefresh.ajaxPath = function() {
  var ajaxPath = Drupal.settings.views.ajax_path;
  if (ajaxPath.constructor.toString().indexOf("Array") != -1) {
    ajaxPath = ajaxPath[0];
  }
  return ajaxPath;
}

/**
 * Returns the widget settings associated with the view. Returns false if the
 * view isn't set to refresh on this flag.
 * 
 * @param viewName
 *   The name of the view we are running the check against.
 * @param viewDisplayId
 *   The display ID of the view we are running the check against.
 * @return
 *   A string containing the theme hook used to theme the view as it is being
 *   refreshed, a boolean false if the view should not be refreshed at all.
 */
viewsFlagRefresh.prototype.widgetSettings = function(viewName, viewDisplayId) {
  var settings = Drupal.settings.viewsFlagRefresh.flags;
  for (var flagName in settings) {
    functionName = viewName + '-' + viewDisplayId;
    if (flagName == this.flagName && functionName in settings[flagName]) {
      return settings[flagName][functionName];
    }
  }
  return false;
}

/**
 * Returns a key / value pair to be uses as settings by the ajax methods.
 * 
 * @param view
 *   a jQuery object containing the view.
 * @param settings
 *   The View's AJAX settings.
 * @param theme
 *   The theme object.
 * @return
 *   The AJAX settings.
 */
viewsFlagRefresh.ajaxSettings = function(view, settings, theme) {
  return {
    url: viewsFlagRefresh.ajaxPath(),
    type: 'GET',
    data: settings,
    success: function(response) {
      // Invokes theme hook, adds content retrieved from call.
      if (response.__callbacks) {
        $.each(response.__callbacks, function(i, callback) {
          eval(callback)(theme.target, response);
          // @todo Use an alternate callback so we don't have to do this??
          $(view).filter(function() { return !$(this).parents('.view').size(); }).each(function() {
            theme.target = this;
            theme.hookInvoke('themeHookPost');
          });
        });
      }
    },
    error: function() {
      // Invokes theme hook, handles errors gracefully.
      theme.hookInvoke('themeHookPost');
      Drupal.Views.Ajax.handleErrors(xhr, viewsFlagRefresh.ajaxPath());
    },
    dataType: 'json'
  };
}

/**
 * Refreshes a view via AJAX if it is configured to do so.
 * 
 * @param settings
 *   The view's AJAX settings passed through Drupal.settings.views.ajaxViews.
 */
viewsFlagRefresh.prototype.ajax = function(settings) {
  // Bails if the view shouldn't be refreshed when this flag is selected.
  var widgetSettings = this.widgetSettings(settings.view_name, settings.view_display_id);
  if (!widgetSettings) {
    return;
  }

  // Calculates the selector for the view.
  var view = '.view-dom-id-' + settings.view_dom_id;
  if (!$(view).size()) {
    view = '.view-id-' + settings.view_name + '.view-display-id-' + settings.view_display_id;
  }
  
  // Locates the view, AJAX refreshes the content.
  $(view).filter(function() { return !$(this).parents('.view').size(); }).each(function() {
    var target = this;
    
    // Instantiates the theme object, invokes the widget's theme hook.
    var theme = new viewsFlagRefresh.theme(target, widgetSettings);
    theme.hookInvoke('themeHook');
    
    // Gets AJAX settings, either refreshes the view or submits the exposed
    // filter form. This latter refreshes the view and maintains the filters.
    var ajaxSettings = viewsFlagRefresh.ajaxSettings(view, settings, theme);
    var exposedForm = $('form#views-exposed-form-' + settings.view_name.replace(/_/g, '-') + '-' + settings.view_display_id.replace(/_/g, '-'));
    if (exposedForm.size()) {
      setTimeout(function() { $(exposedForm).ajaxSubmit(ajaxSettings); }, theme.timeout);
    }
    else {
      setTimeout(function() { $.ajax(ajaxSettings); }, theme.timeout);
    }
  });
}

/**
 * Contructor for our pseudo theme system class.
 * 
 * @param target
 *   A jQuery object containing the content being refreshed.
 * @param settings
 *   The widget settings.
 */
viewsFlagRefresh.theme = function(target, settings) {
  this.timeout  = 0;
  this.target   = target;
  this.settings = settings;
}

/**
 * Invokes a hook in the theme object.
 * 
 * @param hookType
 *   The hook type being invoked, i.e. 'themeHook', 'themeHookPost'.
 * @return
 *   A boolean flagging whether the hook exists.
 */
viewsFlagRefresh.theme.prototype.hookInvoke = function(hookType) {
  if (hookType in this.settings && this.settings[hookType] in this) {
    this[this.settings[hookType]]();
    return true;
  }
  return false;
}

/**
 * Adds a throbber image to the view content while it is being refreshed.
 */
viewsFlagRefresh.theme.prototype.throbber = function() {
  // Hide the content of the view.
  $(this.target).css('visibility', 'hidden');
  
  // Captures parent, as the view is usually in something such as a block.
  var container = $(this.target).parent();
  
  // Adds our throbber to the middle of the view.
  // NOTE: The throbber image is 32px wide.
  var pos = $(container).position();
  this.throbberElement = $('<img src="' + Drupal.settings.viewsFlagRefresh.imagePath + '/throbber.gif" class="views_flag_refresh-throbber" />')
    .css('left', pos.left + ($(container).outerWidth() / 2) - 16)
    .css('top', pos.top + ($(container).outerHeight() / 2) - 16)
    .insertAfter(this.target);
}

/**
 * Cleans up the throbber image.
 * 
 * @param target
 *   A jQuery object containing the view being refreshed.
 */
viewsFlagRefresh.theme.prototype.throbberPost = function() {
  $(this.throbberElement).remove();
}
