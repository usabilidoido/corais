Drupal.behaviors.nodereferenceViewsSelect = function(context) {	
	$('.nvs-field:not(.nvs-processed)', context)
		.addClass('nvs-processed')
		.each(function() {			
			// sorting behavior
			$(this).find('ul').sortable();
			
			// modify link with currently checked elements
			$(this).find('input:checkbox').change(function() {
				// get ids of all checked inputs
				var ids = $(this).parents('.nvs-field').find('input:checkbox:checked').map(function() {
					return $(this).val();
				});
				
				var link = $(this).parents('.nvs-field').siblings('.nvs-control');
				var url = link.attr('href');
				link.attr('href', url.substring(0, url.lastIndexOf('/') + 1) + escape($.makeArray(ids).join(',')));
				
				// remove item
				$(this).parents('li').fadeOut();
			});
			
			// same as above, for radio
						
		});
	
	// add item behavior
	$('.nvs-options:not(.nvs-processed)', context)
		.addClass('nvs-processed')
		.each(function() {
			// add checkbox functionality			
			$(this).find('input:checkbox').change(function() {
				var className = $(this).parents('.nvs-options').attr('className');
				var regexp = new RegExp('(\\s|^)nvs-options-(\\w*)(\\s|$)');
				var match = regexp.exec(className);
				var fieldName = match[2];
				
				var item = $(this).parents('li');		
				
				// remove this behavior
				$(this).unbind('change').removeClass('nvs-processed');
				
				// special case when selection was previously empty
				$('.nvs-field-' + fieldName + ' div.item-list:not(:has(ul))').append('<ul/>');
				
				// append to field
				$('.nvs-field-' + fieldName + ' ul').append(item);
				
				Drupal.attachBehaviors();

				return false;
			});
			
			// add radio button functionality
			$(this).find('input:radio').change(function() {
				var className = $(this).parents('.nvs-options').attr('className');
				var regexp = new RegExp('(\\s|^)nvs-options-(\\w*)(\\s|$)');
				var match = regexp.exec(className);
				var fieldName = match[2];
				
				var item = $(this).parent().parent(); // div.form-item
				
				// remove this behavior
				$(this).unbind('change').removeClass('nvs-processed');
				
				// append to field
				$('.nvs-field-' + fieldName).empty().append(item);
				
				Drupal.attachBehaviors();
				
				Drupal.CTools.Modal.dismiss();

				return false;
			});			
		});
	
	// close link
	$('.nvs-modal-dismiss:not(.nvs-processed)', context)
		.addClass('nvs-processed')
		.click(function() {
			Drupal.CTools.Modal.dismiss();
			
			return false;
		});
	
	// clear selection link
	$('.nvs-clear-selection:not(.nvs-processed)', context)
		.addClass('nvs-processed')
		.click(function() {
			$(this).parents('.form-item').find('.nvs-field').empty();
			
			return false;
		});	
}