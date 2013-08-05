IMAGE FUpload MODULE
--------------------

Image FUpload (Multiupload) which is an addition to image module, 
gives the ability to every user who is allowed to upload images by 
using image module, to upload multiple images with one simple click. 
All selected images are uploaded via a flash module (swfupload) and are 
automatically processed.
So, there's no need any more to upload image by image. 

Author:
Stefan Smarzly (grandcat)

INSTALL
--------------------

1.) Copy this directory to a suitable modules directory, 
    such as sites/all/modules
2.) Download SWFUpload-Core v2.2.* at http://code.google.com/p/swfupload/ and extract
    the following files from the zip file: swfupload.swf, swfupload.js, swfupload.queue.js
    Put these files into sites/all/modules/image_fupload/swfupload subdirectory
3.) Activate the module and its submodules (if needed) in the module list.
    (dependency: image module or/and imagefield module)
4.) Now, the module is active and can be used at node/add/image, 
    if image module is used. If imagefield module is used, the widget of one imagefield of
    any content type has to be replaced by 'image fupload' widget.
5.) Access Permissions have to be set by using the corresponding configuration pages
    of the used modules.

UNINSTALL
--------------------

1.) Deactivate Image FUpload and its submodules at admin/build/modules
2.) Uninstall Image FUpload and its submodules at admin/build/modules/uninstall