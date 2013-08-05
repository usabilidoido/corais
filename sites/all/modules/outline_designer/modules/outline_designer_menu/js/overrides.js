  /**
   * Overload tabledrag onDrop event so that it ajax saves the new parent for the node
   */
  Drupal.tableDrag.prototype.onDrop = function() {
    //row object so we don't have to call it all the time
    var row_obj = this.rowObject.element;
    //get the id of what was dragged
    var drag_mlid = $(row_obj).find('.menu-mlid').val();
    //get the parent id based on the indentations, this equation is a bit evil...
    var p_mlid = $(row_obj).find('.menu-plid').val();
    //alert("#edit-mlid:"+ drag_mlid +"-plid");
    var weight = $(row_obj).find('.menu-weight').val();
    // standard function to get / set results via AJAX
    Drupal.outline_designer.ajax_call('menu', 'drag_drop', drag_mlid, p_mlid, weight, null);
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
        var reweight_mlid;
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
          reweight_mlid = this.id.replace('edit-mlid:','').replace('-weight','');
          Drupal.outline_designer.ajax_call('menu', 'reweight', reweight_mlid, this.value, null, 'none');
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