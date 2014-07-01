$(document).ready(function() {
	var $connect = $( '#connect' );
	var $close = $( '#close' );
	var $send = $('#send');
	var $sendjson = $('#sendjson');
	var $words = $('#words');
	var $words2 = $('#words2');
	// var server = 'ws://173.69.137.204:8383/';
	var server = 'ws://68.173.83.103:8383/';
	var connection = null;

	$connect.click(function() {
		if ('WebSocket' in window) {
			connection = new WebSocket(server);
			bindconn();
			print_msg('Connected to server: ' + server);
			$close.removeAttr("disabled");
		} else {
			print_msg('Websockets not supported in this browser.');
		}

		$connect.attr("disabled","disabled");
	});

	$send.click(function() {
		if (connection) {
			connection.send('Hey server, whats up?');
		} else {
			print_msg('Connection not ready yet.');
		}
	});

	$sendjson.click(function() {
		var message = {
			'key': 'bereket',
			'value': 'Is amazing, like duh...'
		};
		if (connection) {
			connection.send(JSON.stringify(message));
		} else {
			print_msg('Connection not ready yet.');
		}
	});

	$close.click(function() {
		if (connection) {
			connection.close();
			connection = null;
			$connect.removeAttr("disabled");
			$close.attr("disabled","disabled");
			print_msg('Server has been closed.');
		} else {
			print_msg('Connection not ready yet.');
		}
	});

	var bindconn = function() {
		connection.onopen = function(){
			print_msg('Connection open!');
		}
		connection.onclose = function(){
			print_msg('Connection closed.');
		}
		connection.onerror = function(error){
			print_msg('Error detected: ' + error.data);
		}
		connection.onmessage = function(e) {
			print_data(e.data);
		};
	}


	var print_msg = function(msg) {
		$words.text(function(i, oldText) {
		    return msg;
		});
	}
	var print_data = function(msg) {
		$words2.text(function(i, oldText) {
		    return msg;
		});
	}

});