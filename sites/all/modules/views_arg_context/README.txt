$Id: README.txt,v 1.2.2.2 2010/10/25 20:27:58 brynbellomy Exp $

Many thanks to Dave Kopecek (http://drupal.org/user/33578) for writing the first version of this README.

Usage:

This module uses the context module's context_get() function to deliver data to a view through one of that view's arguments.  This is particularly useful in two cases.  First, when you don't want the view to get its arguments from the URL (but rather from a controller like hook_nodeapi), or when the view cannot accept arguments (as is frequently the case in a views block). You'll need to create a small helper module to set values specific to your needs.

Example:

Say you have a CCK content type called 'department' and have told Drupal to render a views block with some relevant data every time a department node is displayed.  In order for the block to render this data, it needs to know which department node is being shown.  Traditionally, this would only be possible by calling arg(x), which is hackish, as it fails to allow the controller (in this case, hook_nodeapi) to setup and sanitize data for what is displayed.  It would be better to pass the NID of the department to the view as an argument from the controller itself (nodeapi).


1. Create a helper module for your project (this might be optional in your case... see "AUTOMATICALLY GENERATED DATA" at the end of this README):


mysite_helper.info file:

; $Id: README.txt,v 1.2.2.2 2010/10/25 20:27:58 brynbellomy Exp $
name = Mysite Helper
description = Provides helper functions for My Site
dependencies[] = views
dependencies[] = context
dependencies[] = views_arg_context
core = 6.x
 

mysite_helper.module file:


<?php
// $Id: README.txt,v 1.2.2.2 2010/10/25 20:27:58 brynbellomy Exp $

/**
* Set the context argument for context module using views_arg_context module
* See http://drupal.org/node/723792
**/

function mysite_helper_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL) {
  if($node->type == 'department') {
    context_set('department', 'current_dept_nid', $node->nid);
  }
}


2. Enable your module.

3. Create a new view (or edit an existing one).
-- Set your View to accept the argument "Node: nid".
-- You should now be editing the "Node: nid" argument.
-- Click the radio button for "Provide default argument".
-- Select "Active Context" for the Default argument type.
-- Fill in the Namespace and Attribute fields (in our example the namespace is "department" and the attribute is "current_dept_nid").
-- Update the argument settings and save your view.

Your view will now be passed the NID of the current page when its node type is 'department'.

You can use this module to send all kinds of information through Views' argument machinery, including text strings and multiple argument strings (of the format "arg1+arg2+arg3").


*** AUTOMATICALLY GENERATED DATA
There are certain pieces of information automatically exposed to Views by this module, meaning that many users won't have to write custom modules in many use cases.  Here is a current list:

Namespace   Attribute     Argument to be used with
=========   =========     ========================
node        type          Node: type
node        nid           Node: nid
node        uid           Node: uid
node        name          User: name
node        title         Node: title
taxonomy    tids-and      Taxonomy: Term ID or Term ID (with depth) **note: this filters nodes containing ALL of the terms of the current node
taxonomy    tids-or       Taxonomy: Term ID or Term ID (with depth) **note: this filters nodes containing ANY of the terms of the current node
user        uid           User: Uid
user        name          User: Name
user        language      User: Language
user        email         User: E-mail

When using tids-and or tids-or, you MUST check the 'Allow multiple terms per argument' box when configuring your Taxonomy argument.