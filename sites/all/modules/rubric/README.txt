//ELMS: Rubricator -- Create online rubrics and associate them to nodes.  Particularly useful when paired with the Assignment Studio.
//Copyright (C) 2009  The Pennsylvania State University
//
//Bryan Ollendyke
//bto108@psu.edu
//
//Keith D. Bailey
//kdb163@psu.edu
//
//12 Borland
//University Park,  PA 16802
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License,  or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License along
//with this program; if not,  write to the Free Software Foundation,  Inc.,
//51 Franklin Street,  Fifth Floor,  Boston,  MA 02110-1301 USA.

-----------------
Required Modules
-----------------
AJAX - The ajax module makes it so that in special submission cases on the node form that it won't scrap your progress due to an odd caching issue in Druapl 6 when it comes to failing validation of a form.  The AJAX module helps in the proper handling of the Rubricator.

-----------------
Optional Modules
-----------------
Assignment Studio -- Rubricator is a plugin for the Assignment Studio but does work as a stand alone product as well.

-----------------
Installation
-----------------
  *Download from drupal.org
  *Place in your modules folder of choice and then activate it at admin/build/modules
  *Go to admin/user/permissions to configure who can create the node type that will have rubrics associated to it.
  *Go to admin/settings/rubric to set which content type should have rubrics associated to it
  *Start creating rubrics!
  
-----------------
Known Issues
-----------------
  *May need to lock the interface while ajax calls that affect IDs are taking place.  There is the potential (related to performance) of an ID not to clone and change in time before adding a column within it.  Therefore this will splice which factor a column is associated to on the screen and in the database.  I've only been able to cause this to happen once in the hundred plus hours I've been developing this.
  *Nothing other then the potential performance issues :)
-----------------
Compatability
-----------------
The Rubricator should work in the follow browsers:
Camino
Chrome
Firefox 3 (Mac & PC)
Flock (Mac & PC)
IE 8
Netscape
Opera 9.6 (Mac & PC)
Safari 4 (Mac & PC)
Seamonkey (Mac & PC)
Webkit

Known to not work in:
Maxthon 2

-----------------
Notes
-----------------
This project is very complex and thus far largely doesn't exist elsewhere.  As such, it's design is very much open for debate / discussion.  We are willing to work through making changes to the end product to meet needs or make it match different instructional models if they are ones we ourselfs could implement.

All Feedback is very appriciated.  While this product does serve our department first, I hope that it meets the needs of others and that they can help refine our process / product in order to move us all forward.