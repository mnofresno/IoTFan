var updateStatus = function() {
  $.get('/status').then(function (res) {
    $('#state').text(res.updatedStatus === 'on' ? 'Encendido' : 'Apagado');
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