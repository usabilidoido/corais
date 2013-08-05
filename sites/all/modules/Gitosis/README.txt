CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Related modules
* Troubleshooting

INTRODUCTION
============
Gitosis module automates user access to a set of git repositories. Each group 
defines a list of git repositories which are accessible by all the users in it.

This module adds the following interactions:
* admin/settings/gitosis: allows you to set the url to your gitosis-admin 
  repository.
* When editing a group, a section called Gitosis lets you define git repositories 
  for it.
* When a user edits his account, a section called "SSH Keys" let's her add public 
  keys (thanks to sshkey module).
* There are three drush tasks involved:
  * 'gitosis' is the main task that will update your gitosis configuration 
    periodically.
  * 'gitosis-load-config' takes your current gitosis.conf and saves it at 
    admin/settings/gitosis > Main configuration.
  * 'gitosis-load-keys' looks for public keys at gitosis's keydir directory and
    attempts to match them with your users email, adding them if so.

INSTALLATION
============
1. You must have installed gitosis previously and had successfully cloned
   "gitosis-admin.git" repository. This means that the command
     git clone git@my.domain:gitosis-admin.git"
   should clone the repository with no problems. For more instructions on how to 
   install gitosis, refer to the following pages:
     * http://eagain.net/gitweb/?p=gitosis.git;a=blob;f=README.rst
     * http://scie.nti.st/2007/11/14/hosting-git-repositories-the-easy-and-secure-way
2. Make sure that the modules Content (CCK), Text (CCK), Organic Groups and 
   Sshkey are enabled. Drush is also needed.
2. The Content Type where Gitosis repositories will be managed must be called 
   "group". Otherwise the module will fail to add the field to manage repositories 
   in a group. This is configured by default if you are using Open Atrium.
3. Clear your cache and go to admin/settings/gitosis. In the first field, set your 
   gitosis-admin git url. In order to test it, run the following drush task:
     [path to drush] gitosis-load-config
   If the task completed with no errors, it will load the contents of your gitosis.conf
   file into the field "Main configuration for gitosis.conf" at admin/settings/gitosis.
4. If you want users to add themselves their public keys, grant the following permissions 
   to role "Authenticated user": manage own SSH public keys and view own SSH public keys.
   If there are public keys at keydir/ that you want to load into Drupal, run the 
   following task:
     [path to drush] gitosis-load-keys
6. As a first step, and without defining any repositories yet, run the following task:
     [path to drush] gitosis
   This just has updated your gitosis-admin loading the main configuration from 
   "Main configuration" and "Extra configuration" fields. If there were any new keys, 
   they were added too. The way to verify it is to clone the gitosis admin to a local 
   folder and review it.
6. Now go to one group with users which have ssh keys and edit it. Add a repository 
   and then run again gitosis drush task. If you update your local copy of 
   gitosis-admin, the new group shoud appear at gitosis.conf and all the users in the 
   group with a public key should be listed there.
7. In order to automate the process of updating your gitosis-conf repository, you need 
   to set a crontab that runs it. The frequency of it depends on you, but the suggestion 
   is to make it run every minute, such as the following example:
     crontab -e # opens your crontab console. Then type:
     */5 * * * * /path/to/php /path/to/drush gitosis --root=/root/directory/of/your/site
   For example, in my local box I have:
     */5 * * * * /usr/bin/php /home/juampy/drupal/sites/all/modules/drush/drush.php \ 
                                   gitosis --root=/home/juampy/Projects/drupal/site

RELATED MODULES
===============
http://drupal.org/project/sshkey
http://drupal.org/project/og
http://drupal.org/project/cck
http://drupal.org/project/drush

TROUBLESHOOTING
===============
* What happens if I block myself access to gitosis?
  You must have root access to the server where gitosis is installed and do the following:
  1. Gitosis install all repositories (including gitosis-admin) inside the home of the
    git user (or the user you normally use to clone repositories). This is usually at
    /home/git/repositories/
  2. Within that directory, you can edit gitosis.conf you grant yourself access again. If
    you need to add your public key, you can find the keydir directory at
    /home/git/repositories/gitosis-admin.git/gitosis-export/keydir
  3. Try to clone the gitosis-admin repository to a local folder. Once done, add the
    gitosis-admin configuration to "Main configuration" field at admin/settings/gitosis.

* Although I can clone gitosis-admin repository in the command line, "drush gitosis"
  outputs an error stating that the repository could not be cloned.
  Gitosis uses your default temp directory (normally /tmp) to clone the gitosis-adming.git
  repository, update its contents and push changes. This is defined at gitosis.module and
  used at gitosis.drush.inc. I have found a weird case in which I had to use a relative
  path to make it work. Here is a diff that explain the changes:
    diff --git a/gitosis.drush.inc b/gitosis.drush.inc
    index aa67015..2c3e59e 100644
    --- a/gitosis.drush.inc
    +++ b/gitosis.drush.inc
    @@ -184,7 +184,7 @@ function _gitosis_load_gitosis_admin() {
         if (is_dir(GITOSIS_REPOSITORY)) {
           $result = exec('rm -rf ' . GITOSIS_REPOSITORY);
         }
    -    $command = 'git clone ' . variable_get('gitosis_repository', '') . ' ' . GITOSIS_REPOSITORY; 
    +    $command = 'git clone ' . variable_get('gitosis_repository', '') . ' ../tmp/gitosis-admin'; 
         $result = exec($command, $response, $return_code);
       }
       if (!file_exists(GITOSIS_REPOSITORY . 'gitosis.conf')) {
    diff --git a/gitosis.module b/gitosis.module
    index 2c1d907..8955f02 100644
    --- a/gitosis.module
    +++ b/gitosis.module
    @@ -4,7 +4,7 @@
      * @file
      */
     
    -define('GITOSIS_REPOSITORY', sys_get_temp_dir() . DIRECTORY_SEPARATOR . 'gitosis-admin' . DIRECTORY_SEPARATOR);
    +define('GITOSIS_REPOSITORY', '../tmp/gitosis-admin/');

