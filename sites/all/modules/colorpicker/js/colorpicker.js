// $Id$

Drupal.colorpickers = [];

/**
 * Attaches the colorpicker behavior to all colorpicker textfields
 */
Drupal.behaviors.colorpicker = function(context) {
  $("input.colorpicker_textfield:not(.colorpicker-processed)", context).each(function() {
    var index = $('body').find('input.colorpicker_textfield').index($(this));
    Drupal.colorpickers[index] = new Drupal.colorpicker($(this).addClass('colorpicker-processed'));
    Drupal.colorpickers[index].init();
  });
};

/**
 * Creates an instance of a colorpicker
 */
Drupal.colorpicker = function(input) {
  var ref = {};

  ref.input = input;
  ref.picker_showing = false;
  ref.border;
  ref.button;
  ref.wrapper;
  ref.farb;

  /**
   * 
   */
  ref.init = function() {
    ref.attachPicker();
    ref.addEventHandlers();
  };

  /**
   *
   */
  ref.attachPicker = function() {
    // Add an opener button directly after the input field
    ref.border = $('<div />').addClass('picker_border').appendTo(ref.input.parent().find('.picker_wrapper'));
    ref.button = $('<div />').addClass('picker_button').appendTo(ref.border);

    // Add a wrapper for the Farbtastic colorpicker
    ref.wrapper = $('<div />').addClass('colorwrapper').appendTo(ref.border);

    // create the farbtastic colorpicker
    ref.farb = $.farbtastic(ref.wrapper);
    ref.farb.linkTo(ref.input);

    // Add a close button for the wrapper of the colorpicker
    ref.close_button = $('<div />').addClass('close_button').appendTo(ref.wrapper);
  };


  /**
   * Attach the event handlers
   */
  ref.addEventHandlers = function() {
    ref.button.click(function() {
      ref.picker_showing = true;
      ref.showPicker();
    });

    ref.input.focus(function() {
      if (!ref.picker_showing) {
        ref.picker_showing = true;
        ref.showPicker();
      };
      ref.picker_showing = false;
    }).blur(function() {
      // Hide all colorwrappers
      $('.colorwrapper').hide();
    }).keyup(function() {
      if($(this).val().indexOf('#') !== 0) {
        $(this).val('#' + $(this).val());
      };
    }).keydown(function(e) {
      var key = e.charCode || e.keyCode || -1;
      switch (key) {
        case 27: // ESC
          $(this).blur();
        default:
          return validHexKey(key);
      };
    });

    ref.close_button.click(function() {
      // Hide all colorwrappers
      $('.colorwrapper').hide();
    });
  };

  /**
   * Show the picker
   */
  ref.showPicker = function() {
    // Hide all colorwrappers and show the clicked one
    $('.colorwrapper').hide();

    // Check if we're inside a fieldset
    // If so, make sure the colorpicker isn't partly hidden when the fieldset is too small
    // We'll then place the colorpicker outside of the fieldset
    if (ref.button.parents('fieldset.collapsible').size() > 0) {
      var offset = ref.button.offset();
      ref.wrapper.appendTo('body').css({'top': offset.top + 'px', 'left':(offset.left + 30) + 'px'});
    };
    
    if($(ref.input).val().indexOf('#') !== 0) {
      $(ref.input).val('#' + $(ref.input).val());
    };

    ref.wrapper.show();
    ref.input.focus();
  };

  return ref;
};

/*
 * Checks whether a given key is a valid hex character 
 *
 * @param e Object The keypress event's object
 * @return Boolean true if the key is a valid hex character, false if it isn't
 **/
validHexKey = function(e) {
  var key = (typeof(e) == 'object') ? e.charCode || e.keyCode || -1 : e;
  var valid_keys = [
    48, // 0
    49, // 1
    50, // 2
    51, // 3
    52, // 4
    53, // 5
    54, // 6
    55, // 7
    56, // 8
    57, // 9
    65, // A
    66, // B
    67, // C
    68, // D
    69, // E
    70, // F
    8, // BACKSPACE
    9, // TAB
    16, // SHIFT
    27, // ESC
    37, // ARROW LEFT
    39, // ARROW RIGHT
    224 // COMMAND
  ];
  return ($.inArray(key, valid_keys) > -1);
};