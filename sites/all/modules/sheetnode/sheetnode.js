(function ($) {
// START jQuery

Drupal.behaviors.sheetnode = function(context) {
  // Abort early if sheetnode settings not in place (which happens in case of Views style settings).
  if (typeof(Drupal.settings.sheetnode) == 'undefined') return;

  $(".sheetview", context).not(".sheetview-processed").each(function() {
    if (typeof(Drupal.settings.sheetnode[$(this).attr('id')]) != 'undefined') {
      Drupal.sheetnode.sheetviews.push(new Drupal.sheetnode(Drupal.settings.sheetnode[$(this).attr('id')], this, context));
    }
  });
}

Drupal.sheetnode = function(settings, container, context) {
  this.settings = settings;
  this.$container = $(container);
  this.context = context;
  var self = this;
  window.setTimeout(function(){ self.start(); }, 1);
}

Drupal.sheetnode.sheetviews = [];

Drupal.sheetnode.prototype.functionsSetup = function() {
  // Abort if we were already here.
  if (typeof(SocialCalc.Formula.FunctionList["ORG.DRUPAL.FIELD"]) != 'undefined') return;

  var self = this;

  // ORG.DRUPAL.FIELD server-side function.
  SocialCalc.Formula.FunctionList["ORG.DRUPAL.FIELD"] = [function(fname, operand, foperand, sheet) {
    var scf = SocialCalc.Formula;
    var oid, entity, field;

    field = scf.OperandValueAndType(sheet, foperand);
    oid = scf.OperandValueAndType(sheet, foperand);
    entity = scf.OperandValueAndType(sheet, foperand);
    if (isNaN(parseInt(oid.value))) {
      oid.value = self.settings.context['oid'];
      entity.value = self.settings.context['entity-name'];
    }

    $.ajax({
      type: 'POST',
      url: Drupal.settings.basePath+'sheetnode/field',
      data: 'oid='+oid.value+'&entity='+escape(entity.value)+'&field='+escape(field.value),
      dataType: 'json',
      async: false,
      success: function (data) {
        operand.push(data);
      }
    });
  }, -1, "drupalfield", "", "drupal"];
  SocialCalc.Constants["s_fdef_ORG.DRUPAL.FIELD"] = 'Returns a field from the specified Drupal entity (node, user, etc.)';
  SocialCalc.Constants.s_farg_drupalfield = 'field-name, [oid, entity-name]';

  // ORG.DRUPAL.TOKEN server-side function.
  SocialCalc.Formula.FunctionList["ORG.DRUPAL.TOKEN"] = [function(fname, operand, foperand, sheet) {
    var scf = SocialCalc.Formula;
    var oid, entity, token;

    token = scf.OperandValueAndType(sheet, foperand);
    oid = scf.OperandValueAndType(sheet, foperand);
    entity = scf.OperandValueAndType(sheet, foperand);
    if (isNaN(parseInt(oid.value))) {
      oid.value = self.settings.context['oid'];
      entity.value = self.settings.context['entity-name'];
    }

    $.ajax({
      type: 'POST',
      url: Drupal.settings.basePath+'sheetnode/token',
      data: 'oid='+oid.value+'&entity='+escape(entity.value)+'&token='+escape(token.value),
      dataType: 'json',
      async: false,
      success: function (data) {
        operand.push(data);
      }
    });
  }, -1, "drupaltoken", "", "drupal"];
  SocialCalc.Constants["s_fdef_ORG.DRUPAL.TOKEN"] = 'Returns a token value from the specified Drupal entity (node, user, etc.)';
  SocialCalc.Constants.s_farg_drupaltoken = 'token, [oid, entity-name]';

  // ORG.DRUPAL.URLQUERY client-side function.
  SocialCalc.Formula.FunctionList["ORG.DRUPAL.PARSEURL"] = [function(fname, operand, foperand, sheet) {
    var scf = SocialCalc.Formula;
    var key = scf.OperandValueAndType(sheet, foperand);
    var parse = parseUri(location.href);
    var PushOperand = function(t, v) {operand.push({type: t, value: v});};
    if (typeof(parse.queryKey[key.value]) == 'undefined') {
      PushOperand("e#VALUE!", 0);
    }
    else {
      PushOperand(isNaN(parseInt(parse.queryKey[key.value])) ? 't' : 'n', parse.queryKey[key.value]);
    }
  }, 1, "drupalparseurl", "", "drupal"];
  SocialCalc.Constants["s_fdef_ORG.DRUPAL.PARSEURL"] = 'Returns the value of a key in the current URL query string.';
  SocialCalc.Constants.s_farg_drupalparseurl = 'key';

  // Update function classes.
  SocialCalc.Constants.function_classlist.push('drupal');
  SocialCalc.Constants.s_fclass_drupal = "Drupal";
}

Drupal.sheetnode.prototype.loadsheetSetup = function() {
  var self = this;
  SocialCalc.RecalcInfo.LoadSheet = function(sheetname) {
    var found = false;

    // Avoid returning this sheet.
    if (self.settings.aliases.indexOf(sheetname) != -1) {
      // TODO: Should really return true and inform SocialCalc that this is the same sheet.
      // However SocialCalc does not support aliases currently.
      return false;
    }

    // First examine on-screen spreadsheets.
    $.each(Drupal.sheetnode.sheetviews, function(index, sheetnode) {
      if (sheetnode.settings.aliases.indexOf(sheetname) != -1) {
        var data = sheetnode.spreadsheet.CreateSpreadsheetSave();
        var parts = sheetnode.spreadsheet.DecodeSpreadsheetSave(data);
        if (parts.sheet) {
          data = data.substring(parts.sheet.start, parts.sheet.end);
          SocialCalc.RecalcLoadedSheet(sheetname, data, false, true);
          found = true;
        }
      }
    });
    if (found) return true;

    // Next call backend to find the reference.
    data = $.ajax({
      type: 'POST',
      url: Drupal.settings.basePath+'sheetnode/load',
      data: 'sheetname='+escape(sheetname),
      dataType: 'text',
      async: false
    }).responseText;
    if (data !== null) {
      SocialCalc.RecalcLoadedSheet(sheetname, data, true);
      found = true;
    }

    return found;
  }
}

Drupal.sheetnode.prototype.focusSetup = function() {
  $('.form-text,.form-textarea,.form-select', this.context).not('.socialcalc-input').focus(function(e) {
    SocialCalc.CmdGotFocus(this);
  });
}

Drupal.sheetnode.prototype.callbackSetup = function() {
  var self = this;
  this.spreadsheet.editor.StatusCallback.sheetnode = {
    func: function(editor, status, arg, params) {
      if (status == 'calcfinished') {
        if (!self.settings.saveElement && self.settings.viewMode == Drupal.sheetnode.viewModes.htmlTable) {
          self.$container.html(SocialCalc.SpreadsheetViewerCreateSheetHTML(self.spreadsheet));
        }
      }
      if (status == 'doneposcalc' && self.settings.saveElement) {
        window.setTimeout(function() {
          $('#'+self.settings.saveElement, self.$form).val(self.spreadsheet.CreateSpreadsheetSave());
        }, 0);
      }
    },
    params: {}
  };
}

Drupal.sheetnode.viewModes = {
  readOnly: 0,
  fiddleMode: 1,
  htmlTable: 2
}

Drupal.sheetnode.prototype.start = function() {
  var self = this;
  var showEditor = this.settings.saveElement || this.settings.viewMode == Drupal.sheetnode.viewModes.fiddleMode;
  var showToolbar = this.settings.saveElement || (this.settings.showToolbar && this.settings.viewMode == Drupal.sheetnode.viewModes.fiddleMode);
  
  // SocialCalc initialization.
  SocialCalc.ConstantsSetImagePrefix(this.settings.imagePrefix);
  SocialCalc.Constants.defaultCommentClass = "cellcomment";
  SocialCalc.Constants.defaultReadonlyClass = "readonly";
  this.spreadsheet = showEditor ? new SocialCalc.SpreadsheetControl(this.settings.containerElement+"-") : new SocialCalc.SpreadsheetViewer(this.settings.containerElement+"-");
  if (showToolbar) {
    // Remove unwanted tabs.
    this.spreadsheet.tabs.splice(this.spreadsheet.tabnums.clipboard, 1);
    this.spreadsheet.tabs.splice(this.spreadsheet.tabnums.audit, 1);
    if (!this.settings.permissions['edit sheetnode settings']) {
      this.spreadsheet.tabs.splice(this.spreadsheet.tabnums.settings, 1);
    }
    this.spreadsheet.tabnums = {};
    for (var i=0; i<this.spreadsheet.tabs.length; i++) {
      this.spreadsheet.tabnums[this.spreadsheet.tabs[i].name] = i;
    }
  }
  else {
    this.spreadsheet.tabbackground="display:none;";
    this.spreadsheet.toolbarbackground="display:none;";
  }

  // Trigger event to alert plugins that we've created the spreadsheet.
  this.$container.trigger('sheetnodeCreated', {spreadsheet: this.spreadsheet});

  // Read in data and recompute.
  if (typeof(this.settings.value) == 'string') {
    var parts = this.spreadsheet.DecodeSpreadsheetSave(this.settings.value);
    if (parts && parts.sheet) {
      this.spreadsheet.ParseSheetSave(this.settings.value.substring(parts.sheet.start, parts.sheet.end));
    }
  }
  if (self.settings.saveElement || self.settings.viewMode != Drupal.sheetnode.viewModes.htmlTable) {
    if (showEditor) {
      this.spreadsheet.InitializeSpreadsheetControl(this.settings.containerElement, 700, this.$container.width());
    }
    else {
      this.spreadsheet.InitializeSpreadsheetViewer(this.settings.containerElement, 700, this.$container.width());
    }
    if (parts && parts.edit) {
      this.spreadsheet.editor.LoadEditorSettings(this.settings.value.substring(parts.edit.start, parts.edit.end));
    }
  }

  // Call our setup functions.
  this.focusSetup();
  this.functionsSetup();
  this.loadsheetSetup();
  this.callbackSetup();

  // DOM initialization.
  if (this.settings.saveElement) {
    this.$form = this.$container.parents('form').submit(function() {
      self.save();
      return true;
    });
    // Special handling for Views AJAX.
    try {
      $('input[type=submit]', Drupal.settings.views.ajax.id).click(function() {
        self.save();
      });
    }
    catch (e) {
      // Do nothing.
    }
  }
  $(window).resize(function() {
    self.resize();
  });
  $('div#SocialCalc-edittools', this.$container).parent('div').attr('id', 'SocialCalc-toolbar');
  $('td#SocialCalc-edittab', this.$container).parents('div:eq(0)').attr('id', 'SocialCalc-tabbar');
  $('input:text', this.$container).addClass('form-text socialcalc-input');
  $('input:radio', this.$container).addClass('form-radio socialcalc-input');
  $('input:checkbox', this.$container).addClass('form-checkbox socialcalc-input');
  $('textarea', this.$container).addClass('form-textarea socialcalc-input');
  $('select', this.$container).addClass('form-select socialcalc-input');
  $('input:button', this.$container).addClass('form-submit socialcalc-input');
  $('div#SocialCalc-sorttools td:first', this.$container).css('width', 'auto');
  $('div#SocialCalc-settingsview', this.$container).css('border', 'none').css('width', 'auto').css('height', 'auto');

  // Lock cells requires special permission.
  if (showToolbar && !this.settings.permissions['edit sheetnode settings']) {
    $('span#SocialCalc-locktools', this.$container).css('display', 'none');
  }

  // Prepare for fullscreen handling when clicking the SocialCalc icon.
  $('td#'+SocialCalc.Constants.defaultTableEditorIDPrefix+'logo img', this.$container).attr('title', Drupal.t('Fullscreen')).click(function() { self.fullscreen() });
  
  // Signal that we've processed this instance of sheetnode.
  this.$container.addClass('sheetview-processed');

  // Trigger event to alert plugins that we've built the spreadsheet.
  this.$container.trigger('sheetnodeReady', {spreadsheet: this.spreadsheet});

  // Force a recalc to refresh all values and scrollbars.
  this.spreadsheet.editor.EditorScheduleSheetCommands('recalc');
}

Drupal.sheetnode.prototype.fullscreen = function() {
  if (this.$container.hasClass('sheetview-fullscreen')) { // Going back to normal:
    // Restore saved values.
    this.$container.removeClass('sheetview-fullscreen');
    if (this.beforeFullscreen.index >= this.beforeFullscreen.parentElement.children().length) {
      this.beforeFullscreen.parentElement.append(this.$container);
    } else {
      this.$container.insertBefore(this.beforeFullscreen.parentElement.children().get(this.beforeFullscreen.index));
    }
    this.spreadsheet.requestedHeight = this.beforeFullscreen.requestedHeight;
    this.resize();
    $('body').css('overflow', 'auto');
    window.scroll(this.beforeFullscreen.x, this.beforeFullscreen.y);
  }
  else { // Going fullscreen:
    // Save current values.
    this.beforeFullscreen = {
      parentElement: this.$container.parent(),
      index: this.$container.parent().children().index(this.$container),
      x: $(window).scrollLeft(), y: $(window).scrollTop(),
      requestedHeight: this.spreadsheet.requestedHeight
    };

    // Set values needed to go fullscreen.
    $('body').append(this.$container).css('overflow', 'hidden');
    this.$container.addClass('sheetview-fullscreen');
    this.resize();
    window.scroll(0,0);
  }
}

Drupal.sheetnode.prototype.resize = function() {
  // Adjust width and height if needed.
  if (this.$container.hasClass('sheetview-fullscreen')) {
    this.spreadsheet.requestedHeight = this.$container.height();
  }
  this.spreadsheet.requestedWidth = this.$container.width();
  this.spreadsheet.DoOnResize();
}

Drupal.sheetnode.prototype.save = function() {
  var self = this;
  $('#'+this.settings.saveElement, this.$form).val(this.spreadsheet.CreateSpreadsheetSave());
  $('#edit-log', this.$form).each(function() {
    var audit = self.spreadsheet.sheet.CreateAuditString();
    var log = $(this).val();
    if (!log.length) {
      $(this).val(audit);
    }
    else {
      $(this).val(log + '\n' + audit);
    }
  });
}

// END jQuery
})(jQuery);

// parseUri 1.2.2
// (c) Steven Levithan <stevenlevithan.com>
// MIT License
// http://blog.stevenlevithan.com/archives/parseuri

// Sublicensed as GPL by infojunkie <karim.ratib@gmail.com>
// http://programmers.stackexchange.com/questions/105912/can-you-change-code-distributed-under-the-mit-license-and-re-distribute-it-unde

function parseUri (str) {
  var o   = parseUri.options,
    m   = o.parser[o.strictMode ? "strict" : "loose"].exec(str),
    uri = {},
    i   = 14;

  while (i--) uri[o.key[i]] = m[i] || "";

  uri[o.q.name] = {};
  uri[o.key[12]].replace(o.q.parser, function ($0, $1, $2) {
    if ($1) uri[o.q.name][$1] = $2;
  });

  return uri;
};

parseUri.options = {
  strictMode: false,
  key: ["source","protocol","authority","userInfo","user","password","host","port","relative","path","directory","file","query","anchor"],
  q:   {
    name:   "queryKey",
    parser: /(?:^|&)([^&=]*)=?([^&]*)/g
  },
  parser: {
    strict: /^(?:([^:\/?#]+):)?(?:\/\/((?:(([^:@]*)(?::([^:@]*))?)?@)?([^:\/?#]*)(?::(\d*))?))?((((?:[^?#\/]*\/)*)([^?#]*))(?:\?([^#]*))?(?:#(.*))?)/,
    loose:  /^(?:(?![^:@]+:[^:@\/]*@)([^:\/?#.]+):)?(?:\/\/)?((?:(([^:@]*)(?::([^:@]*))?)?@)?([^:\/?#]*)(?::(\d*))?)(((\/(?:[^?#](?![^?#\/]*\.[^?#\/.]+(?:[?#]|$)))*\/?)?([^?#\/]*))(?:\?([^#]*))?(?:#(.*))?)/
  }
};

// https://developer.mozilla.org/en/JavaScript/Reference/Global_Objects/Array/indexOf#Compatibility
if (!Array.prototype.indexOf) {
  Array.prototype.indexOf = function (searchElement /*, fromIndex */ ) {
    "use strict";
    if (this == null) {
      throw new TypeError();
    }
    var t = Object(this);
    var len = t.length >>> 0;
    if (len === 0) {
      return -1;
    }
    var n = 0;
    if (arguments.length > 0) {
      n = Number(arguments[1]);
      if (n != n) { // shortcut for verifying if it's NaN
          n = 0;
      } else if (n != 0 && n != Infinity && n != -Infinity) {
          n = (n > 0 || -1) * Math.floor(Math.abs(n));
      }
    }
    if (n >= len) {
      return -1;
    }
    var k = n >= 0 ? n : Math.max(len - Math.abs(n), 0);
    for (; k < len; k++) {
      if (k in t && t[k] === searchElement) {
        return k;
      }
    }
    return -1;
  }
}

