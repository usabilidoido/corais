Description
-----------
With the flat_book module you can display subtrees of a big complicated book on a single page. This can make the book much more readable if the book has a lot of pages with a small amount of content. The user will be able to navigate to different sections of the book via the provided table of contents. 

Installation
------------
Install flat_book as you would any drupal module. You can download the module from drupal.org and place it in your modules directory. Alternatively, if you have drush installed, you can type:

drush dl flat_book

You then must enable the module by pointing your browser to http://www.example.com/admin/build/modules/list and clicking the check box for the flat_book module. With drush you can type:

drush en flat_book

Configuration
-------------

Flat book supports three types of flattening: Sitewide, bookwide, and branches. When you first install the module, all books are set to use the sitewide flatten depth (set to 3 by default). This may be changed on the book settings page (located at admin/content/book/settings). You may override this setting for any particular book on the "edit order and titles" page, available for any book at admin/content/book/%bid. On this page, you can choose a bookwide flatten depth for this book, and/or choose particular branches to be flattened. 

Author
------
Jonathan Pullano [critical_patch]
Project sponsored by Evolving Web

Support
-------
If you experience problems with the flat_book module, please post in the module issue queue. DO NOT post in the forums, it is very unlikely that I will see your post. The author and Evolving Web are not responsible for any damage done to you site or data lost through the use of the flat_book module.
