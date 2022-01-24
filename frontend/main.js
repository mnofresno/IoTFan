var updateStatus = function() {
  $.get('/status').then(function (res) {
    $('#state').text(res);
  });
};

$('document').ready(function() {
  updateStatus();
  
  $('#poweron').click(function() {
    $.get('/poweron').then(updateStatus);
  });

  $('#poweroff').click(function() {
    $.get('/poweroff').then(updateStatus);
  });
});