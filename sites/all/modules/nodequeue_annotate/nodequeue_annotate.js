
Drupal.behaviors.nodequeue_annotate = function(context) {
  
  // Shorthand the module's settings
  var settings = Drupal.settings.nodequeue_annotate;
  
  // Bind click behavior to Nodequeue Annotate links
  $('a.nodequeue-annotate-ajax-toggle', context).click(nodequeue_annotate_link_click);
  
  /**
   * Click callback for the nodequeue annotate links.
   */
  function nodequeue_annotate_link_click(e) {
    
    // Import variables form hidden span
    var data = $('.nodequeue-annotate-javascript-data', this).text();
    data = Drupal.parseJson(data);
    
    // Create the container for the form
    nodequeue_annotate_container_create(data, e);
    
    // Set the currently active class on the clicked element
    $(this).addClass('nodequeue-annotate-active');
    
    // Create the form to append
    var toAppend = '';
    
    toAppend += '<div class="nodequeue-annotate-header">';
    toAppend += '<a>X</a> ';
    toAppend += '<h4>' + Drupal.t((data.action == 'add') ? settings.js_title_add : settings.js_title_edit, { '%node_title': data.node_title , '%subqueue_title': data.subqueue_title }) + '</h4>';
    toAppend += '</div> ';
    
    toAppend += '<form>';
    
    toAppend += '<div>';
    toAppend += '<label id="nodequeue-annotate-annotation-label" for="nodequeue-annotate-annotation">' + Drupal.t(settings.js_textarea_label) + '</label> ';
    toAppend += '<textarea id="nodequeue-annotate-annotation" name="annotation" rows="6" cols="50">' + data.annotation + '</textarea>';
    toAppend += '</div> ';
    
    toAppend += '<div class="nodequeue-annotate-buttons">';
    if (data.action == 'remove-node') {
      toAppend += '<input id="nodequeue-annotate-form-remove" type="submit" value="' + Drupal.t(settings.js_button_remove) + '" /> ';
    }
    toAppend += '<input id="nodequeue-annotate-form-cancel" type="submit" value="' + Drupal.t(settings.js_button_cancel) + '" />';
    toAppend += '<input id="nodequeue-annotate-form-submit" type="submit" value="' + Drupal.t(data.action == 'add' ? settings.js_button_add : settings.js_button_edit) + '" />';
    toAppend += '<span id="nodequeue-annotate-throbber"> </span>';
    toAppend += '</div>';
    
    toAppend += '</form>';
    
    // And append it
    $('#nodequeue-annotate-inner').append(toAppend);
    
    // Bind events
    $('.nodequeue-annotate-header > a').bind('click', nodequeue_annotate_container_remove);
    $('#nodequeue-annotate-form-cancel').bind('click', nodequeue_annotate_container_remove);
    $('#nodequeue-annotate-form-submit').bind('click', nodequeue_annotate_submit_save);
    $('#nodequeue-annotate-form-remove').bind('click', nodequeue_annotate_submit_remove);
    $('#nodequeue-annotate-annotation').bind('keypress', nodequeue_annotate_submit_on_enter);
    
    // Impose maxlength
    $('#nodequeue-annotate-annotation').bind('blur focus scroll click mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave change select submit keydown keypress keyup', impose_max_length);
    
    // Make sure the container is contained in the viewport
    if (parseInt($('#nodequeue-annotate-inner').css('top')) + $('#nodequeue-annotate-inner').outerHeight() > $(window).height()) {
      $('#nodequeue-annotate-inner').css('top', $(window).height() - $('#nodequeue-annotate-inner').outerHeight());
    }
    
    // Return false to prevent link following
    return false;
  }
  
  /**
   * Create the container HTML, set it's position and apply visual effects.
   */
  function nodequeue_annotate_container_create(data, e) {
    
    // Remove existing containers before adding one
    nodequeue_annotate_container_remove();
    
    // Add the container's HTML
    $('body').append(
      '<div id="nodequeue-annotate-container">' + 
        '<div id="nodequeue-annotate-background"></div>' +
        '<div id="nodequeue-annotate-inner"></div>' +
      '</div>');
    
    // Set the container's effects and position
    $('#nodequeue-annotate-background').fadeTo('normal', 0.6).click(nodequeue_annotate_container_remove);
    $('#nodequeue-annotate-inner').css('top', e.clientY).css('left', e.clientX);
    
  }
  
  /**
   * Remove the container's HTML and deactivate the active link.
   */
  function nodequeue_annotate_container_remove() {
    $('#nodequeue-annotate-container').remove();
    $('.nodequeue-annotate-active').removeClass('nodequeue-annotate-active');
    
    // Return false to prevent the browser from submitting the vanilla form by itself
    return false; 
  }
  
  /**
   * Submit callback for the annotation form.
   */
  function nodequeue_annotate_submit_save(e) {
    
    // Submitting the form for a node that is already in the subqueue means the user wants to edit the annotation
    if (nodequeue_annotate_active_data('action') == 'remove-node') {
      data = {
        action: 'edit',
        annotation: $('#nodequeue-annotate-annotation').val()
      };
    }
    else {
      data = {
        action: 'add',
        annotation: $('#nodequeue-annotate-annotation').val()
      };
    }
    
    // Submit the form with the ajax caller
    nodequeue_annotate_submit(data);
    
    // Return false to prevent the browser from submitting the vanilla form by itself
    return false;
  }
  
  /**
   * Submit the form if the user pressed "enter" in the textarea.
   */
  function nodequeue_annotate_submit_on_enter(e) {
    if (e.keyCode == '13') {
      $('#nodequeue-annotate-form-submit').click();
      return false;
    }
  }
  
  /**
   * Click callback for the link next to the annotation form.
   */
  function nodequeue_annotate_submit_remove(e) {
    data = {
      action: 'remove-node',
      annotation: ''
    };
    
    // Submit the form with the ajax caller
    nodequeue_annotate_submit(data);
    
    // Return false to prevent the browser from submitting the vanilla form by itself
    return false;
  }
  
  /**
   * Submit the request with ajax.
   */
  function nodequeue_annotate_submit(data) {
    var qid, sqid, nid;
    
    // Set IDs for the url
    qid = nodequeue_annotate_active_data('qid');
    sqid = nodequeue_annotate_active_data('sqid');
    nid = nodequeue_annotate_active_data('nid');
    
    // Sanity check for PHP side
    data.js = 1;
    
    $('#nodequeue-annotate-throbber').css('visibility', 'visible');
    $.ajax({
      'action': data.action, // This has no effect on the AJAX request, but is used by the callbacks
      'data': data,
      'dataType': 'text',
      'error': nodequeue_annotate_submit_callback,
      'success': nodequeue_annotate_submit_callback,
      'type': 'POST',
      'url': Drupal.settings.basePath + "admin/content/nodequeue/"+qid+"/annotate-js/"+sqid+"/"+nid
    });
    
  }
  
  /**
   * Ajax callback for success or failure of form submission.
   */
  function nodequeue_annotate_submit_callback(data, textStatus) {
    var json;
    
    // Shorthand the module's settings
    var settings = Drupal.settings.nodequeue_annotate;
    
    // Stop the throbbing
    $('#nodequeue-annotate-throbber').css('visibility', 'hidden');
    
    if (textStatus == 'success') {
      
      // Save a copy of the data as a JSON compatible string
      json = data;
      
      // Parse the JSON data
      data = Drupal.parseJson(data);
      
      // Start by emptying the link entirely, we will be replacing it's content
      $('.nodequeue-annotate-active').empty();
      
      // Replace the link itself with the provided link text
      if (data.action == 'add') {
        // The new action is to add. Show the link
        $('.nodequeue-annotate-active').append(data.link_add);
        
        // Save the JSON compatible string
        $('.nodequeue-annotate-active').append('<span class="nodequeue-annotate-javascript-data">' + json + '</span>');
      }
      else if (data.link_remove.length > 0) {
        // The new action is to remove and a remove link text exists. Show the link
        $('.nodequeue-annotate-active').append(data.link_remove);
        
        // If there is an annotation in the data and the truncate length is positive, add the annotation text to the link
        if (data.annotation.length > 0 && settings.truncate_length > 0) {
          var truncated_annotation = truncate_utf8(data.annotation, settings.truncate_length, true, true);
          $('.nodequeue-annotate-active').append(' / ' + Drupal.theme('placeholder', truncated_annotation));
        }
        
        // Save the JSON compatible string
        $('.nodequeue-annotate-active').append('<span class="nodequeue-annotate-javascript-data">' + json + '</span>');
      }
      else if (settings.link_replacement.length > 0) {
        // The new action is to remove, no remove text appears but a replacement exists. Show the replacement.
        var replacement = Drupal.t(settings.link_replacement, { '%node_title': data.node_title , '%subqueue_title': data.subqueue_title , '%annotation': data.annotation });
        $('.nodequeue-annotate-active').replaceWith(replacement);
      }
      else {
        // The new action is to remove, no remove text appears and no replacement text exists. Remove.
        $('.nodequeue-annotate-active').parent().remove();
      }
      
      // Make sure the container doesn't change its size when we replace the text.
      $('#nodequeue-annotate-inner').css('height', $('#nodequeue-annotate-inner').height());
      $('#nodequeue-annotate-inner').css('width', $('#nodequeue-annotate-inner').width());
      
      // Shorthand the module's settings
      var settings = Drupal.settings.nodequeue_annotate;
      
      // Report a success message
      var message;
      
      switch (this.action) {
        case 'add':
          message = settings.js_success_add;
          break;
          
        case 'edit':
          message = settings.js_success_edit;
          break;
          
        case 'remove-node':
          message = settings.js_success_remove;
          break;
      }
      message = Drupal.t(message, { '%node_title': data.node_title , '%subqueue_title': data.subqueue_title });
      $('#nodequeue-annotate-inner > form').replaceWith('<div class="nodequeue-annotate-success">' + message + '</div>');
      
      // Callback to remove the container
      setTimeout(nodequeue_annotate_container_remove, 2500);
    }
    else {
      alert(Drupal.t("Unknown error occured."));
    }
    
  }
  
  /**
   * Get the requested data from the active link.
   */
  function nodequeue_annotate_active_data(name) {
    var data = $('.nodequeue-annotate-active > .nodequeue-annotate-javascript-data').text();
    data = Drupal.parseJson(data);
    
    return eval("data." + name);
  }
  
  /**
   * Truncate a UTF-8-encoded string safely to a number of characters.
   * 
   * Javascript translation from PHP.
   *
   * @param $string
   *   The string to truncate.
   * @param $len
   *   An upper limit on the returned string length.
   * @param $wordsafe
   *   Flag to truncate at last space within the upper limit. Defaults to FALSE.
   * @param $dots
   *   Flag to add trailing dots. Defaults to FALSE.
   * @return
   *   The truncated string.
   */
  function truncate_utf8(string, len, wordsafe, dots) {
    wordsafe = typeof(wordsafe) != 'undefined' ? wordsafe : false;
    dots = typeof(dots) != 'undefined' ? dots : false;

    if (string.length <= len) {
      return string;
    }

    if (dots) {
      len -= 4;
    }

    if (wordsafe) {
      string = string.substr(0, len + 1); // leave one more character
      last_space = string.lastIndexOf(' ');
      if (last_space != 0) { // space exists AND is not on position 0
        string = string.substr(0, last_space);
      }
      else {
        string = string.substr(0, len);
      }
    }
    else {
      string = string.substr(string, 0, len);
    }

    if (dots) {
      string += ' ...';
    }

    return string;
  }
  
  /**
   * Impose a maximum length of 255 characters on input for textarea.
   */
  function impose_max_length(e) {
    var value = $(this).val();
    if (value.length > 255) {
      value = value.substr(0, 255);
      $(this).val(value);
    }
  }
  
};

