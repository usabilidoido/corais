/************************************************************************************************************
//ELMS: Outline Designer - Usability improvements for speedy outline creation
//Copyright (C) 2008-2010  The Pennsylvania State University
//
//Bryan Ollendyke
//bto108@psu.edu
//
//Keith D. Bailey
//kdb163@psu.edu
//
//12 Borland
//University Park, PA 16802

************************************************************************************************************/  
//This script will only be applied tothe admin/content/book page
(function ($) {
Drupal.settings.outline_designer = new Array();

//extend the drupal js object by adding in an outline_designer name-space
Drupal.outline_designer = Drupal.outline_designer || { functions: {} };

/**
 * Get rid of tabledrag messages about needing to save
 */ 
Drupal.theme.tableDragChangedWarning = function () { 
  return '<div></div>';
};
  
/**
 * events to update interface
 */
$(document).ready(function() {
  //due to a previous glitch that loaded inefficiently, this will force it to only load once
  $('#book-admin-edit').parent().attr('id','od-book-edit');
  $('body').append('<div id="od_growl" align="center"></div>');
  //if everything's been told to close, close it all
  if (Drupal.settings.outline_designer.collapseToggle == 1) {
    Drupal.outline_designer.collapseAll();
  }
  document.onkeyup = function(e) {
    if(document.all) {
			e = event;
		}
    if(e.keyCode==27){  // ESC pressed
      Drupal.outline_designer.ui_reset();
    }
  };
 });
 
//popup event listeners
Drupal.behaviors.outlineDesignerCancelButton = function (context) {
  $('.od_cancel_button', context).click(function(){ 
    Drupal.outline_designer.ui_reset();
  });
};
Drupal.behaviors.outlineDesignerSubmitButton = function (context) {
  $('.od_submit_button', context).click(function(){
    Drupal.outline_designer.form_submit(Drupal.settings.outline_designer.activeAction);
  });
};
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
Drupal.behaviors.outlineDesignerCloseOverlay = function (context) {
  //things for when the popup is active to give ways of closing it
  $('#od_popup_overlay', context).click(function(){
    Drupal.outline_designer.ui_reset();
  });
};
Drupal.behaviors.outlineDesignerEscapeOverlay = function (context) {
  $('#od_popup', context).keydown(function(e){
    if(document.all) {
			e = event;
		}
    if(e.keyCode == 13){  // Enter pressed
    Drupal.outline_designer.form_submit(Drupal.settings.outline_designer.activeAction);
    return false;
    }
  });
};

/**
 * behaviors specific to the outline designer for overloading functions
 */
Drupal.behaviors.outline_designer = function (context) {
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
    $.ajax({
      type: "POST",
      url: Drupal.settings.outline_designer.ajaxPath + Drupal.settings.outline_designer.token +"/rename/" + Drupal.settings.outline_designer.activeNid + "/" + title,
      success: function(msg){
        $("#reload_table").trigger('change');
        if(msg == 0) {
          Drupal.outline_designer.growl("You don't have sufficient permissions!");
        }
        else {
          Drupal.outline_designer.growl(msg);
        }
      }
    });
  }
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
  Drupal.settings.outline_designer.context_menu = [];
  if ($.inArray("nid", unavailableContextMenuItems) == -1) {  
      Drupal.settings.outline_designer.context_menu.push({"Node":{icon: Drupal.settings.outline_designer.path +"images/node.png",disabled:true}},$.contextMenu.separator);
  }

  if ($.inArray("add_content", unavailableContextMenuItems) == -1) {
      Drupal.settings.outline_designer.context_menu.push({"Add Content":{ onclick:function(menuItem,menu) { Drupal.outline_designer.form_render('add_content'); }, icon: Drupal.settings.outline_designer.path +"images/add_content.png", disabled:false } });
  }
  if ($.inArray("rename", unavailableContextMenuItems) == -1) {
      Drupal.settings.outline_designer.context_menu.push({"Rename":{ onclick:function(menuItem,menu) { Drupal.outline_designer.form_render('rename'); }, icon: Drupal.settings.outline_designer.path +"images/rename.png", disabled:false  } });
  }
  if ($.inArray("edit", unavailableContextMenuItems) == -1) {
      Drupal.settings.outline_designer.context_menu.push({"Edit":{ onclick:function(menuItem,menu) { Drupal.outline_designer.form_render('edit'); }, icon: Drupal.settings.outline_designer.path +"images/edit.png", disabled:false  } });
  }
  if ($.inArray("view", unavailableContextMenuItems) == -1) {
      Drupal.settings.outline_designer.context_menu.push({"View":{ onclick:function(menuItem,menu) { Drupal.outline_designer.form_render('view'); }, icon: Drupal.settings.outline_designer.path +"images/view.png", disabled:false } });
  }
  if ($.inArray("delete", unavailableContextMenuItems) == -1) {
      Drupal.settings.outline_designer.context_menu.push({"Delete":{ onclick:function(menuItem,menu) { Drupal.outline_designer.form_render('delete'); }, icon: Drupal.settings.outline_designer.path +"images/delete.png", disabled:false } });
  }
  if ($.inArray("duplicate", unavailableContextMenuItems) == -1) {
      Drupal.settings.outline_designer.context_menu.push({"Duplicate":{ onclick:function(menuItem,menu) { Drupal.outline_designer.form_render('duplicate'); }, icon: Drupal.settings.outline_designer.path +"images/duplicate.png", disabled:false  } });
  }
  if ($.inArray("change_type", unavailableContextMenuItems) == -1) {
      Drupal.settings.outline_designer.context_menu.push({"Change Type":{ onclick:function(menuItem,menu) { Drupal.outline_designer.form_render('change_type'); }, icon: Drupal.settings.outline_designer.path +"images/change_type.png", disabled:false } });
  }

  //binding isn't working in Opera / IE correctly or at all
    $('.outline_designer_edit_button').contextMenu(Drupal.settings.outline_designer.context_menu, {theme: Drupal.settings.outline_designer.theme,
												   beforeShow: function () { if ($.inArray("nid", unavailableContextMenuItems) == -1) { $(this.menu).find('.context-menu-item-inner:first').css('backgroundImage','url(' + $("#node-" + Drupal.settings.outline_designer.activeNid +"-icon").attr('src') +')').empty().append("nid " + Drupal.settings.outline_designer.activeNid);}
      },
      useIframe: false,
      shadow: false
    });
  //whenever the screen gets altered, make sure we close everything that should be
  Drupal.outline_designer.collapseInit();
};

//handle rendering of the form
Drupal.outline_designer.form_render = function(render_item) {
  Drupal.settings.outline_designer.activeAction = render_item;
  switch(render_item) {
    case 'edit':
      window.open(Drupal.settings.basePath + '?q=node/' + Drupal.settings.outline_designer.activeNid + '/edit','_blank');
    break;
    case 'view':
      window.open(Drupal.settings.basePath + '?q=node/' + Drupal.settings.outline_designer.activeNid,'_blank');
    break;
    case 'rename':
      $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').css('display','none');
      $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').css('display','');
      $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title').focus();
    break;
    //do this for the rest
    default:
      if (render_item == 'change_type') {
        $(".od_submit_button").val('Change Type');
      }
      else if (render_item == 'duplicate') {
		$("#od_duplicate_title").val("@title");
        $(".od_submit_button").val('Duplicate');
      }
      else if (render_item == 'add_content') {
        $(".od_submit_button").val('Add Content');
      }
      else if (render_item == 'delete') {
        $(".od_submit_button").val('Delete');
      }
      var output = '';
      output+= '<img src="'+ Drupal.settings.outline_designer.path +"images/"+ render_item +".png" +'" class="od_statusbar_img" />';
      output+= render_item.charAt(0).toUpperCase() + render_item.slice(1).replace('_',' ');
      if (render_item != 'add_content') {
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
      $('#od_'+ render_item).appendTo('#od_popup .popup-content');
      $('#od_popup .popup-statusbar').html(output);
      $('#od_popup_overlay').css('display','block');
      $('#od_popup').css('display','block');
      if (render_item == 'add_content') {
        $("#od_add_content input.type_radio").val([Drupal.settings.outline_designer.defaultType]);
        $("#od_add_content_title").focus();
        Drupal.settings.outline_designer.activeType = Drupal.settings.outline_designer.defaultType;
      }
      else if (render_item == 'change_type') {
        $("#od_change_type input.type_radio").val([Drupal.outline_designer.get_active_type()]);
      Drupal.settings.outline_designer.activeType = Drupal.outline_designer.get_active_type();
      }
      else if (render_item == 'duplicate') {
        $("#od_duplicate_title").focus();
      }
    break;
  }
};

//handle submission of any of the forms, most likely via ajax
Drupal.outline_designer.form_submit = function(submit_item) {
  switch (submit_item) {
    case 'add_content':
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
        $.ajax({
          type: "POST",
          url: Drupal.settings.outline_designer.ajaxPath + Drupal.settings.outline_designer.token +"/add_content/" + title + "/" + Drupal.settings.outline_designer.activeType + "/" + Drupal.settings.outline_designer.activeNid,
          success: function(msg){
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
          }
        });
      }
    break;
    case 'delete':
      var multiple = $('#od_delete_multiple:checked').length;
  $.ajax({
    type: "POST",
    url: Drupal.settings.outline_designer.ajaxPath + Drupal.settings.outline_designer.token +"/delete/" + Drupal.settings.outline_designer.activeNid + "/" + multiple,
    success: function(msg){
      $("#reload_table").trigger('change');
      if(msg == 0) {
        Drupal.outline_designer.growl("You don't have sufficient permissions!");
      }
      else {
        Drupal.outline_designer.growl(msg);
      }
    }
  });  
    break;
    case 'duplicate':
	  var dup_title = $.param($("#od_duplicate_title"));
	  dup_title = dup_title.replace(/%2F/g,"@2@F@"); //weird escape for ajax with /
	  dup_title = dup_title.replace(/%23/g,"@2@3@"); //weird escape for ajax with #
	  dup_title = dup_title.replace(/%2B/g,"@2@B@"); //weird escape for ajax with +
	  dup_title = dup_title.substr(1);
	  var times_to_dup = Number($("#od_duplicate_number").val()) + 1.0;
	  var tmp_dup_title = '';
	  for(var i=0; i<times_to_dup; i++) {
		tmp_dup_title = dup_title.replace('%40i',(i+1)); //account for iteration token if used
        $.ajax({
        type: "POST",
        url: Drupal.settings.outline_designer.ajaxPath + Drupal.settings.outline_designer.token +"/duplicate/" + Drupal.settings.outline_designer.activeNid + "/" + $('#od_duplicate_multiple:checked').length + "/" + tmp_dup_title,
        success: function(msg){
          if(msg == 0) {
            Drupal.outline_designer.growl("You don't have sufficient permissions!");
          }
          else {
            Drupal.outline_designer.growl(msg);
          }
        }
        });
		//this way the iteration doesn't trigger i times
	    if (times_to_dup == (i+1)) {
		  $("#reload_table").trigger('change');
	    }
      }
    break;
    case 'change_type':
      $.ajax({
        type: "POST",
        url: Drupal.settings.outline_designer.ajaxPath + Drupal.settings.outline_designer.token +"/change_type/" + Drupal.settings.outline_designer.activeNid + '/' + Drupal.settings.outline_designer.activeType,
        success: function(msg){
          if(msg == 0) {
						Drupal.outline_designer.growl("You don't have sufficient permissions!");
					}
					else {
            $("#node-" + Drupal.settings.outline_designer.activeNid +"-icon").attr('src',Drupal.settings.basePath + Drupal.settings.outline_designer.types[Drupal.settings.outline_designer.activeType][1]);
            Drupal.outline_designer.growl(msg);
          }
        }
      });
    break;
    default:
      alert(Drupal.t("Can't Submit "+ submit_item));
    break;
  }
  Drupal.outline_designer.ui_reset();
};

//remove the interface and get it set for the next action
Drupal.outline_designer.ui_reset = function() {
  $('#od_popup').css('display','none');
  $('#od_popup_overlay').css('display','none');
  $('#od_popup .popup-statusbar').html('');
  $('#od_popup .popup-content .od_uiscreen').appendTo('#od_popup_toolbox');
  //reset default settings for all input fields
  $("#od_duplicate_multiple").attr("checked", true);
  $("#od_delete_multiple").attr("checked", false);
  $("#od_popup_toolbox input.type_radio").attr('checked',false);
  $("#od_add_content_title").val('');
  //reset button names
  $(".od_submit_button").val('Save');
};

Drupal.outline_designer.growl_helper = function() {
	$(".od_msg:last").remove();
};

Drupal.outline_designer.growl = function(msg) {
  $("#od_growl").prepend('<div class="od_msg messages status">' + msg + '</div>');  
  setTimeout(Drupal.outline_designer.growl_helper, 2000);
};

Drupal.outline_designer.get_active_title = function() {
  return $('#edit-table-book-admin-' + Drupal.settings.outline_designer.activeNid + '-title-span').html();
};

Drupal.outline_designer.get_active_type = function() {
  return $("#node-" + Drupal.settings.outline_designer.activeNid +"-icon").attr('alt');
};

//scaling functionality
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
//expand / collapse functionality
Drupal.outline_designer.toggle_expand = function(obj,state) {
  var depth = obj.children().children('div.indentation').size();
  var traveldepth = 100;
  var tmpobj = obj;
  while (depth < traveldepth){
    tmpobj = tmpobj.next();
    traveldepth = tmpobj.children().children('div.indentation').size();
    if (depth < traveldepth) {
      if(state == 'closed') {
        tmpobj.css('display','none');
      }
      else {
        tmpobj.css('display','block');
        //if something's marked closed we want to flip that to open too to make things easier. Init is then run after the fact to make sure everything's closed that should be
        if (tmpobj.children().children('img.od-toggle-open').attr('alt') == 'closed') {
          tmpobj.children().children('img.od-toggle-open').attr('alt','open');
          tmpobj.children().children('img.od-toggle-open').attr('src',tmpobj.children().children('img.od-toggle-open').attr('src').replace('images/closed.png','images/open.png'));
        }
      }
    }
  }
};

//everything will get returned as open by default so go through and collapse things that have been collapsed
Drupal.outline_designer.collapseInit = function() {
  for(var i in Drupal.settings.outline_designer.collapseList) {
    if ($('#'+ Drupal.settings.outline_designer.collapseList[i]).length == 1) {
      $('#'+ Drupal.settings.outline_designer.collapseList[i]).attr('alt','closed');
      $('#'+ Drupal.settings.outline_designer.collapseList[i]).attr('src',$('#'+ Drupal.settings.outline_designer.collapseList[i]).attr('src').replace('images/open.png','images/closed.png'));
      Drupal.outline_designer.toggle_expand($('#'+ Drupal.settings.outline_designer.collapseList[i]).parent().parent(),$('#'+ Drupal.settings.outline_designer.collapseList[i]).attr('alt'));
    }
  }
  //scale interface here as well. -2 is so that it ignores the scale and keeps the current global
  Drupal.outline_designer.scale(-2);
};

//Collapse all branches
Drupal.outline_designer.collapseAll = function() {
  $('.od-toggle-open').each(function(){
    $(this).attr('alt','closed');
    $(this).attr('src',$(this).attr('src').replace('images/open.png','images/closed.png'));
    //only push if it's not in the list already
    if ($.inArray($(this).attr('id'), Drupal.settings.outline_designer.collapseList) == -1) {
      Drupal.settings.outline_designer.collapseList.push($(this).attr('id'));
    }
    Drupal.outline_designer.toggle_expand($(this).parent().parent(),$(this).attr('alt'));
  });
};

//Open all branches
Drupal.outline_designer.openAll = function() {
  $('.od-toggle-open').each(function(){
    $(this).attr('alt','open');
    $(this).attr('src',$(this).attr('src').replace('images/closed.png','images/open.png'));
    Drupal.settings.outline_designer.collapseList = new Array();
    Drupal.outline_designer.toggle_expand($(this).parent().parent(),$(this).attr('alt'));
  });
};
})(jQuery);