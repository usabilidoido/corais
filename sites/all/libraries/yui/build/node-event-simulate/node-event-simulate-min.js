/*
YUI 3.5.1 (build 22)
Copyright 2012 Yahoo! Inc. All rights reserved.
Licensed under the BSD License.
http://yuilibrary.com/license/
*/
YUI.add("node-event-simulate",function(a){a.Node.prototype.simulate=function(c,b){a.Event.simulate(a.Node.getDOMNode(this),c,b);};},"3.5.1",{requires:["node-base","event-simulate"]});