Drupal.behaviors.quizProgress = function() {
  // tooltip
  $('.quiz-progress-block-wrapper a').tooltip({ showURL: false });
  
  $('#block-quiz_progress-0 a.quiz-progress').click(function() {
    var element_id = $(this).attr('id');
    var quiz_jump_id = element_id.replace('quiz-progress-id-', '');
    if (quiz_jump_id > 0) {
      $("#quiz-jumper").val(quiz_jump_id);
      $("#quiz-jumper").change();      
    }
  })

  
  if (Drupal && Drupal.settings && Drupal.settings.quiz_progress && Drupal.settings.quiz_progress.show_kipped == 1) {    
    $("#quiz-question-answering-form #edit-submit").click(function() {
      var jump_qid = $("#quiz-jumper").val();
      if ($("#quiz-progress-id-" + jump_qid + '"').length > 0) {
        var element_id = $("#quiz-progress-id-" + jump_qid + '"').parent().next().children().attr("id");
        var quiz_jump_id = element_id.replace('quiz-progress-id-', '');
        if (quiz_jump_id > 0) {
          $("#quiz-jumper").val(quiz_jump_id);
          $("#quiz-jumper").change();
        }
      }
    })
  }  
};



