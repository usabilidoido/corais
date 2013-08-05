(function ($) {
// START jQuery

Drupal.sheetnodeCK = Drupal.sheetnodeCK || {};

Drupal.behaviors.sheetnodeCK = function(context) {
  CKEDITOR.config.resize_enabled = false;
  $('div.sheetview', context).bind('sheetnodeCreated', function(e, data) {
    var spreadsheet = data.spreadsheet;
    if (typeof spreadsheet.formulabuttons == 'undefined') return;
    spreadsheet.formulabuttons['ckeditor'] = {
      image: Drupal.settings.sheetnodeCK.imagePrefix+"/ckeditor.png", 
      skipImagePrefix: true,
      tooltip: "CKEditor - HTML Editor",
      command: Drupal.sheetnodeCK.DoMultiline
    };
  });
}

Drupal.sheetnodeCK.DoMultiline = function() {

   var SCLocSS = SocialCalc.LocalizeSubstrings;

   var str, ele, text, ch;

   var scc = SocialCalc.Constants;
   var spreadsheet = SocialCalc.GetSpreadsheetControlObject();
   var editor = spreadsheet.editor;
   var wval = editor.workingvalues;

   var spreadsheet = SocialCalc.GetSpreadsheetControlObject();
   var idp = spreadsheet.idPrefix+"ckeditor";

   ele = document.getElementById(idp+"dialog");
   if (ele) return; // already have one

   switch (editor.state) {
      case "start":
         wval.ecoord = editor.ecell.coord;
         wval.erow = editor.ecell.row;
         wval.ecol = editor.ecell.col;
         editor.RangeRemove();
         text = SocialCalc.GetCellContents(editor.context.sheetobj, wval.ecoord);
         break;

      case "input":
      case "inputboxdirect":
         text = editor.inputBox.GetText();
         break;
      }

   editor.inputBox.element.disabled = true;

   text = SocialCalc.special_chars(text);
   if (text.charAt(0) == "'") {
      text = text.substring(1);
      }

   str = '<textarea id="'+idp+'textarea" style="width:580px;height:120px;margin:10px 0px 0px 6px;">'+text+'</textarea>'+
         '<div style="width:580px;text-align:right;padding:6px 0px 4px 6px;font-size:small;">'+
         SCLocSS('<input type="button" value="%loc!Set Cell Contents!" style="font-size:smaller;" onclick="Drupal.sheetnodeCK.DoMultilinePaste();">&nbsp;'+
         '<input type="button" value="%loc!Cancel!" style="font-size:smaller;" onclick="Drupal.sheetnodeCK.HideMultiline();"></div>'+
         '</div>');

   var main = document.createElement("div");
   main.id = idp+"dialog";

   main.style.position = "absolute";

   var vp = SocialCalc.GetViewportInfo();
   var pos = SocialCalc.GetElementPositionWithScroll(spreadsheet.spreadsheetDiv);
         
   main.style.top = ((vp.height/4)-pos.top)+"px";
   main.style.left = ((vp.width/4)-pos.left)+"px";
   main.style.zIndex = 100;
   main.style.backgroundColor = "#FFF";
   main.style.border = "1px solid black";

   main.style.width = "600px";

   main.innerHTML = '<table cellspacing="0" cellpadding="0" style="border-bottom:1px solid black;"><tr>'+
      '<td style="font-size:10px;cursor:default;width:100%;background-color:#999;color:#FFF;">'+
      SCLocSS("&nbsp;%loc!CKEditor Input Box!")+'</td>'+
      '<td style="font-size:10px;cursor:default;color:#666;" onclick="Drupal.sheetnodeCK.HideMultiline();">&nbsp;X&nbsp;</td></tr></table>'+
      '<div style="background-color:#DDD;">'+str+'</div>';

   SocialCalc.DragRegister(main.firstChild.firstChild.firstChild.firstChild, true, true, {MouseDown: SocialCalc.DragFunctionStart, MouseMove: SocialCalc.DragFunctionPosition,
                  MouseUp: SocialCalc.DragFunctionPosition,
                  Disabled: null, positionobj: main},
                  spreadsheet.spreadsheetDiv);

   spreadsheet.spreadsheetDiv.appendChild(main);

   CKEDITOR.replace(idp+"textarea", {
      toolbar : Drupal.settings.sheetnodeCK.toolbar
      });

   ele = document.getElementById(idp+"textarea");
   //ele.focus();
   SocialCalc.CmdGotFocus(ele);
//!!! need to do keyboard handling: if esc, hide?

   }

Drupal.sheetnodeCK.HideMultiline = function() {

   var scc = SocialCalc.Constants;
   var spreadsheet = SocialCalc.GetSpreadsheetControlObject();
   var editor = spreadsheet.editor;

   var ele = document.getElementById(spreadsheet.idPrefix+"ckeditordialog");

   SocialCalc.DragUnregister(ele);

   SocialCalc.KeyboardFocus();

   if (ele.parentNode) {
      CKEDITOR.instances[spreadsheet.idPrefix+"ckeditortextarea"].destroy(true);
      ele.parentNode.removeChild(ele);
      }

   switch (editor.state) {
      case "start":
         editor.inputBox.DisplayCellContents(null);
         break;

      case "input":
      case "inputboxdirect":
         editor.inputBox.element.disabled = false;
         editor.inputBox.Focus();
         break;
      }

   }

Drupal.sheetnodeCK.DoMultilinePaste = function() {

   var spreadsheet = SocialCalc.GetSpreadsheetControlObject();
   var editor = spreadsheet.editor;
   var wval = editor.workingvalues;

   var ele = document.getElementById(spreadsheet.idPrefix+"ckeditortextarea");

   var text = CKEDITOR.instances[spreadsheet.idPrefix+"ckeditortextarea"].getData();

   Drupal.sheetnodeCK.HideMultiline();

   switch (editor.state) {
      case "start":
         wval.partialexpr = "";
         wval.ecoord = editor.ecell.coord;
         wval.erow = editor.ecell.row;
         wval.ecol = editor.ecell.col;
         break;
      case "input":
      case "inputboxdirect":
         editor.inputBox.Blur();
         editor.inputBox.ShowInputBox(false);
         editor.state = "start";
         break;
      }

   editor.EditorSaveEdit(text);

   }

// END jQuery
})(jQuery);

