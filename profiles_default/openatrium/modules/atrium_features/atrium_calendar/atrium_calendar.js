if (Drupal.jsEnabled) {
  $(document).ready(function() {
  $("div.atrium_calendar_event_type div.tid-options").toggle();
  $("#edit-tid").toggle();
    if ( $('#edit-tid').size() > 0 ) {
      atrium_calendar_event_type_init();
    }
  });
}

function atrium_calendar_event_type_init () {
  // Add "selected" class to selected terms
  var active = $('#edit-tid').val();
  if (active){
  for(var i=0; i<active.length; i++){
      $('div.atrium_calendar_event_type div.tid-options').children('div').children('span.tid-'+active[i]).addClass('selected').removeClass('deactive');
  }
  }else{
    var mySelect = document.getElementById("edit-tid");
    for (var i = 0; i < mySelect.options.length; i++){
      var option = mySelect.options[i];
      $('div.atrium_calendar_event_type div.tid-options').children('div').children('span.tid-'+option.value).addClass('selected').removeClass('deactive');
      document.getElementById("edit-tid").options[i].selected = true;
    }
  }
  // Add click handler
  $('div.atrium_calendar_event_type div.tid-options').children('div').children('span').click(function(){
    var mySelect = document.getElementById("edit-tid");
    var classes = $(this).attr('class').split(' ');
    var tid;
    
    $.each(classes, function(i ,n) {
      var pos = n.search('tid-');
      if (pos != -1) {
        tid = n.substr(pos+4);
        return;
      };
    });
    
    var selectId;
    for (var i = 0; i < mySelect.options.length; i++){
      var option = mySelect.options[i];
      if (option.value == tid){
        selectId = i;
      }
    }

    if ($(this).is('.selected')){
      document.getElementById("edit-tid").options[selectId].selected = false;
    }
    else{
      document.getElementById("edit-tid").options[selectId].selected = true;
    }
    
    $(this).toggleClass('selected').toggleClass('deactive');
  });
}
