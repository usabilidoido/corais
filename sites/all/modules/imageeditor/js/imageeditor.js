Drupal.behaviors.imageeditor = function(context) {
  
  var $imageeditor_divs = $('.imageeditor', context);
  var $imageeditor_divs_existing = $('.imageeditor.imageeditor-existing', context);
  
  $imageeditor_divs.find('div.editors').find('div').click(
    function() {
      var tmp = $(this).attr('class').split(' ');
      var codename = tmp[0].replace(/-/g, '_');
      if (codename != 'aviary_feather' && codename != 'paintweb') {
        var launch_type = Drupal.settings.imageeditor[codename]['launch_type'];
        var image_url_param = Drupal.settings.imageeditor[codename]['image_url_param'];
        var save_url_param = Drupal.settings.imageeditor[codename]['save_url_param'];
        var save_url_default = Drupal.settings.imageeditor[codename]['save_url_default'];
        var field_name = $(this).parent().parent().find('.field-name').attr('value');
        var delta = $(this).parent().parent().find('.delta').attr('value');
        var url = $(this).parent().parent().find('.url').attr('value');
        var options = Drupal.settings.imageeditor[codename].options;
        if (typeof(url) != 'undefined') {
          options[image_url_param] = url;
          options[save_url_param] = save_url_default + '/' + field_name + '/' + delta + '/0/' + Drupal.settings.imageeditor[field_name]['imageeditor_replace'] + '/1';
          if (codename == 'picnik') {options['_imageid'] = field_name + '_' + delta;}
        }
        else {
          delete options[image_url_param];
          options[save_url_param] = save_url_default + '/' + field_name + '/' + delta + '/1/' + Drupal.settings.imageeditor[field_name]['imageeditor_replace'] + '/1';
          if (codename == 'picnik') {delete options['_imageid'];}
        }
        Drupal.imageeditor[launch_type].show(options);
      }
    }
  );
  
  $imageeditor_divs.find('div.paintweb').click(
    function() {
      var field_name = $(this).parent().parent().find('.field-name').attr('value');
      var delta = $(this).parent().parent().find('.delta').attr('value');
      var url = $(this).parent().parent().find('.url').attr('value');
      Drupal.settings.imageeditor.paintweb.save_url = Drupal.settings.imageeditor.paintweb.save_url_default + '/' + field_name + '/' + delta + '/1/' + Drupal.settings.imageeditor[field_name]['imageeditor_replace'] + '/1';
      pw = new PaintWeb();
      pw.config.configFile = Drupal.settings.imageeditor.paintweb.options.configFile;
      if (url) {
        var img = document.createElement('img');
        img.src = url;
        pw.config.imageLoad = img;
        Drupal.settings.imageeditor.url = url;
      }
      else {
        var canvas = document.createElement('canvas');
        canvas.setAttribute('width', 600);
        canvas.setAttribute('height', 600);
        var img = document.createElement('img');
        img.src = canvas.toDataURL('image/png');
        pw.config.imageLoad = img;
        Drupal.settings.imageeditor.url = 'undefined';
      }
      Drupal.imageeditor.overlay.show();
      pw.config.guiPlaceholder = $('.imageeditor-internal').get(0);
      pw.config.viewportWidth = $('.imageeditor-internal').width() + 'px';
      pw.config.viewportHeight = ($('.imageeditor-internal').height() - 140) + 'px';
      pw.init(function (ev) {
        if (ev.state === PaintWeb.INIT_ERROR) {
          alert('PaintWeb failed loading!');
        }
      });
      pw.events.add('imageSave', paintweb_save);
      //delete pw;
      delete img;
      delete canvas;
    }
  );
  
  $imageeditor_divs_existing.find('div.aviary-feather').click(
    function() {
      var field_name = $(this).parent().parent().find('.field-name').attr('value');
      var delta = $(this).parent().parent().find('.delta').attr('value');
      var url = $(this).parent().parent().find('.url').attr('value');
      imageeditor_first_run = 'yes';
      if (_featherLoaded) {
        aviaryeditor($(this).parent().parent().parent().find('.imagefield-preview').find('img').attr('id', field_name + '_' + delta).attr('id'), url);
      }
    }
  );
  
  $imageeditor_divs_existing.find('div.uploaders').find('div').click(
    function() {
      var tmp = $(this).attr('class').split(' ');
      var codename = tmp[0].replace(/-/g, '_');
      var $url = $(this).parent().parent().find('.url');
      var filepath = $(this).parent().parent().find('.filepath').attr('value');
      $.ajax({
        type: 'GET',
        url: Drupal.settings.imageeditor[codename].upload_url,
        async: false, // explicitly need the user to wait while we load...
        data: {'filepath': filepath},
        success: function(data) {$url.attr('value', data);},
        error: function(msg) {alert("Failed uploading: " + msg);}
      });
    }
  );
  
  //to show as overlay over the image preview
  $imageeditor_divs_existing.each(
    function(index) {
      var field_name = $(this).find('.field-name').attr('value');
      if (Drupal.settings.imageeditor[field_name]['imageeditor_icons_position'] == 1) {
        $(this).parent().css({'position': 'relative', 'width': '100px'}).hover(
          function() {
            $(this).find('.imageeditor').css({'position': 'absolute', 'top': 0}).show();
          },
          function() {
            $(this).find('.imageeditor').hide();
          }
        ).find('.imageeditor').hide();
      }
    }
  );
};

function paintweb_save(ev) {
  ev.preventDefault();
  var tmp = ev.dataURL.split(',');
  Drupal.imageeditor.overlay.hide();
  
  $.ajax({
    type: 'POST',
    url: Drupal.settings.imageeditor.paintweb.save_url,
    async: false, // explicitly need the user to wait while we load...
    data: {'data': tmp[1], 'url': Drupal.settings.imageeditor.url},
    success:
      function(data) {
        Drupal.imageeditor.overlay.show({
          'launch_url': Drupal.settings.imageeditor.paintweb.save_url,
          'image': data
        });
      },
    error: function(msg) {alert("Failed saving: " + msg);}
  });
  Drupal.settings.imageeditor.url = 'undefined';
  
  //pw.events.dispatch(new appEvent.imageSaveResult(true));
  //pw.events.dispatch(new appEvent.imageSaveResult(false));
};

var _featherLoaded = false;
Feather_OnLoad = function() {
  _featherLoaded = true;
};

Feather_OnSave = function(imageId, newurl) {
  imageeditor_url = newurl;
  if (imageeditor_first_run != 'no') {
    imageeditor_id = 'edit-' + imageId.replace(/_/g, '-');
    imageeditor_field_name = imageId.replace(/^(.*)_.*$/, '$1');
    if (Drupal.settings.imageeditor[imageeditor_field_name]['imageeditor_replace']) {
      $('input[name="' + imageId + '_filefield_remove"]').mousedown();
    }
    else {
      $('input[name="' + imageeditor_field_name + '_add_more"]').mousedown();
    }
    imageeditor_first_run = 'no';
  }
};

Feather_OnClose = function() {
  if ((typeof(imageeditor_field_name) != 'undefined') && (imageeditor_field_name != 'undefined')) {
    if (Drupal.settings.imageeditor[imageeditor_field_name]['imageeditor_replace']) {
      $('a[id="' + imageeditor_id + '-remote-source"]').click();
      $('input[id="' + imageeditor_id + '-filefield-remote-url"]').val(imageeditor_url);
      $('input[id="' + imageeditor_id + '-filefield-remote-transfer"]').mousedown();
    }
    else {
      starts_with = 'edit-' + imageeditor_field_name.replace(/_/g, '-');
      $('a[id^="' + starts_with + '-"][id$="-remote-source"]:first').click();
      $('input[id^="' + starts_with + '-"][id$="-filefield-remote-url"]:first').val(imageeditor_url);
      $('input[id^="' + starts_with + '-"][id$="-filefield-remote-transfer"]:first').mousedown();
    }
    imageeditor_url = 'undefined';
    imageeditor_id = 'undefined';
    imageeditor_field_name = 'undefined';
    imageeditor_first_run = 'undefined';
  }
};

if (typeof(Drupal.imageeditor) == 'undefined') {
Drupal.imageeditor = function() {
  function windowSize() {
    var w = 0, h = 0;
    if(!(document.documentElement.clientWidth == 0)) {
      w = document.documentElement.clientWidth;
      h = document.documentElement.clientHeight;
    }
    else {
      w = document.body.clientWidth;
      h = document.body.clientHeight;
    }
    return {width:w,height:h};
  }
  function buildUrl(opt) {
    var url = opt.launch_url;
    var first_attr = 'yes';
    if (url.indexOf('?') > -1) {first_attr = 'no';} //for non-clean URLs
    for (var attribute in opt) {
      if(attribute !== 'launch_url') {
        if (first_attr == 'yes') {
          url += "?"+ attribute +"="+ escape(opt[attribute]);
          first_attr = 'no';
        }
        else {
          url += "&"+ attribute +"="+ escape(opt[attribute]);
        }
      }
    }
    return url;
  }
  var bo = {
    ie: window.ActiveXObject,
    ie6: window.ActiveXObject && (document.implementation != null) && (document.implementation.hasFeature != null) && (window.XMLHttpRequest == null),
    quirks: document.compatMode==='BackCompat'
  };
  return {
    overlay: {
      show: function(options) {
        var iframe = document.createElement('iframe'),
        div = document.createElement('div'),
        idiv = document.createElement('div'),
        div_close = document.createElement('div');
        iframe.className = 'imageeditor-iframe';
        div.className = 'imageeditor-external';
        idiv.className = 'imageeditor-internal';
        div_close.className = 'imageeditor-close';
        
        if((bo.ie && bo.quirks) || bo.ie6) {
          var size = windowSize();
          div.style.position = 'absolute';
          div.style.width = size.width + 'px';
          div.style.height = size.height + 'px';
          div.style.setExpression('top', "(t=document.documentElement.scrollTop||document.body.scrollTop)+'px'");
          div.style.setExpression('left', "(l=document.documentElement.scrollLeft||document.body.scrollLeft)+'px'");
        }
        
        if((bo.ie && bo.quirks) || bo.ie6) {
          idiv.style.position = 'absolute';
          idiv.style.setExpression('top', "40+((t=document.documentElement.scrollTop||document.body.scrollTop))+'px'");
          idiv.style.setExpression('left', "40+((l=document.documentElement.scrollLeft||document.body.scrollLeft))+'px'");
        }
        
        document.body.appendChild(div);
        idiv.style.width = (div.offsetWidth - 80) +'px';
        idiv.style.height = (div.offsetHeight - 160) +'px';
        document.body.appendChild(idiv);
        document.body.appendChild(div_close);
        
        if (typeof options != 'undefined') {
          iframe.style.width = (div.offsetWidth - 80) +'px';
          iframe.style.height = (div.offsetHeight - 80) +'px';
          iframe.src = buildUrl(options);
          idiv.appendChild(iframe);
        }
        
        $('div.imageeditor-close').click(
          function() {
            Drupal.imageeditor.overlay.hide();
          }
        );
      },
      hide: function() {
        $('.imageeditor-external').remove();
        $('.imageeditor-internal').remove();
        $('.imageeditor-close').remove();
      }
    },
    popup: {
      show: function(options) {
        popup_window = window.open(buildUrl(options), 'imageeditor', 'location=no,menubar=no,resizable=yes,scrollbars=yes,status=no,titlebar=yes,toolbar=no,channelmode=yes,fullscreen=yes');
        popup_window.focus();
      }
    }
  };
}();
}
