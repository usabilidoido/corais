// $Id: imagepicker.js,v 1.1.2.7 2010/08/10 21:04:19 hutch Exp $
// Original Id: upload_progress.js,v 1.1 2007/11/17 06:47:47 pfournier Exp
/**
 * @file
 * Javascript functions for progress bar status on node creation forms
 *
 * @author Patrick Fournier <patrick at patrickfournier dot com>
 * @author Fabio Varesano <fvaresano at yahoo dot it>
 * Adapted by Bob Hutchinson for imagepicker upload form
*/

/**
 * Hide the node form and show the busy div
*/
Drupal.imagepicker_upload_progress_hide_timeout = function() {
  var delay = Drupal.settings["imagepicker_upload_progress"]["delay"];
  setTimeout('imagepicker_upload_progress_hide()', delay*1000);
}

function imagepicker_upload_progress_hide() {
  $('#imagepicker-upload-form').hide();
  $("#sending").show();
  $("#imagepicker_upload_progress_cancel_link").click(Drupal.imagepicker_upload_progress_show);
}

Drupal.imagepicker_upload_progress_show = function() {
  $('#imagepicker-upload-form').show();
  $("#sending").hide();

  // "reload" the form
  window.location = window.location;
}

// Global killswitch
if (Drupal.jsEnabled) {
  Drupal.behaviors.imagepicker = function() {
    // Attaches the upload behaviour to the imagepicker upload form.
    $('#imagepicker-upload-form').submit(Drupal.imagepicker_upload_progress_hide_timeout);

    // exif info toggle
    $('#imgp_trig').click(function() {
      $('#imgp_targ').toggle('slow');
    });

    // form enhancement
    if ($("#edit-imagepicker-quota-byrole").attr('checked')) {
      $("#wrap_imagepicker_quota_role").show();
    }
    else {
      $("#wrap_imagepicker_quota_role").hide();
    }
    $("#edit-imagepicker-quota-byrole").change(function() {
      if ($("#edit-imagepicker-quota-byrole").attr('checked')) {
        $("#wrap_imagepicker_quota_role").show();
      }
      else {
        $("#wrap_imagepicker_quota_role").hide();
      }
    });

    if ($("#edit-imagepicker-import-enabled").attr('checked')) {
      $("#wrap-imagepicker-import").show();
    }
    else {
      $("#wrap-imagepicker-import").hide();
    }
    $("#edit-imagepicker-import-enabled").change(function() {
      if ($("#edit-imagepicker-import-enabled").attr('checked')) {
        $("#wrap-imagepicker-import").show();
      }
      else {
        $("#wrap-imagepicker-import").hide();
      }
    });

    if ($("#edit-imagepicker-exifinfo-enable").attr('checked')) {
      $("#wrap-imagepicker-exifinfo-external").show();
    }
    else {
      $("#wrap-imagepicker-exifinfo-external").hide();
    }
    $("#edit-imagepicker-exifinfo-enable").change(function() {
      if ($("#edit-imagepicker-exifinfo-enable").attr('checked')) {
        $("#wrap-imagepicker-exifinfo-external").show();
      }
      else {
        $("#wrap-imagepicker-exifinfo-external").hide();
      }
    });

    if ($("#edit-imagepicker-upload-progress-enabled").attr('checked')) {
      $("#wrap-imagepicker-upload-progress").show();
    }
    else {
      $("#wrap-imagepicker-upload-progress").hide();
    }
    $("#edit-imagepicker-upload-progress-enabled").change(function() {
      if ($("#edit-imagepicker-upload-progress-enabled").attr('checked')) {
        $("#wrap-imagepicker-upload-progress").show();
      }
      else {
        $("#wrap-imagepicker-upload-progress").hide();
      }
    });

    if ($("#edit-group-public").attr('checked')) {
      $("#wrap-group-public-roles").show();
    }
    else {
      $("#wrap-group-public-roles").hide();
    }
    $("#edit-group-public").change(function() {
      if ($("#edit-group-public").attr('checked')) {
        $("#wrap-group-public-roles").show();
      }
      else {
        $("#wrap-group-public-roles").hide();
      }
    });
  };
}
