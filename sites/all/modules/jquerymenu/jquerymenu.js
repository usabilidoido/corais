// $Id: jquerymenu.js,v 1.7 2010/05/05 07:50:55 aaronhawkins Exp $
Drupal.behaviors.jquerymenu = function(context) {
  jqm_showit = function() {
    $(this).children('.jqm_link_edit').fadeIn();
  }
  jqm_hideit = function() {
    $(this).children('.jqm_link_edit').fadeOut();
  }
  $('ul.jquerymenu li').hover(jqm_showit, jqm_hideit);

  $('ul.jquerymenu .active').parents('li').removeClass('closed').addClass('open');
  $('ul.jquerymenu .active').parents('li').children('span.parent').removeClass('closed').addClass('open');
  $('ul.jquerymenu .active').parents('li').removeClass('closed').addClass('open');
  $('ul.jquerymenu .active').parents('li').children('span.parent').removeClass('closed').addClass('open');  
  
  jqm_mouseenter = function(momma1) {
    if ($(momma1).hasClass('closed')){
      if (Drupal.settings.jquerymenu.animate === 1) {
        $($(this).siblings('ul').children()).hide().fadeIn('3000');
        $(momma1).children('ul').slideDown('700');
      }
      $(momma1).removeClass('closed').addClass('open');
      $(this).removeClass('closed').addClass('open');
    }    
  }
  
  jqm_mouseleave = function(momma1){
    if ($(momma1).hasClass('open')){
      if (Drupal.settings.jquerymenu.animate === 1) {
        $(momma1).children('ul').slideUp('700');
        $($(this).siblings('ul').children()).fadeOut('3000');
      }
      $(momma1).removeClass('open').addClass('closed');
      $(this).removeClass('open').addClass('closed');
    }
  }
  
  var selector = "li.parent span.parent";
  if (Drupal.settings.jquerymenu.click_to_expand) {
    selector = selector + ', li.parent > a';
  }
  
  if (Drupal.settings.jquerymenu.hover === 1) {
    $('ul.jquerymenu:not(.jquerymenu-processed)', context).addClass('jquerymenu-processed').each(function(){
      $(this).find(selector).hover(jqm_mouseenter(momma), jqm_mouseleave(momma));
    });
    $('ul.jquerymenu-processed span.parent').remove();
  }

  else if (Drupal.settings.jquerymenu.hover === 0) {
    $('ul.jquerymenu:not(.jquerymenu-processed)', context).addClass('jquerymenu-processed').each(function(){
      $(this).find(selector).click(function(){
        momma = $(this).parent();
        if ($(momma).hasClass('closed')){
          if (Drupal.settings.jquerymenu.animate === 1) {
            $(momma).children('ul').slideDown('700');
            $($(this).siblings('ul').children()).hide().fadeIn('3000');
          }  
          $(momma).removeClass('closed').addClass('open');
          $(this).removeClass('closed').addClass('open');
          return false;
        }
        else{
          if (Drupal.settings.jquerymenu.animate === 1) {          
            $(momma).children('ul').slideUp('700');
            $($(this).siblings('ul').children()).fadeOut('3000');
          }
          $(momma).removeClass('open').addClass('closed');
          $(this).removeClass('open').addClass('closed');
          return false;
        }
      });
    });
  }

}