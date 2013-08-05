
Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.page_title = function() {
  var pt = $('input#edit-page-title').val();
  if (pt) {
    return Drupal.t('Page title: @pt', { '@pt': pt });
  }
  else {
    return Drupal.t('No page title');
  }
}



Drupal.behaviors.pageTitleCounter = function(context) {
  $('#edit-page-title-wrapper', context).each(function() {
    var wrapper = this;

    var inputBox = $('input[name=page_title]', wrapper);

    var valueBox = $('div.description', wrapper)
                    .append('<br/><span class="counter">Characters Entered: <span class="value">0</span></span>')
                    .find('.value')
                    .text(getLength(inputBox));

    $('input[name=page_title]', wrapper).keyup(function(e) { $(valueBox).text(getLength(inputBox)); });
  });


  function getLength(element) { return $(element).val().length; }
}
