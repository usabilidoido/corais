Imagepicker module for Drupal 6.x

If you have any questions or suggestions please contact me at
http://drupal.org/user/52366

This module will help you to upload images and insert them into your nodes in a
very easy way. To set up it follow next 5 steps and you should be done.

	1. Installation
Upload whole imagepicker directory into sites/all/modules
Login to Drupal site with a user which has an administrative rights and go to
Administer > Site building > Modules. Scroll down to Other modules section, you
should now see Imagepicker module listed there. Tick it and press the button to
save configuration. Imagepicker module will create it's directory structure in
your files directory (configurable via Administer > Site configuration > File
system) and all images will be saved in these directories depending on the name
of user who is uploading images.
After installation go to Administer > Site configuration > Imagepicker and
select your preferences and Save Configuration.
Also remember to go to Administer > User management > Permissions and
set up permissions according to your needs.

NOTE: Once you install Imagepicker module, it is not recomended to change
Drupal's files directory path. But if you do so, please make sure, that you will
copy whole imagepicker directory to the new location as well.

	2. Enabling imagepicker
Imagepicker uses some HTML, which will be stripped out in input formats that
use the HTML filter. If you want to enable Imagepicker module for use with such
input formats, you will have to reconfigure filters.

	3. Configuring filters
Go to Administer > Site configuration > Input formats. You should see a list of
all available input formats in your site. Click the configure link next to each
input format to see if it has the HTML filter is enabled. If it has, click on the
configure tab and add <img>, <div> and <span> to the HTML filter's
"Allowed tags".

To see these changes you will need to clear Drupal's menu cache, otherwise you
will see Access denied page instead of Imagepicker.

	4. Clearing menu cache
Login to your site's database and truncate cache_menu table. Of course, you can
use Drupal's set of functions to clear it's cache, so it's up to you. One more
simple way to clear menu cache: go to Administer > Site building > Menus, then
select Add menu tab and click on the Submit button without adding any menu item.
This will rebuild Drupal's menu. Strange, huh? But this is Drupal, it uses lots
of functions even if it doesn't need to... Anyway, this method doesn't work all
the time, so the best way to do it is to do it stright in your database.

	5. Using Imagepicker
Login with any user, who has rights to post any type of nodes and can use Full
HTML filters. Go to a node creation page, etc. Create content > Story. Somewhere
under node body you should see expanded Imagepicker module's field. Everything
else is stright forward from this point. Browse for an image file on your local
computer, enter maximum size in pixels of bigger thumbnail side, enter maximum
size in pixels of bigger image side if you want to scale your image or leave it
empty otherwise (this means - do not scale image), enter title and description
for your image and press Upload button. If you have made any mistakes, you will
see error message, or you will be redirected to 'Insert image' page otherwise.
In 'Insert image' page just check options you want and press Insert button. You
also can edit or delete your image here. Try using the 'Browse' tab if you want
to use already uploaded image, find your image and click on it. Images are
sorted by date - latest goes into the front by default, you can change this with
the Order dropdown.

NOTE: Image title will be used as image's alternative text and a title for link.
Description will be used only in image's page (imagepicker/image/{image_id})

	Compatibility with browsers
I have tested this module with Firefox, IE6.5, IE7 and Safari. For
all other browsers - try it yourself, but I think it should work with most of
them.


	Compatibility with tinyMCE
I have tested this module with tinyMCE module (tinyMCE version - 2.0) and it
works fine with Firefox 1.5 and 2.0. Altough it has some problems with IE6 and
IE7. On these browsers you firstly need to select an image, check all options
you want, then click on tinyMCE wysiwyg editor window (focus it) and only then
click on the Insert image button.
After inserting an image into tinyMCE editor (doesn't matter which browser you
are using) your cursor will be before '</a></div>' HTML tags. This might cause
some problems, so be aware of it.

	My Imagepicker in My Account
Imagepicker can now be accessed in My Account, this is the best place to build
up a collection of images. Here you can create Groups and add images to one or
more groups. There is also an Admin mode where your images are listed.

	Imagepicker Administrator
In the 2 series there is the ability for an administrator to
view/add/edit/delete anything.
The directories under files/imagepicker are now stored under uid. Older systems
should still run. See the configuration tab 'Validate Files'

	Imagepicker external hook
There is now an external hook in the upload submit function,
called 'imagepicker_upload_extra'. So other modules can do things to images.
It passes the new image id using the Drupal function module_invoke_all().

