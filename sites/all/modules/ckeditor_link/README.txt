CKEDITOR LINK - A PLUGIN TO EASILY CREATE LINKS TO DRUPAL INTERNAL PATHS
http://drupal.org/project/ckeditor_link



REQUIREMENTS
The CKEditor module or the Wysiwyg module
The CKEditor editor
Clean URLs need to be enabled.



INSTALLATION
Copy the ckeditor_link folder to your sites/all/modules directory.
Go to admin/build/modules and enable the module.

*Set permissions*
Go to admin/user/permissions and grant the CKEditor Link related permissions to
the desired roles.

*When using the CKEditor module*
Go to admin/settings/ckeditor and edit the desired profile.
Under "Editor appearance" > "Plugins", check the "CKEditor Link" box.
Save changes.

*When using the Wysiwyg module*
Go to admin/settings/wysiwyg and edit the CKEditor profile.
Under "Buttons and plugins", check both "Link" and "CKEditor Link" boxes.
Save changes.

*Set up CKEditor Link Filter*
Go to admin/settings/filters and edit the desired input format.
Check the "CKEditor Link Filter" box.
Save changes.

If you use other path converting filters like Pathologic or Path Filter, make
sure that CKEditor Link Filter comes before them:
Edit the input format again.
Click on the "Rearrange" tab.
Drag and drop CKEditor Link Filter before these filters in the list.
Save changes.

*Configure CKEditor Link*
Go to admin/settings/ckeditor_link.
Change settings as desired.
Save changes.



EXTENDING CKEDITOR LINK
Developers, see the ckeditor_link.api.php file.



CONTACT
Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
http://www.absyx.fr
