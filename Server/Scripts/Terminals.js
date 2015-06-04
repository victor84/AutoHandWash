function Refresh() {

    var mainHub = $.connection.mainHub;

    mainHub.client.refreshCounters = function (terminalName, counters) {
        var content = "<tr id = " + "'" + counters.TerminalId + "'" + ">";
        content += '<td>' + terminalName + '</td>';
        content += '<td>' + counters.ViewState + '</td>';
        content += '<td>' + counters.CommonInput + '</td>';
        content += '<td>' + counters.Balance / 100 + '</td>';
        content += '<td>' + counters.PressurizedWater + '</td>';
        content += '<td>' + counters.NoPressurizedWater + '</td>';
        content += '<td>' + counters.Foam + '</td>';
        content += '<td>' + counters.Wax + '</td>';
        content += '<td>' + counters.Vacuum + '</td>';
        content += '<td>' + counters.Osmose + '</td>';
        content += '<td>' + counters.Air + '</td>';
        content += '<td class="col-md-2 text-center"><a href="terminals/fillbalance/' + counters.TerminalId + '" role="button" class="btn btn-success btn-xs"><i class="fa fa-rub"></i>Пополнить счет</a></td>';
        content += "</tr>";
        $("#" + counters.TerminalId).replaceWith(content);
    };

    mainHub.client.refreshStatusBar = function (groupId, message) {
        var content = "<div id = " + "'" + groupId + "'" + " class='pull-right' >" + message + "</div>";
        $("#" + groupId).replaceWith(content);
    };

    $.connection.hub.start().done();
}