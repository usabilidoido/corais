
/**
* The sticky notes namespace
*/
var StickyNotes = StickyNotes || { positions: [], zIndex : 0 };

$(window).ready(function() {
  StickyNotes.startup();
});

/**
* Startup function, set up sticky notes and attach callbacks
*/
StickyNotes.startup = function() {
  
  // set up some attributes
  var pattern = Drupal.settings.sticky_notes.current_pattern;
  var path = Drupal.settings.sticky_notes.current_path;
  var title = Drupal.settings.sticky_notes.origin_title;
  
  StickyNotes.query = (Drupal.settings.sticky_notes.clean_urls_enabled ? '?' : '&') + 'pattern=' + pattern + '&path=' + path + '&title=' + title;
  StickyNotes.container = $(Drupal.settings.sticky_notes.container_selector);
  StickyNotes.items = [];
  StickyNotes.positions = [];
  StickyNotes.elements_selector = 'div.sticky-notes-note-item-wrapper';
  StickyNotes.zIndex = 0;
  StickyNotes.hide_on_page_load = Drupal.settings.sticky_notes.hide_on_page_load
  StickyNotes.toggle_visibility_state = Drupal.settings.sticky_notes.toggle_visibility_state;
  StickyNotes.visibility_state_memory = Drupal.settings.sticky_notes.visibility_state_memory;
  StickyNotes.hidden = StickyNotes.hide_on_page_load && StickyNotes.toggle_visibility_state;
  StickyNotes.base_url = Drupal.settings.sticky_notes.base_url;
  StickyNotes.expose = false;
  StickyNotes.autoSize = Drupal.settings.sticky_notes.auto_size;
  StickyNotes.settings = {
    width: Drupal.settings.sticky_notes.note_width,
    height: Drupal.settings.sticky_notes.note_height,
    fontBody: 12,
    fontAuthor: 10,
    lineHeight: 16
  };
  StickyNotes.urls = Drupal.settings.sticky_notes.urls;
  
  // integrate the info box
  StickyNotes.loadInfoBox();
  
  // the sticky notes container
  StickyNotes.container.prepend(Drupal.theme('sticky_notes_wrapper'));
  StickyNotes.wrapper = $('div#sticky-notes-wrapper');
  // set the z-index so that fading works smoothly
  StickyNotes.wrapper.css('z-index', Drupal.settings.sticky_notes.minimal_z_index);
  
  // react as soon as the info box is loaded
  $(document).bind('sticky_notes_info_box_loaded', function() {
        
    // initial loading of all sticky notes for this page
    StickyNotes.loadNotes();
    
    // attach behaviours to the info box
    StickyNotes.attachInfoBoxBehaviours();
    
  });
  
  // react as soon as the notes have been loaded
  $(document).bind('sticky_notes_loaded', function() {
    StickyNotes.init();
    StickyNotes.updateOptionsVisibility();
  });
  
  $(window).resize(function() {
    StickyNotes.windowResizeHandler();
  });
  
  // mark this page as processed
  $('body').addClass('sticky-notes-enabled');
  
};

/**
* Load the sticky notes info box for this page
*/
StickyNotes.loadInfoBox = function() {
  $.ajaxSetup ({ cache: false});
  $.getJSON(StickyNotes.urls.info_box + StickyNotes.query, function(data) {
    
    if (data) {
      
      $('body').append(Drupal.theme('sticky_notes_info_box'));
      var wrapper = $('#sticky-notes-info-box-wrapper');
      wrapper.replaceWith(data);
      
      $(document).trigger('sticky_notes_info_box_loaded');
    	
    }
    
  });
};

/**
* Load the notes for this page
*/
StickyNotes.loadNotes = function() {
  $.ajaxSetup ({ cache: false});
  $.getJSON(StickyNotes.urls.load + StickyNotes.query, function(data) {
    
    StickyNotes.wrapper.hide();
    StickyNotes.items = $(data).html();
    StickyNotes.wrapper.html(StickyNotes.items);
    // check if the notes should be hidden on initial page load
    StickyNotes.setVisibility(StickyNotes.toggle_visibility_state ? (StickyNotes.readCookie('sticky_notes_visibility') == 'visible' && !StickyNotes.hide_on_page_load) : false);
    $(document).trigger('sticky_notes_loaded');
  });
};

/**
* Update the notes for this page
*/
StickyNotes.updateNotes = function() {
  $.ajaxSetup ({ cache: false});
  $.getJSON(StickyNotes.urls.load + StickyNotes.query, function(data) {
    StickyNotes.items = $(data).html();
    StickyNotes.wrapper.html(StickyNotes.items);
    if (StickyNotes.readCookie('sticky_notes_visibility') == 'visible') {
      StickyNotes.setVisibility(true);
    }
    $(document).trigger('sticky_notes_loaded');
  });
};

StickyNotes.updatePositionsArray = function(nid, element) {
  StickyNotes.positions[nid] = {
    left: $(element).css('left').replace('px', ''),
    top: $(element).css('top').replace('px', ''),
    width: $(element).css('width').replace('px', ''),
    height: $(element).css('height').replace('px', ''),
    font_size: $(element).css('fontSize').replace('px', '')
  };
}

/**
* Init the sticky notes, attach behaviours, update page counter ...
*/
StickyNotes.init = function() {

  StickyNotes.zIndex = Drupal.settings.sticky_notes.minimal_z_index;

  $(StickyNotes.elements_selector).each(function() { 
    // save the initial element positions
    var nid = parseInt($(this).find('span.sticky-note-nid').html(), 10);
    StickyNotes.updatePositionsArray(nid, $(this));
      
    // find the one with the highest z-index
    if ($(this).css('z-index') > StickyNotes.zIndex) {
      StickyNotes.zIndex = $(this).css('z-index');
    }
  });

  // make sure that a note the user clicks on will pop up immediately
  $(StickyNotes.elements_selector).click(function() {
    if (StickyNotes.zIndex > $(this).css('z-index')) {
      $(this).css('z-index', ++StickyNotes.zIndex);
    }
    if ($(this).hasClass('ui-draggable')) {
      var nid = parseInt($(this).find('span.sticky-note-nid').html(), 10);
      StickyNotes.savePosition(
        nid,
        $(this).css('left').replace('px', ''),
        $(this).css('top').replace('px', ''),
        $(this).css('width').replace('px', ''),
        $(this).css('height').replace('px', '')
      );
    }
  });
  
  // make them resizable
  if (Drupal.settings.sticky_notes.resizable) {
    $(StickyNotes.elements_selector).each(function() {
      $(this).resizable({
        containment: 'document',
        alsoResize:  '#' + $(this).attr('id') + ' .sticky-notes-note-item',
        stop: function(event, ui) {
        
          // get the nid of this note
    		  var nid = parseInt(ui.helper.find('span.sticky-note-nid').html(), 10);

    	    // Save the positon of the note
    		  StickyNotes.saveSize(nid, $('#sticky-note-' + nid).css('width'), $('#sticky-note-' + nid).css('height'));
  		  
        }
      });
    });
  }
  
  // make them draggable
	StickyNotes.makeDraggable(StickyNotes.elements_selector);

	// attach destination string to all actions, so that we have a context for
	// the node form
	$('.sticky-notes-note-item-actions a').each(function() {
	  $(this).attr('href', $(this).attr('href') + StickyNotes.query);
	});

  // attach the modalframe behaviour to the actions buttons
  StickyNotes.attachModalFrameBehaviours('div.sticky-notes-note-item-actions a');
  
  // update the notes page count
  StickyNotes.updateNotesCount();
  
};

/**
* Retrieve the overlay. If none exists yet it will be created
*/
StickyNotes.getOverlay = function(reset) {
  
  var overlay = $('#sticky-notes-overlay');

  // define the overlay, inpired by Popups API
  if ($(overlay).length == 0) {
    
    // apend the overlay to the container element, normally I would like to add
    // the overlay to the body, but in order to have the sticky notes not
    // covered by the overlay it must be in the same conatiner as the notes
    $(StickyNotes.container).append(Drupal.theme('sticky_notes_overlay'));
    overlay = $('#sticky-notes-overlay');
    
    // compensate positioning of the container element
    $(overlay).css('left', -$(StickyNotes.container).offset().left);
    $(overlay).css('top', -$(StickyNotes.container).offset().top);
    
    // set opacity and z-index
    $(overlay).css('opacity', '0.8');
    $(overlay).css('z-index', Drupal.settings.sticky_notes.minimal_z_index - 1);
    // Doing absolute positioning, so make overlay's size equal the entire body.
    var $doc = $(document);
    $(overlay).width($doc.width()).height($doc.height());
  }

  return $(overlay);
};

/**
* Resize the overlay, so that it always covers the whole document
*/
StickyNotes.resizeOverlay = function() {  
  var $doc = $(document);
  StickyNotes.getOverlay().width($doc.width()).height($doc.height())
};

/**
* Update the visibility of the options links in the info box, so that they are
* hidden of there are no notes on the current page
*/
StickyNotes.updateOptionsVisibility = function() {
  // show the additional options if there are any notes on the page
  if ($(StickyNotes.elements_selector).length > 0) {
    $('#sticky-notes-options').show();
  } else {
    $('#sticky-notes-options').hide();
  }
}

/**
* Show all sticky notes of this page as a grid. Kind of exposé function.
*/
StickyNotes.showGrid = function() {
  
  StickyNotes.expose = true;
  
  // if the notes are currently hidden make sure they will appear and that
  // they will disappear once expose view is left
  var hidden = StickyNotes.hidden;
  if (StickyNotes.hidden) {
    StickyNotes.setVisibility(true);
  }
  
  // show the overlay
  StickyNotes.getOverlay().show().click(function() {
    $(this).hide();
    StickyNotes.expose = false;
    StickyNotes.showPositioned(StickyNotes.elements_selector);
    if (hidden) {
      StickyNotes.setVisibility(false);
    }
    $(this).unbind('click');
  });
  
  // save the original positions
  $(StickyNotes.elements_selector).each(function() {
    // get the current nid
    var nid = parseInt($(this).find('span.sticky-note-nid').html(), 10);
    // update the positions array
    StickyNotes.updatePositionsArray(nid, $(this));
  });
  
  StickyNotes.positionGrid('slow');
  
  // disable the view links in the info box
  StickyNotes.disableInfoBoxOptions();
  
  // disable dragging
  $(StickyNotes.elements_selector).draggable('destroy');
};

/**
* Do the real grid positioning. This function might be called by the resize handler.
*/
StickyNotes.positionGrid = function(animation_speed) {
  
  if (typeof animation_speed == 'undefined') {
    animation_speed = 1; // do it really fast
  }
  
  var window_width = $(window).width();
  var window_height = $(window).height();
  var vertical_offset = $(window).scrollTop() - $('#sticky-notes-wrapper').offset().top;
  var horizontal_offset = 0 - $('#sticky-notes-wrapper').offset().left;
  var relative_offset = 0.2;
  var cols = 0;
  var rows = 0;
  var left, top;
  var note_width = parseInt(StickyNotes.settings.width);
  var note_height = parseInt(StickyNotes.settings.height);
  var font_size_body = parseInt(StickyNotes.settings.fontBody);
  var font_size_author = parseInt(StickyNotes.settings.fontAuthor);
  var line_height = parseInt(StickyNotes.settings.lineHeight);
  
  if (StickyNotes.autoSize) {
  
    // how many notes do we have?
    var note_count = $(StickyNotes.elements_selector).length;
  
    // calculate the necessary reduction to display all notes in the current
    // view port
    var reduction = 100 / (note_width * note_height) * (Math.sqrt((window_width * window_height) / note_count));
  
    // make sure that the notes will not be magnified
    if (reduction > 1) {
      reduction = 1;
    }
    // apply the reduction
    note_width = note_width * reduction;
    note_height = note_height * reduction;
    font_size_body = StickyNotes.settings.fontBody * reduction;
    font_size_author = StickyNotes.settings.fontAuthor * reduction;
    line_height = StickyNotes.settings.lineHeight * reduction;
  
    // calculate the offset that the notes should have from each other
    var offset = note_width * relative_offset;
  
    var max_cols = Math.floor(window_width / (note_width + offset) - horizontal_offset);
    var horizontal_margin = (window_width - ((max_cols - 1) * (note_width + offset)) - note_width);
  
    var max_rows = Math.floor(window_height / (note_height + offset) - vertical_offset);
    var vertical_margin = (window_height - ((max_rows - 1) * (note_height + offset)) - note_height);
  
    max_cols = 0;
    max_rows = 1;
    // hm, this is ugly, but I can't find a reliable method to calculate in
    // advance how many rows and columns we will get, this is because of the
    // variable top and left margins of the virtual container area
    // so we just simualte a first positioning to get those infos
    $(StickyNotes.elements_selector).each(function() {
    
      // calculate the target coordinates
      if (cols * (note_width + offset) + offset + horizontal_margin / 2 >= (window_width - note_width)) {
        max_rows++;
        max_cols = cols;
        cols = 0;
      }
      cols++;
    });
  
    var horizontal_margin = (window_width - ((max_cols - 1) * (note_width + offset)) - note_width);
    var vertical_margin = (window_height - ((max_rows - 1) * (note_height + offset)) - note_height);
  
    rows = 0;
    cols = 0;
  }
  else {
    offset = 50;
    vertical_margin = offset;
    horizontal_margin = offset;
  }
  
  // now we can move each element to its new positions
  $(StickyNotes.elements_selector).each(function() {
    
    // calculate the target coordinates
    if (cols * (note_width + offset) + offset + horizontal_margin / 2 < (window_width - note_width)) {
      left = cols * (note_width + offset) + horizontal_offset;
    } else {
      rows++;
      cols = 0;
      left = cols * (note_width + offset) + horizontal_offset;
    }
    
    left = left + horizontal_margin / 2;
    top = vertical_offset + rows * (note_height + offset) + vertical_margin / 2;
    
    // and trigger the aninmation
    $(this).animate({
      left: left,
      top: top,
      width: note_width,
      height: note_height
    }, animation_speed).find('.sticky-notes-note-item').animate({
      width: note_width,
      height: note_height
    }, animation_speed).find('.sticky-notes-note-item-body').animate({
      fontSize: font_size_body,
      lineHeight: line_height
    }, animation_speed);
    $(this).find('.sticky-notes-note-item-author').animate({
      fontSize: font_size_author
    }, animation_speed);
    
    cols++;  
    
  });
  
};

/**
* Move the sticky notes to there positions, as set by the user
* Also make sure they have the right size, since this function get's also
* called when leaving exposé mode.
*/
StickyNotes.showPositioned = function() {
  $(StickyNotes.elements_selector).each(function() {
    var nid = parseInt($(this).find('span.sticky-note-nid').html(), 10);
    $(this).animate({
      left: StickyNotes.positions[nid].left,
      top: StickyNotes.positions[nid].top,
      width: StickyNotes.positions[nid].width,
      height: StickyNotes.positions[nid].height
    }, "slow").find('.sticky-notes-note-item').animate({
        width: StickyNotes.positions[nid].width - 4,    // those 4 pixels is the
        height: StickyNotes.positions[nid].height - 4   // border between wrapper and item
      }, "slow");
    $(this).find('.sticky-notes-note-item-body').animate({fontSize: StickyNotes.settings.fontBody, lineHeight: StickyNotes.settings.lineHeight}, 'slow');
    $(this).find('.sticky-notes-note-item-author').animate({fontSize: StickyNotes.settings.fontAuthor});
  });
  StickyNotes.getOverlay().hide();
  StickyNotes.makeDraggable(StickyNotes.elements_selector);
  StickyNotes.enableInfoBoxOptions();
};

/**
* React on window resizing
*/
StickyNotes.windowResizeHandler = function() {
  // only do this when needed, as to say in expose view
  if (StickyNotes.autoSize && StickyNotes.expose) {
    StickyNotes.resizeOverlay();
    StickyNotes.positionGrid();
  }
};

/**
* Small wrapper function to set the visibility of the notes
*/
StickyNotes.setVisibility = function(visible) {
  visible = StickyNotes.toggle_visibility_state ? visible : true;
  if (visible) {
    StickyNotes.showAll();
  } else {
    StickyNotes.hideAll();
  }
}

/**
* Show all sticky notes
*/
StickyNotes.showAll = function() {
  $(StickyNotes.wrapper).fadeIn();
  $('#sticky-notes-options-display-normal').hide();
  $('#sticky-notes-options-display-hidden').show();
  StickyNotes.hidden = false;
  StickyNotes.createCookie('sticky_notes_visibility', 'visible', StickyNotes.visibility_state_memory);
};

/**
* Hide all sticky notes
*/
StickyNotes.hideAll = function() {
  $(StickyNotes.wrapper).hide();
  $('#sticky-notes-options-display-normal').show();
  $('#sticky-notes-options-display-hidden').hide();
  StickyNotes.hidden = true;
  StickyNotes.createCookie('sticky_notes_visibility', 'hidden', StickyNotes.visibility_state_memory);
};

/**
* Disable the info box options links
*/
StickyNotes.disableInfoBoxOptions = function() {
  $('#sticky-notes-options a').unbind('click').addClass('disabled');
};

/**
* Enable the info box options links
*/
StickyNotes.enableInfoBoxOptions = function() {
  // show all sticky notes in a grid to give overview
  $('#sticky-notes-options-display-expose').click(function() {
    if ($(StickyNotes.elements_selector).length > 0) {
      StickyNotes.showGrid(StickyNotes.elements_selector);
    }
    return false;
  });

  // hide all notes on the current page when the link is clicked
  $('#sticky-notes-options-display-hidden').click(function() {
    StickyNotes.setVisibility(false);
    return false;
  });

  // show all notes on the current page when the link is clicked
  $('#sticky-notes-options-display-normal').click(function() {
    StickyNotes.setVisibility(true);
    return false;
  });
  
  $('#sticky-notes-options a').removeClass('disabled');
};

/**
* Make all sticky notes draggable
*/
StickyNotes.makeDraggable = function() {

  // make all notes draggables
	$(StickyNotes.elements_selector).draggable({
		containment:'document',
		start:function(e,ui){ if (StickyNotes.zIndex > ui.helper.css('z-index')) {ui.helper.css('z-index', ++StickyNotes.zIndex) }},
		stop:function(e,ui){
	  
		  // get the nid of this note
		  var nid = parseInt(ui.helper.find('span.sticky-note-nid').html(), 10);
      
	    // Save the positon of the note
		  StickyNotes.savePosition(nid, ui.position.left, ui.position.top);
	  
		}
	});
};

/**
* Update the current pages note counter
*/
StickyNotes.updateNotesCount = function() {
  $('#sticky-notes-page-count').html(Drupal.formatPlural($(StickyNotes.elements_selector).length, '1 note', '@count notes'));
};

/**
* Save the given positions for the given node nid, the handling of the
* z-index happens on the server side, in assumption that the given node must
* be the one on the top
*/
StickyNotes.savePosition = function(nid, x, y) {
  if ($('#sticky-note-' + nid).hasClass('editable')) {
    StickyNotes.positions[nid]['left'] = x;
    StickyNotes.positions[nid]['top'] = y;
    $.get(StickyNotes.urls.save_position + '/' + nid + '/' + x + '/' + y);
  }
};

/**
* Save the given positions for the given node nid, the handling of the
* z-index happens on the server side, in assumption that the given node must
* be the one on the top
*/
StickyNotes.saveSize = function(nid, width, height) {
  if ($('#sticky-note-' + nid).hasClass('editable')) {
    StickyNotes.positions[nid]['width'] = width;
    StickyNotes.positions[nid]['height'] = height;
    $.get(StickyNotes.urls.save_size + '/' + nid + '/' + width + '/' + height);
  }
};

/**
* Attach the modalframe bahviour to the elements matching the given selector
*/
StickyNotes.attachModalFrameBehaviours = function(selector, show_notes_after_submit) {

  $(selector + ':not(.modalframe-sticky-notes-processed)').addClass('modalframe-sticky-notes-processed').click(function() {
    var element = this;
  
    // Hide the messages before opening a new dialog.
    $('.modalframe-sticky-notes-messages').hide('fast');
  
    // Build modal frame options.
    var modalOptions = {
      url: $(element).attr('href'),
      autoFit: true,
      width: 450,
      height: 212,
      onSubmit: function() {
        StickyNotes.updateNotes();
        if (show_notes_after_submit) {
          StickyNotes.setVisibility(true);
        }
      }
    };
  
    // make sure all notes are displayed normally, if we are in exposé view
    // this will return to normal view
    StickyNotes.showPositioned(StickyNotes.elements_selector);
  
    // Open the modal frame dialog.
    Drupal.modalFrame.open(modalOptions);
    
    // Prevent default action of the link click event.
    return false;
  });
};

/**
* Attach behaviours and handlers to the info box
*/
StickyNotes.attachInfoBoxBehaviours = function() {
  
  // set up modalframe behaviours
  StickyNotes.attachModalFrameBehaviours('#sticky-notes-add-note-button a', true);
  
  // implement info box display on hover
  $('div#sticky-notes-info-box-wrapper .hide-info-box').bind('mouseenter', function() {
    $('#sticky-notes-info-box .content').fadeIn('fast');
  }).bind('mouseleave', function() {
    $('#sticky-notes-info-box .content').fadeOut('fast');
  });

  StickyNotes.enableInfoBoxOptions();
  StickyNotes.setVisibility(!((StickyNotes.readCookie('sticky_notes_visibility') == 'hidden') || Drupal.settings.sticky_notes.hide_on_page_load));
  
  // attach destination string to links to node/add/sticky-notes
	$('a[href$=sticky-notes/add]').each(function() {
	  $(this).attr('href', $(this).attr('href') + StickyNotes.query);
	});
	
	StickyNotes.updateOptionsVisibility();
  
};

/**
 * The sticky notes themes.
 */
Drupal.theme.prototype.sticky_notes_wrapper = function() {
    return '<div id="sticky-notes-wrapper" />';
};

Drupal.theme.prototype.sticky_notes_overlay = function() {
    return '<div id="sticky-notes-overlay" class="ui-widget-overlay" />';
};

Drupal.theme.prototype.sticky_notes_info_box = function() {
  return '<div id="sticky-notes-info-box-wrapper" />';
};

/**
* Cookie handling functions
*
* Taken from the examples at http://www.quirksmode.org/js/cookies.html
*
*/
StickyNotes.createCookie = function(name, value, days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
		var expires = "; expires=" + date.toGMTString();
	}
	else var expires = "";
	document.cookie = name + "=" + value+expires + "; path=/";
};

StickyNotes.readCookie = function(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0; i < ca.length; i++) {
		var c = ca[i];
		while (c.charAt(0) == ' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) {
		  return c.substring(nameEQ.length, c.length);
		}
	}
	return null;
};

StickyNotes.eraseCookie = function(name) {
	createCookie(name, "", -1);
}