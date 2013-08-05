//This script will only be applied to the admin/content/book page
(function ($) {
/**
 * events to update interface
 */
$(document).ready(function() {
  //due to a previous glitch that loaded inefficiently, this will force it to only load once
  $('#book-admin-edit').parent().attr('id','od-book-edit');
 });

/**
 * behaviors specific to the outline designer for overloading functions
 */
Drupal.behaviors.outline_designer_book = function (context) {
  //collapse / expand functionality
  $('.od-toggle-open').bind('click', function(e){
    if ($(this).attr('alt') == 'open') {
      $(this).attr('alt','closed');
      $(this).attr('src',$(this).attr('src').replace('images/open.png','images/closed.png'));
      Drupal.settings.outline_designer.collapseList.push($(this).attr('id'));
      Drupal.outline_designer.toggle_expand($(this).parent().parent(),$(this).attr('alt'));
    }
    else {
      $(this).attr('alt','open');
      $(this).attr('src',$(this).attr('src').replace('images/closed.png','images/open.png'));
      Drupal.settings.outline_designer.collapseList.splice($.inArray($(this).attr('id'), Drupal.settings.outline_designer.collapseList), 1);
      Drupal.outline_designer.toggle_expand($(this).parent().parent(),$(this).attr('alt'));
      //if we opened something and pop-ed an item off the array we need to clean up for nested elements potentially being rendered as open
      Drupal.outline_designer.collapseInit();
    }
  });
  //set the active node id everytime an edit icon is clicked on
  $('.outline_designer_edit_button').bind('click',function(e){
    Drupal.settings.outline_designer.activeNid = $(this).attr('id').replace("-"," ").substring(5);
    
  });
  $("tr.draggable td").each(function(i){
    if((i+1) % 6 == 0 || (i+1) % 6 == 4 || (i+1) % 6 == 5) {
    this.style.display = 'none';
    }
  });  
  //replace text fields with span's w/ their same content
  $("#book-outline .form-text", context).each(function(){
    $(this).css('display','none');
    $(this).parent().append('<span id="'+ $(this).attr('id') +'-span">' + $(this).val() + '</span>');
  });
  //whenever you doubleclick on a title, switch it to the rename state
  $("#book-outline span").bind('dblclick',function(e){
    Drupal.settings.outline_designer.activeNid = $(this).attr('id').replace("edit-table-book-admin-","").replace("-title-span","");
    Drupal.outline_designer.form_render('rename');
  });
  //whenever you aren't active on a field, remove it
  $('#book-outline input').blur(function(){
    $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').css('display','');
  $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').val($('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').html());
  $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').css('display','none');
  });
  //if you hit enter, submit the title; if you hit esc then reset the field
  $('form').submit( function(){
   return false;
   } );
  $('#book-outline input').keydown(function(e){
    if(document.all) {
      e = event;
    }
      if(e.keyCode==13){  // Enter pressed
        Drupal.outline_designer_ops.rename_submit();
        return false;
      }  
      if(e.keyCode == 27){  // ESC pressed
      $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').css('display','');
  $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').val($('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').html());
  $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').css('display','none');
      }
    });
    //bind the context menu and set it's properties
  var unavailableContextMenuItems = Drupal.settings.outline_designer.unavailableContextMenuItems;
  var ContextOperations = Drupal.settings.outline_designer.operations;
  Drupal.settings.outline_designer.context_menu = [];
  // loop through all buttons and build menu dynamically
  for (od_button in ContextOperations) {
    // special case for nid as thats disabled
    if ($.inArray(od_button, unavailableContextMenuItems) == -1) {
      var tmp = {}
      // stupid but we need to nest this way for lib to work, account for nid too
      if (od_button == 'nid') {
        tmp[ContextOperations[od_button]['title']] = {icon: Drupal.settings.basePath + ContextOperations[od_button]['icon'], disabled:true };
        Drupal.settings.outline_designer.context_menu.push(tmp,$.contextMenu.separator);
      }
      else {
        tmp[ContextOperations[od_button]['title']] = {onclick:function(menuItem,menu) {Drupal.outline_designer.form_render(Drupal.outline_designer.get_key(menuItem.textContent)); }, icon: Drupal.settings.basePath + ContextOperations[od_button]['icon'], disabled:false };
        Drupal.settings.outline_designer.context_menu.push(tmp);
      }
    }
  }
  //binding isn't working in Opera / IE correctly or at all
    $('.outline_designer_edit_button').contextMenu(Drupal.settings.outline_designer.context_menu, {theme: Drupal.settings.outline_designer.theme, beforeShow: function () { if ($.inArray("nid", unavailableContextMenuItems) == -1) { $(this.menu).find('.context-menu-item-inner:first').css('backgroundImage','url(' + $("#node-" + Drupal.settings.outline_designer.activeNid +"-icon").attr('src') +')').empty().append("nid " + Drupal.settings.outline_designer.activeNid);}
      },
      useIframe: false,
      shadow: false
    });
  //whenever the screen gets altered, make sure we close everything that should be
  Drupal.outline_designer.collapseInit();
};

Drupal.outline_designer.get_active_title = function() {
  return $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').html();
};

Drupal.outline_designer.get_active_type = function() {
  return $("#node-" + Drupal.settings.outline_designer.activeNid +"-icon").attr('alt');
};

// scaling functionality that overloads default
Drupal.outline_designer.scale = function(scale){
  if(scale == 1 && Drupal.settings.outline_designer.factor != 2){
  Drupal.settings.outline_designer.factor = Drupal.settings.outline_designer.factor + 0.25;
  }else if(scale == -1 && Drupal.settings.outline_designer.factor != 1){
    Drupal.settings.outline_designer.factor = Drupal.settings.outline_designer.factor - 0.25;
  }else if(scale == 0){
    Drupal.settings.outline_designer.factor = 1;
  }
  //account for initial page load, stupid IE thing
  if(Drupal.settings.outline_designer.factor == null && scale == -2) {
    Drupal.settings.outline_designer.factor = 1;
  }
  if(Drupal.settings.outline_designer.factor == 1){
    $("#book-outline img").css('width','').css('height','');
    $("#book-outline").css('font-size','');
    $("#book-outline .form-item").css('margin-top','');
  }else{
    $("#book-outline img").css('width',Drupal.settings.outline_designer.factor + 'em').css('height',Drupal.settings.outline_designer.factor + 'em');
    $("#book-outline").css('font-size',Drupal.settings.outline_designer.factor + 'em');
    $("#book-outline .form-item").css('margin-top',(Drupal.settings.outline_designer.factor/4) + 'em');
  }
};

// quazi theme function for popup rendering
Drupal.outline_designer.render_popup = function(render_title) {
  var output = '';
  output+= '<img src="'+ Drupal.settings.basePath + Drupal.settings.outline_designer.operations[Drupal.settings.outline_designer.activeAction]['icon'] +'" class="od_statusbar_img" />';
  output+= Drupal.settings.outline_designer.operations[Drupal.settings.outline_designer.activeAction]['title'];
  // spec
  if (render_title == true) {
    if (Drupal.outline_designer.get_active_title().length > 20) {
      title = Drupal.outline_designer.get_active_title().substring(0,20) +'...';
    }
    else {
      title = Drupal.outline_designer.get_active_title();
    }
    output+= ' - <img src="'+  $("#node-" + Drupal.settings.outline_designer.activeNid +"-icon").attr('src') +'" class="od_statusbar_img" />'+ title + ' (nid:'+ Drupal.settings.outline_designer.activeNid +')';
  }
  else {
    output+= '<span class="tmpimage"></span><span class="tmptitle"></span>';
  }
  $('#od_'+ Drupal.settings.outline_designer.activeAction).appendTo('#od_popup .popup-content');
  $('#od_popup .popup-statusbar').html(output);
  $('#od_popup_overlay').css('display','block');
  $('#od_popup').css('display','block');
}
// define function for edit
  Drupal.outline_designer_ops.edit = function() {
    window.open(Drupal.settings.basePath + '?q=node/' + Drupal.settings.outline_designer.activeNid + '/edit','_blank');
  };
  // define function for view
  Drupal.outline_designer_ops.view = function() {
    window.open(Drupal.settings.basePath + '?q=node/' + Drupal.settings.outline_designer.activeNid,'_blank');
  };
  // define function for rename
  Drupal.outline_designer_ops.rename = function() {
    $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').css('display','none');
    $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').css('display','');
    $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').focus();
  };
  // define function for change_type
  Drupal.outline_designer_ops.change_type = function() {
    $(".od_submit_button").val('Change Type');
    // function call
    Drupal.outline_designer.render_popup(true);
    $("#od_change_type input.type_radio").val([Drupal.outline_designer.get_active_type()]);
    Drupal.settings.outline_designer.activeType = Drupal.outline_designer.get_active_type();
  };
  // define function for duplicate
  Drupal.outline_designer_ops.duplicate = function() {
    $("#od_duplicate_title").val("@title");
    $(".od_submit_button").val('Duplicate');
    Drupal.outline_designer.render_popup(true);
    $("#od_duplicate_title").focus();
  };
  // define function for add_content
  Drupal.outline_designer_ops.add_content = function() {
    $(".od_submit_button").val('Add Content');
    // function call
    Drupal.outline_designer.render_popup(false);
    $("#od_add_content input.type_radio").val([Drupal.settings.outline_designer.defaultType]);
    $("#od_add_content_title").focus();
    Drupal.settings.outline_designer.activeType = Drupal.settings.outline_designer.defaultType;
  };
  // define function for delete
  Drupal.outline_designer_ops.delete = function() {
    $(".od_submit_button").val('Delete');
    // function call
    Drupal.outline_designer.render_popup(true);
  };
  // submit handlers
  Drupal.outline_designer_ops.edit_submit = function() {};
  Drupal.outline_designer_ops.view_submit = function() {};
  Drupal.outline_designer_ops.rename_submit = function() {
    $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').css('display','');
    $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').css('display','none');
    if ($('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').html() != $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').val()) {
      var title = $.param($('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title'));
      var titleary = title.split('=',1);
      //need to remove the name space
      title = title.replace(titleary,'');
      title = title.substr(1);
      title = title.replace(/%2F/g,"@2@F@"); //weird escape for ajax with /
      title = title.replace(/%23/g,"@2@3@"); //weird escape for ajax with #
      title = title.replace(/%2B/g,"@2@B@"); //weird escape for ajax with +
      title = title.replace(/%26/g,"@2@6@"); // Fix ampersand issue &
      Drupal.outline_designer.ajax_call(Drupal.settings.outline_designer.type, 'rename', Drupal.settings.outline_designer.activeNid, title, null);
    }  
  };
  // submit handler for change type
  Drupal.outline_designer_ops.change_type_submit = function() {
    Drupal.outline_designer.ajax_call(Drupal.settings.outline_designer.type, 'change_type', Drupal.settings.outline_designer.activeNid, Drupal.settings.outline_designer.activeType, null, 'Drupal.outline_designer_ops.change_type_submit_success');
  };
  // callback function for change type submit
  Drupal.outline_designer_ops.change_type_submit_success = function(msg) {
    $("#reload_table").trigger('change');
    if(msg == 0) {
      Drupal.outline_designer.growl("You don't have sufficient permissions!");
    }
    else {
      $("#node-" + Drupal.settings.outline_designer.activeNid +"-icon").attr('src',Drupal.settings.basePath + Drupal.settings.outline_designer.types[Drupal.settings.outline_designer.activeType][1]);
      Drupal.outline_designer.growl(msg);
    }
  };
  Drupal.outline_designer_ops.duplicate_submit = function() {
    var dup_title = $.param($("#od_duplicate_title"));
    dup_title = dup_title.replace(/%2F/g,"@2@F@"); //weird escape for ajax with /
    dup_title = dup_title.replace(/%23/g,"@2@3@"); //weird escape for ajax with #
    dup_title = dup_title.replace(/%2B/g,"@2@B@"); //weird escape for ajax with +
    dup_title = dup_title.substr(1);
    var times_to_dup = Number($("#od_duplicate_number").val()) + 1.0;
    var tmp_dup_title = '';
    for(var i=0; i<times_to_dup; i++) {
      tmp_dup_title = dup_title.replace('%40i',(i+1)); //account for iteration token if used
      // standard ajax call
      Drupal.outline_designer.ajax_call(Drupal.settings.outline_designer.type, 'duplicate', Drupal.settings.outline_designer.activeNid, $('#od_duplicate_multiple:checked').length, tmp_dup_title);
      //this way the iteration doesn't trigger i times
      if (times_to_dup == (i+1)) {
        $("#reload_table").trigger('change');
      }
    }
  };
  Drupal.outline_designer_ops.add_content_submit = function() {
    var title = $.param($("#od_add_content_title"));
    title = title.replace(/%2F/g,"@2@F@"); //weird escape for ajax with /
    title = title.replace(/%23/g,"@2@3@"); //weird escape for ajax with #
    title = title.replace(/%2B/g,"@2@B@"); //weird escape for ajax with +
    title = title.substr(1);
    if (title == "") {
      alert(Drupal.t("You must enter a title in order to add content!"));
      return false;
    }
    else {
      Drupal.outline_designer.ajax_call(Drupal.settings.outline_designer.type, 'add_content', title, Drupal.settings.outline_designer.activeType, Drupal.settings.outline_designer.activeNid, 'Drupal.outline_designer_ops.add_content_submit_success');
    }
  };
  // callback function for add_content submit
  Drupal.outline_designer_ops.add_content_submit_success = function(msg) {
    var msg_split = new RegExp(";msg:");
    msg_split.test(msg);
    msg = RegExp.rightContext;
    if ($.inArray('collapse-'+ Drupal.settings.outline_designer.activeNid, Drupal.settings.outline_designer.collapseList) != -1) {
      Drupal.settings.outline_designer.collapseList.splice($.inArray('collapse-'+ Drupal.settings.outline_designer.activeNid, Drupal.settings.outline_designer.collapseList), 1);
    }
    $("#reload_table").trigger('change');
    if(msg == 0) {
      Drupal.outline_designer.growl("You don't have permissions to add this content");
    }
    else {
      Drupal.outline_designer.growl(msg);
    }
  };
  Drupal.outline_designer_ops.delete_submit = function() {
    var multiple = $('#od_delete_multiple:checked').length;
    Drupal.outline_designer.ajax_call(Drupal.settings.outline_designer.type, 'delete', Drupal.settings.outline_designer.activeNid, multiple, null);
  };
  
  // reset handlers
  Drupal.outline_designer_ops.edit_reset = function() {};
  Drupal.outline_designer_ops.view_reset = function() {};
  Drupal.outline_designer_ops.rename_reset = function() {};
  Drupal.outline_designer_ops.change_type_reset = function() {};
  Drupal.outline_designer_ops.duplicate_reset = function() {
    $("#od_duplicate_multiple").attr("checked", true);
  };
  Drupal.outline_designer_ops.add_content_reset = function() {
    $("#od_add_content_title").val('');
  };
  Drupal.outline_designer_ops.delete_reset = function() {
    $("#od_delete_multiple").attr("checked", false);
  };
  
  // special behaviors for the overlay
  Drupal.behaviors.outlineDesignerChangeType = function (context) {
    $("#od_change_type input.type_radio", context).click(function(e){
      Drupal.settings.outline_designer.activeType = $(this).val();
    });
  };
  Drupal.behaviors.outlineDesignerAddContentTitle = function (context) {
    $("#od_add_content_title", context).keyup(function(e){
      $(".popup-statusbar .tmptitle").empty().append($("#od_add_content_title").val());
    });
  };
  Drupal.behaviors.outlineDesignerAddContent = function (context) {
    $("#od_add_content input.type_radio", context).click(function(e){
      Drupal.settings.outline_designer.activeType = $(this).val();
      $(".popup-statusbar .tmpimage", context).empty().append($(this).next().clone());
    });
  };
})(jQuery);