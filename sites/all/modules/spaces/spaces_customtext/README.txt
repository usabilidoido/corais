
Spaces Custom Text
------------------

This module allows space administrators to customize a limited number of
strings for a given space through the use of the global $conf
locale_custom_text_[langcode] variable. It is heavily inspired by the
stringoverrides modules.

 
Known Incompatibilities
-----------------------

Because it uses the core localization system to customize strings, this module
is incompatible with any normal usage of the localization system. This includes
core locale module.

See spaces_customtext.module for more details.
