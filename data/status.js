var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function getReadings() {
    websocket.send("getReadings");
}

function initWebSocket() {
    websocket = new WebSocket(gateway);

    websocket.onopen = () => {
        console.log("WebSocket Opened");
        getReadings();
    };

    websocket.onclose = () => {
        console.log("WebSocket Closed");
        setTimeout(initWebSocket, 2000);
    };

    websocket.onmessage = function (event) {
        let json = JSON.parse(atob(event.data));

        if (json.espnow)
            setConnected(true);
        else
            setConnected(false);
    }
}

function setConnected(state) {
    const lamp = document.getElementById("statusLamp");
    const text = document.getElementById("statusText");
    console.log(state);

    if (state) {
        lamp.classList.remove("lamp-off");
        lamp.classList.add("lamp-on");
        text.innerText = "Connected";
    }
    else {
        lamp.classList.remove("lamp-on");
        lamp.classList.add("lamp-off");
        text.innerText = "Unconnect";
    }
}

window.addEventListener("load", () => {
    setConnected(false); // เริ่มต้น
});

