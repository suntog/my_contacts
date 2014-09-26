(function($) {
  $(document)
    .ready(function() {
    /*** Expand profile contact list ***/


      $('#my-contacts-block-expand').click(function(e) {
        $(this).parent().siblings('#my-contacts-list-wrapper').children('.my-contacts').toggleClass('no-text');
        $(this).parent().parent('.content').toggleClass('auto-height');
        var buttonText = $(this).text();
        if (buttonText == 'expand') {
          $(this).text('collapse');
        } else {
          $(this).text('expand');
        }
      });

    });
}(jQuery));


