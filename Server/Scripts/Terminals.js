function RefreshCounters() {

    var mainHub = $.connection.mainHub;

    mainHub.client.refreshCounters = function (terminalName, counters) {
        var content = "<tr id = " + "'" + counters.TerminalId + "'" + ">";
        content += '<td>' + terminalName + '</td>';
        content += '<td>' + counters.CommonInput + '</td>';
        content += '<td>' + counters.Balance + '</td>';
        content += '<td>' + counters.PressurizedWater + '</td>';
        content += '<td>' + counters.NoPressurizedWater + '</td>';
        content += '<td>' + counters.Foam + '</td>';
        content += '<td>' + counters.Wax + '</td>';
        content += '<td>' + counters.AgainstOfMidges + '</td>';
        content += '<td>' + counters.Vacuum + '</td>';
        content += '<td>' + counters.Osmose + '</td>';
        content += '<td>' + counters.Air + '</td>';
        content += "</tr>";
        $("#" + counters.TerminalId).replaceWith(content);
    };

    $.connection.hub.start().done();
}