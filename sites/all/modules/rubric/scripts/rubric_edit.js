//ELMS: Rubric - This is a plugin for the Assignment Studio to add rubric functionality making assessment much faster.  Also has assessment creation tool sets.
//Copyright (C) 2008  The Pennsylvania State University
//
//Bryan Ollendyke
//bto108@psu.edu
//
//Keith D. Bailey
//kdb163@psu.edu
//
//12 Borland
//University Park,  PA 16802
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License,  or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License along
//with this program; if not,  write to the Free Software Foundation,  Inc.,
//51 Franklin Street,  Fifth Floor,  Boston,  MA 02110-1301 USA.

//extend the drupal js object by adding in an assignment_studio name-space
/*
Issues to resolve --
 -Implementation
 --Start to hook into node
 --hook into database
*/
Drupal.rubric = Drupal.rubric || { functions: {} };

$(document).ready(function(){
  $('*').keydown(function(e){
	Drupal.rubric.functions.shortcut_keys(e);
  });
  $('#rubric_gui_points').keydown(function(e){
	if ((e.keyCode >= 48 && e.keyCode <= 57) || e.keyCode==8 || e.keyCode==9 || e.keyCode==46 || (e.keyCode>=96 && e.keyCode<=105) || e.keyCode==110 || e.keyCode==190 || (e.keyCode>=37 && e.keyCode<=40)) {
    }
	else {
	  return false;	
	}
  }).change(function(){
	  $.ajax({
      type: "POST",
	  url: Drupal.settings.basePath + "?q=rubric/ajax/edit/level/" + Drupal.settings.rubric.active_id.replace('rubric_level_','') +"/points/"+ $('#rubric_gui_points').val() +"/",
	  success: function(msg) {
	    $('#'+ Drupal.settings.rubric.active_id +' .rubric_category_criterion_level_points').html($('#rubric_gui_points').val());
	var length = $('#'+ Drupal.settings.rubric.active_id).parent().children().length;
	for(var i=0; i<length; i++){
	  for(var j=0; j<length; j++) {
        a = parseInt($('#'+ Drupal.settings.rubric.active_id).parent().children().eq(i).children('.level_gui').children('.rubric_category_criterion_level_points').html());
		a_obj = $('#'+ Drupal.settings.rubric.active_id).parent().children().eq(i);
	    b = parseInt($('#'+ Drupal.settings.rubric.active_id).parent().children().eq(j).children('.level_gui').children('.rubric_category_criterion_level_points').html());
	    b_obj = $('#'+ Drupal.settings.rubric.active_id).parent().children().eq(j);
		if (a > b) {
		  a_obj.clone(true).insertAfter(b_obj);
		  b_obj.clone(true).insertAfter(a_obj);
		  a_obj.remove();
		  b_obj.remove();
		}
	  }
	}
	//once we're done moving stuff around should perform the reweighting query to make sure the database aligns w. the screen
      },
	});
  });
  $('#rubric_gui_title').keydown(function(e){
    if (e.keyCode == 13) {
      $('#rubric_gui_title').change();
	  return false;	
	}
  }).change(function(e){
	var element, el_id, el_field;
	var title = $('#rubric_gui_title').val();
	title = Drupal.rubric.functions.scrub_feedback(title);
	switch(Drupal.settings.rubric.active_title){
	  case 'rubric_category_title':
	    element = 'category';
		el_id = Drupal.settings.rubric.active_id.replace('rubric_category_','');
		el_field = 'title';
	  break;
	  case 'rubric_category_criterion_title':
	    element = 'criterion';
		el_id = Drupal.settings.rubric.active_id.replace('rubric_criterion_','');
		el_field = 'title';
	  break;
	  case 'rubric_category_criterion_level_title':
	    element = 'level';
		el_id = Drupal.settings.rubric.active_id.replace('rubric_level_','');
		el_field = 'text';
	  break;
	}
	$.ajax({
      type: "POST",
	  url: Drupal.settings.basePath + "?q=rubric/ajax/edit/"+ element +"/" + el_id +"/"+ el_field +"/"+ title +"/",
	  success: function(msg) {
	    $('#'+ Drupal.settings.rubric.active_id +' .'+ Drupal.settings.rubric.active_title).html($('#rubric_gui_title').val());
		Drupal.rubric.functions.equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
      },
	});
  });
  $('#rubric_gui_description').change(function(e){
	var element, el_id, el_field;
	var description = $('#rubric_gui_description').val();
	description = Drupal.rubric.functions.scrub_feedback(description);
	switch(Drupal.settings.rubric.active_description){
	  case 'rubric_category_description':
	    element = 'category';
		el_id = Drupal.settings.rubric.active_id.replace('rubric_category_','');
		el_field = 'description';
	  break;
	  case 'rubric_category_criterion_description':
	    element = 'criterion';
		el_id = Drupal.settings.rubric.active_id.replace('rubric_criterion_','');
		el_field = 'description';
	  break;
	  case 'rubric_category_criterion_level_description':
	    element = 'level';
		el_id = Drupal.settings.rubric.active_id.replace('rubric_level_','');
		el_field = 'suggested_feedback';
	  break;
	}
	$.ajax({
      type: "POST",
	  url: Drupal.settings.basePath + "?q=rubric/ajax/edit/"+ element +"/" + el_id +"/"+ el_field +"/"+ description +"/",
	  success: function(msg) {
	    $('#'+ Drupal.settings.rubric.active_id +' .'+ Drupal.settings.rubric.active_description).html($('#rubric_gui_description').val());
		Drupal.rubric.functions.equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
      },
	});
  });
  //bind all of the info click buttons to the popups
  $('.rubric_image_info').click(function(){
	if (Drupal.settings.rubric.mode == 'preview') {
      $('#rubric_tooltip_title').html($(this).prev().prev().html());
      $('#rubric_tooltip_description').html($(this).prev().html());
      $('#rubric_tooltip').css({display: 'block', top: this.offsetTop + 13, left: this.offsetLeft + 13});
	}
  });
  
  //tool tip close
  $('#rubric_tooltip_close').click(function(){
	if (!$('#rubric_gui_description').hasClass('rubric_gui_show')) { 
      $('#rubric_tooltip').css('display','');
	  $('#rubric_tooltip_title').html('');
	  $('#rubric_tooltip_description').html('');
	}
  });

  //GUI
  $('.rubric_gui_mode_option').click(function() {										  
	Drupal.rubric.functions.set_mode(this.id.replace('rubric_gui_mode_',''));
  });
  $('.rubric_category_criterion_level, .rubric_category_criterion_header, .rubric_category_header').click(function(){
    if ($(this).hasClass('rubric_category_criterion_level')) {
      Drupal.settings.rubric.active_id = this.id;
	  $('.rubric_image_arrow-up, .rubric_image_arrow-down').css('display','');
	  $('#rubric_gui_points_text, #rubric_gui_points').css('display','inline');
	}
	else {
	  Drupal.settings.rubric.active_id = $(this).parent().attr('id');
	  $('.rubric_image_arrow-up, .rubric_image_arrow-down').css('display','inline');
	  $('#rubric_gui_points_text, #rubric_gui_points').css('display','');
	}
	switch(Drupal.settings.rubric.mode) {
      case 'preview':
	    if ($(this).hasClass('rubric_category_criterion_level')) {
          $(this).siblings().removeClass('rubric_category_criterion_level_selected');
          $(this).addClass('rubric_category_criterion_level_selected');
		  var total = 0;
		  var feedback = '';
		  $('.rubric_category_criterion_level_selected .rubric_category_criterion_level_points').each(function(){
            total = total + parseInt($(this).html());
          });
		  $('#rubric_gui_preview_points').html('Score: ' + total.toString(10));
		  $('.rubric_category_criterion_level_selected .rubric_category_criterion_level_description').each(function(){
            feedback = feedback + '<br/>' + $(this).html();
          });
		  $('#rubric_gui_preview_feedback').html(feedback);
		}
	  break;
	  case 'add':
        if ($(this).hasClass('rubric_category_header')) {
          var clone = $('#'+ Drupal.settings.rubric.active_id).clone(true).insertAfter($('#'+ Drupal.settings.rubric.active_id));
		  $.ajax({
		      type: "POST",
	          url: Drupal.settings.basePath + "?q=rubric/ajax/add/category/" + Drupal.settings.rubric.active_id.replace('rubric_category_','') +"/"+ Drupal.settings.rubric.nid +"/",
		      success: function(msg) {
				  var new_category_id = 'rubric_category_' + msg;
			      clone.attr('id',new_category_id);
				  clone.children('.rubric_category_criteria').children().each(function(){
					var active_criterion = this;
					$.ajax({
					  type: "POST",
					  url: Drupal.settings.basePath + "?q=rubric/ajax/add/criterion/" + $(active_criterion).attr('id').replace('rubric_criterion_','') +"/"+ new_category_id.replace('rubric_category_','') +"/",
					  success: function(msg) {
						var active_criterion_id = 'rubric_criterion_' + msg;
						$(active_criterion).attr('id',active_criterion_id);
						$(active_criterion).children('.rubric_category_criterion_levels').children().each(function(){
						  var child_obj = this;
						  $.ajax({
							type: "POST",
							url: Drupal.settings.basePath + "?q=rubric/ajax/add/level/" + $(child_obj).attr('id').replace('rubric_level_','') +"/"+ active_criterion_id.replace('rubric_criterion_','') +"/",
							success: function(msg) {
							  $(child_obj).attr('id','rubric_level_' + msg);
							},
						  });
						});
					  },
					});
				  });
				  Drupal.rubric.functions.reweight();
			  }
		  });
        }
        else if ($(this).hasClass('rubric_category_criterion_header')) {
		  $.ajax({
		      type: "POST",
	          url: Drupal.settings.basePath + "?q=rubric/ajax/add/criterion/" + Drupal.settings.rubric.active_id.replace('rubric_criterion_','') +"/"+ $('#'+Drupal.settings.rubric.active_id).parent().parent().attr('id').replace('rubric_category_','') +"/",
		      success: function(msg) {
				active_criterion_id = 'rubric_criterion_' + msg;
	            clone = $('#'+Drupal.settings.rubric.active_id).clone(true).insertAfter($('#'+Drupal.settings.rubric.active_id));
				clone.attr('id',active_criterion_id);
				clone.children('.rubric_category_criterion_levels').children().each(function(){
				  var child_obj = this;
				  $.ajax({
				    type: "POST",
					url: Drupal.settings.basePath + "?q=rubric/ajax/add/level/" + $(child_obj).attr('id').replace('rubric_level_','') +"/"+ active_criterion_id.replace('rubric_criterion_','') +"/",
					success: function(msg) {
				      $(child_obj).attr('id','rubric_level_' + msg);
					},
				  });
				});
				Drupal.rubric.functions.reweight();
			  },
		    });
        }
        else {
          if ($(this).hasClass('rubric_8-level')) {
            alert('8 is the max number of levels that you can have in any given criterion');
          }
          else {
            for(var i=2; i<8; i++) {
              if ($(this).hasClass('rubric_'+ i.toString(10) +'-level')) {
		        $(this).removeClass('rubric_'+ i.toString(10) +'-level');
		        $(this).siblings().removeClass('rubric_'+ i.toString(10) +'-level');
		        $(this).siblings().addClass('rubric_'+ (i+1).toString(10) +'-level');
		        $(this).addClass('rubric_'+ (i+1).toString(10) +'-level');
		        i=8;
	          }
            }
			$.ajax({
		      type: "POST",
	          url: Drupal.settings.basePath + "?q=rubric/ajax/add/level/" + Drupal.settings.rubric.active_id.replace('rubric_level_','') +"/"+ $('#'+Drupal.settings.rubric.active_id).parent().parent().attr('id').replace('rubric_criterion_','') +"/",
		      success: function(msg) {
	            clone = $('#'+ Drupal.settings.rubric.active_id).clone(true).insertAfter('#'+ Drupal.settings.rubric.active_id).removeClass('rubric_gui_active_'+ Drupal.settings.rubric.mode);
			    clone.attr('id','rubric_level_' + msg);
				Drupal.rubric.functions.equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
			  },
		    });
          }
        }
      break;
	  case 'edit':
	  $('.rubric_gui_active_edit').removeClass('rubric_gui_active_edit');
	  $('.rubric_gui_active_edit_click').removeClass('rubric_gui_active_edit_click');
	  $(this).addClass('rubric_gui_active_edit_click');
	  $('#rubric_gui_title').attr('disabled','');
	  $('#rubric_gui_description').attr('disabled','');
	  if ($(this).hasClass('rubric_category_criterion_level')) {
		$('#rubric_gui_points').attr('disabled','');
	    $('#rubric_gui_points').val($(this).children('.level_gui').children('.rubric_category_criterion_level_points').html()).focus().select();
	    $('#rubric_gui_title').val($(this).children('.rubric_category_criterion_level_title').html());
	   $('#rubric_gui_description').val($(this).children('.rubric_category_criterion_level_description').html());
	    Drupal.settings.rubric.active_title = 'rubric_category_criterion_level_title';
        Drupal.settings.rubric.active_description = 'rubric_category_criterion_level_description';
	  }
	  else if ($(this).hasClass('rubric_category_criterion_header')) {
		$('#rubric_gui_points').attr('disabled','disabled');
		$('#rubric_gui_points').val('');
		$('#rubric_gui_title').val($(this).children('.rubric_category_criterion_title').html()).focus().select();
		$('#rubric_gui_description').val($(this).children('.rubric_category_criterion_description').html());
		Drupal.settings.rubric.active_title = 'rubric_category_criterion_title';
		Drupal.settings.rubric.active_description = 'rubric_category_criterion_description';
	  }
	  else {
		$('#rubric_gui_points').attr('disabled','disabled');
		$('#rubric_gui_points').val('');
		$('#rubric_gui_title').val($(this).children('.rubric_category_title').html()).focus().select();
		$('#rubric_gui_description').val($(this).children('.rubric_category_description').html());
		Drupal.settings.rubric.active_title = 'rubric_category_title';
		Drupal.settings.rubric.active_description = 'rubric_category_description';
	  }
	  break;
	  case 'delete':
      if ($(this).hasClass('rubric_2-level')) {
        alert('Two is the mininum number of levels that you can have for a criterion');
      }
      else {
	   if ($(this).hasClass('rubric_category_header')) {
         if ($(this).parent().siblings().length == 0) {
		    alert('Every rubric must have at least 1 category.');
	      }
	      else {
	        if (confirm('Are you sure you want to delete this ENTIRE category? (this cannot be undone)')) {
			  var tmp_obj = this;
			  var category_id = $(this).parent().attr('id').replace('rubric_category_','');
			  $.ajax({
		        type: "POST",
	            url: Drupal.settings.basePath + "?q=rubric/ajax/delete/category/" + Drupal.settings.rubric.nid +"/"+ category_id +"/",
		        success: function(msg) {
		          $(tmp_obj).parent().remove();
				  Drupal.rubric.functions.equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
				},
		      });
	        }
	      }
	    }
	    else if ($(this).hasClass('rubric_category_criterion_header')) {
	      if ($(this).parent().siblings().length == 0) {
		    alert('Every category must have at least 1 criterion.');
	      }
	      else {
            if (confirm('Are you sure you want to delete this ENTIRE criterion? (this cannot be undone)')) {
              var tmp_obj = this;
			  var criterion_id = $(this).parent().attr('id').replace('rubric_criterion_','');
			  var category_id = $(this).parent().parent().parent().attr('id').replace('rubric_category_','');
			  $.ajax({
		        type: "POST",
	            url: Drupal.settings.basePath + "?q=rubric/ajax/delete/criterion/" + category_id +"/"+ criterion_id +"/",
		        success: function(msg) {
		          $(tmp_obj).parent().remove();
				  Drupal.rubric.functions.equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
				},
		      });
	        }
	      }
        }
	    else {
          for(var i=2; i<=8; i++) {
            if ($(this).hasClass('rubric_'+ i.toString(10) +'-level')) {
		      $(this).removeClass('rubric_'+ i.toString(10) +'-level');
		      $(this).siblings().removeClass('rubric_'+ i.toString(10) +'-level');
		      $(this).siblings().addClass('rubric_'+ (i-1).toString(10) +'-level');
		      $(this).addClass('rubric_'+ (i-1).toString(10) +'-level');
		      i=9;
	        }
          }
		  var criterion_id = $(this).parent().parent().attr('id').replace('rubric_criterion_','');
		  var level_id = $(this).attr('id').replace('rubric_level_','');
		  $(this).remove();
		  $.ajax({
			type: "POST",
			url: Drupal.settings.basePath + "?q=rubric/ajax/delete/level/" + criterion_id +"/"+ level_id +"/",
			success: function(msg) {
			  Drupal.rubric.functions.equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
			},
		  });
	    }
      }
	  break;
	}
  }).hover(Drupal.rubric.functions.gui_mouseover,Drupal.rubric.functions.gui_mouseout);
  
  $('.rubric_image_arrow-up').click(function(){
    if ($('#'+ Drupal.settings.rubric.active_id).parent().children().eq(0).attr('id') == Drupal.settings.rubric.active_id && $('#'+ Drupal.settings.rubric.active_id).hasClass('rubric_category_criterion') && $('#'+ Drupal.settings.rubric.active_id).parent().children().length != 1) {
	  var old_category_id = $('#'+ Drupal.settings.rubric.active_id).parent().parent().attr('id').replace('rubric_category_','');
	  $('#'+ Drupal.settings.rubric.active_id).appendTo($('#'+ Drupal.settings.rubric.active_id).parent().parent().prev().children('.rubric_category_criteria'));
	  //ajax for last criterion in new category
	  $.ajax({
        type: "POST",
	    url: Drupal.settings.basePath + "?q=rubric/ajax/move/criterion/" + Drupal.settings.rubric.active_id.replace('rubric_criterion_','') +"/"+ old_category_id +"/"+ $('#'+ Drupal.settings.rubric.active_id).parent().parent().attr('id').replace('rubric_category_','') +"/",
	    success: function(msg) {
		  Drupal.rubric.functions.reweight();
	    }
	  });
	}
	else {
	  $('#'+ Drupal.settings.rubric.active_id).insertBefore($('#'+ Drupal.settings.rubric.active_id).prev());
	  Drupal.rubric.functions.reweight();
	}
  });
  $('.rubric_image_arrow-down').click(function(){
    if ($('#'+ Drupal.settings.rubric.active_id).parent().children().eq($('#'+ Drupal.settings.rubric.active_id).parent().children().length-1).attr('id') == Drupal.settings.rubric.active_id && $('#'+ Drupal.settings.rubric.active_id).hasClass('rubric_category_criterion') && $('#'+ Drupal.settings.rubric.active_id).parent().children().length != 1) {
      var old_category_id = $('#'+ Drupal.settings.rubric.active_id).parent().parent().attr('id').replace('rubric_category_','');
	  $('#'+ Drupal.settings.rubric.active_id).prependTo($('#'+ Drupal.settings.rubric.active_id).parent().parent().next().children('.rubric_category_criteria'));
	  $.ajax({
        type: "POST",
	    url: Drupal.settings.basePath + "?q=rubric/ajax/move/criterion/" + Drupal.settings.rubric.active_id.replace('rubric_criterion_','') +"/"+ old_category_id +"/"+ $('#'+ Drupal.settings.rubric.active_id).parent().parent().attr('id').replace('rubric_category_','') +"/",
	    success: function(msg) {
		  Drupal.rubric.functions.reweight();
	    }
	  });
	}
	else {
	  $('#'+ Drupal.settings.rubric.active_id).insertAfter($('#'+ Drupal.settings.rubric.active_id).next());
	  Drupal.rubric.functions.reweight();
	}
  });
  $('#rubric_categories').parent().parent().removeClass('collapsed');
  Drupal.rubric.functions.equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
  $('#rubric_categories').parent().parent().addClass('collapsed');
  //set things up initially
  $('#rubric_gui_mode_' + Drupal.settings.rubric.mode).click();
});
Drupal.rubric.functions.reweight = function() {
  var count = 0;
  $('#'+ Drupal.settings.rubric.active_id).parent().children().each(function(){
	if( $('#'+ Drupal.settings.rubric.active_id).hasClass('rubric_category_criterion')) {
	  var obj_id = $(this).parent().parent().attr('id').replace('rubric_category_','');
	  var el = 'criterion';
	  var current_id = $(this).attr('id').replace('rubric_criterion_','');
	}
	else {
	  var obj_id = Drupal.settings.rubric.nid;
	  var el = 'category';
	  var current_id = $(this).attr('id').replace('rubric_category_','');
	}
	$.ajax({
	  type: "POST",
	  url: Drupal.settings.basePath + "?q=rubric/ajax/weight/"+ el +"/" + current_id +"/"+ obj_id +"/"+ count +"/",
	  success: function(msg) {
	  }
	});
	count++;
  });	
}

Drupal.rubric.functions.equalHeight = function(group) {
  var tallest = 0;
  group.height('');
  group.each(function() {
	thisHeight = $(this).height();
	if(thisHeight > tallest) {
		tallest = thisHeight;
	}
  });
  group.height(tallest);
};
Drupal.rubric.functions.shortcut_keys = function (e) {
  if (document.all) e = event;
  if(e.keyCode == 13) {
	return false;  
  }
  //ALT + P
  if (e.ctrlKey && e.altKey && e.keyCode==80){
	  $('.rubric_gui_active_add, .rubric_gui_active_edit, .rubric_gui_active_delete').removeClass('rubric_gui_active_add').removeClass('rubric_gui_active_edit').removeClass('rubric_gui_active_delete').addClass('rubric_gui_active_preview');
    Drupal.rubric.functions.set_mode('preview');
  }
  //ALT + A
  if (e.ctrlKey &&e.altKey && e.keyCode==65){
	$('.rubric_gui_active_preview, .rubric_gui_active_edit, .rubric_gui_active_delete').addClass('rubric_gui_active_add').removeClass('rubric_gui_active_edit').removeClass('rubric_gui_active_delete').removeClass('rubric_gui_active_preview');
    Drupal.rubric.functions.set_mode('add');
  }
  //ALT + E
  if (e.ctrlKey &&e.altKey && e.keyCode==69){
	  $('.rubric_gui_active_preview, .rubric_gui_active_add, .rubric_gui_active_delete').addClass('rubric_gui_active_edit').removeClass('rubric_gui_active_add').removeClass('rubric_gui_active_delete').removeClass('rubric_gui_active_preview');
    Drupal.rubric.functions.set_mode('edit');
  }
  //ALT + D
  if (e.ctrlKey &&e.altKey && e.keyCode==68){
	  $('.rubric_gui_active_preview, .rubric_gui_active_edit, .rubric_gui_active_add').addClass('rubric_gui_active_delete').removeClass('rubric_gui_active_edit').removeClass('rubric_gui_active_add').removeClass('rubric_gui_active_preview');
    Drupal.rubric.functions.set_mode('delete');
  }
};

Drupal.rubric.functions.gui_mouseover = function () {
  $(this).addClass('rubric_gui_active_'+ Drupal.settings.rubric.mode);	
};
Drupal.rubric.functions.gui_mouseout = function () {
  $('.rubric_gui_active_'+ Drupal.settings.rubric.mode).removeClass('rubric_gui_active_'+ Drupal.settings.rubric.mode);
};
Drupal.rubric.functions.set_mode = function (mode) {
	$('#rubric_gui_title').attr('disabled','disabled').val('');
	$('#rubric_gui_description').attr('disabled','disabled').val('');
	$('#rubric_gui_points').attr('disabled','disabled').val('');
	$('.rubric_gui_mode_option').css('background-color','');
	Drupal.settings.rubric.mode = mode;
	switch (Drupal.settings.rubric.mode) {
	  case 'preview':
	  $('#rubric_gui_mode_'+ Drupal.settings.rubric.mode).css('background-color','#AAAAFF');
        $('#rubric_gui_preview_pane').css('display','block');
	    $('#rubric_gui_edit').css('display','none');
		$('#rubric_gui_preview_points').html('Score: ');
		$('#rubric_gui_preview_feedback').html('');
		$('.rubric_gui_active_edit_click').removeClass('rubric_gui_active_edit_click');
	  break;
	  case 'add':
	    $('#rubric_gui_mode_'+ Drupal.settings.rubric.mode).css('background-color','#66FF66');
		$('#rubric_gui_preview_pane').css('display','none');
        $('#rubric_gui_edit').css('display','none');
		$('.rubric_category_criterion_level_selected').removeClass('rubric_category_criterion_level_selected');
		$('.rubric_gui_active_edit_click').removeClass('rubric_gui_active_edit_click');
	  break;
	  case 'edit':
	  $('#rubric_gui_mode_'+ Drupal.settings.rubric.mode).css('background-color','#FFFF99');
	    $('#rubric_gui_preview_pane').css('display','none');
        $('#rubric_gui_edit').css('display','block');
		$('.rubric_category_criterion_level_selected').removeClass(' rubric_category_criterion_level_selected');
	  break;
	  case 'delete':
	  $('#rubric_gui_mode_'+ Drupal.settings.rubric.mode).css('background-color','#FF9999');
	  $('#rubric_gui_preview_pane').css('display','none');
        $('#rubric_gui_edit').css('display','none');
		$('.rubric_category_criterion_level_selected').removeClass('rubric_category_criterion_level_selected');
		$('.rubric_gui_active_edit_click').removeClass('rubric_gui_active_edit_click');
	  break;
	}
};

Drupal.rubric.functions.scrub_feedback = function(feedback) {
	var tmpfeedback = feedback.replace(',','@comma@');
	tmpfeedback = tmpfeedback.replace('#','@pound@');
	tmpfeedback = tmpfeedback.replace('&','@amp@');
	tmpfeedback = tmpfeedback.replace('+','@plus@');
	tmpfeedback = tmpfeedback.replace('/','@fwdslash@');
	tmpfeedback = tmpfeedback.replace('\\','@bckslash@');
	
	return tmpfeedback;
};