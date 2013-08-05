DESCRIPTION
--------------------------
Enable site admins to limit group invites to current site users only. Also provides 
ability to limit invitees by role.


INSTALLATION
---------------
- Organic Groups module is required. 
- Enable og_invite_restrict module.
- Give "access user profiles" permissions to anyone who has access to the OG invite 
form.
- Select which roles can be invited via "can be invited to join groups" permission 
on admin/user/permissions - if you do not give any roles permission to join groups, then 
the og/invite/<group id> page's autocomplete function will never return any possible 
matches!
- That's it!


INTEGRATION WITH OTHER MODULES
---------------
If the RealName module (http://www.drupal.org/project/realname) is installed, OG 
Invite Restrict will display users' real names in addition to usernames.

TODO/BUGS/FEATURE REQUESTS
----------------
- see http://drupal.org/project/issues/og_invite_restrict

CREDITS
----------------------------
Authored and maintained by michael anello <manello AT gmail DOT com>
Sponsored by Ozmosis - http://www.ozmosis.com/ and LuthierBuilt - http://luthierbuilt.net/
