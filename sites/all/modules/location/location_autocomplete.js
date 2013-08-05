
/**
 * Twiddle the province autocomplete whenever the user changes the country.
 */
Drupal.behaviors.location = function(context) {
  $('select.location_dropdown_province option').each(function() {
    if (!$(this).is('hidden')) {
      var country_input = $('.location_auto_country', $(this).parents('fieldset:first, .views-exposed-form:first'));
      var klass = 'location_dropdown_join_' + country_input.val();
      if(!$(this).hasClass(klass)) {
        $(this).addClass(klass);
      }
    }
  });
  $('select.location_auto_country:not(.location-processed)', context).change(function(e) {
    var countryItem = $(this);
    var result = this.className.match(/(location_auto_join_[^ ]*)/);
    var type, provinceItem;
    if (result) {
      provinceItem = $('.location_auto_province.' + result);
      type = 'autocomplete';
    }
    else {
      // No joining class found, fallback to searching the immediate area.
      provinceItem = $('.location_auto_province', $(this).parents('fieldset:first, .views-exposed-form:first'));
      if (provinceItem && provinceItem.length) {
        type = 'autocomplete';
      }
      else {
        provinceItem = $('.location_dropdown_province', $(this).parents('fieldset:first, .views-exposed-form:first'));
        type = 'select';
      }
    }

    if (provinceItem && provinceItem.length) {
      switch(type) {
        case 'select':
          location_update_provinces(countryItem, provinceItem);
          break;

        case 'autocomplete':
        default:
          // Unbind events on province field and empty its value.
          provinceItem.unbind().val('');
          provinceItem.each(function(i) {
            // Get the (hidden) *-autocomplete input element.
            var input_autocomplete = $('#' + this.id + '-autocomplete');
            // Update autocomplete url.
            input_autocomplete.val(input_autocomplete.val().substr(0, input_autocomplete.val().lastIndexOf('/') + 1) + $(countryItem).val());

            // Mark as not processed.
            input_autocomplete.removeClass('autocomplete-processed');
          });
          // Reprocess.
          Drupal.behaviors.autocomplete(document);
          break;
      }
    }
  }).addClass('location-processed');
};

/**
 * Invoke the ajax request to fetch provinces for the specified country.
 */
function location_update_provinces(countryItem, provinceItem) {
  if (!countryItem.val().length) {
    return;
  }
  var country = countryItem.val();
  if (country == 'xx') {
    locationFillProvinceList(provinceItem, country);
  }
  else {
    provinceItem.find('option').remove();
    provinceItem.append('<option>Loading...</option>');
    return $.ajax({
      url : Drupal.settings.basePath + Drupal.settings.location_fetch_provinces_url + '/' + country,
      data : { input_id : provinceItem.attr('id'), country : country },
      dataType : 'json',
      success : location_update_provinces_callback,
      error : function() { alert('Error in network connection.  Please reload the page and try again (1).'); }
    });
  }
}

/**
 * On ajax request completion, update the appropriate select menu.
 */
function location_update_provinces_callback(data, textStatus) {
  var regexS = "[\\?&]input_id=([^&#]*)";
  var regex = new RegExp( regexS );
  var input_id = regex.exec(this.url);

  regexS = "[\\?&]country=([^&#]*)";
  regex = new RegExp( regexS );
  var country = regex.exec(this.url);

  if (!input_id || input_id.length < 2 || !$('#' + input_id[1]).length) {
    alert('Error in network connection. Please reload the page and try again (2).');
    return;
  }

  locationFillProvinceList($('#' + input_id[1]), country, data);
}

function locationFillProvinceList(provinceItem, country, data) {
  data = data || [];
  provinceItem.find('option').remove();
  provinceItem.append('<option value="">' + Drupal.t(Drupal.settings.please_select) + '</option>');
  provinceItem.append('<option value="xx">' + Drupal.t(Drupal.settings.not_listed) + '</option>');
  if (!$(data).length) {
    provinceItem.val('xx');
  }
  else {
    $.each(data, function(key, value) {
      provinceItem.append('<option value="' + key + '" class="location_dropdown_join_' + country + '">' + value + '</option>');
    });
  }
}
