Readme
------

This is a simple filter module. It automatically converts links to google videos and youtube videos into embedded code.

For example.  If people input stuff like:

------
http://video.google.com/videoplay?docid=1083482561197698956&sourceid=docidfeed&hl=en

http://youtube.com/watch?v=wWBmShJDMMM
------

It will turn into:

------
<embed style="width:400px; height:326px;" id="VideoPlayback" type="application/x-shockwave-flash" src="http://video.google.com/googleplayer.swf?docId=1083482561197698956&hl=en" flashvars=""> </embed>

<object width="425" height="350"><param name="movie" value="http://www.youtube.com/v/wWBmShJDMMM"></param><param name="wmode" value="transparent"></param><embed src="http://www.youtube.com/v/wWBmShJDMMM" type="application/x-shockwave-flash" wmode="transparent" width="425" height="350"></embed></object>
------



