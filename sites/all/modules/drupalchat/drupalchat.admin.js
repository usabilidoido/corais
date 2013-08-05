$(document).ready(function() {
    $('#drupalchat-colorpicker1').farbtastic('#edit-drupalchat-chat-topbar-color');
	$('#drupalchat-colorpicker2').farbtastic('#edit-drupalchat-chat-topbar-text-color');
	$('#drupalchat-colorpicker3').farbtastic('#edit-drupalchat-font-color');
	$("fieldset > legend:contains('Theme Customization')").parent().hide();
	$("input[name=drupalchat_polling_method]").change(function() {
	    $('#edit-drupalchat-external-api-key').attr('disabled', 'disabled');
	    $('#edit-drupalchat-external-api-key-wrapper').hide();
		$("fieldset > legend:contains('Theme Customization')").parent().hide();
		$('#edit-drupalchat-stop-word-list-wrapper').hide();
		$('#edit-drupalchat-use-stop-word-list-wrapper').hide();
		$('#edit-drupalchat-stop-links-wrapper').hide();
		$('#edit-drupalchat-allow-anon-links-wrapper').hide();
		$('#edit-drupalchat-allow-single-message-delete-wrapper').hide();
		$('#edit-drupalchat-allow-clear-room-history-wrapper').hide();
		$('#edit-drupalchat-show-admin-list-wrapper').hide();
		$('#edit-drupalchat-user-picture-wrapper').hide();
		$('#edit-drupalchat-anon-name-set-wrapper').hide();
		$('#edit-drupalchat-anon-use-name-wrapper').hide();
    $('#edit-drupalchat-load-chat-async-wrapper').hide();
    $('#edit-drupalchat-minimize-chat-user-list-wrapper').hide();
		$('#edit-drupalchat-user-latency-wrapper').show();
		$('#edit-drupalchat-refresh-rate-wrapper').show();
		$('#edit-drupalchat-send-rate-wrapper').show();
		$('#edit-drupalchat-rel-wrapper').show();
		$('#edit-drupalchat-ur-name-wrapper').show();
		if ($("input[name=drupalchat_polling_method]:checked").val() == '0') {
	    	$('#edit-drupalchat-refresh-rate').removeAttr('disabled');
	    	$('#edit-drupalchat-send-rate').removeAttr('disabled');
	    	$('#edit-drupalchat-refresh-rate-wrapper').fadeIn();
	    	$('#edit-drupalchat-send-rate-wrapper').fadeIn();	    	
	    }
	    else if ($("input[@name=drupalchat_polling_method]:checked").val() == '1') {
	    	$('#edit-drupalchat-refresh-rate').attr('disabled', 'disabled');
	    	$('#edit-drupalchat-send-rate').attr('disabled', 'disabled');
	    	$('#edit-drupalchat-refresh-rate-wrapper').hide();
	    	$('#edit-drupalchat-send-rate-wrapper').hide();
	    }
		else if($("input[@name=drupalchat_polling_method]:checked").val() == '3') {
		  $('#edit-drupalchat-external-api-key').removeAttr('disabled');
	    $('#edit-drupalchat-external-api-key-wrapper').fadeIn();
		  $("fieldset > legend:contains('Theme Customization')").parent().fadeIn();
		  $('#edit-drupalchat-user-latency-wrapper').hide();
		  $('#edit-drupalchat-refresh-rate-wrapper').hide();
		  $('#edit-drupalchat-send-rate-wrapper').hide();
		  $('#edit-drupalchat-rel-wrapper').hide();
		  $('#edit-drupalchat-ur-name-wrapper').hide();
		  $('#edit-drupalchat-stop-word-list-wrapper').show();
		  $('#edit-drupalchat-use-stop-word-list-wrapper').show();
		  $('#edit-drupalchat-stop-links-wrapper').show();
		  $('#edit-drupalchat-allow-anon-links-wrapper').show();
		  $('#edit-drupalchat-allow-single-message-delete-wrapper').show();
		  $('#edit-drupalchat-allow-clear-room-history-wrapper').show();
		  $('#edit-drupalchat-show-admin-list-wrapper').show();
		  $('#edit-drupalchat-user-picture-wrapper').show();
		  $('#edit-drupalchat-anon-name-set-wrapper').show();
		  $('#edit-drupalchat-anon-use-name-wrapper').show();
      $('#edit-drupalchat-load-chat-async-wrapper').show();
      $('#edit-drupalchat-minimize-chat-user-list-wrapper').show();
		}
	});

  $("input[name=drupalchat_rel]").change(function() {
      if ($("input[name=drupalchat_rel]:checked").val() == '1') {
        $('#edit-drupalchat-ur-name').removeAttr('disabled');
        $('#edit-drupalchat-ur-name-wrapper').fadeIn();     
      }
      else {
        $('#edit-drupalchat-ur-name').attr('disabled', 'disabled');
        $('#edit-drupalchat-ur-name-wrapper').hide();
      }
  });	
	
	$("input[name=drupalchat_polling_method]").change();
	$("input[name=drupalchat_rel]").change();
});

