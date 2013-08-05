INTRODUCTION:
The Mass Contact module started out as a modified version of the core Contact
module. It allows anyone with permission to send a single message to a role or
group of roles (or even to all registered users). Over time, it evolved to
something more than just a basic contact module to a group of users.


FEATURES:
Here are a list of some of the features this module has:
 * You can send a message to one or more groups (referred to as categories) of
   users, which are specified by role.
 * Large recipient lists may be broken up into smaller chunks.
 * The site administrator can control how many messages are allowed to be sent
   by a single person in an hour.
 * The message may be sent such that the recipients' e-mail addresses are
   hidden from each other.
 * The message may be sent as plain text or HTML, even specifying the input
   format filter to use.
 * The message may include one or more binary file attachments.
 * The site administrator may specify different texts to be placed at the
   beginning and/or the end of every message that is sent out.
 * A copy of the message may be saved as a node.
 * Users may opt-out, by category, of receiving mass mailings on their account
   settings page.


INSTALLATION:
This module is installed in a standard way. Generic instructions for how to do
that can be found here: http://drupal.org/getting-started/install-contrib


CONFIGURATION AND SETUP:
The place where you create categories and modify the module's settings is
found in the same place as core's Contact module, the Site building section of
the Administer page (admin/build/mass_contact).

You need to add at least one category before sending any mass e-mails, which
can be done at the same location where the administrative settings are.


MORE INFORMATION:
This module works by sending a single e-mail to your mail server with the
recipients' e-mail addresses in either the 'To:' or 'Bcc:' field. The mail
server is then responsible for parsing out the recipients' addresses and
forwarding the message along to everyone.

Here is some scaling information:
 * This module retrieves user ids and emails in a scaled way: no
 * This module sends email in a scaled way: yes, within server limits
 * This module keeps connections up while the long process continues: no

Here are all the menu items/links that are available and what they do:
URL               | Label             | Description        | To have access to
                  |                   |                    | this URL, users
                  |                   |                    | must have this
                  |                   |                    | permission
------------------------------------------------------------------------------
/admin/build/     | Mass Contact form | The main           | administer mass
mass_contact      |                   | administrative     | contact
                  |                   | interface, which   |
                  |                   | defaults to the    |
                  |                   | Category list page |
                  |                   | below.             |
------------------------------------------------------------------------------
/admin/build/     | Category list     | List the currently | administer mass
mass_contact/list |                   | defined            | contact
                  |                   | categories.        |
------------------------------------------------------------------------------
/admin/build/     | Add category      | Add a new          | administer mass
mass_contact/add  |                   | category.          | contact
------------------------------------------------------------------------------
/admin/build/     | Edit Mass Contact | Edit an existing   | administer mass
mass_contact/     | category (the     | category.          | contact
edit/$category_id | 'edit' operation  |                    |
                  | in the Category   |                    |
                  | list)             |                    |
------------------------------------------------------------------------------
/admin/build/     | Delete Mass       | Delete an existing | administer mass
mass_contact/     | Contact category  | category.          | contact
delete/           | (the 'delete'     |                    |
$category_id      | operation in the  |                    |
                  | Category list)    |                    |
------------------------------------------------------------------------------
/admin/build/     | Settings          | Administrative     | administer mass
mass_contact/     |                   | settings to modify | contact
settings          |                   | how Mass Contact   |
                  |                   | operates. There    |
                  |                   | are three sub      |
                  |                   | pages under this   |
                  |                   | one.               |
------------------------------------------------------------------------------
/mass_contact     | Mass Contact      | The main Mass      | send mass
                  |                   | Contact form for   | contact e-mails
                  |                   | sending messages.  |
------------------------------------------------------------------------------
/node/add/        | Mass Contact      | The form for       | create
mass_contact      |                   | adding a Mass      | mass_contact
                  |                   | Contact content    | content
                  |                   | item.              |
An easier to view table can be found here:
http://drupal.org/node/760548#comment-2912412


TROUBLESHOOTING AND MISCELLANEOUS:
 * There is something the site administrator and/or the sender needs to keep
   in mind when breaking up a large recipient list into smaller chunks and
   sending the message as BCC (hiding the recipients from each other), and
   that is that the sender will receive a copy of the message for every group
   of recipients the list is broken up into. That is normal behavior and
   cannot be changed.
 * If your category permissions are not showing up correctly, check your
   category name and make sure you don't have any stray characters or any
   characters that Drupal doesn't allow.
 * If you experience "return-path" errors when sending e-mail, you can try the
   Return-Path module (http://drupal.org/project/returnpath) to see if that
   solves your problem.
