// $Id$

/**
 * Scripts for handling on-the-fly node creation from Flex.
 */


/**
 * Plugin JavaScript object.
 */
var GraphmindRelationship = GraphmindRelationship || {};

/**
 * Indicates that this window fired the popup for node creation.
 */
var isWindowLiveFlag = false;

/**
 * Global var for storing the parent node ID.
 */
var parentNID;

/**
 * Time of the last exit.
 */
var lastExitTry = new Date().getTime();


/**
 * On load event.
 */
$(function(){
  /**
   * Called only when a popup window finished the node creation for the Flex plugin.
   */
  if (
    Drupal.settings.hasOwnProperty('graphmindRelationship') &&
    Drupal.settings.graphmindRelationship.hasOwnProperty('child_nid')
  ) {
    if (
      window.opener &&
      window.opener.hasOwnProperty('isWindowLiveFlag') &&
      window.opener.isWindowLiveFlag
    ) {
      window.opener.GraphmindRelationship.returnCreationRequest(Drupal.settings.graphmindRelationship.child_nid);
      window.close();
    }
  }
  
  if (
    Drupal.settings.hasOwnProperty('graphmindRelationshipCloseWindow') &&
    window.opener &&
    window.opener.hasOwnProperty('isWindowLiveFlag') &&
    window.opener.isWindowLiveFlag
  ) {
    window.close();
  }
  
  if (Drupal.settings.hasOwnProperty('graphmindRelationshipCloseWarning')) {
    jQuery(window).bind('beforeunload', function() {
      var now = new Date().getTime();
      if (now > lastExitTry + 30000) {
        lastExitTry = now;
        return 'You may have unsaved changes! Are you sure about leaving the page without saving it?';
      }
    });
  }
});


/**
 * Open popup window. Should be called from Flex through ExternalInterface.
 */
GraphmindRelationship.openNodeCreation = function(parent_nid, type) {
  parentNID = parent_nid;
  
  window.open(
    Drupal.settings.basePath + 'node/add' + (type ? ('/' + type) : '') + '?graphmind_relationship_parent=' + parent_nid,
    'nodeCreation',
    'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=1,resizable=1,width=800,height=600'
  );
    
  isWindowLiveFlag = true;
}


/**
 * Popup window calls this to send results back to Flex.
 */
GraphmindRelationship.returnCreationRequest = function(child_nid) {
  GraphmindRelationship.getFlashObject().sendCreationRequestBackToFlex(parentNID, child_nid);
}


/**
 * Returns the Flex object.
 */
GraphmindRelationship.getFlashObject = function() {
  if (navigator.appName.indexOf("Microsoft") != -1) {
     return window['GraphMind'];
  } else {
     return document['GraphMind'];
  }
}


/**
 * Opens a normal popup window.
 */
GraphmindRelationship.openPopupWindow = function(url) {
  isWindowLiveFlag = true;
  window.open(
    url,
    'graphmindGeneralPopup',
    'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=1,resizable=1,width=800,height=600'
  );
}


/**
 * Show the node page in the preview area - if exists.
 */
GraphmindRelationship.loadNodeInBlock = function(nid) {
  var container = $('#graphmindNodePreview');
  if (container.length >= 1) {
    $.ajax({
      type: 'GET',
      url: Drupal.settings.basePath + 'graphmind_relationship/node_preview/' + nid,
      success: function(result) {
        container.html(result);
      },
      error: function(xhr, ajaxOptions, thrownError) {
        container.html(Drupal.t('Missing view.'));
      }
    });
  }
}
