function info() {
    $.getJSON('https://api.github.com/repos/somacar/somacar', function (data) {
        // use data.stargazers_count
        $('#github-watch span').text(data.watchers);
        $('#github-star span').text(data.stargazers_count);
        $('#github-href').attr('href', data.html_url).text(data.html_url);
    });
}

function img() {
    $('#img-submit').on('click', function () {
        var cont = $('#img-content').val();
        if (cont.length > 0) {
            draw(cont);
            $('.content_wrapper').remove();
            ribbon_setup();
            $('.fullscreen-gallery').css('display', 'block');
        }
    });
    jQuery(window).resize(function ($) {
        "use strict";
        ribbon_setup();
    });
    jQuery(window).load(function ($) {
        "use strict";
        ribbon_setup();
    });
}

function ribbon_setup() {
    "use strict";
    var setHeight = window_h - header.height() - 20;
    var setHeight2 = window_h - header.height() - $('.slider_info').height() - 20;
    jQuery('.fs_grid_gallery').height(window_h - header.height() - 1);
    jQuery('.currentStep').removeClass('currentStep');
    jQuery('.slide1').addClass('currentStep');
    jQuery('.num_current').text('1');

    jQuery('.num_all').text(jQuery('.fw_gallery_list li').size());
    jQuery('.ribbon_wrapper').height(setHeight2);
    jQuery('.fw_gallery_list').height(setHeight2);
    jQuery('#img-container').height(setHeight2);
}