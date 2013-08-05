$(document).ready(function(){
  $('#quizreports-revision-changer').change(function(d){
    location.href = Drupal.settings.basePath + Drupal.settings.quiz_reports.basePath + '/' + $(this).val();
  });
});