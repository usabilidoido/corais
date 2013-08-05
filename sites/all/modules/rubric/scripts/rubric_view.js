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
  //bind the collapsey functionality for categories
  $('.rubric_category_header').toggle(function(){
    $(this).next().css('display','none');
	$(this).css('cursor','s-resize');
  },function(){
	$(this).next().css('display','block');
	$(this).css('cursor','n-resize');
  });
  //bind all of the info click buttons to the popups
  $('.rubric_image_info').click(function(){
      $('#rubric_tooltip_title').html($(this).prev().prev().html());
      $('#rubric_tooltip_description').html($(this).prev().html());
      $('#rubric_tooltip').css({display: 'block', top: this.offsetTop + 13, left: this.offsetLeft + 13});
  });
  //bind all of the comment click buttons to the popups
  $('.rubric_image_comment').click(function(){
      $('#rubric_tooltip_title').html($(this).parent().next().html());
      $('#rubric_tooltip_description').html($(this).parent().next().next().html());
      $('#rubric_tooltip').css({display: 'block', top: this.offsetTop + 13, left: this.offsetLeft + 13});
  });
  
  //tool tip close
  $('#rubric_tooltip_close').click(function(){
	if (!$('#rubric_gui_description').hasClass('rubric_gui_show')) { 
      $('#rubric_tooltip').css('display','');
	  $('#rubric_tooltip_title').html('');
	  $('#rubric_tooltip_description').html('');
	}
  });
  equalHeight($(".rubric_category_criterion_level, .rubric_category_criterion_header"));
});

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