"use strict"

let isOpen = false;

let ws = new WebSocket("ws://localhost:8080");
ws.onopen = handleConnectCB;
ws.onclose = handleCloseCB;
ws.onerror = handleErrorCB;
ws.onmessage = handleMsg;


function handleConnectCB() {
    isOpen = true;
    console.log("socket opened");
    console.log("Handshake Completes");
}

function handleCloseCB() {
    isOpen = false;
    chatPara.innerHTML += ("<br>" + "The server has left the building...Goodbye");
}

function handleErrorCB() {
    alert("Error occurs")
}

// To check whether all char in "room" are lowercase
let isLower = true;
function checkRoomName() {
    for ( let i = 0; i < roomBox.value.length; i++ ) {
        if ( roomBox.value.charAt(i) === roomBox.value.charAt(i).toUpperCase() ||
            roomBox.value.charAt(i) === ' ' || Number.isInteger(roomBox.value.charAt(i)) ) {
            isLower = false;
            alert("Invalid room name");
            return;
        }
    }
    isLower = true;
}



////////////////////////////////////////////////////////////////////////////////////
// Get the value of "Room":
let roomBox = document.getElementById("txt_room");
let room = roomBox.value;
// Get the value of "Name":
let nameBox = document.getElementById("txt_name");
let name = nameBox.value;

roomBox.addEventListener("keypress", function( e ) {
    if ( e.keyCode == 13 ) {
        checkRoomName();
        if ( nameBox.value != null && roomBox.value != null && isLower && isOpen) {
            ws.send( "join " + nameBox.value + " " + roomBox.value );
        }
    }
});
nameBox.addEventListener("keypress", function( e ) {
    if ( e.keyCode == 13 ) {
        checkRoomName();
        if ( nameBox.value != null && roomBox.value != null && isLower && isOpen) {
            ws.send( "join " + nameBox.value + " " + roomBox.value );
        }
    }
});


// Get the value of "Enter your text":
let enterBox = document.getElementById("txt_enter");
let text = enterBox.value;
enterBox.addEventListener("keypress", function( e ) {
    if ( e.keyCode == 13 ) {
        if ( text != null ) {
            ws.send( nameBox.value + " " + enterBox.value );
            enterBox.value = '';
        }
    }
})




////////////////////////////////////////////////////////////////////////////////
let peoplePara = document.getElementById("people");
let chatPara = document.getElementById("chat");

function handleMsg( e ) {
    // e contains the dat about the message (e) that caused this callback to be called;
    let msg = e.data;
    // '{ "type": "message", "name": "Davison"....}'
    let obj = JSON.parse( msg );

    if ( obj.type === "message" ) {
        chatPara.innerHTML += ( "<br>" + obj.user + ": " + obj.message );
    }

    else if  ( obj.type ===  "join" ) {
        peoplePara.innerHTML += ( "<br>" + obj.user );
        chatPara.innerHTML += ( "<br>" + obj.user + " has entered the room" );
    }

    else if ( obj.type === "leave" ) {
        peoplePara.innerHTML -= ( "<br>" + obj.user );
        chatPara.innerHTML += ( "<br>" + obj.user + " has left the room" );
    }
}




