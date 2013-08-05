Course - an e-learning module for Drupal

Features

     * Allows any content-type to be used as an e-learning course,
       containing any number of graded or ungraded course objects.
     * Course object API to define learning objects to be added to a
       workflow
          + Built in support for Drupal nodes to be part of a course
            requirement workflow
          + Built in support for several course objects (see Getting
            started)
          + Extensible to allow other content/assessments or non-Drupal
            (external) objects to be delivered and tracked
     * Course API to allow access to taking courses/enrolling into courses
     * Framework for external learning application integration (such as
       Moodle)
     * Views 3 integration, including several default views for course
       listings and user status

Dependencies

     * PHP 5.3+
     * Chaos tools for ahah/modal forms.
     * Views 3.x (optional) for most reports and user transcript.
     * AHAH Helper 2.x for some form functionality.
     * The Drupal 6 version of Course requires Autoload 2.x.

Getting started

    1. Enable course module, and a bundled course object:
          + course_book - Use Drupal Books as course objects
          + course_certificate - Award a Certificate on course
            completion
          + course_content - Use any content type as a course object
          + course_object_manual - Arbitrary steps which must be marked
            complete by an administrator before a learner may proceed past
            them
          + course_poll - Poll requirement
          + course_quiz - Graded Quiz object
          + course_scorm - Exposes CCK Scorm Field as Course objects
          + course_webform - Webform submission requirement
    2. Set up the "Course outline" block at admin/build/blocks
    3. Go to Create content -> Course
    4. Add new course objects, "Quiz" will be available
    5. Go to the "take course" tab, and set up questions for your quizzes.
    6. Take the course!

Enrollments and attendance

     * Course comes bundled with course_signup, to use Signup as a
       registration and attendance management system. Attendance can be a
       requirement for completion of a course.

E-commerce support

     * 6.x - Course comes bundled with course_uc, which provides
       Ubercart actions to enroll a user after purchasing a course
       product.
     * 7.x - Course is planning to support Commerce.

Course credit

     * This module has moved to its own project.
     * Course credit will allow an admin to assign and map credit
       types to learner profiles and courses. Learners will then be able
       to receive or claim credit that they are eligible for on completion
       of a course.
     * Credit can appear in a completed activities view and is exposed to
       Token for use in a module like Certificate.
     * Course credit also includes course_restrict_credit, which restricts
       claiming credit to only one of many similar courses.

Reporting

     * Course report areas for global (course-level) reports and
       individual (object-level) reports.
     * API to allow course objects to provide their own reports.

Extras

     * Course Moodle Objects for integrating Moodle
     * Course Requirements for requiring completion of other courses
       for enrollment in another (prerequisites)

Credits

     * This project is sponsored by DLC Solutions for EthosCE

API Documentation
     * Please see course.api.php for hook definitions
     * Class structure and more documentation can be viewed at
       http://api.dlcdev.com
     * Check out the example course object implementations in modules/
