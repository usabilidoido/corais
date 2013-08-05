
  /**
   * Overload tabledrag onDrop event so that it ajax saves the new parent for the node
   */
  Drupal.tableDrag.prototype.onDrop = function() {
    //row object so we don't have to call it all the time
    var row_obj = this.rowObject.element;
    //get the id of what was dragged
    var drag_nid = $(row_obj).find('img').attr('id').replace('node-','').replace('-icon','');
    //get the parent id based on the indentations, this equation is a bit evil...
    var p_nid;
    var weight = $("#edit-table-book-admin-"+ drag_nid +"-weight").val();
    var active_indent = Math.max($('.indentation', row_obj).size());
    //if we're at level 0 then the node is at the book root
    if(active_indent != 0){
    var tmp_indent = -1;
    var tmp_obj = row_obj;
    //keep walking backwards until we find the node we need
    while (tmp_indent != (active_indent-1) ) {
      tmp_obj = $(tmp_obj).prev();
      tmp_indent = Math.max($('.indentation', tmp_obj).size());
    }
    p_nid = $(tmp_obj).find('img').attr('id').replace('node-','').replace('-icon','');
    } else {
    p_nid = Drupal.settings.outline_designer.rootNid;
    }
    $.ajax({
    type: "POST",
    url: Drupal.settings.outline_designer.ajaxPath + Drupal.settings.outline_designer.token +"/drag_drop/" + drag_nid + "/" + p_nid + "/" + weight,
    success: function(msg){
      //could implement some kind of history / undo list here if we want to
      $("#reload_table").trigger('change');
      Drupal.outline_designer.growl(msg);
    }
    });
    return null;
  };
  /**
   * Override to fix an issue with Core Drupal when it comes to empty books
   **/

  /**
   * Annoying but we need to override this entire function to attach the ajax event for submitting weights behind the sceens
   */
  Drupal.tableDrag.prototype.updateField = function(changedRow, group) {
    var rowSettings = this.rowSettings(group, changedRow);
    var sourceRow = '';
		var previousRow = '';
		var nextRow = '';
		var useSibling = '';
    // Set the row as it's own target.
    if (rowSettings.relationship == 'self' || rowSettings.relationship == 'group') {
     sourceRow = changedRow;
    }
    // Siblings are easy, check previous and next rows.
    else if (rowSettings.relationship == 'sibling') {
    previousRow = $(changedRow).prev('tr').get(0);
    nextRow = $(changedRow).next('tr').get(0);
    sourceRow = changedRow;
    if ($(previousRow).is('.draggable') && $('.' + group, previousRow).length) {
      if (this.indentEnabled) {
      if ($('.indentations', previousRow).size() == $('.indentations', changedRow)) {
        sourceRow = previousRow;
      }
      }
      else {
      sourceRow = previousRow;
      }
    }
    else if ($(nextRow).is('.draggable') && $('.' + group, nextRow).length) {
      if (this.indentEnabled) {
      if ($('.indentations', nextRow).size() == $('.indentations', changedRow)) {
        sourceRow = nextRow;
      }
      }
      else {
      sourceRow = nextRow;
      }
    }
    }
    // Parents, look up the tree until we find a field not in this group.
    // Go up as many parents as indentations in the changed row.
    else if (rowSettings.relationship == 'parent') {
    previousRow = $(changedRow).prev('tr');
    while (previousRow.length && $('.indentation', previousRow).length >= this.rowObject.indents) {
      previousRow = previousRow.prev('tr');
    }
    // If we found a row.
    if (previousRow.length) {
      sourceRow = previousRow[0];
    }
    // Otherwise we went all the way to the left of the table without finding
    // a parent, meaning this item has been placed at the root level.
    else {
      // Use the first row in the table as source, because it's garanteed to
      // be at the root level. Find the first item, then compare this row
      // against it as a sibling.
      sourceRow = $('tr.draggable:first').get(0);
      if (sourceRow == this.rowObject.element) {
      sourceRow = $(this.rowObject.group[this.rowObject.group.length - 1]).next('tr.draggable').get(0);
      }
      useSibling = true;
    }
    }
  
    // Because we may have moved the row from one category to another,
    // take a look at our sibling and borrow its sources and targets.
    this.copyDragClasses(sourceRow, changedRow, group);
    rowSettings = this.rowSettings(group, changedRow);
  
    // In the case that we're looking for a parent, but the row is at the top
    // of the tree, copy our sibling's values.
    if (useSibling) {
    rowSettings.relationship = 'sibling';
    rowSettings.source = rowSettings.target;
    }
  
    var targetClass = '.' + rowSettings.target;
    var targetElement = $(targetClass, changedRow).get(0);
  
    // Check if a target element exists in this row.
    if (targetElement) {
    var sourceClass = '.' + rowSettings.source;
    var sourceElement = $(sourceClass, sourceRow).get(0);
    switch (rowSettings.action) {
      case 'depth':
      // Get the depth of the target row.
      targetElement.value = $('.indentation', $(sourceElement).parents('tr:first')).size();
      break;
      case 'match':
      // Update the value.
      targetElement.value = sourceElement.value;
      break;
      case 'order':
      var siblings = this.rowObject.findSiblings(rowSettings);
      if ($(targetElement).is('select')) {
        // Get a list of acceptable values.
        var values = new Array();
        $('option', targetElement).each(function() {
        values.push(this.value);
        });
        var maxVal = values[values.length - 1];
        // Populate the values in the siblings.
        var tmpVal;
        var reweight_nid;
        $(targetClass, siblings).each(function() {
        // If there are more items than possible values, assign the maximum value to the row. 
        tmpVal = this.value;
        if (values.length > 0) {
          this.value = values.shift();
        }
        else {
          this.value = maxVal;
        }
        if(tmpVal != this.value) {
          reweight_nid = this.id.replace('edit-table-book-admin-','').replace('-weight','');
          $.ajax({
          type: "POST",
          url: Drupal.settings.outline_designer.ajaxPath + Drupal.settings.outline_designer.token +"/reweight/" + reweight_nid + "/" + this.value,
          success: function(msg){
            //could implement some kind of history / undo list here if we want to
            //enable this to get reweighting message support but it's annoying
            //$("#reload_table").trigger('change');
          }
          });
        }
        });
      }
      else {
        // Assume a numeric input field.
        var weight = parseInt($(targetClass, siblings[0]).val()) || 0;
        $(targetClass, siblings).each(function() {
          this.value = weight;
          weight = weight + 1;
        });
      }
      break;
    }
    }
  };