
var reference_preview_cache = {};

/* Reference Preview Behavior
 * 
 * Using jQuery, add a new div below the input element,
 * then add a trigger to reference_preview when the
 * element changes
*/
Drupal.behaviors.reference_preview = function (context) {
  for (var field_id in Drupal.settings.reference_preview) {
    
    var content = "<div id='reference-preview-" + field_id + "' class='reference-preview' style='display:none'></div>";
    
    var selectors =
      // matches autocomplete element
      'input[id^=edit-'+ field_id +'-][id$=-nid-nid]:not(.reference-preview-processed),'
      // matches checkboxes
      + '.form-checkboxes input[id^=edit-'+ field_id +'-][type=checkbox]:not(.reference-preview-processed),'
      // matches radios
      + '.form-radios input[id^=edit-'+ field_id +'-][type=radio]:not(.reference-preview-processed),'
      // matches select element options (need to do each for multi)
      + 'select[id=edit-'+ field_id +'-nid-nid] option:not(.reference-preview-processed)';

    $(selectors).addClass('reference-preview-processed').each(function() {
      var wrapper_id = null;
      // Select/Options are special cases since the values are all one element
      if ($(this).is('option')) {
        var $parent = $(this).parents('select');
        $parent.change(Drupal.reference_preview_select);
        wrapper_id =  $parent.attr('id') + '-' + $(this).val() + "-reference-preview";
      }
      else {
        $(this).change(Drupal.reference_preview);
        wrapper_id = $(this).attr('id') + "-reference-preview";
      }

      var content = "<div id='" + wrapper_id + "' class='reference-preview' style='display:none'></div>";

      // There's some assumptions baked in here about where the reference
      // preview should reside, but need to put it somewhere.

      // Add the reference preview class.
      if ($(this).is('[type=text]')) {
        $(this).after(content);
      }
      else if ($(this).is('[type=checkbox]')) {
        $(this).parents('label').after(content);
      }
      else if ($(this).is('[type=radio]')) {
        $(this).parents('.form-radios').after(content);
      }
      else if ($(this).is('option')) {
        $parent.after(content);
      }

      // Attach the field ID and wrapper to the input element for later
      jQuery.data(this, 'reference_preview_field_id', field_id);
      jQuery.data(this, 'reference_preview_wrapper_id', wrapper_id);
      $(this).change();
    });
  }
}

/**
 * Reference Preview wrapper.
 */
Drupal.reference_preview = function(event) {
  Drupal.reference_preview_preview(event.target);
}

/**
 * Refernce preview select wrapper.
 *
 * Calls the event for each value so that each selected gets a preview.
 */
Drupal.reference_preview_select = function(event) {
  $("option:selected", event.target).each(function () {
    Drupal.reference_preview_preview(this);
  });
}

/**
 * Reference preview preview function.
 */
Drupal.reference_preview_preview = function(target) {
  var $field = $(target);
  var field_id = jQuery.data(target, 'reference_preview_field_id');
  var reference_preview_id = jQuery.data(target, 'reference_preview_wrapper_id');
  var $reference_preview = $field.parents('.form-item').find('div.reference-preview#' + reference_preview_id);
  // Parse out "[nid: 123]" from the input element $field
  var nid = null;
  var is_option = $field.is('option');
  if ($field.is('[type=checkbox], [type=radio]')) {
    if ($field.is(':checked')) {
      nid = $field.val();
    }
  }
  else if (is_option) {
    nid = $field.val();
  }
  else {
    var regex = new RegExp("\[nid:[0-9]*\]");
    var m = regex.exec($field.val()); // matches [nid: 123]
    if (m !== null) {
      nid = m[0].substring(5, m[0].length -1 );
    }
  }

  // If we found a match, then get the nid, and then do the AHAH
  if (nid) {

    // Clear out uneeded previews.
    // Events never get called for unselected radio/select items
    if ($field.is('[type=radio]')) {
      $field.parents('.form-radios').parent().find('.reference-preview:not(#' + reference_preview_id + ')').html('');
    }
    else if (is_option) {
      $field.parents('select').find('option:not(:selected)').each(function () {
        wrapper_id = jQuery.data(this, 'reference_preview_wrapper_id');
        $field.parents('.form-item').find('div.reference-preview#' + wrapper_id).html('').hide();
      });
    }

    // If there is not already a preview present for this nid
    // (or a loading preview), add it.
    if ($reference_preview.find('#node-' + nid).length == 0) {

      // We cache the preview so don't have to recall the ajax
      // which is faster despite browser caching of ajax
      // using field id as field's have different settings.
      var reference_cache_key = field_id + '_' + nid;
      if (reference_preview_cache[reference_cache_key]) {
        $reference_preview.html(reference_preview_cache[reference_cache_key]).show();
        // Attach behaviours so dynamic content will work.
        Drupal.attachBehaviors($reference_preview);
      }
      else {
  
        // Load the ajax preview, with nid so earleir 'no node yet' returns false
        // the load the preview
        $reference_preview
          .html('<div class="ahah-progress ahah-progress-throbber"><div class="throbber" id="node-' + nid + '">&nbsp;</div></div>')
          .load(Drupal.settings.basePath + 'reference_preview_ahah/'+nid+'/'+field_id, 
            function(response, status, xhr) {

              // Error out if given no or incorrect data.
              if (status == "error" || response == '') {
                $reference_preview.html(Drupal.t('Unable to load preview.'));
              }
              else {
                // cache the response for refuse later.
                reference_preview_cache[reference_cache_key] = response;
                $reference_preview.find('a[target=]').attr('target', '_blank');

                // Attach behaviours so dynamic content will work.
                Drupal.attachBehaviors($reference_preview);
              }
            }).show();
      }
    }
  }
  else {
    // Clear out any previews.
    $reference_preview.html('').hide();
    if ($field.is('[type=radio]')) {
      $field.parents('.form-radios').parent().find('.reference-preview').html('').hide();
    }
    else if (is_option) {
      $field.parents('.form-item').find('div.reference-preview').html('').hide();
    }
  }
}
