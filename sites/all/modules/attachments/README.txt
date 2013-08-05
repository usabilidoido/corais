The Attachments module provides administrator and user with some additional tools for working with file attachments. Giving a clearer interface

* Delete or keep / Show or hide

The user interface for keeping or deleting file attachments has been a 
problem to users for a long time (see http://drupal.org/node/239800) and
it's unlikely to get solved until Drupal 8. The problem is that it is not
intuitive.

The Attachments module modifies the interface, as seen by the user, to
make the process clearer. It removes the checkbox and replaces it with a
button which displays the current action (Keep/Remove), it does the same
to the "List" checkbox which is also unclear and displays "Show/Hide"
instead.

The changes are implemented through jQuery (Progressive Enhancement) and
do not change the underlying functionality.

* Permitted file extensions

The display of which files extensions are accepted on the upload page can
be replaced by a new, configurable, display list.

Rather than just providing a simple list extensions can be grouped (such
as by overall type: Image, Document, Spreadsheet and so on) and the extensions
which belong to those groups can be set. So that when the attachment upload
element is displayed a much clearer list of extensions is provided.

There is a default group for any extensions not covered in the groups.

This feature is implemented in the module itself and does not require jQuery.

* File upload limit

The ability to limit the size of individual file uploads, and also to limit
the total file size limit per user already exists in Drupal. However this
Attachments module provides the ability to specify a (configurable) limit to
the number of files that can be uploaded per node. (On the example site this
is set to 2.) When the limit is reached the upload element is replaced by a
warning.

This feature is implemented in the module itself and does not require jQuery.

* File attachment display style

The final enhancement is to allow an alternate display style to the usual
table format. You can select a list style for file attachments instead which
displays the attachments as an unordered list. Currently the effect is global
for all content types that have file attachments, but will be enhanced to be
content-type specific.

This feature is implemented in the module itself and does not require jQuery.

* Example website

An example website is provided at: http://attachments.thewordsmith.net

You can log in as testuser1 to testuser5, the password is the same as the
username, each user can create two pages and each page can have two
attachments of less than 1Mb each. A block at the side displays the pages
belonging to the current user so they can be accessed and changed.

Steve Turnbull (adaddinsane)
