function EnablePresentSettings() {
    $(document).ready(function () {
        $('#HasPresent').prop('checked', true);
        $('#presentSetting').prop("class", "collapse in");
    });
};