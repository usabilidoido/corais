// $Id: plugin_manager_search.js,v 1.1.2.15 2008/12/26 21:00:59 jabapyth Exp $

// Remove all of the rows from a table.
function table_clear(element) {
  var table = element.deleteRow && element || document.getElementById(element);
  while(table.rows.length > 0) {
      table.deleteRow(0);
  }
}

// Load and display the JSON feed of the queue.
function table_show(table, feed, post, button_action, notag) {
  feed = Drupal.settings.plugin_manager.ajaxPrefix + feed;
  $.post(feed, post, function(data) {
    table_clear(table);
    var count = 0;
    // Create a new row for every item.
    for (var key in data) {
      count += 1;
      // Create a button to remove the item from the queue.
      var button = document.createElement('button');
      button.innerHTML = button_action;
      button.short_name = key;

      button.onclick = function() {
        // Remove from the queue and remove the row from the queue table.
        if (this.innerHTML == Drupal.t('Remove')) {
          queue_remove(this.short_name);
        } else
        {
          queue_add(this.short_name);
        }
        this.parentNode.parentNode.parentNode.removeChild(this.parentNode.parentNode);
      };

      // Add the row to the end of the table.
      table = table.insertRow && table || document.getElementById(table);
      var row = table.insertRow(-1);
      row.insertCell(0).appendChild(button);
      row.insertCell(1).innerHTML = data[key].title;
      if (!notag){
          row.insertCell(2).innerHTML = data[key].tag;
          row.insertCell(3).innerHTML = data[key].link;
      }else
          row.insertCell(2).innerHTML = data[key].link;
    }
    table.parentNode.parentNode.firstChild.firstChild.innerHTML = table.parentNode.parentNode.getAttribute('name')+" ("+count+")";
  }, "json");
}

function queue_show() {
  table_show($('#install_queue table')[0], 'queue', '', Drupal.t('Remove'),true);
}

// Add a plugin to the global install queue.
function queue_add(plugin) {
  table_show($('#install_queue table')[0], 'queue/add/', {'plugin' : plugin}, Drupal.t('Remove'),true);
}

// Remove a plugin from the global install queue.
function queue_remove(plugin) {
  $.post(Drupal.settings.plugin_manager.ajaxPrefix + 'queue/remove/', {'plugin' : plugin},function(x){
      $('#install_queue legend a').html(Drupal.t('Install Queue')+' ('+x+')');
  });
}

// Clear the global install queue.
function queue_clear() {
  if (confirm(Drupal.t("Clear Queue: Are you sure?"))) {
    table_show('queue', 'queue/clear', {'confirm' : 'TRUE'}, Drupal.t('Remove'));
  }
}

function search_term() {
  var tag = 'All';
  var term = document.getElementById('edit-sinput').value;
  var table = $('#results table')[0];
  show_results();
  table.insertRow(-1).insertCell(0).innerHTML = 'Loading...';
  table_show(table, 'term/' + tag + '/' + term, '', Drupal.t('Add'));
}

function load_tag(a){
    if (a.loaded)return;
    var table = a.parentNode.parentNode.lastChild.appendChild(document.createElement('table'));
    table.insertRow(-1).insertCell(0).innerHTML = 'Loading...';
    table_show(table,'tag/'+a.innerHTML, '', Drupal.t('Add'), true);
    a.loaded = true;
}

function load_letter(a){
    if (a.loaded)return;
    var table = a.parentNode.parentNode.lastChild.appendChild(document.createElement('table'));
    table.insertRow(-1).insertCell(0).innerHTML = 'Loading...';
    table_show(table,'letter/'+a.innerHTML, '', Drupal.t('Add'), true);
    a.loaded = true;
}

function show_tags(){
    $('#edit-tag-button').attr('disabled',true).addClass('selected');
    $('#edit-letter-button').attr('disabled',false).removeClass('selected');
    $('#edit-search-button').removeClass('selected');
    $('#tags').removeClass('search_hidden');
    $('#letters').addClass('search_hidden');
    $('#results').addClass('search_hidden');
}

function show_letters(){
    $('#edit-tag-button').attr('disabled',false).removeClass('selected');
    $('#edit-letter-button').attr('disabled',true).addClass('selected');
    $('#edit-search-button').removeClass('selected');
    $('#tags').addClass('search_hidden');
    $('#letters').removeClass('search_hidden');
    $('#results').addClass('search_hidden');
}

function show_results(){
    $('#edit-tag-button').attr('disabled',false).removeClass('selected');
    $('#edit-letter-button').attr('disabled',false).removeClass('selected');
    $('#edit-search-button').addClass('selected');
    $('#tags').addClass('search_hidden');
    $('#letters').addClass('search_hidden');
    $('#results').removeClass('search_hidden');
    $('#results table').html('<br/>');
}

function register_clicks(){
  $('#tags legend a').each(function(i,a){
      $(a).click(function(){
          load_tag(a);
      });
  });
  $('#letters legend a').each(function(i,a){
      $(a).click(function(){
          load_letter(a);
      });
  });
  $('#results div')[0].appendChild(document.createElement('table'));
  $('#install_queue').mouseover(function(x){$(this).removeClass('collapsed');$('#install_queue .fieldset-wrapper').slideDown();})
  $('#install_queue').mouseout(function(x){$(this).addClass('collapsed');$('#install_queue .fieldset-wrapper').slideUp();})
}

$(document).ready( function () {
  register_clicks();
  queue_show();
});

