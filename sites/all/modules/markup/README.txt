
Description
-----------
This tiny module adds the ability for site admins to add "Markup" widgets to 
the form. These essentially let designers of cck content-types insert 
additional markup into the node/edit form to display to content-authors. This
is the equivalent of adding additional elements to $form of the type '#markup'.

The module does not add anything to the $node object for the content being 
created, and utilizes form_alter to remove unnecessary fields from the 
'_content_admin_field' form when this widget is being created.

Maintainers
----------------
smoothify - http://drupal.org/user/115335
ebeyrent - http://drupal.org/user/23897
cYU  -  http://drupal.org/user/202205


Install
-------
- Copy the unpacked folder "markup" in your modules folder.
- Go to the modules administration page (admin/build/modules) and activate the
  module (you will find it in the "CCK" package)

Upgrade
-------
- Replace the contents of the markup modules folder and run update.php.
- Visit the field / widget settings of each markup field you have and set the
  new 'Display Style' setting.

Usage
------
Once the module is activated, you can add a markup field to a content type on
the manage fields page for your chosen type. 

To Do
-----

- Allow use of PHP Code to create markup
- Add Markup View companion module to allow displaying a view.
- Add hooks for integration with AHAH Dependent Fields.

Support
-------
Check the issue queue of this module for more information:
http://drupal.org/project/issues/markup

