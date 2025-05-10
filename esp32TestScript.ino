
#include "WiFi.h"
#include "WebServer.h"
#include "WebSocketsServer.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "ArduinoOTA.h"

//int builtinled = T2;
int Relay = 25;
const char* ssid = "Uma Nelayam 4G";
const char* password = "9980337734";
const char* ota_password = "12345";


unsigned long lastManualActionTime = 0; // Timestamp of the last manual action
const unsigned long manualTimeout = 90000; // 5 seconds timeout (adjust as needed)


float distance;
float duration;

int triggerpin = 13;
int echo = 12 ;

int second_echo = 14;
int second_trigger = 27;

float seconddistance;
float secondduration;

bool manualOverride = false;


// No changes made today 

WebServer server(80);

WebSocketsServer WS = WebSocketsServer(81);

// function HOME Page containing HTML CSS and Javascript 
void welcome() {

  char test[] PROGMEM = R"===(<!DOCTYPE html>

<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WaterLevel Controller</title>

    <script type="text/javascript"> 


socket = new WebSocket('ws://192.168.1.92:81');
let heartbeatInterval;

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
    } catch (error) {
        console.error("[socket]Error parsing message: ", error);
    }
};

socket.onclose = function (e) {
    console.log("[socket]socket.onclose");
};

socket.onerror = function (error) {
    console.log("[socket]socket.onerror: ", error);
};




    </script>


</head>

<body>
    <header>
        <h1>Water Level Controller </h1>
    </header>
    <main>
        <section>
            <h2>SUMP Water Level</h2> 
            <label for="fname">Water Percentage</label><br>
            <input type="text" id="swaterper" name="fname" value="CM"><br><br>
            <progress id="file" value="%" max="100">  </progress><br><br>


        </section>
        <section>
            <h2>Sintex Water Tank</h2>
            <label for="fname">Water Percentage</label><br>
            <input type="text" id="twaterper" name="fname" value="CM"><br><br>
            <progress id="watertankper" value ="%" max="100"> 90% </progress><br><br>



            
        </section>
    </main>
    <footer>
        <p>&copy; 2023 WaterLevel Controller</p>
    </footer>


</html>


)===";

 server.send(200,"text/html",test);

}




//function  Json data Posing Json out of Ultrsonic sensor to webpage

void senddata(){

 StaticJsonDocument<300> JSONData; 

  JSONData["FIrstUltrasonicSensor"] = distance;
  JSONData["SecondUltrasonicSensor"] = seconddistance;



  char data[300];
  serializeJson(JSONData,data);
server.send(200,"application/json",data);

}



void webSocketEvent(uint8_t num,WStype_t type,uint8_t * payload, size_t length)
{

 switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = WS.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        WS.sendTXT(num, "0");
      }
      break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      // send message to client
      // webSocket.sendTXT(num, "message here");
      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
      
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      //hexdump(payload, length);
      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
  }

}


void setup() {


  Serial.begin(115200);
  ArduinoOTA.setPassword(ota_password);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");

  Serial.println("Connecting to the network");

  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print("...");

    Serial.print("wifi connected ");

    Serial.print(WiFi.localIP());
  }



// Define the route to turn ON the relay
    server.on("/relayOn", HTTP_GET, []() {
        lastManualActionTime = millis(); // Record the time of the manual action
        digitalWrite(Relay, HIGH); // Turn ON the relay
        server.send(200, "text/plain", "Relay is ON");
    });

    // Define the route to turn OFF the relay
    server.on("/relayOff", HTTP_GET, []() {
        lastManualActionTime = millis(); // Record the time of the manual action
        digitalWrite(Relay, LOW); // Turn OFF the relay
        server.send(200, "text/plain", "Relay is OFF");
    });

    // Start the server

  ArduinoOTA.begin();
  server.on("/", welcome);

  server.on("/test",HTTP_GET,senddata);
  server.begin();
  WS.begin();
  WS.onEvent(webSocketEvent);



  // put your setup code here, to run once:
  pinMode(echo,INPUT);
  pinMode(triggerpin,OUTPUT);
 // pinMode(led, OUTPUT);
  pinMode(Relay, OUTPUT);
  pinMode(second_echo,INPUT);
  pinMode(second_trigger,OUTPUT);

  digitalWrite(Relay, LOW); 

}
void loop() {

  server.handleClient();
  server.send(200, "text/html");
  server.send(404, "text/html");

  ArduinoOTA.handle();

    if (millis() - lastManualActionTime > manualTimeout) {
        // Automatic control logic
        if (distance > 70 && seconddistance < 70) {
            digitalWrite(Relay, HIGH); // Turn the relay ON
        } else if (distance <= 20) {
            digitalWrite(Relay, LOW); // Turn the relay OFF
        }
    }

//Ultrasonic 

digitalWrite(triggerpin,LOW);
delayMicroseconds(20);


digitalWrite(triggerpin,HIGH);
delayMicroseconds(20);
digitalWrite(triggerpin,LOW);

duration = pulseIn(echo,HIGH);
distance =(duration/2)*0.034;


digitalWrite(second_trigger,LOW);
delayMicroseconds(20);

digitalWrite(second_trigger,HIGH);
delayMicroseconds(20);
digitalWrite(second_trigger,LOW);

secondduration = pulseIn(second_echo,HIGH);
seconddistance =(secondduration/2)*0.034;


Serial.print("The distance of the first is \n");
Serial.print(distance);
Serial.print("The distance of the second is \n");
Serial.print(seconddistance);

//if(distance > 70 && seconddistance < 70 ){


//digitalWrite(Relay,HIGH);

//}else if(distance <= 20 )
//{
  //digitalWrite(Relay, LOW);

//}

//Webserver 



//WebSockets 
//String usvalue = String(seconddistance,2);
//String value = String(distance,2);
  //          WS.loop();
    //        WS.broadcastTXT(value);
     //       WS.broadcastTXT(usvalue);
 StaticJsonDocument<200> JSONData;
    JSONData["swaterper"] = distance;
    JSONData["twaterper"] = seconddistance;

    char data[200];
    serializeJson(JSONData, data);
    WS.broadcastTXT(data);
    WS.loop();



}
