Introduction
=============
Comment Login adds a user login section to the standard node comment form. This allows users to log in at the same time 
that they post a comment, and remain logged in thereafter. This module has the effect of disallowing anonymous commenting,
but still showing the form to anonymous users with the requirement that they also supply a valid account ti post.

Install
=============
Activate as usual. There is no special admin form or page (other than the node type admin page).

Usage
=============
Enable the comment login functionality in the node type administration form. In the Comment fieldset, you will be able to
select "No Login Form" (the standard behavior) or to embed the login form above or below the comment form elements. There will be
no changes to the comment form if the user is already logged in when the form is rendered. This module therefore has no visible effect
if anonymous commenting is not allowed.

Note that as with the standalone login form, the user and password fields are marked as required, so the form will not validate without
something entered in this fields. It will also fail validation if the login credentials are invalid.

Known Issues
=============
Although the module does not execute the login validation functions when the "Preview" button is clicked, basic validation will
still throw an error on "Preview" stating that username and password are required (when, semantically, they aren't for previewing).

You may want to theme in a link to user registration.


Credits
===========
Matt Johnson (johnson.matthew.david@gmail.com)

Sponsorship by New York Observer (see http://drupal.org/nyobserver)