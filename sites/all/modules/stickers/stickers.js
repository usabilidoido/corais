// $Id: stickers.js,v 1.1 2010/10/08 14:26:49 Countzero Exp $ 

Drupal.behaviors.stickers = function (context) {
    var stickers = Drupal.settings.stickers;

    var hp = $('#header').position();

    if (location.href.indexOf('imce') == -1) {
        $.each (stickers.stickers, function (i, val) {
            $('body').append(val['html']);
            var sleft = parseInt(val['x'], 10) + parseInt(hp.left, 10);
            var stop = parseInt(val['y'], 10) + parseInt(hp.top, 10);        
            $('#stickers-' + val['nid']).css({'left' : sleft + "px", 'top' : stop + "px"});
        });
    }

    var current_sticker = '#stickers-' + stickers.params.cursticker;
    
    $('.imgstick').show();
    $('#stickers-' + stickers.params.cursticker).draggable({
        stop: function(event, ui) {
                            var xy = ui.offset;
                            $('#edit-field-x-0-value').val(Math.floor(ui.offset.left - hp.left));
                            $('#edit-field-y-0-value').val(Math.floor(ui.offset.top - hp.top));                            
                        }
    });

    var url = window.location.pathname;

    $("#valx").slider({
			value: $('#edit-field-x-0-value').val(),
			min: 0,
			max: 900,
			step: 1,
            slide: function (ev, ui) {
                        var element = $(ev.target);
                        $('#edit-field-x-0-value').val(ui.value);
                        $(current_sticker).css('left', parseInt(hp.left, 10) + parseInt(ui.value) + 'px');
            }
    });    
    $("#valy").slider({
			value: $('#edit-field-y-0-value').val(),
			min: 0,
			max: 900,
			step: 1,
            slide: function (ev, ui) {
                        var element = $(ev.target);
                        $('#edit-field-y-0-value').val(ui.value);
                        $(current_sticker).css('top', parseInt(hp.top, 10) + parseInt(ui.value) + 'px');
            }
    });    

    $(window).bind('resize', function() {
        stickers_place_photos();
    });   
};
function stickers_place_photos() {
    var stickers = Drupal.settings.stickers;
    var hp = $('#header').position();
    $.each (stickers.stickers, function (i, val) {
        var sleft = parseInt(val['x'], 10) + parseInt(hp.left, 10);
        var stop = parseInt(val['y'], 10) + parseInt(hp.top, 10);
        $('#stickers-'+val['nid']).css('left' , sleft+'px');        
        $('#stickers-'+val['nid']).css('top' , stop+'px');        
    });
}