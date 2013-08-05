Drupal.behaviors.attachments = function (context) {
  var $settings = Drupal.settings.attachments ;
  
  $('#upload-attachments thead th', context)
    .each(function() {
      var $this = $(this) ;
      switch ($this.text().toLowerCase()) {
      case 'delete': $this.text($settings['delete']) ; break ;
      case 'list': $this.text($settings['list']) ; break ;
      }
    }) ;
  
  $('#upload-attachments tbody td input:checkbox', context)
    .each(function() {
      var $this = $(this) ;
      var $type = $this.attr('id').split('-').pop() ;
      var $text, $btn, $text_checked, $text_unchecked ;
      switch ($type) {
        case 'remove':
          $text = $this.attr('checked') ? $settings.remove : $settings.keep ;
          $btn = $('<button title="'+$settings.togkeep+'">'+$text+'</button>') ;
          $text_checked = $settings.keep ;
          $text_unchecked = $settings.remove ;
          break ;
        case 'list':
          $text = $this.attr('checked') ? $settings.show : $settings.hide ;
          $btn = $('<button title="'+$settings.togshow+'">'+$text+'</button>') ;
          $text_checked = $settings.hide ;
          $text_unchecked = $settings.show ;
          break ;
      }
      $btn.click(function() {
        var $checked = $this.attr('checked') ;
        $btn.text($checked?$text_checked:$text_unchecked) ;
        $this.attr('checked', !$checked) ;
        return false ;
      }) ;
      $this.parent().prepend($btn) ;
      $this.hide() ;
    }) ;
}