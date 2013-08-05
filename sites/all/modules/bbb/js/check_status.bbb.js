(function($) {
    Drupal.behaviors.bbbCheckStatus = {
        attach : function(context, settings) {
            Drupal.bbbCheckStatus();
            setInterval("Drupal.bbbCheckStatus();", 5000);
        }
    };
    Drupal.bbbCheckStatus = function() {
        var url = bbb_check_status_url;
        $.getJSON(url, function(data) {
            console.log(data);
            if (data.running == true) {
                location.href = location.href + '/meeting/attend';
            }
        });
    };
})(jQuery);