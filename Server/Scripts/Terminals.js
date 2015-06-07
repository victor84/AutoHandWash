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

    mainHub.client.refreshMessages = function (groupId, terminalName, terminalLog) {

        var newMessage = "<li><a href='#'><div><strong>" + terminalName + "</strong><span class='pull-right text-muted'><em>" + terminalLog.ViewDateTimeTerminal + "</em></span></div><div>" + terminalLog.Message + "</div></a></li><li class='divider'></li>";
        var allMessage = "<li id='allmessages_" + groupId + "'><a href='/terminalslogs'><div>Все сообщения</div></a></li>";
        var limitMessages = 10;
        var content = "";
        var messages = "";
        var list = $("#" + "messages_" + groupId).find('li');
        var length = list.length;
        if (length == 1)
        {
            if (list[0].id == "notmessages_" + groupId)
            {
                messages = newMessage + allMessage;
            }
        }
        else
        {
            var j = 0;
            var arr = [];
            for (var i = 0; i < length - 1; i++)
            {
                li = list[i];
                var innerHTML = li.innerHTML;
                if (innerHTML != "")
                {
                    arr[j] = innerHTML;
                    j++;
                }
            }
            var countMessages = arr.length;
            var endIndex = 0;
            if (countMessages >= limitMessages)
            {
                endIndex = 1;
            }
            for (var i = 0; i < countMessages - endIndex; i++)
            {
                messages = messages + "<li>" + arr[i] + "</li><li class='divider'></li>";
            }
            messages = newMessage + messages + allMessage;
        }
        content = "<ul id='messages_" + groupId + "' class='dropdown-menu dropdown-messages'>" + messages + "</ul>";
        $("#" + "messages_" + groupId).replaceWith(content);
    };

    $.connection.hub.start().done();
}