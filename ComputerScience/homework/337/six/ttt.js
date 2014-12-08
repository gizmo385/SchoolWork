var playerID;
var gameID;
var currentGame;
var newGameButton;
var leaveGameButton;
var openGamesDiv;
var myTurn;

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
    newGameButton = document.getElementById("newGame").addEventListener("click", newGame);

    // See if we're part of a game already by making an ajax call to the myGame command, and handling the result
    ajax( "cmd=myGame", gotMyGame );

    // See if there are any existing games by making an ajax call to the getGames command, and handling the result
    ajax( "cmd=getGames", gotOpenGames );

    // Start a board refresh timer with a two second interval (2000ms), set refreshBoardTimer as the callback
    setTimeout( refreshBoardTimer, 2000 );
}

function gotMyGame(response) {
    // See if we got a game state from our response
    // If the game_state property is 'open', display a message saying we're waiting for another player to join.
    // If the game_state is set to 'playing' or 'complete', update the game board state by calling updateGameState()
    if( response.data ) {
        var game = response.data;
        gameID = game.game_id;

        // Determine the game state
        var gameState = game.game_state;
        if( gameState == "open" ) {
            updateMessage( "Waiting for additional player..." );
        } else if( gameState == "playing" || gameState == "complete" ) {
            updateGameState(game);
        } else {
            console.log( "gameState = " + gameState );
        }
    }

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
    for( var i = 0; i < response.data.length; i++ ) {
        var game = response.data[i];

        // Create a new div and add the event listener
        var openGameElement = document.createElement("div");
        openGameElement.className = "openGameButton";
        openGameElement.innerHTML = game.game_id;
        openGameElement.addEventListener( "click", joinGame );

        // Add the element to the list of elements
        openGamesDiv.appendChild( openGameElement );
    }
}

// Callback function for the click event on the new game button.
function newGame() {
    // Send an AJAX command to create a new game
    ajax( "cmd=newGame", newGameResult );
}

function newGameResult(response) {
    // Handle the display of any information from the response to our newGame command
    updateMessage( response.message );
    gameID=response.gameID;
}

// Event handler attached to each game to join
function joinGame(event) {
    // the 'this' variable should be a given TD HTMLElement. Retrieve the game_id we set earlier
    var gameToJoin =  this.innerHTML;

    // Send a new AJAX command to join the given gameID, and set joinGameResult as the callback
    ajax( "cmd=joinGame&gameID=" + gameToJoin, joinGameResult );
}

// Handle the response from joining a game
function joinGameResult(response) {
    // Store the game ID in our global gameID variable for later use.
    var data = response.data;
    gameID = data.game_id;

    // Update the message field with the new message from this response.
    updateMessage( response.message );
}

// Event handler attached to teach TD position element.
function positionClick(event) {
    // Send a new AJAX command to record a new move. Pass gotMoveResult as the callback
    var positionNumber = this.id.substr( this.id.length - 1 );

    if( gameID ) {
        if( myTurn ) {
            ajax( "cmd=move&gameID=" + gameID + "&position=" + positionNumber, gotMoveResult );
        } else {
            alert( "It isn't your turn!" );
        }
    }
}

// Handle result from a move
function gotMoveResult(response) {
    // Update the message field
    if( response.message != undefined ) {
        updateMessage( response.message );
    } else if( response.error != undefined ) {
        updateMessage( response.error );
    }

    // Update the game board with the new game state returned
    updateGameState( response.data );
}

// Update the game board with a given game state
function updateGameState(game) {
    // Update each position on the Board
    var gameBoard = game.board;
    console.log( gameBoard );
    for( var i = 1; i <= 9; i++ ) {
        var boardPosition = gameBoard[i];

        var element = document.getElementById( "position" + (i) );

        if( boardPosition == "X" ) {
            element.innerHTML = "<img src=\"X.png\" class=\"ttt_icon\">";
        } else if( boardPosition == "O" ) {
            element.innerHTML = "<img src=\"O.png\" class=\"ttt_icon\">";
        } else {
            element.innerHTML = "";
        }
    }

    // Did someone win? If so display the winner and return. No further processing.
    if( game.winner ) {
        currentGame.innerHTML = "<img src=\"" + game.winner + "\" class=\"ttt_icon\"> wins!";
        updateMessage( game.winner + " wins!" );
        return;
    }

    // Are you X or O?
    var playerType = game.you.player_type;
    currentGame = document.getElementById( "currentGame" );
    currentGame.innerHTML = "You are <img src=\"" + playerType + ".png\" class=\"ttt_icon\">";

    // Who's turn is it?
    if( game.you.player_id == game.player_turn.player_id ) {
        myTurn = true;
    } else {
        myTurn = false;
    }
}

// Callback from our refresh timeout
function refreshBoardTimer() {
    // Send a new AJAX call for myGame, set gotMyGame as the callback
    ajax( "cmd=myGame", gotMyGame );

    // Send a new AJAX call for getGames, set gotOpenGames as the callback
    ajax( "cmd=getGames", gotOpenGames);

    // Set a new timeout for 2 seconds with refreshBoardTimer as the callback again
    setTimeout( refreshBoardTimer, 2000 );
}

// Update the message text on the screen
function updateMessage(message) {
    // Determine the current time (hours:minutes:seconds)
    var dateObject = new Date();
    var currentTime = (dateObject.getHours() % 12) + ":" + dateObject.getMinutes() + ":" +
        dateObject.getSeconds();

    // Update the #messages element with the new text and a timestamp
    var messages = document.getElementById("messages");
    messages.value = messages.value + "<" + currentTime + "> " + message + "\n";
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
            // 200 is an HTTP OK response
            if( req.status == 200 ) {
                callback( JSON.parse(req.responseText) );
            } else {
                console.log( "Request response = \"" + req.responseText + "\"" );
            }
        }
    }

    // Set up our HTTP Request
    req.open( "get", url );

    // Finally initiate the request
    req.send( null );
}
