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
  $('.rubric_grading_feedback_textarea').live('keyup',function(e){
      var selected_criterion = $('#rubric_criterion_'+ $(this).parent().parent().attr('id').replace('rubric_grading_criterion_','')).children('.rubric_category_criterion_levels').children('.rubric_category_criterion_level_selected');
	  var canned_feedback = selected_criterion.children('.rubric_category_criterion_level_title').html() +' - '+ selected_criterion.children('.rubric_category_criterion_level_description').html();
		if ($(this).val() !=  canned_feedback) {
			$(this).addClass('rubric_grading_input_changed');
		}
		else {
			$(this).removeClass('rubric_grading_input_changed');
		}
  });
  //lock down the points input fields to only accept number key presses
  $('.rubric_grading_earned_input, .rubric_grading_total_earned_input').live('keydown',function(e){
	if ((e.keyCode >= 48 && e.keyCode <= 57) || e.keyCode==8 || e.keyCode==9 || e.keyCode==46 || (e.keyCode>=96 && e.keyCode<=105) || e.keyCode==110 || e.keyCode==190 || (e.keyCode>=37 && e.keyCode<=40)) {
		if ($('.rubric_grading_total_earned_input').hasClass('rubric_grading_input_changed') && !$(this).hasClass('rubric_grading_total_earned_input')) {
		if (!confirm('You have changed the total score and are now altering one of the criterion scores. This will reset the total points to the sum of its parts, are you sure you want to do this?')) {
			return false;
		}
		else {
		  Drupal.rubric.functions.set_total_points();	
		}
	  }
    }
	else {
	  return false;	
	}
  }).live('keyup',function(e){
	  if ((e.keyCode >= 48 && e.keyCode <= 57) || e.keyCode==8 || e.keyCode==9 || e.keyCode==46 || (e.keyCode>=96 && e.keyCode<=105) || e.keyCode==110 || e.keyCode==190 || (e.keyCode>=37 && e.keyCode<=40)) {
	  if ($(this).parent().parent().hasClass('rubric_grading_total_row')) {
		var total_earned = 0;
		$('.rubric_grading_earned input').each(function(){
		  if ($(this).val() != '') {
			total_earned+= parseInt($(this).val());
		  }
		});
		
		if ($('.rubric_grading_total_earned_input').val() != total_earned) {
		  $(this).addClass('rubric_grading_input_changed');
		}
		else {
		  $(this).removeClass('rubric_grading_input_changed');
		}
	  }
	  else {
		var criterion_id = $(this).parent().parent().attr('id').replace('rubric_grading_criterion_','');
		if ($(this).val() != parseInt($('#rubric_criterion_'+ criterion_id).children('.rubric_category_criterion_levels').children('.rubric_category_criterion_level_selected').children('.level_gui').children('.rubric_category_criterion_level_points').html()) ) {
			$(this).addClass('rubric_grading_input_changed');
		}
		else {
			$(this).removeClass('rubric_grading_input_changed');
		}
		Drupal.rubric.functions.set_total_points();
	  }
	}
  }).live('change',function(){
  //safety net for the keyup event not firing  
    if (!$(this).parent().parent().hasClass('rubric_grading_total_row')) {
		var criterion_id = $(this).parent().parent().attr('id').replace('rubric_grading_criterion_','');
		if ($(this).val() != parseInt($('#rubric_criterion_'+ criterion_id).children('.rubric_category_criterion_levels').children('.rubric_category_criterion_level_selected').children('.level_gui').children('.rubric_category_criterion_level_points').html()) ) {
			$(this).addClass('rubric_grading_input_changed');
		}
		else {
			$(this).removeClass('rubric_grading_input_changed');
		}
		Drupal.rubric.functions.set_total_points();
	  }
  });
  $('.rubric_image_lock').live('click',function(){
    $(this).attr('src',$(this).attr('src').replace('lock.png','lock-open.png')).removeClass('rubric_image_lock').addClass('rubric_image_lock-open');
	$(this).prev().removeAttr('disabled');
  });
  $('.rubric_image_lock-open').live('click',function(){
    if ($(this).parent().parent().hasClass('rubric_grading_total_row')) {
		var level_value = 0;
		$('.rubric_grading_earned input').each(function(){
		  if ($(this).val() != '') {
			level_value+= parseInt($(this).val());
		  }
		});
	}
	else {
    var criterion_id = $(this).parent().parent().attr('id').replace('rubric_grading_criterion_','');
	var level_value = $('#rubric_criterion_'+ criterion_id).children('.rubric_category_criterion_levels').children('.rubric_category_criterion_level_selected').children('.level_gui').children('.rubric_category_criterion_level_points').html();
	}
	var confirm_msg = '';
	if ($(this).prev().val() != level_value) {
		if ($('.rubric_grading_total_earned_input').hasClass('rubric_grading_input_changed')) {
		  confirm_msg = "If you lock this value it will reset to the original value ("+ level_value +") and will also override the total score. Are you sure you want to reset this?";
		}
		else {
		  confirm_msg = "If you lock this value it will reset to the original value ("+ level_value +"). Are you sure you want to reset this?";
		}
      if (confirm(confirm_msg)) {
        $(this).attr('src',$(this).attr('src').replace('lock-open.png','lock.png')).addClass('rubric_image_lock').removeClass('rubric_image_lock-open');
	    $(this).prev().attr('disabled',true);
	    $(this).prev().val(level_value);
		$(this).prev().removeClass('rubric_grading_input_changed');
		Drupal.rubric.functions.set_total_points();
	  }
	}
	else {
	  $(this).attr('src',$(this).attr('src').replace('lock-open.png','lock.png')).addClass('rubric_image_lock').removeClass('rubric_image_lock-open');
	  $(this).prev().attr('disabled',true);
	}
  });
  
  $('.rubric_category_header').live('click',function(){
    if ($(this).next().css('display') == 'none') {
	  $(this).next().css('display','block');
	  $(this).css('cursor','n-resize');
	}
	else {
	  $(this).next().css('display','none');
	  $(this).css('cursor','s-resize');
	}
  });
  //bind all of the info click buttons to the popups
  $('.rubric_image_info').live('click',function(){
      $('#rubric_tooltip_title').html($(this).prev().prev().html());
      $('#rubric_tooltip_description').html($(this).prev().html());
      $('#rubric_tooltip').css({display: 'block', top: this.offsetTop + 13, left: this.offsetLeft + 13});
  });
  //tool tip close
  $('#rubric_tooltip_close').live('click',function(){
	if (!$('#rubric_gui_description').hasClass('rubric_gui_show')) { 
      $('#rubric_tooltip').css('display','');
	  $('#rubric_tooltip_title').html('');
	  $('#rubric_tooltip_description').html('');
	}
  });
  //click handler for levels
  $('.rubric_category_criterion_level').live('click',function(){
	var accept_change = true;
	var criterion_id = $(this).parent().parent().attr('id').replace('rubric_criterion_','');
	if ($('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_earned input').hasClass('rubric_grading_input_changed') && $('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_feedback textarea').hasClass('rubric_grading_input_changed')) {
      accept_change = confirm('You have overridden the awarded feedback and score for this criterion. This score and feedback will be overwritten if you change your assessment level, change anyway?');
	}
	else if ($('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_earned input').hasClass('rubric_grading_input_changed')) {
      accept_change = confirm('You have overridden the awarded score for this criterion. This score will be overwritten if you change your assessment level, change anyway?');
	}
	else if ($('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_feedback textarea').hasClass('rubric_grading_input_changed')) {
      accept_change = confirm('You have overridden the awarded feedback for this criterion. This feedback will be overwritten if you change your assessment level, change anyway?');
	}
	if (accept_change) {
		$(this).siblings().removeClass('rubric_category_criterion_level_selected');
		$(this).addClass('rubric_category_criterion_level_selected');
		$('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_earned input, #rubric_grading_criterion_'+ criterion_id +' .rubric_grading_feedback textarea').removeClass('rubric_grading_input_changed');
		var tmp = $('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_earned input').next();
		tmp.attr('src',tmp.attr('src').replace('lock-open.png','lock.png')).addClass('rubric_image_lock').removeClass('rubric_image_lock-open');
		$('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_earned').attr('disabled',true);
		var earned_points = $(this).children('.level_gui').children('.rubric_category_criterion_level_points').html();
		var feedback = $(this).children('.rubric_category_criterion_level_title').html() +' - '+ $(this).children('.rubric_category_criterion_level_description').html();
		$('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_earned input').val(earned_points);
		$('#rubric_grading_criterion_'+ criterion_id +' .rubric_image_lock').css('display','inherit');
		$('#rubric_grading_criterion_'+ criterion_id +' .rubric_grading_feedback textarea').val(feedback);
		var complete = true;
		$('.rubric_grading_earned input').each(function(){
		  if ($(this).val() == '') {
			complete = false;
		  }
		});
		Drupal.rubric.functions.set_total_points();
		if (complete) {
		  $('.rubric_grading_total_earned .rubric_image_lock').css('display','inherit');
		}
		else {
		  $('.rubric_grading_total_earned .rubric_image_lock').css('display','');
		}
	}
  });
  equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
});

Drupal.rubric.functions.set_total_points = function() {
	var total_earned = 0;
	$('.rubric_grading_earned input').each(function(){
	  if ($(this).val() != '') {
		total_earned+= parseInt($(this).val());
	  }
	});
    if ($('.rubric_grading_total_earned_input').hasClass('rubric_grading_input_changed')) {
		$('.rubric_grading_total_earned_input').val(total_earned);  
		$('.rubric_grading_total_earned_input').removeClass('rubric_grading_input_changed');
		$('.rubric_grading_total_earned_input').next().attr('src',$('.rubric_grading_total_earned_input').next().attr('src').replace('lock-open.png','lock.png')).addClass('rubric_image_lock').removeClass('rubric_image_lock-open');
		$('.rubric_grading_total_earned_input').attr('disabled',true);
	}
	else {
	  $('.rubric_grading_total_earned_input').val(total_earned);
	}
};

function equalHeight(group) {
  tallest = 0;
  group.each(function() {
	thisHeight = $(this).height();
	if(thisHeight > tallest) {
		tallest = thisHeight;
	}
  });
  group.height(tallest);
}