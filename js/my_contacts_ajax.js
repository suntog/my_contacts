(function($) {
  $(document)
    .ready(function() {
    /*** Add contact to profile contact list ***/


      $('.my-contacts-create-contact-btn').live('click', function(e) {
          e.preventDefault();
          var cid = $(this).children('.button-cid').text();
          var uid = $(this).children('.button-uid').text();

          $.ajax({
              type: "POST",
              url: "/my_contacts/ajax-add-contact",
              data: {
                  "uid": uid,
                  "cid": cid,
              },
              dataType: 'json',
              success: function(output) {
                  var success = output.success;
                  alert(success);
              }
          });

        //return FALSE;
      });

    });
}(jQuery));


