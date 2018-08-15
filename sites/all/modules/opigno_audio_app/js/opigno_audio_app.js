audiojs.events.ready(function() {
    var as = audiojs.createAll();
    if (as.length==1)
    {
        as[0].settings.autoplay=true;
    }
    console.log(as);
});