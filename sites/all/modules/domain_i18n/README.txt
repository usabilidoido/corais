// $Id: README.txt,v 1.2 2009/06/08 23:17:05 nonsie Exp $

/**
 * @file
 * README file for Domain Internationalization.
 */

INSTALLATION:
============

1. Copy module folder to 'sites/all/modules/'.
2. Modify settings.php and add the following to the bottom of the file:
$conf['domain_i18n_variables'] = array(
'variable_name',
);
Replace 'variable_name' with the name of the variable you wish to customize per domain and language. 
For example to customize user registration email subject and body insert the following:
$conf['domain_i18n_variables'] = array(
'user_mail_register_no_approval_required_body',
'user_mail_register_no_approval_required_subject',
);

LANGUAGE NEGOTIATION
====================
Domain Internationalization uses languages enabled for the default domain unless Domain Locale module is installed.
With Domain Locale installed language selection is based on the language set available for a specific domain.

USING DOMAIN INTERNATIONALIZATION WITH DOMAIN CONF MODULE
=========================================================
Domain Internationalization can be used in combination with Domain Conf, however if the variable defined using 
Domain Conf also exists in Domain Internationalization variables array in settings.php it will be overriden by 
Domain Internationalization module.

USING DOMAIN INTERNATIONALIZATION WITH I18N MODULE
==================================================
Domain Internationalization can be used in combination with Internationalization module, however the order of defining 
variables for these two modules in settings.php in cruicial if both modules use the same variable!
If the variable defined using Internationalization module multilingual variables in settings.php also exists in Domain 
Internationalization variables array in settings.php and Domain Internationalization settings are defined first then 
it will be overriden by Domain Internationalization module.
It also works the other way around - if the variable defined using Internationalization module multilingual variables 
in settings.php also exists in Domain Internationalization variables array in settings.php and Domain 
Internationalization settings are defined last then it will be overriden by Internationalization module. 

