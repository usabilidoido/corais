$(document).ready(function(){
  // Merge options from Drupal.settings.treeview  
  var options = $.extend({}, Drupal.settings.treeview);
  
  // Initialize all unordered list that has class treeview
  $("ul.treeview").treeview(Drupal.settings.treeview);
});