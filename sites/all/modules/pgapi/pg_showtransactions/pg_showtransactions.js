Drupal.FilterTransactionsDisableFields = function() {
	$('dl.multiselect input[@type=\'text\'], dl.multiselect select').each(function() {
	    var el = $(this);
	    el.attr('disabled',true);
	    el.css('background','#DDDDDD');
	});
}

Drupal.FilterTransactions = function() {

	Drupal.FilterTransactionsDisableFields();
	$('dl.multiselect input[@type=\'radio\']').click(function() {
		Drupal.FilterTransactionsDisableFields();
	    el = $(this);
	    $('dl.multiselect *[@name=\''+el.val()+'\']').attr('disabled',false).css('background','#FFFFFF');
	});

}

if (Drupal.jsEnabled) {
    $(document).ready(Drupal.FilterTransactions);
}