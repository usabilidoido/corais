$Id: README.txt,v 1.1 2010/07/13 21:41:28 bonked Exp $
Readme
------

This is a simple filter module. It automatically converts links to vimeo videos into embedded code.

For example.  If people input stuff like:

------
http://www.vimeo.com/10384510
------

It will turn into:

------
<object width="400" height="225"><param name="allowfullscreen" value="true" /><param name="allowscriptaccess" value="always" /><param name="movie" value="http://vimeo.com/moogaloop.swf?clip_id=10384510&amp;server=vimeo.com&amp;show_title=1&amp;show_byline=1&amp;show_portrait=0&amp;color=&amp;fullscreen=1" /><embed src="http://vimeo.com/moogaloop.swf?clip_id=10384510&amp;server=vimeo.com&amp;show_title=1&amp;show_byline=1&amp;show_portrait=0&amp;color=&amp;fullscreen=1" type="application/x-shockwave-flash" allowfullscreen="true" allowscriptaccess="always" width="400" height="225"></embed></object>
------



