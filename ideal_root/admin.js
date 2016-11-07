jQuery(document).ready(function(){
    var $ = jQuery;
    var apiUrl = $('[name="ideal_root_url"]').val();
    var tokenInput = $('[name="ideal_root_token"]');
    var popup;
    var form;

    function showLoginForm() {
        var template = '' +
            '<div id="irLoginForm">' +
            '<form autocomplete="off">' +
            '<h3>Root Login</h3>' +
            '<a class="irHideLoginForm" href="#">&times;</a>' +
            '<p><strong>Enter your credentials to retrieve authorization token</strong></p>' +
            '<div class="message"></div>' +
            '<input class="form-text" name="_ir_e" type="text" placeholder="email"/>' +
            '<input class="form-text" name="_ir_p" type="password" placeholder="password" autocomplete="off"/>' +
            '<input type="submit" value="Submit" class="form-submit" autocomplete="off"/>' +
            '</form>' +
            '</div>';
        popup = $(template).appendTo($('body'));
        form = popup.find('form');
        form.submit(handleSubmit);
        popup.find('.irHideLoginForm').click(hideLoginForm);
    }
    function hideLoginForm() {
        if (popup) {
            popup.remove();
        }
    }
    function handleSubmit() {
        var formData = form.serializeArray();
        var data = {
            email: formData[0].value,
            password: formData[1].value,
            action: 'get_token',
            url: apiUrl
        };
        setLoadingState(true);
        $.ajax({
            url: '/admin/config/ideal_root/ajax',
            data: data,
            dataType: 'json',
            success: handleResponse,
            error: handleErrorResponse
        });
        return false;
    }

    function handleResponse(response) {
        if (response && response.token) {
            tokenInput.val(response.token);
            hideLoginForm();
        } else {
            setError('Error while retrieving token');
            setLoadingState(false);
        }
    }
    function handleErrorResponse(response) {
        console.log(response);
        var data;
        try {
            data = JSON.parse(response.response)
        } catch (e) {

        }
        if (response.status == 0 ) {
            setError('Connection failed');
        } else if (data && data.error) {
            setError(data.error);
        } else {
            setError(response.responseText || response.statusText);
        }
        setLoadingState(false);
    }
    function setError($msg) {
        form.find('.message').html('<div class="messages error">' + $msg + '</div>');
    }
    function setLoadingState($val) {
        form.find('input').attr('disabled', !!$val);
    }
    function init() {
        $('#irGetTokenBtn').click(showLoginForm);
    }

    init();
});
