
Installation
--------------
1. Put the bestreply directory in your site modules directory.  
   See http://drupal.org/node/70151 for tips on where to install contributed modules.
2. Enable bestreply via admin/build/modules.
3. Add some styles to your style.css to theme the best reply differently from other comments.
   See Themeing below.
	
Settings
--------------
admin/settings/bestreply
Lists the best replys, author, who they were marked by, and when they were marked.

admin/settings/bestreply/settings
Set the name, text you wish to use for best reply links.
Select if you want the best reply moved to the first comment position,
and if you want the original comment collapsed in place.  
Check the node types you want to be able to mark a comment as the best reply.

Best Reply now integrates with the UserPoints module http//drupal.org/project/userpoints
You can configure points for Best Reply at /admin/settings/userpoints


Access Control  
---------------
view bestreply: User can see link to view the best reply.
mark bestreply: User can mark best reply if they are the node author.
clear bestreply: User can clear best reply if they are the node author
moderate bestreply: User can mark, change and clear best reply at any time.
administer bestreply: User can change admin settings for best reply.

Theming
------------
Modify comment.tpl.php add:
<?php if ($comment->bestreply) print 'id="bestreply"'; ?> 

eg. <div <?php if ($comment->bestreply) print 'id="bestreply"'; ?>  class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ($comment->status == COMMENT_NOT_PUBLISHED) ? ' comment-unpublished' : ''; print ' '. $zebra; ?>">
 
Now the best reply comment will be marked with an id of bestreply.
If you are moving the best reply to the top, the original comment will be marked with the class brclosed.

You can style that by adding to your style.css something along the lines of:
#bestreply { background:skyblue;} 
.brclosed { background:#CCC;}
or 
#bestreply { background: url(medal.gif) top right no-repeat;}


