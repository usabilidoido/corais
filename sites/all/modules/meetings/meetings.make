; $Id: meetings.make,v 1.1.2.8 2010/08/16 08:38:13 auzigog Exp $

; About this file
; ---------------
; Use this file to easily install all of the dependencies for the Meeting
; module. Of course, you need drush and drush_make installed first.
; Simply run the following command:
;
;     drush make --no-core meetings.make

core = 6.x

; Project dependencies
projects[] = cck
projects[] = ctools
projects[] = date
projects[] = features
projects[] = filefield
projects[] = jquery_ui
projects[] = messaging
projects[] = notifications
projects[] = notifications_team
projects[] = strongarm
projects[] = token
projects[] = views
projects[] = votingapi

; Meetings itself
projects[] = meetings

; Libraries
libraries[jquery_ui][download][type] = "get"
libraries[jquery_ui][download][url] = "http://jquery-ui.googlecode.com/files/jquery.ui-1.6.zip"
libraries[jquery_ui][directory_name] = "jquery.ui"
libraries[jquery_ui][destination] = "modules/jquery_ui"