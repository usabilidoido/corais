// $Id: README.txt,v 1.3 2009/07/01 02:17:28 johnfyoung Exp $

LinkImageField provides an "LinkImage" widget type to CCK. This module extends the 
ImageField widget, allowing a user to provide a URL link for it.

Dependencies
------------
 * ImageField
 * FileField
 * Content

5.x Version was written by Kamal Challa
6.x Version was written by John Young

Install
-------

1) Copy the linkimagefield folder to the modules folder in your installation.

2) Enable the module using Administer -> Site building -> Modules
   (/admin/build/modules).

3) Create a new linkimagefield through the CCK's interface. Visit Administer ->
   Content management -> Content types (admin/content/types), then click
   Manage fields on the type you want to add an image upload field. Select
   "File" as the field type and "LinkImage" as the widget type to create a new
   field.
   
4) To establish the proper display, make sure that the correct formatter is 
   selected. Visit Administer -> Content management -> Content types 
   (admin/content/types), then click Display fields on the type with the 
   Link Image Field.  Make sure "LinkImage" is selected for Teaser and Body.