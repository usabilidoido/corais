Drupal.behaviors.langDropdown = function(context) {
  $('#edit-lang-dropdown-select').change(function() {
    window.location.href = this.options[this.selectedIndex].value;
  });

  if (Drupal.settings.langDropdown) {
    var flags = Drupal.settings.langDropdown.jsWidget.languageicons;
    if (flags) {
      $.each(flags, function(index, value) {
        $('#edit-lang-dropdown-select option[value=' + index + ']').attr('title', value);
      });
    }

    var msddSettings = Drupal.settings.langDropdown.jsWidget;

    try {
      $('#edit-lang-dropdown-select').msDropDown({
        visibleRows: msddSettings.visibleRows,
        rowHeight: msddSettings.rowHeight,
        animStyle: msddSettings.animStyle,
        mainCSS: msddSettings.mainCSS
      });
    } catch (e) {
      if (console) { console.log(e); }
    }

    $('form#lang-dropdown-form').after('<div style="clear:both;"></div>');

  }
}
