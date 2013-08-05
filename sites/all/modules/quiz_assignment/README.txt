QUIZ ASSIGNMENT
Allows creators of quiz to add a quiz assignment question type. This is very usefull if uploading of a file is part of a quiz/test.
Project URL: http://drupal.org/project/quiz_assignment


SPONSORS
Norwegian Center for Integrated Care and Telemedicine, Norway, http://www.telemed.no


INTRODUCTION
This module aims to solve the problem of handing in files as part of a quiz. 

When added to a quiz, the user will be given a file input field to use to upload locally stored files to the server. The quiz ownerer can then download this file and score it.



INSTALLATION
To install, unpack the module to your modules folder, and simply enable the module at Admin > Build > Modules.


CONFIGURATION
Settings are found here: admin/quiz/settings/questions_settings

Path to local storage: 
Define your path here. If left empty, Drupals storage system is used. Default is empty. If files are stored outside of Drupals storage system, they will be streamed through a script at "node/%/quiz_assignment/download/%".

Separate files for each quiz:
If checked, files will be placed in subdirectories under the local path defined. Default is checked.

Keep original filenames (TODO):
If checked, original filename will be used on the uploaded file when stored on server. If unchecked, the file uploaded will be given a more secure name. Default is unchecked.