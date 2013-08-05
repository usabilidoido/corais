
/**
 * @file
 * Common marker routines.
 */

/*global $, Drupal, GEvent, GInfoWindowTab, GLatLng, GLatLngBounds */

Drupal.gmap.addHandler('gmap', function (elem) {
  var obj = this;
  
  var infowindow = null;

  obj.bind('init', function () {
    if (obj.vars.behavior.autozoom) {
      obj.bounds = new google.maps.LatLngBounds();
    }
  });

  obj.bind('addmarker', function (marker) {
    marker.opts.position = new google.maps.LatLng(marker.latitude, marker.longitude);
    marker.opts.map = obj.map;
    var m = Drupal.gmap.factory.marker(marker.opts);
    marker.marker = m;
    google.maps.event.addListener(m, 'click', function () {
      obj.change('clickmarker', -1, marker);
    });
    if (obj.vars.behavior.highlight) {
      google.maps.event.addListener(m, 'mouseover', function () {
        var highlightColor = '#' + obj.vars.styles.highlight_color;
        highlightMarker(obj.map, marker, 'hoverHighlight', highlightColor);
      });
      google.maps.event.addListener(m, 'mouseout', function () {
        unHighlightMarker(obj.map, marker, 'hoverHighlight');
      });
    }
    if (obj.vars.behavior.extramarkerevents) {
      google.maps.event.addListener(m, 'mouseover', function () {
        obj.change('mouseovermarker', -1, marker);
      });
      google.maps.event.addListener(m, 'mouseout', function () {
        obj.change('mouseoutmarker', -1, marker);
      });
      google.maps.event.addListener(m, 'dblclick', function () {
        obj.change('dblclickmarker', -1, marker);
      });
    }
    /**
     * Perform a synthetic marker click on this marker on load.
     */
    if (marker.autoclick || (marker.options && marker.options.autoclick)) {
      obj.deferChange('clickmarker', -1, marker);
    }
    if (obj.vars.behavior.autozoom) {
      obj.bounds.extend(new google.maps.LatLng(marker.latitude, marker.longitude));
    }
    // If the highlight arg option is used in views highlight the marker.
    if (marker.opts.highlight == 1) {
      highlightMarker(obj.map, marker, 'viewHighlight', marker.opts.highlightcolor);
    }
  });

  // Default marker actions.
  obj.bind('clickmarker', function (marker) {
    // Close infowindow if open to prevent multiple windows
    if (infowindow != null){
      infowindow.close();
    }
    infowindow = new google.maps.InfoWindow();
    if (marker.text) {
      infowindow.setContent(marker.text);
      infowindow.open(obj.map, marker.marker);
    }
    // Info Window Query / Info Window Offset
    else if (marker.iwq || (obj.vars.iwq && typeof marker.iwo != 'undefined')) {
      var iwq, iwo;
      if (obj.vars.iwq) {
        iwq = obj.vars.iwq;
      }
      if (marker.iwq) {
        iwq = marker.iwq;
      }
      iwo = 0;
      if (marker.iwo) {
        iwo = marker.iwo;
      }
      // Create a container to store the cloned DOM elements.
      var el = document.createElement('div');
      // Clone the matched object, run through the clone, stripping off ids, and move the clone into the container.
      jQuery(iwq).eq(iwo).clone(false).find('*').removeAttr('id').appendTo(jQuery(el));
      marker.setContent(el);
      infowindow.open(obj.map, marker.marker);
    }
    // AJAX content
    else if (marker.rmt) {
      obj.rmtcache = obj.rmtcache || {};
      
      // Cached RMT.
      if (obj.rmtcache[marker.rmt]) {
        infowindow.setContent(data);
        infowindow.open(obj.map, marker.marker);
      }
      else {
        var uri = marker.rmt;
        // If there was a callback, prefix that.
        // (If there wasn't, marker.rmt was the FULL path.)
        if (obj.vars.rmtcallback) {
          uri = obj.vars.rmtcallback + '/' + marker.rmt;
        }
        // @Bevan: I think it makes more sense to do it in this order.
        // @Bevan: I don't like your choice of variable btw, seems to me like
        // @Bevan: it belongs in the map object, or at *least* somewhere in
        // @Bevan: the gmap settings proper...
        //if (!marker.text && Drupal.settings.loadingImage) {
        //  marker.marker.openInfoWindowHtml(Drupal.settings.loadingImage);
        //}
        $.get(uri, {}, function (data) {
          obj.rmtcache[marker.rmt] = data;
          marker.marker.openInfoWindowHtml(data);
        });
      }
    }
    // Tabbed content
    else if (marker.tabs) {
      var data = "";
      //tabs in an infowindow is no longer supported in API ver3. 
      for (var m in marker.tabs) {
        data += marker.tabs[m];
      }
      infowindow.setContent(data);
      infowindow.open(obj.map, marker.marker);
    }
    // No content -- marker is a link
    else if (marker.link) {
      open(marker.link, '_self');
    }
  });

  obj.bind('markersready', function () {
    // If we are autozooming, set the map center at this time.
    if (obj.vars.behavior.autozoom) {
      if (!obj.bounds.isEmpty()) {
        obj.map.fitBounds(obj.bounds);
        var listener = google.maps.event.addListener(obj.map, "idle", function() {
          if (obj.vars.maxzoom) {
            var maxzoom = parseInt(obj.vars.maxzoom)
            if (obj.map.getZoom() > maxzoom) obj.map.setZoom(maxzoom); 
            google.maps.event.removeListener(listener);             
          }
        });
      }
    }
  });

  obj.bind('clearmarkers', function () {
    // Reset bounds if autozooming
    // @@@ Perhaps we should have a bounds for both markers and shapes?
    if (obj.vars.behavior.autozoom) {
      obj.bounds = new google.maps.LatLngBounds();
    }
  });

  // @@@ TODO: Some sort of bounds handling for deletemarker? We'd have to walk the whole thing to figure out the new bounds...
});
