Drupal.behaviors.initImagepickerThickbox = function(context) {
  var settings = Drupal.settings.imagepicker_thickbox;
  var myq = "?KeepThis=true&TB_iframe=true&height="+settings.height+"&width="+settings.width+"&";
  $("#imgp_tb a[href$='/imagepicker']").addClass('thickbox').each(function() {
    this.href = this.href.replace(/imagepicker/,"imagepicker"+myq);
  });
  myq = "&KeepThis=true&TB_iframe=true&height="+settings.height+"&width="+settings.width+"";
  $("#imgp_tb a[href$='?q=imagepicker']").addClass('thickbox').each(function() {
    this.href = this.href.replace(/imagepicker/,"imagepicker"+myq);
  });
}
