$('document').ready(function() {
  $('#poweron').click(function() {
    // alert('on');
    $.get('/poweron');
  });
  $('#poweroff').click(function() {
    // alert('off');
    $.get('/poweroff');
  });
});