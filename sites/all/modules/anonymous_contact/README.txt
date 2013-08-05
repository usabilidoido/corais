Anonymous Contact allows people that are not logged into the site to contact 
users via e-mail via a contact form. I had a look around for this and was 
surprised to find there weren't any modules that already handled this. I did
find interest in it though so hopefully this module will see a bit of use.

I would strongly suggest that it be used with some form of captcha. The 
CAPTCHA module works. Mollom doesn't for some reason.

At this point the module is quick and dirty, but works for what I need. It 
doesn't touch the existing contact module at all.

I would like to add;

    * Restrict those that can be contacted to a role or roles
    * Figure out how to get this form visible to Mollom
    * Allow Users to control if they can be contacted through this

I'm open to patches to cover the above. If you would like access as a 
co-maintainer I'm happy to take requests for that also. I'm too damned busy to
be able to spend too much time on this at the moment.

Thanks to Simon Gurnsey for funding the development and allowing me to 
contribute this back to the community.

USING ANONYMOUS CONTACT

Ensure that at least the Anonymous role has access to it through permissions.

The Anonymous Contact form can be accessed through 2 paths;
http://mydomain.com/contact_form/[uid]
http://mydomain.com/user/[uid]/contact_form
