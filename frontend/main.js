$('document').ready(function() {
  $('#poweron').click(function() {
    $.get('/poweron');
  });
  $('#poweroff').click(function() {
    $.get('/poweroff');
  });
});