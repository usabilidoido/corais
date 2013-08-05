Booknav 1.0
===========
A module for showing full book navigation. 

Booknav aims to solve the following problems:

1. Two previous/next links blocks. Theses can i.e. be place above and below the blocks described below.
   This block can be configured to include all books on a site. It can also be configured to only be
   shown when reading a book. 

2. Create a expandable/collapsable treeview menu block for books.

3. Create a block for each book that implements problem 2.

4. Optionally, use top node title as title for block. In that case, the top node is removed from the book
   menu structure. 


About developers
================

Developed and maintained by National Center for Integrated Care and Telemedicine.
http://telemed.no/index.php?language=en

Contributors
fuzzy76 (http://drupal.org/user/239588)
borgewarvik (http://drupal.org/user/175185)

Booknav uses the jQuery plugin Treeview by Jörn Zaefferer:

  "Lightweight and flexible transformation of an unordered list into an expandable 
   and collapsable tree, great for unobtrusive navigation enhancements. Supports 
   both location and cookie based persistence."
  
http://bassistance.de/jquery-plugins/jquery-plugin-treeview/
http://plugins.jquery.com/project/treeview

Treeview is dual licensed under the MIT and GPL licenses.
http://www.opensource.org/licenses/mit-license.php
http://www.gnu.org/licenses/gpl.html

The plugin is not included in this module. See installation instruction below how to 
download and include plugin in module. THIS MODULE WILL NOT WORK WITHOUT THE PLUGIN!


Installation
============
1. Download module from http://drupal.org/project/booknav and extract it in
   the sites/all/modules/ directory.

2. Download the plugin and extract plugin into sites/all/modules/booknav/ directory.
   Make sure you have these two paths correct:
    
    JS:  booknav/jquery-treeview/jquery.treeview.js
    CSS: booknav/jquery-treeview/jquery.treeview.css

3. Enable module in Administer -> Site bulding -> Modules. This module requires the Book
   module to be enabled. This module is found in core, but are not enabled by default.
  
4. Visit Administer -> Site configuration -> Booknav settings and familiarize yourself 
   with the different settings/options for this module.

5. Create a book and place the blocks this module provides where you want them to be visible.