
/**
 * @file node_export_file_admin.js
 *  Admin DHTML for node_export_file module.
 **/

Drupal.behaviors.nodeExportFileAdmin = function () {
  var assets_state = false; // Start hidden
  var file_mode = null;
  var assets_div = $('div#edit-node-export-file-assets-path-wrapper');

  // On load, hide or show the assets path
  file_mode = $('input[name="node_export_file_mode"]:checked').val();
  assets_state = _node_export_file_get_state(file_mode);

  _node_export_file_toggle_export_mode_warning(file_mode);
  _node_export_file_assets_toggle(assets_state, assets_div, 'hide');

  $('input[name="node_export_file_mode"]').change(function () {
    assets_state = _node_export_file_get_state($(this).val())
    _node_export_file_assets_toggle(assets_state, assets_div, 'slide');
    _node_export_file_toggle_export_mode_warning($(this).val());
  });

  $('input[name="node_export_code"]').change(function () {
    _node_export_file_toggle_export_mode_warning($('input[name="node_export_file_mode"]').val());
  });
};

/**
 * Get state based on input value.
 **/
function _node_export_file_get_state(value) {
  switch (value) {
    default:
    case 'remote':
      return false;

    case 'local':
      return true;
  }
}

/**
 * Basic show/hide controller.
 **/
function _node_export_file_assets_toggle(state, div, effect) {
  if (state) {
    if (effect == 'hide') {
      div.show();
    } else {
      div.slideDown();
    }
  } else {
    if (effect == 'hide') {
      div.hide();
    } else {
      div.slideUp();
    }
  }
}

/**
 * Toggles the file mode warning message.
 */
function _node_export_file_toggle_export_mode_warning(state) {
  if (state == 'inline') {
    if ($('#edit-node-export-code-copy:checked').length == 0) {
      $('#node-export-file-mode-message').slideUp();
    }
    else {
      $('#node-export-file-mode-message').slideDown();
    }
  }
  else {
    $('#node-export-file-mode-message').slideUp();
  }
}

