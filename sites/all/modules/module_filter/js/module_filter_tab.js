
Drupal.ModuleFilter = Drupal.ModuleFilter || {};
Drupal.ModuleFilter.textFilter = '';
Drupal.ModuleFilter.timeout;
Drupal.ModuleFilter.tabs = {};
Drupal.ModuleFilter.enabling = {};
Drupal.ModuleFilter.disabling = {};

Drupal.behaviors.moduleFilter = function() {
  // Set the focus on the module filter textfield.
  $('#edit-module-filter-name').focus();

  $('#module-filter-squeeze').css('min-height', $('#module-filter-tabs').height());

  $('#module-filter-left a.project-tab').each(function(i) {
    Drupal.ModuleFilter.tabs[$(this).attr('id')] = new Drupal.ModuleFilter.Tab(this);
  });

  // Move anchors to top of tabs.
  $('a.anchor', $('#module-filter-left')).remove().prependTo('#module-filter-tabs');

  $('#edit-module-filter-name').keyup(function() {
    if (Drupal.ModuleFilter.textFilter != $(this).val()) {
      Drupal.ModuleFilter.textFilter = this.value;
      if (Drupal.ModuleFilter.timeout) {
        clearTimeout(Drupal.ModuleFilter.timeout);
      }
      Drupal.ModuleFilter.timeout = setTimeout('Drupal.ModuleFilter.filter("' + Drupal.ModuleFilter.textFilter + '")', 500);
    }
  });

  Drupal.ModuleFilter.showEnabled = $('#edit-module-filter-show-enabled').is(':checked');
  $('#edit-module-filter-show-enabled').change(function() {
    Drupal.ModuleFilter.showEnabled = $(this).is(':checked');
    Drupal.ModuleFilter.filter($('#edit-module-filter-name').val());
  });
  Drupal.ModuleFilter.showDisabled = $('#edit-module-filter-show-disabled').is(':checked');
  $('#edit-module-filter-show-disabled').change(function() {
    Drupal.ModuleFilter.showDisabled = $(this).is(':checked');
    Drupal.ModuleFilter.filter($('#edit-module-filter-name').val());
  });
  Drupal.ModuleFilter.showRequired = $('#edit-module-filter-show-required').is(':checked');
  $('#edit-module-filter-show-required').change(function() {
    Drupal.ModuleFilter.showRequired = $(this).is(':checked');
    Drupal.ModuleFilter.filter($('#edit-module-filter-name').val());
  });
  Drupal.ModuleFilter.showUnavailable = $('#edit-module-filter-show-unavailable').is(':checked');
  $('#edit-module-filter-show-unavailable').change(function() {
    Drupal.ModuleFilter.showUnavailable = $(this).is(':checked');
    Drupal.ModuleFilter.filter($('#edit-module-filter-name').val());
  });

  if (Drupal.settings.moduleFilter.visualAid == 1) {
    $('#projects tbody td.checkbox input').change(function() {
      if ($(this).is(':checked')) {
        Drupal.ModuleFilter.updateVisualAid('enable', $(this).parents('tr'));
      }
      else {
        Drupal.ModuleFilter.updateVisualAid('disable', $(this).parents('tr'));
      }
    });
  }

  // Check for anchor.
  var url = document.location.toString();
  if (url.match('#')) {
    // Make tab active based on anchor.
    var anchor = '#' + url.split('#')[1];
    $('a[href="' + anchor + '"]').click();
  }
  // Else if no active tab is defined, set it to the all tab.
  else if (Drupal.ModuleFilter.activeTab == undefined) {
    Drupal.ModuleFilter.activeTab = Drupal.ModuleFilter.tabs['all-tab'];
  }
}

Drupal.ModuleFilter.visible = function(checkbox) {
  if (Drupal.ModuleFilter.showEnabled) {
    if ($(checkbox).is(':checked') && !$(checkbox).is(':disabled')) {
      return true;
    }
  }
  if (Drupal.ModuleFilter.showDisabled) {
    if (checkbox.size() && (!$(checkbox).is(':checked') && !$(checkbox).is(':disabled'))) {
      return true;
    }
  }
  if (Drupal.ModuleFilter.showRequired) {
    if ($(checkbox).is(':checked') && $(checkbox).is(':disabled')) {
      return true;
    }
  }
  if (Drupal.ModuleFilter.showUnavailable) {
    if (!checkbox.size() || (!$(checkbox).is(':checked') && $(checkbox).is(':disabled'))) {
      return true;
    }
  }
  return false;
}

Drupal.ModuleFilter.filter = function(string) {
  var stringLowerCase = string.toLowerCase();
  var flip = 'odd';

  if (Drupal.ModuleFilter.activeTab.id == 'all-tab') {
    var selector = '#projects tbody tr td > strong';
  }
  else {
    var selector = '#projects tbody tr.' + Drupal.ModuleFilter.activeTab.id + '-content td > strong.project-name';
  }

  $(selector).each(function(i) {
    var $row = $(this).parent().parent();
    var module = $(this).text();
    var moduleLowerCase = module.toLowerCase();

    if (moduleLowerCase.match(stringLowerCase)) {
      if (Drupal.ModuleFilter.visible($('td.checkbox input', $row))) {
        $row.removeClass('odd even');
        $row.addClass(flip);
        $row.show();
        flip = (flip == 'odd') ? 'even' : 'odd';
      }
      else {
        $row.hide();
      }
    }
    else {
      $row.hide();
    }
  });
}

Drupal.ModuleFilter.Tab = function(element) {
  this.id = $(element).attr('id');
  this.element = element;

  $(this.element).click(function() {
    Drupal.ModuleFilter.tabs[$(this).attr('id')].setActive();
  });
}

Drupal.ModuleFilter.Tab.prototype.setActive = function() {
  if (Drupal.ModuleFilter.activeTab) {
    $(Drupal.ModuleFilter.activeTab.element).parent().removeClass('active');
  }
  // Assume the default active tab is #all-tab. Remove its active class.
  else {
    $('#all-tab').parent().removeClass('active');
  }

  Drupal.ModuleFilter.activeTab = this;
  $(Drupal.ModuleFilter.activeTab.element).parent().addClass('active');
  Drupal.ModuleFilter.activeTab.displayRows();

  // Clear filter textfield and refocus on it.
  $('#edit-module-filter-name').val('');
  $('#edit-module-filter-name').focus();
}

Drupal.ModuleFilter.Tab.prototype.displayRows = function() {
  var flip = 'odd';
  var selector = (Drupal.ModuleFilter.activeTab.id == 'all-tab') ? '#projects tbody tr' : '#projects tbody tr.' + this.id + '-content';
  $('#projects tbody tr').hide();
  $('#projects tbody tr').removeClass('odd even');
  $(selector).each(function(i) {
    if (Drupal.ModuleFilter.visible($('td.checkbox input', $(this)))) {
      $(this).addClass(flip);
      flip = (flip == 'odd') ? 'even' : 'odd';
      $(this).show();
    }
  });

  if (typeof Drupal.ModuleFilter.enabledCount == 'function') {
    Drupal.ModuleFilter.enabledCount(Drupal.ModuleFilter.activeTab);
  }
}

Drupal.ModuleFilter.Tab.prototype.updateEnabling = function(amount) {
  this.enabling = this.enabling || 0;
  this.enabling += amount;
  if (this.enabling == 0) {
    delete(this.enabling);
  }
}

Drupal.ModuleFilter.Tab.prototype.updateDisabling = function(amount) {
  this.disabling = this.disabling || 0;
  this.disabling += amount;
  if (this.disabling == 0) {
    delete(this.disabling);
  }
}

Drupal.ModuleFilter.Tab.prototype.updateVisualAid = function() {
  var visualAid = '';
  if (this.enabling != undefined) {
    visualAid += '<span class="enabling">' + Drupal.t('+@count', { '@count': this.enabling }) + '</span>';
  }
  if (this.disabling != undefined) {
    visualAid += '<span class="disabling">' + Drupal.t('-@count', { '@count': this.disabling }) + '</span>';
  }

  if (!$('span.visual-aid', $(this.element)).size() && visualAid != '') {
    $(this.element).prepend('<span class="visual-aid"></span>');
  }

  $('span.visual-aid', $(this.element)).empty().append(visualAid);
}

Drupal.ModuleFilter.updateVisualAid = function(type, row) {
  // Find row class.
  var classes = row.attr('class').split(' ');
  for (var i in classes) {
    // Remove '-content' so we can use as id.
    var id = classes[i].substring(0, classes[i].length - 8);
    if (Drupal.ModuleFilter.tabs[id] != undefined) {
      break;
    }
  }

  if (Drupal.ModuleFilter.activeTab.id == 'all-tab') {
    var allTab = Drupal.ModuleFilter.activeTab;
    var projectTab = Drupal.ModuleFilter.tabs[id];
  }
  else {
    var allTab = Drupal.ModuleFilter.tabs['all-tab'];
    var projectTab = Drupal.ModuleFilter.activeTab;
  }

  var name = $('td > strong', row).text();
  switch (type) {
    case 'enable':
      if (Drupal.ModuleFilter.disabling[id + name] != undefined) {
        delete(Drupal.ModuleFilter.disabling[id + name]);
        allTab.updateDisabling(-1);
        projectTab.updateDisabling(-1);
        row.removeClass('disabling');
      }
      else {
        Drupal.ModuleFilter.enabling[id + name] = name;
        allTab.updateEnabling(1);
        projectTab.updateEnabling(1);
        row.addClass('enabling');
      }
      break;
    case 'disable':
      if (Drupal.ModuleFilter.enabling[id + name] != undefined) {
        delete(Drupal.ModuleFilter.enabling[id + name]);
        allTab.updateEnabling(-1);
        projectTab.updateEnabling(-1);
        row.removeClass('enabling');
      }
      else {
        Drupal.ModuleFilter.disabling[id + name] = name;
        allTab.updateDisabling(1);
        projectTab.updateDisabling(1);
        row.addClass('disabling');
      }
      break;
  }

  allTab.updateVisualAid();
  projectTab.updateVisualAid();
}
