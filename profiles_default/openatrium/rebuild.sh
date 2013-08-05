#!/bin/bash
#
# This command expects to be run within the Open Atrium profile directory. To
# generate a full distribution you must be in a git checkout.
#
# To use this command you must have `drush make` and `git` installed.
#

if [ -f drupal-org.make ]; then
  echo -e "\nThis command can be used to run drupal-org.make in place, or to generate"
  echo -e "a complete distribution of Open Atrium.\n\nWhich would you like?"
  echo "  [1] Rebuild Open Atrium in place."
  echo "  [2] Build a full Open Atrium distribution"
  echo -e "Selection: \c"
  read SELECTION

  if [ $SELECTION = "1" ]; then

    # Run openatrium.make only.
    echo "Building Open Atrium install profile..."
    drush make --download-mechanism='drush_make' --working-copy --no-core --contrib-destination=. drupal-org.make

  elif [ $SELECTION = "2" ]; then

    # Generate a complete tar.gz of Drupal + Open Atrium.
    echo "Building Open Atrium distribution..."

    MAKE=$(cat build-openatrium.make)

    TAG=`git describe --tags --abbrev=0`
    if [ -n $TAG ]; then
      if [ $TAG = "fatal: No names found, cannot describe anything." ]; then
        MAKETAG=""
        VERSION="head"
      else
        MAKETAG="projects[openatrium][download][tag] = $TAG"
        VERSION="${TAG:4}"
      fi
      MAKE="$MAKE\n$MAKETAG\n"
      echo -e "$MAKE"
      NAME=`echo "atrium-$VERSION" | tr '[:upper:]' '[:lower:]'`
      echo -e "$MAKE" | drush make --download-mechanism='drush_make' --yes --tar - $NAME
    else
      echo 'Could not determine git tag. Is openatium git clone checkout?'
    fi
  else
   echo "Invalid selection."
  fi
else
  echo 'Could not locate file "openatrium.make"'
fi
