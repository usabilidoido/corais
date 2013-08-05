
Drupal.behaviors.attachment = function (context) {
  $('#attach-wrapper input[type=submit]').hide();
  $('#attach-wrapper input[type=submit]')
    .bind('mousedown', Drupal.itweak_upload.progressBar);
  $('#attach-wrapper input[type=file]').change(function(e) {
    $('#attach-wrapper input[type=submit]').trigger('mousedown');
  });
  $('#attach-wrapper #upload-attachments tr').each(function() {
    var row = $(this);
    // Check if already done
    var check_remove_link = row.children('td.file').children('div').children('a.itu-remove');
    if (check_remove_link.size())
      return;	// Already processed
    // Description link
    var rename_input = row.children('td.file').children('div').children('input.rename');
    var name = $('<span>'+ Drupal.checkPlain(rename_input.val()) +'</span>');
    var rename_link = $('<a class="rename">'+ Drupal.t('Rename') +'</a>');
    rename_input.after(rename_link).after(name).hide();
    rename_link.toggle(
      function() {
        name.hide();
        rename_input.show();
        $(this).text(Drupal.t('Cancel'));
      },
      function() {
        name.show();
        rename_input.hide();
        rename_input.val(name.text());
        $(this).text(Drupal.t('Rename'));
      }
    );
    // Remove link
    var remove_input = row.children('td.itu-remove').children('div').children('input.itu-remove');
    row.children('td.itu-remove').hide();
    if (remove_input.attr('checked')) $(this).hide();
    var remove_link = $('<a class="itu-remove">'+ Drupal.t('Remove') +'</a>');
    rename_link.after(remove_link);
    remove_link.click(function() {
      row.hide();
      remove_input.attr('checked', true);
      var has_content = false;
      $('#upload-attachments tbody tr').each(function() {
        if ($(this).css('display') != 'none') has_content = true;
      });
      if (!has_content) $('#upload-attachments').hide();
    });
  });
  if (Drupal.behaviors.attachment.itu_insert)
    Drupal.behaviors.attachment.itu_insert(context);
};

Drupal.itweak_upload = {
  progressBar: function(event) {
    var clickedButton = this;
    var $progressId = $(clickedButton).parents('div#attach-wrapper').find('input.itweak_upload-progress');
    if ($progressId.size()) {
      var originalName = $progressId.attr('name');

      // Replace the name with the required identifier.
      $progressId.attr('name', originalName.match(/APC_UPLOAD_PROGRESS|UPLOAD_IDENTIFIER/)[0]);

      // Restore the original name after the upload begins.
      setTimeout(function() {
        $progressId.attr('name', originalName);
      }, 1000);

      // Show the progress bar if the upload takes longer than 3 seconds.
      setTimeout(function() {
        $(clickedButton).parents('div#attach-wrapper').find('div.ahah-progress-bar').slideDown();
      }, 500);

    }
  }
};
