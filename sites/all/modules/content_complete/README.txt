// $Id: README.txt,v 1.1.2.8 2010/05/16 14:48:17 pvhee Exp $

-- SUMMARY --

Content Complete is a CCK extension module. The module allows privileged users 
to tag CCK fields needed for completion. The module checks these tagged fields 
against the content provided for those content types. The complete percentage can 
be shown to the userin the form of a block, together with quick links to complete the 
missing fields. When the percentage is equal to 100%, the block won't be shown.

This module is related to and based on the "Profile Complete Percent" module 
(http://drupal.org/project/pcp), which implements similar functionality for user 
profiles. This module is an excellent alternative when CCK is used to maintain 
user profiles, and at the same time can be reused in other situations. For
example, you might have a 'settings' content type on your website with configuration 
settings. You could show the administrator how much fields there are left to 
complete the settings using a block.

Tutorial (part 1) - "Introducing Content Complete, A CCK-based Drupal module"
http://nuvole.org/blog/2010/apr/05/measure-node-completion-in-drupal-part-1

Rules integration is provided, see below.

Views integration is new since version 1.2, see below.

For a full description of the module, visit the project page:
  http://drupal.org/project/content_complete

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/content_complete


-- VIEWS -- 

Tutorial (part 2) - "Different Views on completion data"
http://nuvole.org/blog/2010/apr/08/measure-node-completion-in-drupal-part-2


-- RULES -- 

Rules integration is provided with a rule "Content Complete % is >= than x". 
For more information about rules see http://drupal.org/project/rules. Modules which 
provide integration with rules are listed here:
http://groups.drupal.org/node/10270/rules-modules

Tutorial (part 3) - "Setting up an editor workflow using Rules and Content Complete"
http://nuvole.org/blog/2010/apr/12/measure-node-completion-in-drupal-part-3

-- REQUIREMENTS --

CCK.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> Permissions >>
  content_complete module:

  - administer content complete

    Users in roles with the "administer content complete" permission will be able
    to administer the module.

  - access content complete

    Users in roles with the "access content complete" permission will be able
    to see the content complete statistics.

  Note that the content complete block depends on the actual permissions of the 
  viewing user. For example, if the user does not have the permission to edit 
  nodes of type "page" the content complete percentage will be shown, but the quick
  edit links will be hidden.

* Enable completeness checks per content types. Go to Content Management >> 
  Content Types, and click 'edit' on the content type you wish to enable 
  completeness for. Go to 'Content complete' and select 'Enabled' together
  with the fields you would like to have included in the completeness checks.
  
* Several blocks are made available:
 
  - Content Complete: Node Completeness. 
    Can be used to show the completeness for the current node (works for any 
    enabled content type). The block will only appear in node context
    
  - Content Complete: *type name* (First Node).
    For each content type this block will be provided. It will show the
    completeness for the first node of that type the user has edit permissions
    for, or in the case of Content Profile, the current node associated with
    the user.
    
  Note that those blocks can also be created using views, and views arguments.
  
* Rules integration is provided with a rule "Content Complete % is >= than x". To learn
  more about rules, please see http://drupal.org/project/rules

* For styling, there are several classes and ids available. For example, you can use 
  '.cck-complete-percent-bar-leq-25' to style the appearance of the percent bar if the 
  percent is lower or equal (leq) to 25. Likewise, there are classes for leq-50,
  leq-75 and leq-100. Absolute numbers can be styled using '.cck-complete-percent-bar-x'
  (with x replacing the actual percentage).

-- CUSTOMIZATION --

The block output can be entirely overridden in your template files. 
Check content_complete.module for more information.

-- FAQ --

Q: My completeness values are not correct.
A: Try cleaning the completeness cache, go to Content Management >> Content Complete
   and click 'Rebuild cache'.

-- CONTACT --

Current maintainers:
* Peter Vanhee (pvhee) - http://drupal.org/user/108811
* Ronan Berder (hunvreus) - http://drupal.org/user/49057

This project has been sponsored by:
* Nuvole (from 1.2 onwards)
  http://www.nuvole.org
* Youth Agora (until version 1.1)
  Innovating online youth information. 
  Visit http://www.youthagora.org for more information.
  
Version 1.2 has partially been sponsored by:
* The Ottawa Hospital Cancer Centre Breast Cancer Disease Site Group