Drupal.behaviors.comment_registration = function(){

  var register = $('#edit-comment-registration-register');
  var fieldset = $('fieldset.comment-registration');
  
  if ($(register).is(':checked') == false) $(fieldset).hide();
  $(register).click(function(){
	  if($(register).is(':checked')) {
	    $(fieldset).show();
	  } else {
		  $(fieldset).hide();	
	 }
  });
}; 
