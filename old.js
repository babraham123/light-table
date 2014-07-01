$(document).ready(function() {
    var _$connect = $('#connect');
    var _$close = $('#close');
    var _$send = $('#send');
    var _$words = $('#words');
    var _$words2 = $('#words2');
    var _$input = $('#data_input');
    // var server = 'ws://68.173.83.103:8383/';
    var server = 'ws://' + data.ip + ':' + data.port + '/';
    var connection = null;

    _$connect.click(function() {
        if ('WebSocket' in window) {
            connection = new WebSocket(server);
            bindconn();
            print_msg('Connecting to server: ' + server);
            _$close.removeAttr("disabled");
        } else {
            print_msg('Websockets not supported in this browser.');
        }

        _$connect.attr("disabled","disabled");
    });

    _$send.click(function() {
        if (connection) {
            // frames are being too quickly and the buffer is filling up
            // check connection.bufferedAmount, queue calls
            var data = (_$input.val()).replace( /\n/g, " " ).split( " " );
            $.each(data, function(i, msg) {
                if (msg.length > 2) {
                    msg = unescape( encodeURIComponent( msg ) );
                    console.log(msg + " : " + connection.bufferedAmount);
                    
                    // add delay between each call
                    setTimeout(connection.send(msg), 300 * (i+1));
                } else {
                    console.log('Message too small');
                }
            });

        } else {
            print_msg('Connection not ready yet.');
        }
    });

    _$close.click(function() {
        if (connection) {
            connection.close();
            connection = null;
            _$connect.removeAttr("disabled");
            _$close.attr("disabled","disabled");
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
        _$words.text(function(i, oldText) {
            return msg;
        });
    }
    var print_data = function(msg) {
        _$words2.text(function(i, oldText) {
            return msg;
        });
    }


    var mode1 = [
"000:000000 001:222288 002:000000 003:222288 004:000000 005:222288 006:000000 007:222288 008:000000 009:222288",
"010:000000 011:222288 012:000000 013:222288 014:000000 015:222288 016:000000 017:222288 018:000000 019:222288",
"020:000000 021:222288 022:000000 023:222288 024:000000 025:222288 026:000000 027:222288 028:000000 029:222288",
"030:000000 031:222288 032:000000 033:222288 034:000000 035:222288 036:000000 037:222288 038:000000 039:222288",
"040:000000 041:222288 042:000000 043:222288 044:000000 045:222288 046:000000 047:222288 048:000000 049:222288"
    ].join("\n");

    var mode2 = [
"000:990000 001:009900 002:000099 003:990000 004:009900 005:000099 006:990000 007:009900 008:000099 009:990000",
"010:009900 011:000099 012:990000 013:009900 014:000099 015:990000 016:009900 017:000099 018:990000 019:009900",
"020:000099 021:990000 022:009900 023:000099 024:990000 025:009900 026:000099 027:990000 028:009900 029:000099",
"030:990000 031:009900 032:000099 033:990000 034:009900 035:000099 036:990000 037:009900 038:000099 039:990000",
"040:009900 041:000099 042:990000 043:009900 044:000099 045:990000 046:009900 047:000099 048:990000 049:009900"
    ].join("\n");

    var mode3 = [
"000:EBAEE0 001:FFFFFF 002:FF0000 003:00FF00 004:0000FF 005:EBAEE0 006:FFFFFF 007:FF0000 008:00FF00 009:0000FF",
"010:EBAEE0 011:000000 012:000000 013:000000 014:000000 015:340922 016:000000 017:000000 018:000000 019:000000",
"020:ABCDEF 021:000000 022:000000 023:000000 024:000000 025:234526 026:000000 027:25B45C 028:000000 029:000000",
"030:EBAEE0 031:09887A 032:000000 033:AE324F 034:000000 035:EB2342 036:000000 037:000000 038:000000 039:000000",
"040:EBAEE0 041:000000 042:000000 043:000000 044:000000 045:99AE22 046:000000 047:000000 048:000000 049:000000"
    ].join("\n");

    $('#mode1').click(function() {
        _$input.val(mode1);
    });
    $('#mode2').click(function() {
        _$input.val(mode2);
    });
    $('#mode3').click(function() {
        _$input.val(mode3);
    });

});
