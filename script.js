





socket = new WebSocket('ws://192.168.1.92:81');
let heartbeatInterval ;
socket.onopen = function (e) {
    console.log("[socket]socket.onopen");

 heartbeatInterval = setInterval(() => {
            if (socket.readyState === WebSocket.OPEN) {
                socket.send(JSON.stringify({ type: "ping" }));
                console.log("[socket]Heartbeat sent");
            }
        }, 30000); // 30 seconds



};

socket.onmessage = function (e) {
    console.log("[socket]message: ", e.data);

    try {
        // Parse the incoming data as JSON
        const data = JSON.parse(e.data);

        // Update the respective elements based on the data
        if (data.swaterper !== undefined) {
            document.getElementById("swaterper").value = data.swaterper+"CM";
            document.getElementById("file").value = data.swaterper;
        }
        if (data.twaterper !== undefined) {
            document.getElementById("twaterper").value = data.twaterper +"CM" ;
             document.getElementById("watertankper").value = data.twaterper;
        }
    } 
    
    catch (error) {
        console.error("[socket]Error parsing message: ", error);
    }
};

socket.onclose = function (e) {
    console.log("[socket]socket.onclose");
};

socket.onerror = function (error) {
    console.log("[socket]socket.onerror: ", error);
};




function updateWaterLevelFromSocket(data) {
    try {
        // Parse the incoming data as JSON
        const parsedData = JSON.parse(data);

        // Update sump water level if available
        if (parsedData.swaterper !== undefined)
            {
            const sumpwaterlevelone = document.getElementById("water-level1");
            const sumpwaterleveltwo = document.getElementById("water-level2");
            const sumpwaterlevelthree = document.getElementById("water-level3");
            const sumpwaterlevelfour = document.getElementById("water-level4");
            const sumpwaterlevelfive = document.getElementById("water-level5");
            const sumpWaterLevelsix = document.getElementById("water-level6");
            document.getElementById("swaterper").value = parsedData.swaterper+"CM";
            const tableWaterlevel = document.getElementById("tableWaterlevel")
            tableWaterlevel.textContent = parsedData.swaterper;

            

             if(parsedData.swaterper >= 0 && parsedData.swaterper <= 20) {
                
                sumpWaterLevelsix.style.backgroundColor = "green";
                sumpwaterlevelfive.style.backgroundColor = "green";
                sumpwaterlevelfour.style.backgroundColor = "green";
                sumpwaterlevelthree.style.backgroundColor = "green";
                sumpwaterleveltwo.style.backgroundColor = "green";
                sumpwaterlevelone.style.backgroundColor = "green";
                
         }else if(parsedData.swaterper > 20 && parsedData.swaterper <= 40) {
                sumpwaterlevelthree.style.backgroundColor = "orange";
                sumpwaterlevelfour.style.backgroundColor = "orange";
                sumpwaterlevelfive.style.backgroundColor = "orange";
                sumpWaterLevelsix.style.backgroundColor = "orange";

        }else if(parsedData.swaterper > 40 && parsedData.swaterper <= 60) {
                sumpwaterlevelfour.style.backgroundColor = "yellow";
                sumpwaterlevelfive.style.backgroundColor = "yellow";
                sumpWaterLevelsix.style.backgroundColor = "red";
        }else if(parsedData.swaterper > 60 && parsedData.swaterper <= 80) {
                
                sumpWaterLevelsix.style.backgroundColor = "red";
        }else{ "Tank FULL";} 
            
            
        }

        // Update tank water level if available
        if (parsedData.twaterper !== undefined) {
            const tankWaterLevelone = document.getElementById("sumpwater-level1");
            const tankWaterLeveltwo = document.getElementById("sumpwater-level2");
            const tankWaterLevelthree = document.getElementById("sumpwater-level3");
            const tankWaterLevelfour = document.getElementById("sumpwater-level4");
            const tankWaterLevelfive = document.getElementById("sumpwater-level5");
            const tankWaterLevelsix = document.getElementById("sumpwater-level6");
            document.getElementById("twaterper").value = parsedData.twaterper +"CM" ;
            const tableWaterlevel = document.getElementById("tableWaterlevel1")
            tableWaterlevel.textContent = parsedData.twaterper;


            if (parsedData.twaterper >= 0 && parsedData.twaterper <= 20) {
                tankWaterLevelsix.style.backgroundColor = "green";
                tankWaterLevelfive.style.backgroundColor = "green";
                tankWaterLevelfour.style.backgroundColor = "green";
                tankWaterLevelthree.style.backgroundColor = "green";
                tankWaterLeveltwo.style.backgroundColor = "green";
                tankWaterLevelone.style.backgroundColor = "green";


            }else if (parsedData.twaterper > 20 && parsedData.twaterper <= 40) {
                tankWaterLevelsix.style.backgroundColor = "orange";
                tankWaterLevelfive.style.backgroundColor = "orange"
                tankWaterLevelfour.style.backgroundColor = "orange";
                tankWaterLevelthree.style.backgroundColor = "orange";

            } else if (parsedData.twaterper > 40 && parsedData.twaterper <= 60) {
                tankWaterLevelfour.style.backgroundColor = "yellow";
                tankWaterLevelfive.style.backgroundColor = "yellow";
                tankWaterLevelsix.style.backgroundColor = "yellow";
            } else if (parsedData.twaterper > 60 && parsedData.twaterper <= 80) {
                tankWaterLevelone.style.backgroundColor = "red";
            } else {
                tankWaterLevel.style.backgroundColor = "red";
            }   
        }
    } catch (error) {
        console.error("[updateWaterLevelFromSocket] Error parsing data: ", error);
    }
}

// Helper function to determine water level color based on percentage


// Update the WebSocket onmessage handler to use the new function
socket.onmessage = function (e) {
    console.log("[socket]message: ", e.data);
    updateWaterLevelFromSocket(e.data);
};



// Function to close the WebSocket connection


//Date and time function 

document.addEventListener("DOMContentLoaded", () => {
    const date = document.getElementById("datetime");
    const timenow = document.getElementById("currenttime");


    if (date, timenow) {
        date.textContent = new Date().getMonth() + 1 + "/" + new Date().getDate() + "/" + new Date().getFullYear();
        timenow.textContent = new Date().getHours() + ":" + new Date().getMinutes() + ":" + new Date().getSeconds();
    } else {
        console.warn("Element with ID 'datetime' not found.");
    }
});




// Button to turn on and off the pump

function turnOnRelay() {
    fetch('http://192.168.1.92/relayOn', { mode: 'no-cors' }) // Use no-cors mode
        .then(() => {
            console.log("Relay turned ON");
        })
        .catch(error => console.error("Error:", error));
}

function turnOffRelay() {
    fetch('http://192.168.1.92/relayOff', { mode: 'no-cors' }) // Use no-cors mode
        .then(() => {
            console.log("Relay turned OFF");
        })
        .catch(error => console.error("Error:", error));
}






