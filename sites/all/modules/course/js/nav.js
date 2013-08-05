// @todo the only interval ajax we have so far. consider comet for the future
// and maybe use this as a fallback for servers that don't have node.js etc.

Drupal.behaviors.CourseNav = function() {
  var url = Drupal.settings.courseAjaxNavPath;
  if (Drupal.settings.courseAjaxNavPath && Drupal.settings.courseAjaxNavPath.length > 0) {
    setTimeout('CourseFulfillmentCheck();', 2500);
  }
}

function CourseFulfillmentCheck() {
  var url = Drupal.settings.courseAjaxNavPath;
  $.getJSON(url, {}, function (response) {
    if (response.complete) {
      // Load up new nav buttons.
      $('#course-nav').html(response.content);
    }
    else {
      // Call again.
      setTimeout('CourseFulfillmentCheck();', 2500);
    }
  });
}

