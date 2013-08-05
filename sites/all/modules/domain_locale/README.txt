Requirements:
- for the default domain enable all languages you want to enable on one or more subdomains. The reason lies in language_list() in bootstrap.inc
- set language negotiation to "Path prefix only". We are overriding $language in bootstrap hence overwriting any changes that might of been user's preferred language specific. Also user's preferred language might not exist all domains. You can leave the setting to "Path prefix with language fallback" but the language fallback will be overwritten.

Optional:
- make sure all languages have a prefix (by default English does not). This gives unique URLS for all languages.
