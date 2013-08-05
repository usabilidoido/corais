
/**
* Popup dialog when update might delete some comments.
*/
function check_save(event){
  var new_version = $('#edit-revision').attr('checked');
  var comments_kept = $('#edit-keep-comments').attr('checked');
  var new_content = $('#edit-body').val();

  var o=CKEDITOR.instances['edit-body'];
  if (o) new_content=o.getData();

  if (comments_kept) {
    var pre_edit_url = Drupal.settings.co_ment.pre_edit_url;

    event.preventDefault();
    $.ajax({
      url: pre_edit_url,
      type:'POST',
      dataType:"json",
      data: {"new_content": new_content},
      success: function(obj) {
        nb_removed = obj['nb_removed'];
        if (nb_removed == 0) {
          $('#node-form').submit();
        }
        else {
          if (new_version) {
            var message;
            // Drupal.formatPlural is buggy, see http://drupal.org/node/504506.
            if (nb_removed >1) {
              message = Drupal.t('!nb comments apply to text that was modified.', {'!nb' : nb_removed}) + '<br />' + Drupal.t('Should these comments be detached (i.e. kept with no scope) or removed from new version?');
            }
            else {
              message = Drupal.t('1 comment applies to text that was modified.') + '<br />' + Drupal.t('Should this comment be detached (i.e. kept with no scope) or removed from new version?');
            }

            $('#remove_scope_choice_dlg').html(message);
            $('#remove_scope_choice_dlg').dialog('open');
          }
          else {
            var message;
            // Drupal.formatPlural is buggy, see http://drupal.org/node/504506.
            if (nb_removed >1) {
              message = Drupal.t('!nb comments apply to text that was modified.', {'nb' : nb_removed}) + '<br />' + Drupal.t('(We suggest you create a new version.)') + '<br />' + Drupal.t('Should these comments be detached (i.e. kept with no scope) or removed from new version?');
            }
            else {
              message = Drupal.t('1 comment applies to text that was modified.') + '<br />' + Drupal.t('(We suggest you create a new version.)') + '<br />' + Drupal.t('Should this comment be detached (i.e. kept with no scope) or removed from new version?');
            }

            $('#remove_scope_choice_dlg').html(message);
            $('#remove_scope_choice_dlg').dialog('open');
          }
        }
      },
      error: function(xhr, status) {
        e = JSON.parse(xhr.responseText);
        alert(Drupal.t('Error: ') + e.co_ment_error);
      }
    });
  }
  else {
    if (!new_version) {
      var message = Drupal.t('You chose not to create a new version, all comments will be deleted.');
      message += '<br />';
      message += Drupal.t( 'Do you want to continue?');

      $('#remove_scope_choice_dlg').html(message);
      $('#remove_scope_choice_dlg').dialog('open');
    }
    else {
      $('#node-form').submit();
    }
  }
}

Drupal.behaviors.edit_co_ment_confirm = function() {
  var buttons0 = {};
	buttons0[Drupal.t('Detach')] = function() {
    $(this).dialog('close');
    $('#edit-cancel-modified-scopes').val("1");
    $('#node-form').submit();
  };
	buttons0[Drupal.t('Remove')] = function() {
    $(this).dialog('close');
    $('#edit-cancel-modified-scopes').val("0");
    $('#node-form').submit();
  };
	buttons0[Drupal.t('Cancel')] = function() {
    $(this).dialog('close');
  };

  $('#remove_scope_choice_dlg').dialog({
    bgiframe: true,
    autoOpen: false,
    title :Drupal.t('Warning'),
    modal: true,
    buttons:buttons0
   });
  $("form#node-form #edit-submit").click(function(event) { check_save(event); });
};
