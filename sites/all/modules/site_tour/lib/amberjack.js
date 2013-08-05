/*
 * first cut: 17. Oct 2006
 * Amberjack 0.9 - Site Tour Creator - Simple. Free. Open Source.
 * 
 * $Id: amberjack.js,v 1.2 2006/11/24 23:58:55 stefano73 Exp $
 *
 * Copyright (C) 2006 Arash Yalpani <arash@yalpani.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


// Try to be compatible with other browsers
// Only use firebug logging if available
if (typeof console == 'undefined') {
  console = {};
  console.log = function() {};
}

/** 
 * Capsulates some static helper functions
 * @author Arash Yalpani
 *
 * This one is mainly for myself, but you can learn from that.
 *
 *
 * How this library works -
 *
 * Hint: to change Amberjack's default behavior, set values 
 *       prior to the call to Amberjack.open() (in the wizard's output)
 *
 * 1. Amberjack.open() is called through the HTML code the wizard spit out
 *    you should have includet in your site's template file
 *
 * 2. Amberjack.open()...:
 *
 *    2.1. ... checks for tourId and skinId url param...
 *      2.1.1. ...and stops execution, if no tourId was passed by url
 *      2.2.1. ...and sets skinId to default 'model_t' if none  was 
 *                passed by url
 *
 *    2.2. ... reads your web page's DOM structure, searches for the tour 
 *             definition (you should have pasted into your site's 
 *             template), parses it to create the array 'Amberjack.pages' 
 *             and to calculate the tour's params (i.e. number of tour 
 *             pages, closeUrl)
 *
 *    2.3. ... fetches control.tpl.js and style.css from 
 *             http://amberjack.org/src/stable/skin/<skinname>/
 *             (default setting) OR from your own site, if you have set 
 *             Amberjack.BASE_URL's value accordingly 
 *
 *    2.4. ... covers your web page's body with a transparent layer (DIV) if
 *             Amberjack.doCoverBody is 'true' which is the default option
 *
 * 3. In step '2.3', I explained that control.tpl.js is fetched from 
 *    either amberjack.org or your own server. control.tpl.js is the
 *    template file of a skin and what it does is to call the function
 *    AmberjackControl.open('<div ... </div>') like this. The HTML
 *    inside is the control's template.
 *
 *    3.1. AmberjackControl.open() ...
 *      3.1.1. ... fills the template's placeholders with values
 *      3.1.2. ... creates a DIV for the control
 *      3.1.3. ... fills the DIV's content with the assembled skin 
 *                 template (see 2.3)
 *      3.1.4. ... hides the control's close button if no closeUrl
 *                 was specified through wizard's output and
 *                 option 'onCloseClickStay' was not set to true
 *      3.1.4. ... checks for optional Amberjack.ADD_STYLE and
 *                 Amberjack.ADD_SCRIPT and post fetches them if set.
 *                 You can use this to manipulate tour's behaviour
 *                 right after it gets visible. Maximum flexibility!
 * 
 * That's it, basically!
 */


AmberjackBase = {

  /**
   * Proxy alerter
   * @author Arash Yalpani
   * 
   * @param str Text for alert
   *
   * @example alert('An error occurred')
   */  

  alert: function(str) {
    alert('Amberjack alert: ' + str);
  },

  /**
   * Returns FIRST matching element by tagname
   * @author Arash Yalpani
   * 
   * @param tagName name of tags to filter
   * @return first matching dom node or false if none exists
   *
   * @example getByTagName('div') => domNode
   * @example getByTagName('notexistent') => false
   */  
   
  getByTagName: function(tagName) {
    var els = document.getElementsByTagName(tagName);
    if (els.length > 0) {
      return els[0];
    }
    
    return false;
  },
  
  /**
   * Returns an array of matching DOM nodes
   * @author Arash Yalpani
   * 
   * @param tagName name of tags to filter
   * @param attrName name of attribute, matching tags must contain
   * @param attrValue value of attribute, matching tags must contain
   * @param domNode optional: dom node to start filtering from
   * @return Array of matching dom nodes
   *
   * @example getElementsByTagNameAndAttr('div', 'class', 'highlight') => [domNode1, domNode2, ...]
   */
   getElementsByTagNameAndAttr: function(tagName, attrName, attrValue, domNode) {
    if (domNode) {
      els = domNode.getElementsByTagName(tagName);
    }
    else {
      els = document.getElementsByTagName(tagName);
    }

    if (els.length === 0) {
      return [];
    }
    
    var _els = [];
    for (var i = 0; i < els.length; i++) {
      if (attrName == 'class') {
        classNames = '';
        if (els[i].getAttribute('class')) {
          classNames = els[i].getAttribute('class');
        }
        else {
          if (els[i].getAttribute('className')) {
            classNames = els[i].getAttribute('className');
          }
        }

        var reg = new RegExp('(^| )'+ attrValue +'($| )');
        if (reg.test(classNames)) {
          _els.push(els[i]);
        }         
      }
      else {
        if (els[i].getAttribute(attrName) == attrValue) {
          _els.push(els[i]);
        }
      }
    }

    return _els;
  },
  
  /**
   * Returns url param value
   * @author Arash Yalpani
   * 
   * @param url The url to be queried
   * @param paramName The params name
   * @return paramName's value or false if param does not exist or is empty
   *
   * @example getUrlParam('http://localhost/?a=123', 'a') => 123
   * @example getUrlParam('http://localhost/?a=123', 'b') => false
   * @example getUrlParam('http://localhost/?a=',    'a') => false
   */
  
  getUrlParam: function(url, paramName) {
    var urlSplit = url.split('?');
    if (!urlSplit[1]) { // no query
      return false;
    }
    
    var urlQuery = urlSplit[1];
    var paramsSplit = urlSplit[1].split('&');
    for (var i = 0; i < paramsSplit.length; i++) {
      paramSplit = paramsSplit[i].split('=');
      if (paramSplit[0] == paramName) {
        return paramSplit[1] ? paramSplit[1] : false;
      }
    }
    
    return false;
  },
  
  /**
   * Injects javascript or css file into document
   *
   * @author Arash Yalpani
   * 
   * @param url The JavaScript/CSS file's url
   * @param type Either 'script' OR 'style'
   * @param onerror Optional: callback handler if loading did not work
   *
   * @example loadScript('http://localhost/js/dummy.js', function(){alert('could not load')})
   * Note that a HEAD tag needs to be existent in the current document
   */

  postFetch: function(url, type, onerror) {
    if (type === 'script') {
      scriptOrStyle = document.createElement('script');
      scriptOrStyle.type = 'text/javascript';
      scriptOrStyle.src  = url;
    } 
    else {
      scriptOrStyle = document.createElement('link');
      scriptOrStyle.type = 'text/css';
      scriptOrStyle.rel  = 'stylesheet';
      scriptOrStyle.href = url;
    }
    
    if (onerror) { scriptOrStyle.onerror = onerror; }

    var head = AmberjackBase.getByTagName('head');
    if (head) {
      head.appendChild(scriptOrStyle);
      return ;
    }
    
    AmberjackBase.alert('head tag is missing');
  }
};


/**
 * Amberjack Control class
 * @author Arash Yalpani
 */

AmberjackControl = {

  /**
   * Callback handler for template files. Takes template HTML and fills placeholders
   * @author Arash Yalpani
   * 
   * @param tplHtml HTML code including Amberjack placeholders
   *
   * @example AmberjackControl.open('<div>{body}</div>')
   * Note that this method should be called directly through control.tpl.js files
   */

  open: function(tplHtml) {
    var urlSplit = false;
    var urlQuery = false;
    tplHtml = tplHtml.replace(/{skinId}/, Amberjack.skinId);
    if (Amberjack.pages[Amberjack.pageId].prevUrl) {
      var prevUrl = Amberjack.pages[Amberjack.pageId].prevUrl;
      urlSplit = prevUrl.split('?');
      urlQuery = urlSplit[1] ? urlSplit[1] : false;
      if (Amberjack.urlPassTourParams) {
        prevUrl+= (urlQuery ? '&' : '?') + 'tourId=' + Amberjack.tourId + (Amberjack.skinId ? '&skinId=' + Amberjack.skinId : '');
      }
      
      tplHtml = tplHtml.replace(/{prevClick}/,   "location.href='" + prevUrl + "';return false;");
      tplHtml = tplHtml.replace(/{prevClass}/,   '');
    }
    else {
      tplHtml = tplHtml.replace(/{prevClick}/,   'return false;');
      tplHtml = tplHtml.replace(/{prevClass}/,   'disabled');
    }
    
    if (Amberjack.pages[Amberjack.pageId].nextUrl) {
      var nextUrl = Amberjack.pages[Amberjack.pageId].nextUrl;
      urlSplit = nextUrl.split('?');
      urlQuery = urlSplit[1] ? urlSplit[1] : false;
      if (Amberjack.urlPassTourParams && (!Amberjack.hasExitPage || Amberjack.pages[nextUrl].nextUrl)) { // do not append params for exit page (if exit page exists)
        nextUrl+= (urlQuery ? '&' : '?') + 'tourId=' + Amberjack.tourId + (Amberjack.skinId ? '&skinId=' + Amberjack.skinId : '');
      }
      
      tplHtml = tplHtml.replace(/{nextClick}/,       "location.href='" + nextUrl + "';return false;");
      tplHtml = tplHtml.replace(/{nextClass}/,       '');
    }
    else {
      tplHtml = tplHtml.replace(/{nextClick}/,       'return false;');
      tplHtml = tplHtml.replace(/{nextClass}/,       'disabled');
    }

    tplHtml = tplHtml.replace(/{textOf}/,          Amberjack.textOf);
    tplHtml = tplHtml.replace(/{textClose}/,       Amberjack.textClose);
    tplHtml = tplHtml.replace(/{textPrev}/,        Amberjack.textPrev);
    tplHtml = tplHtml.replace(/{textNext}/,        Amberjack.textNext);
    tplHtml = tplHtml.replace(/{currPage}/,        Amberjack.pageCurrent);
    tplHtml = tplHtml.replace(/{pageCount}/,       Amberjack.pageCount);
    
    tplHtml = tplHtml.replace(/{body}/,            AmberjackBase.getElementsByTagNameAndAttr('div', 'title', Amberjack.pageId, document.getElementById(Amberjack.tourId))[0].innerHTML);

    var div = document.createElement('div');
    div.id = 'AmberjackControl';
    div.innerHTML = tplHtml;

    document.body.appendChild(div);

    // Amberjack.doHighlight();

    // No URL was set AND no click-close-action was configured:
    if (!Amberjack.closeUrl && !Amberjack.onCloseClickStay) {
      document.getElementById('ajClose').style.display = 'none';
    }    

    // post fetch a CSS file you can define by setting Amberjack.ADD_STYLE
    // right before the call to Amberjack.open();
    if (Amberjack.ADD_STYLE) {
      AmberjackBase.postFetch(Amberjack.ADD_STYLE, 'style');
    }
    
    // post fetch a script you can define by setting Amberjack.ADD_SCRIPT
    // right before the call to Amberjack.open();
    if (Amberjack.ADD_SCRIPT) {
      AmberjackBase.postFetch(Amberjack.ADD_SCRIPT, 'script');
    }
  },
  
  /**
   * Removes AmberjackControl div from DOM
   * @author Arash Yalpani
   * 
   * @example AmberjackControl.close()
   */  

  close: function() {
    e = document.getElementById('AmberjackControl');
    e.parentNode.removeChild(e);
  }
};


/** 
 * Amberjack's main class
 * @author Arash Yalpani
 */
 
Amberjack = {

  // constants

  BASE_URL: 'http://amberjack.org/src/stable/', // do not forget trailing slash!

  // explicit attributes

  // - set these through url (...&tourId=MyTour&skinId=Safari...)
  // - OR in your tour template right above the call to Amberjack.open()
  
  tourId:    false,     // mandatory: if not set, tour will not open
  skinId:    false,     // optional: if not set, skin "model_t" will be used
  
  // - set these in your tour template right above the call to Amberjack.open()
  
  textOf:    'of',      // text of splitter between "2 of 3"
  textClose: 'x',       // text of close button
  textPrev:  '&laquo;', // text of previous button
  textNext:  '&raquo;', // text of next button
  
  // - set set these in your tour template right above the call to Amberjack.open()
  
  onCloseClickStay     : false, // set this to 'true', if you want the close button to close tour but remain on current page
  doCoverBody          : true,  // set this to 'false' if you don't want your site's page to be covered
  bodyCoverCloseOnClick: false, // set this to 'true', if a click on the body cover should force it to close
  urlPassTourParams    : true,  // set this to false, if you have hard coded the tourId and skinId in your tour 
                                //     template. the tourId and skindId params will not get passed on prev/next button click
  
  

  
  // private attributes - don't touch
  
  pageId:    false,
  pages:     {},   
  pageCount: 0,    
  hasExitPage: false,
  

  /**
   * Initializes tour, creates transparent layer and causes AmberjackControl
   * to open the skin's template (control.tpl.js) into document. Call this
   * manually right after inclusion of this library. Don't forget to pass
   * tourId param through URL to show tour!
   * 
   * Iterates child DIVs of DIV.ajTourDef, extracts tour pages
   *
   * @author Arash Yalpani
   * 
   * @example Amberjack.open()
   * Note that a HEAD tag needs to be existent in the current document
   */
   
  open: function() {
    Amberjack.tourId = Amberjack.tourId ? Amberjack.tourId : AmberjackBase.getUrlParam(location.href, 'tourId');
    Amberjack.skinId = Amberjack.skinId ? Amberjack.skinId : AmberjackBase.getUrlParam(location.href, 'skinId');

    if (!Amberjack.tourId) { // do nothing if tourId is not passed through url
      return;
    }
    
    if (!Amberjack.skinId) { // set default skinId
      Amberjack.skinId = 'model_t';
    }

    var tourDef = false;
    var tourDefElements = AmberjackBase.getElementsByTagNameAndAttr('div', 'class', 'ajTourDef');
    for (i = 0; i < tourDefElements.length; i++) {
      if (tourDefElements[i].getAttribute('id') == Amberjack.tourId) {
        tourDef = tourDefElements[i];
      }
    }

    if (!tourDef) {
      AmberjackBase.alert('DIV with CLASS "ajTourDef" and ID "' + Amberjack.tourId + '" is not defined');
    }

    // Is there a specified closeUrl (title attribute of DIV.ajTourDef)?
    // Don't show close button if not set
    Amberjack.closeUrl = tourDef.getAttribute('title') ? tourDef.getAttribute('title') : false;
  
    var children = tourDef.childNodes;
    var _children = []; // cleaned up version...
    for (i = 0; i < children.length; i++) {
      if (!children[i].tagName || children[i].tagName.toLowerCase() != 'div') { continue ; }
      _children.push(children[i]);
    }

    // init tour pages
    for (i = 0; i < _children.length; i++) {
      Amberjack.pages[_children[i].getAttribute('title')] = {};
    }

    for (i = 0; i < _children.length; i++) {
      if (!_children[i].tagName || _children[i].tagName.toLowerCase() != 'div') { continue ; }

      if (!_children[i].getAttribute('title')) {
        AmberjackBase.alert('attribute "title" is missing');
        return ;
      }
    
      // -- start: check for matching page in divs --
      if (Amberjack.urlMatch(_children[i].getAttribute('title')) && _children[i].innerHTML !== '') {
        Amberjack.pageCurrent = i + 1;
        Amberjack.pageId = _children[i].getAttribute('title');
      }
      // -- end: check for matching page in divs --
    
      Amberjack.pageCount++;
      if (i >= 1 && i < _children.length) {
        Amberjack.pages[_children[i].getAttribute('title')].prevUrl = _children[i - 1].getAttribute('title');
      }
      if (i < _children.length - 1) {
        Amberjack.pages[_children[i].getAttribute('title')].nextUrl = _children[i + 1].getAttribute('title');
      }
    }

    if (_children[i-1].innerHTML === '') { // empty page div reduces pageCount by 1
      Amberjack.pageCount = Amberjack.pageCount - 1;
      Amberjack.hasExitPage = true;
    }

    if (!Amberjack.pageId) {
      AmberjackBase.alert('no matching page in ajTourDef found');
    }

    AmberjackBase.postFetch(Amberjack.BASE_URL + 'skin/' + Amberjack.skinId.toLowerCase() + '/control.tpl.js', 'script');
    AmberjackBase.postFetch(Amberjack.BASE_URL + 'skin/' + Amberjack.skinId.toLowerCase() + '/style.css', 'style');

    if (Amberjack.doCoverBody) {
      Amberjack.coverBody();
    }
  },
  
  /**
   * Checks if passed href is *included* in current location's href
   * @author Arash Yalpani
   * 
   * @param href URL to be matched against
   *
   * @example Amberjack.urlMatch('http://mysite.com/domains/')
   */
  urlMatch: function(href) {
    if (location.href.indexOf(href) == -1) {
      return false;
    }
    
    return true;
  },
  
  getWindowInnerHeight: function() {
    if ('undefined' != typeof document.documentElement && document.documentElement.clientHeight > document.body.scrollHeight) {
      return parseInt(document.documentElement.clientHeight,10);
    }

    return parseInt(document.body.scrollHeight,10);
  },
  
  
  /**
   * Creates transparent layer and places it in the document, in front of 
   * all other layers (through CSS z-index)
   * @author Arash Yalpani
   * 
   * @example Amberjack.coverBody()
   */
  coverBody: function() {
    var div = document.createElement('div');
    div.id = 'ajBodyCover';
    div.style.height = Amberjack.getWindowInnerHeight() + 'px';
    
    if (Amberjack.bodyCoverCloseOnClick) {
      div.onclick = function() {
        Amberjack.uncoverBody();
      };
    }
    
    document.body.appendChild(div);
  },

  /**
   * Removes transparent layer from document
   * @author Arash Yalpani
   * 
   * @example Amberjack.uncoverBody()
   */
  uncoverBody: function() {
    document.body.removeChild(document.getElementById('ajBodyCover'));
  },

  /*
  doHighlight: function() {
    var body = document.body;
    var highlightElements = AmberjackBase.getElementsByTagNameAndAttr('div', 'class', 'ajHighlight', body);
    for (i = 0; i < highlightElements.length; i++) {
      highlightElements[i].style.border = '3px solid red';
      highlightElements[i].style.backgroundColor = '#fee';
    }
  },
  */
  
  
  /**
   * Gets called, whenever the user clicks on the close button of Amberjack control
   * @author Arash Yalpani
   * 
   * @example Amberjack.close()
   */  
  close: function() {
    if (Amberjack.onCloseClickStay) {
      AmberjackControl.close();
      if (Amberjack.doCoverBody) {
        Amberjack.uncoverBody();
      }
      return null;
    }

    if (Amberjack.closeUrl) {
      window.location.href = Amberjack.closeUrl;
    }
    return null;
  }    
};
