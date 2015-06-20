function Refresh() {

    var mainHub = $.connection.mainHub;

    mainHub.client.refreshCounters = function (terminalName, counters, sumPrizeCounters, sumDiscountCardCounters) {

        var tr = document.getElementById(counters.TerminalId);
        var parentElement = tr.parentElement;
        var tagtrs = parentElement.getElementsByTagName('tr');
        var trs = [];
        var j = 0;
        for (var i = 0; i < tagtrs.length; i++)
        {
            if (tagtrs[i].id == counters.TerminalId)
            {
                continue;
            }
            trs[j] = tagtrs[i];
            j++;
        }
        
        var values = [];
        values[0] = "";
        values[1] = "";
        values[2] = counters.CommonInput;
        values[3] = counters.Balance / 100;
        values[4] = counters.PressurizedWater;
        values[5] = counters.NoPressurizedWater;
        values[6] = counters.Foam;
        values[7] = counters.Wax;
        values[8] = counters.Vacuum;
        values[9] = counters.Osmose;
        values[10] = counters.Air;
        values[11] = sumPrizeCounters;
        values[12] = sumDiscountCardCounters;
        values[13] = "";
        values[14] = "";

        var content = "<tr id = " + "'" + counters.TerminalId + "'" + ">";
        content += '<td>' + terminalName + '</td>';
        content += '<td>' + counters.ViewState + '</td>';
        content += '<td>' + counters.CommonInput + '</td>';
        content += '<td>' + counters.Balance / 100 + '</td>';
        content += '<td>' + counters.ViewPressurizedWater + '</td>';
        content += '<td>' + counters.ViewNoPressurizedWater + '</td>';
        content += '<td>' + counters.ViewFoam + '</td>';
        content += '<td>' + counters.ViewWax + '</td>';
        content += '<td>' + counters.ViewVacuum + '</td>';
        content += '<td>' + counters.ViewOsmose + '</td>';
        content += '<td>' + counters.ViewAir + '</td>';
        content += '<td>' + sumPrizeCounters + '</td>';
        content += '<td>' + sumDiscountCardCounters + '</td>';
        content += '<td class="col-md-2 text-center"><a href="terminals/fillbalance/' + counters.TerminalId + '" role="button" class="btn btn-success btn-xs"><i class="fa fa-rub"></i> Пополнить счет</a></td>';
        content += '<td class="col-md-2 text-center"><a href="terminals/changestatus/' + counters.TerminalId + '" role="button" class="btn btn-success btn-xs"><i class="fa fa-gear fa-fw"></i> Изменить статус</a></td>';
        content += "</tr>";
        $("#" + counters.TerminalId).replaceWith(content);

        ChangeTotalValues(trs, values);
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

function TotalValues()
{
    var tables = document.getElementsByTagName('table');
    for (var i = 0; i < tables.length; i++)
    {
        var matrixValues = [];
        var table = tables[i];
        var trs = table.getElementsByTagName('tr');
        var trTotal = trs[trs.length - 1];
        for (var j = 0; j < trs.length - 1; j++)
        {
            if (!matrixValues[j]) matrixValues[j] = [];
            var tr = trs[j];
            var tds = tr.getElementsByTagName('td');
            for (var k = 0; k < tds.length; k++)
            {
                var text = "";
                var td = tds[k];
                if (td.className == 'count')
                {
                    var innerHTML = td.innerHTML;
                    text = isNaN(innerHTML) ? 0 : parseInt(innerHTML);
                }
                if (td.className == 'timespan')
                {
                    var innerHTML = td.innerHTML;
                    var timeSpan = moment.duration(innerHTML);
                    var seconds = timeSpan.asSeconds();
                    text = seconds;
                }
                matrixValues[j][k] = text;
            }
        }

        var totals = ["", "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "", ""];
        for (var k = 0; k < 15; k++)
        {
            for (var j = 1; j < trs.length - 1; j++)
            {
                var text = matrixValues[j][k];
                totals[k] += text;
            }
        }

        for (var k = 4; k < 11; k++) {
            var timeSpan = secondsToTime(totals[k]);
            totals[k] = timeSpan;
        }

        var tds = "";
        for (var k = 0; k < 15; k++) {
            var td = '<td>' + totals[k] + '</td>';
            tds += td;
        }
        trTotal.innerHTML = tds;
    }
}

function ChangeTotalValues(trs, values)
{
    var matrixValues = [];
    var trTotal = trs[trs.length - 1];
    for (var j = 0; j < trs.length - 1; j++) {
        if (!matrixValues[j]) matrixValues[j] = [];
        var tr = trs[j];
        var tds = tr.getElementsByTagName('td');
        for (var k = 0; k < tds.length; k++) {
            var text = "";
            var td = tds[k];
            if (td.className == 'count') {
                var innerHTML = td.innerHTML;
                text = isNaN(innerHTML) ? 0 : parseInt(innerHTML);
            }
            if (td.className == 'timespan') {
                var innerHTML = td.innerHTML;
                var timeSpan = moment.duration(innerHTML);
                var seconds = timeSpan.asSeconds();
                text = seconds;
            }
            matrixValues[j][k] = text;
        }
    }

    var totals = values;
    for (var k = 0; k < 15; k++) {
        for (var j = 0; j < trs.length - 1; j++) {
            var text = matrixValues[j][k];
            totals[k] += text;
        }
    }

    for (var k = 4; k < 11; k++) {
        var timeSpan = secondsToTime(totals[k]);
        totals[k] = timeSpan;
    }

    var tds = "";
    for (var k = 0; k < 15; k++) {
        var td = '<td>' + totals[k] + '</td>';
        tds += td;
    }
    trTotal.innerHTML = tds;
}

function secondsToTime(totalSec)
{
    var hours = parseInt(totalSec / 3600) % 24;
    var minutes = parseInt(totalSec / 60) % 60;
    var seconds = totalSec % 60;

    var result = (hours < 10 ? "0" + hours : hours) + ":" + (minutes < 10 ? "0" + minutes : minutes) + ":" + (seconds < 10 ? "0" + seconds : seconds);
    return result;
}