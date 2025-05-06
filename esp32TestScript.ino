
#include "WiFi.h"
#include "WebServer.h"
#include "WebSocketsServer.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "ArduinoOTA.h"

//int builtinled = T2;
int Relay = 15;
const char* ssid = "Uma Nelayam 4G";
const char* password = "9980337734";
const char* ota_password = "12345";


float distance;
float duration;

int triggerpin = 13;
int echo = 12 ;

int second_echo = 14;
int second_trigger = 27;

float seconddistance;
float secondduration;

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


socket = new WebSocket('ws://192.168.1.77:81');
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

    
<style>
h1{
background-color: rgb(99, 99, 176);
border: 2px solid black;
border-radius: 20px;
color: white;
text-align: center;
padding: 10px;

}

h2{
background-color: rgb(74, 137, 183);
text-align: left ;
color: rgb(26, 23, 23);
}

section{

    border: 2px solid black;
    border-radius: 50px;
    padding: 50px;

}
button{
    background-color: black;
    color: white;
    border: 4px solid black;
    size: 10px;
    border-radius: 10px;


}

.switch {
    position: relative;
    display: inline-block;
    width: 60px;
    height: 34px;
  }
  
  .switch input { 
    opacity: 0;
    width: 0;
    height: 0;
  }
  
  .slider {
    position: absolute;
    cursor: pointer;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-color: #ccc;
    -webkit-transition: .4s;
    transition: .4s;
  }
  
  .slider:before {
    position: absolute;
    content: "";
    height: 26px;
    width: 26px;
    left: 4px;
    bottom: 4px;
    background-color: white;
    -webkit-transition: .4s;
    transition: .4s;
  }
  
  input:checked + .slider {
    background-color: #2196F3;
  }
  
  input:focus + .slider {
    box-shadow: 0 0 1px #2196F3;
  }
  
  input:checked + .slider:before {
    -webkit-transform: translateX(26px);
    -ms-transform: translateX(26px);
    transform: translateX(26px);
  }
  
  /* Rounded sliders */
  .slider.round {
    border-radius: 34px;
  }
  
  .slider.round:before {
    border-radius: 50%;
  }

  #watertank1{
    position: absolute;
    top: 25%;
    left: 42%;
    border: #1274d6c9;
    border-style: solid;
    height: 200px;
    width: 10%;
    background-color: #ffffffc9;
    border-radius: 10px;
    

  }


#sumptank{
    position: absolute;
    top: 63%;
    left: 28%;
    border: #1274d6c9;
    border-style: solid;
    height: 220px;
    width: 30%;
    background-color: #ffffffc9;
    border-radius: 10px;

  }

</style>
</head>

<body>
    <header>
        <h1>Water Level Controller </h1>
    </header>
    <main>
        <section>
            <h2>SUMP Water Level</h2> 
            <p>Sump tank that is pumped up to the water tank</p>
            <label for="fname">Water Percentage</label><br>
            <input type="text" id="swaterper" name="fname" value="CM"><br><br>
            <progress id="file" value="%" max="100">  </progress><br><br>

            <div id="watertank">
               
                <div class="water-level" id="water-level"></div>
                <div class="water-level" id="water-level2"></div>
                <div class="water-level" id="water-level3"></div>
                <div class="water-level" id="water-level4"></div>
                <div class="water-level" id="water-level5"></div>
    
            </div>


            <label class="switch">
                <input type="checkbox" checked>
                <span class="slider round"></span>
              </label> <br>


            <label for="fname">Motar Manual Operations</label><br>
            <button>ON</button>
            <button>OFF</button>

        </section>
        <section>
            <h2>Sintex Water Tank</h2>
            <p>Water Tank Place on the Builing </p>
            <label for="fname">Water Percentage</label><br>
            <input type="text" id="twaterper" name="fname" value="CM"><br><br>
            <progress id="watertankper" value ="%" max="100"> 90% </progress><br><br>



            <label class="switch">
                <input type="checkbox" checked>
                <span class="slider round"></span>
              </label> <br>


            <label for="fname">Motar Manual Operations</label><br>
            
            <button onclick="motaron">ON</button>
            <button onclick="motaroff">OFF</button>
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


}
void loop() {


ArduinoOTA.handle();
//Http Post 

  HTTPClient http;

  http.begin("http://192.168.1.77/config"); 
  http.addHeader("Content-type", "application/json" );
  int httpResponsecoce = http.POST("Posting from esp32");
  
  if (httpResponsecoce > 0) {

    String response = http.getString();
    Serial.print("httpResponsecoce");
    Serial.print("response");

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

if(distance > 70 && seconddistance < 70 ){

digitalWrite(Relay,HIGH);

}else if(distance <= 20 )
{
  digitalWrite(Relay, LOW);

}

//Webserver 
  server.handleClient();
  server.send(200, "text/html");
  server.send(404, "text/html");


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
