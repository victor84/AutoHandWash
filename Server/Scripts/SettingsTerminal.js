function ChangeStatus(status) {
    $(document).ready(function () {
        $("input[name='State']")[status].checked = true;
    });
};