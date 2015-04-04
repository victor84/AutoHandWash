function RealTimeTerminals() {
    // Declare a proxy to reference the hub.
    var chat = $.connection.testHub;

    // Create a function that the hub can call to broadcast messages.
    chat.client.addMessage = function (name, message) {
        // Html encode display name and message.
        var encodedName = $('<div />').text(name).html();
        var encodedMsg = $('<div />').text(message).html();
        // Add the message to the page.
        $('#discussion').append('<li><strong>' + encodedName
            + '</strong>:&nbsp;&nbsp;' + encodedMsg + '</li>');
    };

    // Start the connection.
    $.connection.hub.start().done();
}

function CreateGroups(dictionaryTerminalCounters) {
    alert(dictionaryTerminalCounters);
}