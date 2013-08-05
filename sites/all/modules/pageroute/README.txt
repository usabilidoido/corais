$Id: README.txt,v 1.18.2.3 2009/09/15 13:40:37 fago Exp $

Pageroute Module 6.x-1.x-dev
Documentation see: http://drupal.org/node/310795


Pageroute Module
----------------


Other documentation files:
--------------------------
UPGRADE.txt ... for upgraders from 5.x
For an API look at the drupal handbooks.

Short module overview:
---------------------
pageroute.module - Allows the creation of pageroutes.
pageroute_ui.module - Adminstration pages for pageroute


Description
-----------
The module can be used to provide a user friendly wizard for creating and
editing several nodes. 
You can use the module to create a "route" which leads users through multiple
pages, which can be e.g. node creation forms.

For example this allows you to build a user profile which consists of multiple
content types. Then users can easily create and edit their nodes through the
same pageroute.


Installation
------------
 * Copy the whole pageroute directory to your modules directory and
   activate at least the modules pageroute and pageroute_ui.


Introduction
------------
Pageroute allows you define a route through various pages. There are several different
type of pages available. Pageroute comes with the following page types:
 * node add form
 * node edit form
 * user edit page
 * node display page

For further information regarding the page types have a look at the pageroute_ui page. 
Make sure that you have enabled the pageroute_ui module, then go to:
http://yoursite.example.com/admin/help/pageroute_ui

To create or edit your pageroutes activate the pageroute_ui module and go to
/admin/build/pageroute. Pageroute will automatically generate a new menu item
for each pageroute, which can be customized by using the menu module. Furthermore
it creates customizable back/forward buttons at the bottom of each page,
so that users are lead through the route.

Once you have finished defining your route you may deactivate pageroute_ui.module
again, as it provides only the adminstration pages.
   