$Id$
-----------------------------------
Description
-----------------------------------
This project is a suite of modules that will add time tracking features to any
node or any nodes comments.

Time Tracker Features

  * Track Time on Nodes and/or Comments
  * Track time using time intervals (start and end times) or durations 
  * Integrated Timer to track time as you go
  * Time estimating
  * User Time Sheets to display weekly logged time
  * View for reporting on tracked time
    
Time Tracker Dependencies

  * Views (specifically Views 2)
  * Views Calc
  NOTE: Time Tracker will actually work without views or views calc, 
  you just won't be able to access the time tracker reports page
  
-----------------------------------
Integration
-----------------------------------

We originally developed this project for use with Open Atrium, but recently we
have abstracted the Open Atrium aspects into Features and committed them to 
github. Currently we have two features that extend Time Tracker in Open Atrium

  * Atrium Time Tracker 
    https://github.com/fuseinteractive/Atrium-Time-Tracker
  * Atrium Time Tracker Reports 
    https://github.com/fuseinteractive/Atrium-Time-Tracker-Reports

We highly recommend using these features if you are implementing time tracker 
on your Open Atrium install.

There is some casetracker integration built into Time Tracker. It is mostly
for displaying casetracker project names when viewing any node that is 
deemed a 'case' by case tracker

Time Tracker also has full views integration and comes with a sample 'reports'
view that is ready to use.

-----------------------------------
Installation
-----------------------------------

1) Place this module directory in your "modules" folder (this will usually be
"sites/all/modules/"). Don't install your module in Drupal core's "modules"
folder, since that will cause problems and is bad practice in general. If
"sites/all/modules" doesn't exist yet, just create it.

2) Enable the module.

-----------------------------------
Usage
-----------------------------------


** Time Tracker **
-----------------------------------

To begin tracking time go to the edit content type screen for any content type
(admin/content/node-type/%content-type-name) and scroll to the 'Time Tracker
Settings' fieldset. There, you can choose to track time either on this content
type's nodes or this content type's comments. Tracking time on nodes will give
you a time entry sheet and a table of time entries at the bottom of a node's
content, but before the comments. Tracking time on comments will add a time
entry sheet to the comment form. Keep in mind that if you choose to track time
on a content type's nodes as well as comments, you will only ever see comment
time entries in the comment thread, and node time entries in the time entry
table. However, if you were to use views to view the time entries, you could
see them all with no distinction.

We recommend deciding ahead of time whether or not you would like to track 
time on comments or nodes. We've found that people who just want to jot
down time entries with minimal notes prefer to track time on nodes. However
people who like extensive notes along with their time entires, as well as the
ability to thread conversations based on time entries, prefer to use comments
as their vessel for tracking time.

Settings for Time Tracker can be found at admin/settings/time_tracker. They 
should be mostly self explanatory. We recommend, however, deciding beforehand
if you wish to track time using durations (e.g. 2 hours) or time intervals
(e.g. 1:00pm to 2:00pm).

* Optional Fields *

Some fields are optional. They can be enabled /disabled on the time tracker 
settings page:

 - Billable:   A simple checkbox to flag the time entry as billable
 - Billed:     A simple checkbox to flag the time entry as billed
 - Deductions: This field can be used to log deductions from the total time

* Activities *

Activities are specific classifications for time entries. You can administer
activities at admin/settings/time_tracker/activity/list.

Disabling an activity just makes it so you can't choose it anymore. 
Past time entries can still reference it will display the activity name.

Deleting an activity deletes it completely from the db, thus orphaning any 
past time entries that are referencing it.

* Permissions *

Double check your permissions before getting started:

* add time tracker entries	
	Permission to allow users to track time
	
* view all time tracker entries
* view own time tracker entries
  Allow users to view time entries
	
* edit time entries	
	Allow users to edit time entries
	
* delete time entries	
	Allow users to delete time entries
	
* administer time entries	
	Gives access to additional options when editing a time entry:
	 - Allows locking of time entires (if that particular setting is on)
	 - Allows editing of locked time entries
	 - Allows changing of username associated with a time entry
	
* administer time tracker	
	Access the administration pages
  

** Time Estimate **
-----------------------------------

Adds a time estimate field to content types that are tracking time. This
module is very simple and has no settings page.


** Time Tracker Timer **
-----------------------------------

This module adds a simple timer for tracking time. Users can start the timer
before beginning work and stop the timer when finished. The time entry form
fields are then automatically updated with the timer results for easy time
logging.

This module also comes with an (current user's) Active Timers block as well as
Page which will show all active timers throughout the site (needs permission 
'view all timers')

* Resuming the timer *

The timer can also be resumed once stopped. For duration based time entries
this works as expected, however in order to be able to resume the timer for
interval based time entries, you must enable the deductions field. The time
in which the timer is stopped is logged in the deductions field and the start
and end times are the first time the timer is started, and the last time it
was stopped, respectively.

* Permissions *

The time tracker timer by default disables manual time entry. Manual time
entry can be re-enabled using the permission 'manually enter time'.

You can also separately enable or disable manual time entry for the deductions
field using the permission 'manually enter deductions'


** Time Sheet **
-----------------------------------

The time sheet provides a simple weekly summary of time entries for a give 
user. It can be found at (user/%user_id/time_sheet). There will also be a link
as a local task (tab) on the user profile page.

Settings for the Time Sheet can be found at 
admin/settings/time_tracker/time_sheets


-----------------------------------
Themeing
-----------------------------------

There are theme functions for all html output from the Time Tracker modules.
If something on the screen is produced by Time Tracker, you should be able to
find the theme function near the bottom of the corresponding module.


-----------------------------------
Sponsors
-----------------------------------
Fuse Interactive - Greg Gillingham
http://www.fuseinteractive.ca
   

-----------------------------------
Authors
-----------------------------------
Chris Eastwood
Codi Lechasseur
Andre Chun

If you have any questions or comments about this module, or if you have any
bugs to report (or features to request!) please use issue queue on the Time
Tracker project page on drupal.org (http://drupal.org/project/time_tracker)
