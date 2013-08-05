api = 2
core = 6.x

;Include the definition for how to build Drupal core directly, including patches:
includes[] = drupal-org-core.make

; Download the OpenAtrium Install profile and recursively build all its dependencies:
projects[openatrium][type] = "profile"
projects[openatrium][download][type] = "git"
projects[openatrium][download][url] = "http://git.drupal.org/project/openatrium.git"
projects[openatrium][download][tag] = "6.x-1.7"
