function resizeFrame(obj) {
  var frame=obj.contentDocument ? obj.contentDocument : obj.contentWindow.document;
  var modernBrowser = navigator.userAgent.search(/MSIE [1-8]\./) == -1 ? true : false;
  var padding = modernBrowser ? 60 : 120;
  var minHeight = 525;
  var is_course_outline = frame.getElementById('topicOutline') ? true : false;
  // must check for mediaplugin string, as that content is inserted via js and so we must delay our resize
  var js_content = frame.body.innerHTML.search('mediaplugin') != -1 ? true : false;
  function resizeNow() {
    if (is_course_outline && !modernBrowser) {
      // problem with IE and calculating the course outline iframe
      var newHeight=400;
    } else {
      var newHeight=frame.getElementById('container-highcharts') ? 700 : frame.body.offsetHeight;
    }
    var height = newHeight + padding;
    if (!modernBrowser && height < minHeight) height = minHeight;
    obj.style.height = height + 'px';
  }
  if (js_content) {
    setTimeout(resizeNow,3000);
  } else {
    resizeNow();
  }
  frame.getElementById('page').style.visibility = 'visible';
  var winBtns = frame.body.getElementsByTagName('input');
  for (var i=0;i<winBtns.length;i++) {
    if (winBtns[i].name == 'mform_showadvanced' || winBtns[i].name=='credit_application_required') {
      winBtns[i].onmouseup = function() {
        setTimeout(resizeNow,50);
      }
    }
  }
}
