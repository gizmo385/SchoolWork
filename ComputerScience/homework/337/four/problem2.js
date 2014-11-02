// Hold a global reference to the div#main element.  Initially assign it ... somewhere useful :)
var main;
var radius;

var mouse = {
    x: 0,
    y: 0,
    dx: 0,
    dy: 0
};

var degreesToRadians = function( degrees ) {
    return degrees * (Math.PI / 180);
}

// Initially position a circle based on a given angle.
var positionCircle = function(c, angle) {
    var angleInRadians = degreesToRadians(angle);
    var x = (radius * Math.sin( angleInRadians ));
    var y = (radius * Math.cos( angleInRadians ));

    c.style.left = x + "px";
    c.style.top = y + "px";
};

// Move a circle based on the distance of the approaching mouse
var moveCircle = function(circle, dx, dy) {
    var circlePosition = circle.getBoundedClientRect();
    var centerOfCircle = (circlePosition.top / 2) - (circlePosition.right / 2);
};

var distance = function( x1, y1, x2, y2 ) {
    return Math.sqrt( Math.pow(y2 - y1, 2) + Math.pow(x2 - x1, 2) );
};

// Look at all the circle elements, and figure out if any of them have to move.
var checkMove = function() {
    var circles = document.getElementsByClassName("circle");

    for( i = 0; i < circles.length; i++ ) {
        var circle = circles[i];
        var distanceFromMouse = distance( circle.offsetLeft, circle.offsetTop, mouse.x, mouse.y );
        var movementDistance = .01;

        console.log( "Distance: " + distanceFromMouse );

        if( Math.abs(distanceFromMouse) < 100 ) {
            //The angle of the velocity vector is equal to the inverse sin of delta x divided by
            //the distance between the mouse and the center of the circle
            var translationAngle = Math.asin((mouse.x - circle.offsetLeft) / distanceFromMouse);

            // Forming the right triangle, the adjacent and opposite sides both use this delta
            // in their equations
            var delta = Math.sin(translationAngle) * (distanceFromMouse + movementDistance);
            var newY = delta + circle.offsetTop - mouse.y;
            var newX = delta + circle.offsetLeft - mouse.x;

            circle.style.left = parseFloat(circle.style.left) + newX + "px";
            circle.style.top = parseFloat(circle.style.top) + newY + "px";

            console.log( "New Position: (" + newX + ", " + newY + ")" );
        }
    }
};

// Set up the initial circle of circles.
var setup = function() {
    main = document.getElementById("main");
    radius = 150;
    var numCircles = 15;
    var currentAngle = 360 / numCircles;

    for( i = 0; i < numCircles; i++ ) {
        var newCircle = document.createElement("div");
        newCircle.className = "circle";
        positionCircle( newCircle, (i * currentAngle) );

        main.appendChild( newCircle );
    }
};

window.addEventListener('load', function() {
    setup();
    window.addEventListener( "mousemove", function(mouseEvent) {
        var tempX = mouseEvent.pageX;
        var tempY = mouseEvent.pageY;

        mouse.dx = tempX - mouse.x;
        mouse.dy = tempY - mouse.y;

        mouse.x = mouseEvent.pageX - main.offsetLeft;
        mouse.y = mouseEvent.pageY - main.offsetTop;

        checkMove();
    });
});

