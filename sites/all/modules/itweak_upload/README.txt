
iTweak Upload Module
--------------------
by Ilya Ivanchenko, iva2k@yahoo.com

With iTweak Upload module you can tweak file upload forms and improve attachments display:

* Beautify and improve the upload form for file attachments in nodes and comments
  (created by core upload.module and by comment_upload.module)
* (Option) Preview uploaded image files
* (Option) Display thumbnails for image attachments
* (Option) Display image attachments in a gallery
* (Option) Hide non-image files when there are image attachments in a gallery
* (Option) Show images in animated popup (lightbox2, thickbox, fancybox, shadowbox, highslide), grouped or slideshow (lightbox2, shadowbox, highslide)
* Independent of theme - works with any theme that does not already customize file uploads
* jCarousel (if installed) is used for the image attachments gallery
* jCarousel Lite (if installed) can be used for the image attachments gallery (experimental)
* (Option) Show upload progress percent complete using server-side uploadprogress or APC
* Compatible with popular modules download_count, private_upload
* Insert feature added to the files upload form (Insert module required)
* Insert feature preview of presets

Default options are configurable for the whole site. Initial installation comes with a preset for imagecache and it autodetects Lightbox, Thickbox, Fancybox or Shadowbox mode, so you can get the whole shebang in a couple of clicks.

Further, it is possible to customize each content type with individual options.

Installation 
------------
* For image thumbnails first need to install and enable ImageCache and ImageAPI modules
* For image popups first need to install Lightbox2 (recommended), Thickbox, Fancybox or Shadowbox module
* Copy the module's directory to your modules directory and activate the module
* For upgrading from 6.x-1.x to 6.x-2.x need to run update.php
* For upload progress bar need server-side uploadprogress (PECL) or APC

Usage
-----
* Module starts working once enabled
* There are new site-wide settings on the file uploads form (/admin/settings/uploads)
  under "Attachments display" section
* If "Insert" module is installed and enabled, enable and configure Insert options on the file uploads form (/admin/settings/uploads)
  under "Attachments display" > "Insert" section
* Image and attachment display options are configured in each content type:
  Go to /admin/content/node-type and select specific content type, then edit the "Attachments display" section
* ImageCache preset is required for thumbnails:
* - AttachmentThumbnail preset (default size & crop 60x60) is automatically created by the module
* - AttachmentThumbnail can be modified, or any other preset used
* - AttachmentThumbnail view permissions are automatically added to anonymous and authenticated roles
* iTweak Upload completely takes over the attachments display settings on comments. It removes setting "Attachments on comments" > "Image attachments on comments" in content type forms by comment_upload.module


Further Customizations
----------------------
There is a number of theme functions introduced, which can be customized in a theme.
- theme_itweak_upload_thumbnail()      - Renders thumbnails
- theme_itweak_upload_images()         - Renders image gallery
- theme_itweak_upload_images_body()    - Renders images in node body. Calls theme_itweak_upload_thumbnail() and theme_itweak_upload_images().
- theme_itweak_upload_images_teaser()  - Renders images in node teaser. Calls theme_itweak_upload_images_body().
- theme_itweak_upload_images_comment() - Renders images in comments. Calls theme_itweak_upload_images_body().
Please refer to http://drupal.org on how to customize theme_*() functions.

Included CSS file makes a simple horizontal scroll for images gallery, but it 
can be further styled with theme CSS file or by modifying the included one. 

It is possible to preview other file types besides images with additional 
modules. Developers can look into hook_itweak_upload_preview().

Credits
-------
Version 1.0 is based on code by hunvreus 
http://teddy.fr/blog/improving-file-attachments-form-drupal
(dated 2009-06-06)
Version 2.0 image gallery feature is inspired by Upload Image (http://drupal.org/project/upload_image)
Version 2.1 upload previews feature is inspired by Upload Preview (http://drupal.org/project/upload_preview)
Example picture for Insert preview of presets kindly provided by Tatjana Polyakova (Hawaii, Watercolor 6"x 8", http://tpol-art.com)

