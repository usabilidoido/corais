
if (Drupal.jsEnabled) {
  var moduleFilterTimeOut;
  var moduleFilterTextFilter = '';

  $(document).ready(function() {
    $("#module-filter-wrapper").show();
    $("#edit-module-filter-name").focus();
    $("#edit-module-filter-name").keyup(function() {
      if (moduleFilterTextFilter != $(this).val()) {
        moduleFilterTextFilter = this.value;
        if (moduleFilterTimeOut) {
          clearTimeout(moduleFilterTimeOut);
        }
        moduleFilterTimeOut = setTimeout("moduleFilter('" + moduleFilterTextFilter + "')", 500);
      }
    });

    $('#edit-module-filter-show-enabled').change(function() {
      moduleFilter($('#edit-module-filter-name').val());
    });
    $('#edit-module-filter-show-disabled').change(function() {
      moduleFilter($('#edit-module-filter-name').val());
    });
    $('#edit-module-filter-show-required').change(function() {
      moduleFilter($('#edit-module-filter-name').val());
    });
    $('#edit-module-filter-show-unavailable').change(function() {
      moduleFilter($('#edit-module-filter-name').val());
    });
  });
}

function moduleFilter(string) {
  stringLowerCase = string.toLowerCase();

  $("table.package tbody tr td > strong").each(function(i) {
    var $row = $(this).parent().parent();
    var module = $(this).text();
    var moduleLowerCase = module.toLowerCase();
    var $fieldset = $row.parents('fieldset');

    if (string != '') {
      if ($fieldset.hasClass('collapsed')) {
        $fieldset.removeClass('collapsed');
      }
    }

    if (moduleLowerCase.match(stringLowerCase)) {
      if (moduleFilterVisible($('td.checkbox input', $row))) {
        if (!$row.is(':visible')) {
          $row.show();
          if ($fieldset.hasClass('collapsed')) {
            $fieldset.removeClass('collapsed');
          }
          if (!$fieldset.is(':visible')) {
            $fieldset.show();
          }
        }
      }
      else {
        $row.hide();
        if ($row.siblings(':visible').html() == null) {
          $fieldset.hide();
        }
      }
    }
    else {
      if ($row.is(':visible')) {
        $row.hide();
        if ($row.siblings(':visible').html() == null) {
          $fieldset.hide();
        }
      }
    }
  });
}

function moduleFilterVisible(checkbox) {
  if ($('#edit-module-filter-show-enabled').is(':checked')) {
    if ($(checkbox).is(':checked') && !$(checkbox).is(':disabled')) {
      return true;
    }
  }
  if ($('#edit-module-filter-show-disabled').is(':checked')) {
    if (!$(checkbox).is(':checked') && !$(checkbox).is(':disabled')) {
      return true;
    }
  }
  if ($('#edit-module-filter-show-required').is(':checked')) {
    if ($(checkbox).is(':checked') && $(checkbox).is(':disabled')) {
      return true;
    }
  }
  if ($('#edit-module-filter-show-unavailable').is(':checked')) {
    if (!$(checkbox).is(':checked') && $(checkbox).is(':disabled')) {
      return true;
    }
  }
  return false;
}
