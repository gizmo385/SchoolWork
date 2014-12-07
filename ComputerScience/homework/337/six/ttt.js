var playerID;
var gameID;
var currentGame;
var newGameButton;
var leaveGameButton;
var openGamesDiv;

/**
 * Load Event Listener
 * This will run once the page has loaded, and will setup our behaviors
 */
window.addEventListener('load', function() {
    login();
});

// Log in to the API
function login() {
    // Here's an example of how to call our simple ajax() wrapper function. You'll use this a lot.
    ajax("cmd=login", gotLogin);
}

function gotLogin(response) {
    // Update the message field with the message from this response
    updateMessage( response.message );

    // Display the player ID on the game board
    document.getElementById("playerid").innerHTML = response.playerid;

    // Store the player ID in our global playerID variable for later use
    playerID = response.playerid;

    // Finish setup
    setup();
}

function setup() {
    // Add a click event handler to all the position TD elements on the board
    var positionElements = document.getElementsByClassName("position");
    for( var i = 0; i < positionElements.length; i++ ) {
        var element = positionElements[i].addEventListener("click", positionClick);
    }

    // Add a click event handler to the new game button
    newGameButton = document.getElementById("newgGame").addEventListener("click", event);
    // See if we're part of a game already by making an ajas call to the myGame command, and handling the result
    // See if there are any existing games by making an ajas call to the getGames command, and handling the result
    // Start a board refresh timer with a two second interval (2000ms), set refreshBoardTimer as the callback
}

function gotMyGame(response) {
    // See if we got a game state from our response
    // If the game_state property is 'open', display a message saying we're waiting for another player to join.
    // If the game_state is set to 'playing' or 'complete', update the game board state by calling updateGameState()
}

// Handle Open Games Response
function gotOpenGames(response) {
    // remove all existing game nodes from the #openGames div
    openGamesDiv = document.getElementById('openGames');

    while (openGamesDiv.firstChild) {
        openGamesDiv.removeChild(openGamesDiv.firstChild);
    }

    // Add new elements to the #openGames div for each game reported in the response.
    // Make each new element respond to a 'click' event, and set joinGame as a callback.
    // Attach the game_id to the new element. Remember we can just make up new properties!
}

// Callback function for the click event on the new game button.
function newGame() {
    // Send an AJAX command to create a new game
}

function newGameResult(response) {
    // Handle the display of any information from the response to our newGame command
}

// Event handler attached to each game to join
function joinGame(event) {
    // the 'this' variable should be a given TD HTMLElement. Retrieve the game_id we set earlier
    // Send a new AJAX command to join the given gameID, and set joinGameResult as the callback
}

// Handle the response from joining a game
function joinGameResult(response) {
    // Store the game ID in our global gameID variable for later use.
    // Update the message field with the new message from this response.
}

// Event handler attached to teach TD position element.
function positionClick(event) {
    // Send a new AJAX command to record a new move. Pass gotMoveResult as the callback
}

// Handle result from a move
function gotMoveResult(response) {
    // Update the message field
    // Update the game board with the new game state returned
}

// Update the game board with a given game state
function updateGameState(game) {
    // Update each position on the Board
    // Did someone win? If so display the winner and return. No further processing.
    // Are you X or O?
    // Who's turn is it?
}

// Callback from our refresh timeout
function refreshBoardTimer() {
    // Send a new AJAX call for myGame, set gotMyGame as the callback
    // Send a new AJAX call for getGames, set gotOpenGames as the callback
    // Set a new timeout for 2 seconds with refreshBoardTimer as the callback again
}

// Update the message text on the screen
function updateMessage(text) {
    // Update the #messages element with the new text
    var messages = document.getElementById("messages");
    messages.innerHTML += message + "<br>";
}

// A basic wrapper for our AJAX Calls.
// It takes a full URL to call, along with a callback function on completion.
function ajax(url, callback) {
    // Change the base URL here if you're targeting someone else's API Server
    url = "http://ttt.workbench.uits.arizona.edu/ttt.php?" + url;

    // Attach a random seed to the end of the URL to avoid browser caching.
    url = url + "&seed=" + (new Date()).getTime();

    console.log( "Sending request to: " + url );

    // Create a new XMLHttpRequest Object
    var req = new XMLHttpRequest();

    // Pass Cookie Credentials along with request
    req.withCredentials = true;

    // Create a callback function when the State of the Connection changes
    req.onreadystatechange = function() {
        // Upon completion of the request, call the callback() function passed in with the decoded
        // JSON of the response.
        if( req.readyState == 4 ) {
            callback( JSON.parse(req.responseText) );
        } else {
            console.log( "Request response = \"" + req.responseText + "\"" );
        }
    }

    // Set up our HTTP Request
    req.open( "get", url );

    // Finally initiate the request
    req.send( null );
}
