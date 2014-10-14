// Hold a global reference to the div#main element.  Initially assign it ... somewhere useful :)
var main;

// Initially position a circle based on a given angle.
var positionCirlce = function(c, angle) {

};

// Move a circle based on the distance of the approaching mouse
var moveCircle = function(circle, dx, dy) {

};

// Look at all the circle elements, and figure out if any of them have to move.
var checkMove = function() {

};

// Set up the initial circle of circles.
var setup = function() {
    var numCircles = 15;
    main = document.getElementById("main");
};

window.addEventListener('load', function() {
    setup();
    // Add an event listener to pick up the mouse movements and call checkMove
});

