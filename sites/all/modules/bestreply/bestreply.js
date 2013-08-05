/**
 * @file Bestreply.js adds ajax functionality to bestreply module
 * Mark and clear best reply without page refresh.
 */

// Namespace Object
var BestReply = BestReply || {};

Drupal.behaviors.bestreply = function (context) {
  BestReply.rt = false;
  $('.br_mark, .br_clear:not(.br_processed)', context )
  .addClass('br_processed')
  .click( function () {
    var self = $(this);
    $.ajax({
      url: self.attr('href'),
      type: 'GET',
      data: { js : 'true' },
      dataType: 'json',
      timeout: 4000,
      success: function (json) {
        BestReply.brChange( self, json.action, json.cid);
      },
      error: function (json) {
        BestReply.rt = true; // continue loading the page
      }
    })
    return BestReply.rt;
  })
  .ready(function() {
    // move to top on page load
    BestReply.moveToTop();
  });

}
  
/**
 * Change the links to suit the new bestreply
 *
 * @param ele, Link element that was clicked
 * @param action, Action to preform (clear, replace, mark)
 * @param cid, Comment Id
 */
BestReply.brChange = function (ele, action, cid) {
  var movetotop = (Drupal.settings.bestreply_movetotop == 'yes')?true:false;
  var bp = Drupal.settings.basePath;
  var br_name = Drupal.settings.bestreply_name;
 
  if(action == 'clear'){
    if(movetotop){
      BestReply.removeClone();
    }  
    BestReply.setMark($('.br_clear'), bp+'bestreply/mark/'+cid );
    $('.bestreply_view').remove(); // remove the view link from node
    $('.node .links li:last').not('.last').addClass('last'); // add class last to last element
    $('.node .links li.last').not(':last').removeClass('last'); // remove the last class when it's not on the last element.  
  }
  else if(action == 'replace'){
    // Change the view link
    $('.links .br_view').attr('href', '#comment-'+cid);

    //Change the href
    var href = $('.br_clear').attr('href');
    if(href){
      var nhref = href.replace('clear', 'mark');
    }
    else{
      var nhref = '/bestreply/mark/'+ Drupal.settings.bestreply_ismarked;
    }
    BestReply.setMark($('.br_clear'), nhref );
    BestReply.setClear(ele, bp, cid);
    if(movetotop){
      BestReply.removeClone();
      BestReply.moveToTop();
    }
  }
  else{ // mark
    // Add the View link to the node
    $('.node ul.links').append('<li class="bestreply_view"><a class="br_view" href="#comment-'+cid+'" title="Jump to the '+br_name+'">View '+br_name+'</a></li>');
    BestReply.setClear(ele, bp, cid);
    $('.node .links li:last').not('.last').addClass('last');
    $('.node .links li.last').not(':last').removeClass('last');
    if(movetotop){
      BestReply.moveToTop();
    }   
  }
}

/**
 * Set the link element to Clear
 *
 * @param ele, Link element to change
 * @param bp, Base path
 * @param cid, Comment Id
 * @param br_name, Name used for best reply
 */
BestReply.setClear = function (ele, bp, cid) {
  var br_name = Drupal.settings.bestreply_name;
  $(ele).attr('href', bp+'bestreply/clear/'+cid)
  .attr('title', 'Clear '+br_name)
  .text('Clear '+br_name)
  .removeClass('br_mark')
  .addClass('br_clear');
  $(ele).parents('div.comment').attr('id', 'bestreply');
}

/**
 * Set the link element to Mark
 *
 * @param ele, Link element to change
 * @param nhref, New url
 * @param br_name, Name used for best reply
 */
BestReply.setMark = function (ele, nhref ){
  var br_name = Drupal.settings.bestreply_name;
  $(ele).attr('href', nhref)
  .removeClass('br_clear')
  .addClass('br_mark')
  .attr('title', 'Mark as the '+br_name)
  .text(br_name);

  $('#bestreply').removeAttr('id'); // remove the id from the old bestreply
}

/**
 * Move the best reply to the top
 */
BestReply.moveToTop = function (){
  if($('.comment').eq(0).is('#bestreply')){
    // don't do anything if it is already the first comment.
    return false;
  }
  var movetotop = (Drupal.settings.bestreply_movetotop== 'yes' )?true:false;
  var collapse = (Drupal.settings.bestreply_collapse== 'yes' )?true:false;
  if(movetotop){
    if($('#bestreply').size() > 0 ){ // it's on the same page
      var clone = $('#bestreply').clone(true); //clone and keep event handlers
      $(clone).prependTo('#comments')
      .hide()
      .attr('id', 'bestreplyclone')
      .addClass('brclone')
      .slideDown('slow');
      // remove the id from the original comment
      $('#bestreply').addClass('brclosed').attr('id', '');

      $(clone).attr('id', 'bestreply'); // and give id to the clone
      if(collapse){
        $('.brclosed .links').hide();
        $('.brclosed .content').hide('slow');
        $('.brclosed').append('<a class="expand" href="#expand">[expand]</a>');
        $('.brclosed .expand').click( function() {
           BestReply.expandClosed();
           return false;
        });
      }
    }
    else if ( Drupal.settings.bestreply_ismarked ){ // different page
      var bp = Drupal.settings.basePath;
      var path = bp+'bestreply/getcomment/'+ Drupal.settings.bestreply_ismarked;
      $('#comments').prepend('<div id="brholder"> </div>'); // insert a container
      $('#brholder').load( path ); // load the best reply into the container
    }
  }
}

/**
 * Remove the cloned reply
 */
BestReply.removeClone = function() {
  var collapse = (Drupal.settings.bestreply_collapse=='yes')?true:false;
  //clean up old bestreply if it exists.
  if(collapse){
    BestReply.expandClosed();
  }
  $('.brclosed').removeClass('brclosed');
  $('.brclone').remove(); // remove top clone.
}

BestReply.expandClosed = function() {
  $('.brclosed .links').show();
  $('.brclosed .expand').empty();
  $('.brclosed .content').show('slow');
} 