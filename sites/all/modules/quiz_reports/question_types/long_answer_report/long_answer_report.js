$(document).ready(function(){
    $('.answers .answer .output .short-answer a.quiz-stats-toggle-more').click(function(){
        $(this).parents('.output').find('.short-answer').hide().end().find('.long-answer').show();
    });
    
    $('.answers .answer .output .long-answer a.quiz-stats-toggle-less').click(function(){
        $(this).parents('.output').find('.long-answer').hide().end().find('.short-answer').show();
    });
});