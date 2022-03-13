var setEnableButton = function (status) {
  $('#poweroff').prop("disabled", !status);
  $('#poweron').prop("disabled", !status);
};

var updateStatus = function() {
  setTimeout(() => {
    $.get('/status').then(function (res) {
      $('#state').text(res.updatedStatus === 'on' ? 'Encendido' : 'Apagado');
      setEnableButton(true);
    });
  }, 500);
};

$('document').ready(function() {
  updateStatus();
  
  $('#poweron').click(function() {
    setEnableButton(false);
    $.get('/poweron').then(updateStatus);
  });

  $('#poweroff').click(function() {
    setEnableButton(false);
    $.get('/poweroff').then(updateStatus);
  });
});