// Активация элемента списка
function ActivateLI(id) {
    var elem = document.getElementById(id);
    if (elem) {
        if (elem.nodeName == 'LI') {
            elem.className = "active";
        }
    }
};