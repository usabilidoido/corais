AmberjackControl.open(
  '<div id="ajControl">' +
    '<table cellpadding="0" cellspacing="0">' +
    '<tr id="ajControlNavi">' +
      '<td id="ajPlayerCell">' +
        '<a id="ajPrev" class="{prevClass}" href="javascript:;" onclick="this.blur();{prevClick}"><span>{textPrev}</span></a>' + 
        '<a id="ajNext" class="{nextClass}" href="javascript:;" onclick="this.blur();{nextClick}"><span>{textNext}</span></a>' +
        '<span id="ajCount">{currPage} {textOf} {pageCount}</span>' +
      '</td>' +
      '<td id="ajCloseCell">' +
        '<a id="ajClose" href="javascript:;" onclick="Amberjack.close();return false"><span>{textClose}</span></a>' +
      '</td>' +
    '</tr>' +
    '<tr id="ajControlBody"><td colspan="2">{body}</td></tr>' +
    '<tr id="ajControlInfo"><td colspan="2">Tour powered by <a href="http://amberjack.org">Amberjack</a> &amp; {skinId} skin</td></tr>' +
    '</table>' +
  '</div>'
);