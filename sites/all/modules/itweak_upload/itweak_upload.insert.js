
/**
 * Behavior to dynamically enable/disable Insert configuration checkboxes.
 */
Drupal.behaviors.itweak_upload_insert_admin = function(context) {
  $('input[name="itweak_upload_insert_override_default"]').each(function() {
    $(this).click(function() {
      overrideCheckbox(this);
    });
    overrideCheckbox(this);
  });
  function overrideCheckbox(c) {
    d=!$(c).attr('checked');
    p=$(c).parents('fieldset:eq(0)').find('input,select');
    p.not('*[name$="_override_default"]').each(function() {
      $(this).attr('disabled', d);
    });
  }
};

/**
 * Behavior to add "Insert" buttons.
 */
if (Drupal.behaviors.attachment)
Drupal.behaviors.attachment.itu_insert = function(context) {
  $('#attach-wrapper #upload-attachments tr').each(function() {
    var row = $(this);
    // Check if already done
    var check_insert_link = $('a.itu-insert',row);
    if (check_insert_link.size())
      return;	// Already processed

    // Element to insert after - label before the select
    var insert_div = $('td.itu-insert .insert-style-select',row);
    var label = $('label',insert_div)
    if (label.size()) {
      // Insert link
      var insert_link = $('<a rel="itweak_upload_widget" class="itu-insert" title="'+ Drupal.t('Insert this file into editor at the cursor') +'">'+ Drupal.t('Insert') +'</a> ');
      label.after(insert_link);
      label.hide();
    }
  });

  if (typeof(insertTextarea) == 'undefined') {
    insertTextarea = $('#edit-body').get(0) || false;
  }

  // Keep track of the last active textarea (if not using WYSIWYG).
  $('.node-form textarea:not([name$="[data][title]"])', context).focus(insertSetActive).blur(insertRemoveActive);

  // Add the click handler to the insert button.
  $('a.itu-insert', context).click(insert);

  function insertSetActive() {
    insertTextarea = this;
    this.insertHasFocus = true;
  }

  function insertRemoveActive() {
    if (insertTextarea == this) {
      var thisTextarea = this;
      setTimeout(function() {
        thisTextarea.insertHasFocus = false;
      }, 1000);
    }
  }

  function insert() {
    var widgetType = $(this).attr('rel');
    var settings = Drupal.settings.itweak_upload.widgets[widgetType];
    var wrapper = $(this).parents(settings.wrapper).filter(':first').get(0);
    var style = $('.insert-style', wrapper).val();
    var content = $('input.insert-template[name$="[' + style + ']"]', wrapper).val();
//    var content = '<img src="' + $(this).parents('td.file').children('.details').children('small').text() + '" />';
//    var content = $(this).parents('tr').children('.details').children('small').text();

    // Update replacements.
    for (var fieldName in settings.fields) {
      var fieldValue = $(settings.fields[fieldName], wrapper).val();
      var fieldRegExp = new RegExp('__' + fieldName + '__', 'g');
      if (fieldValue) {
        content = content.replace(fieldRegExp, fieldValue);
      }
    }
    // Cleanup unused replacements.
    content = content.replace(/__([a-z0-9_]+)__/g, '');

    // Check for a maximum dimension and scale down the width if necessary.
    // This is intended for use with Image Resize Filter.
    var widthMatches = content.match(/width[ ]*=[ ]*"(\d*)"/i);
    var heightMatches = content.match(/height[ ]*=[ ]*"(\d*)"/i);
    if (settings.maxWidth && widthMatches && parseInt(widthMatches[1]) > settings.maxWidth) {
      var insertRatio = settings.maxWidth / widthMatches[1];
      var width = settings.maxWidth;
      content = content.replace(/width[ ]*=[ ]*"?(\d*)"?/i, 'width="' + width + '"');

      if (heightMatches) {
        var height = Math.round(heightMatches[1] * insertRatio);
        content = content.replace(/height[ ]*=[ ]*"?(\d*)"?/i, 'height="' + height + '"');
      }
    }

    // Insert the text.
    Drupal.insert.insertIntoActiveEditor(content);
  }
};

