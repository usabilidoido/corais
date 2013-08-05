// $Id$
Drupal.behaviors.hide_submit_admin_settings = function(context) {
    function hs_custom_image_toggle(use_custom_link) {
        $img = $("#edit-hide-submit-custom-image-link-wrapper", context);
        if (use_custom_link) {
            $img.show();
        }
        else {
            $img.hide(); 
        }
    }
    var $toggleCustomImage = $("#edit-hide-submit-toggle-custom-image", context);
    $toggleCustomImage.each(function() {
      hs_custom_image_toggle($(this).attr("checked"));
    });
    $toggleCustomImage.click(function() {
      hs_custom_image_toggle($(this).attr("checked"));
    });
}
