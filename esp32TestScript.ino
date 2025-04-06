
#include "WiFi.h"
#include "WebServer.h"
#include "WebSocketsServer.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

//int builtinled = T2;
int Relay = 5;
const char* ssid = "Uma Nelayam 4G";
const char* password = "9980337734";


float distance;
float duration;

int triggerpin = 15;
int echo =4 ;

int second_echo = 18;
int second_trigger = 19;

float seconddistance;
float secondduration;


WebServer server(80);

WebSocketsServer WS = WebSocketsServer(81);


void welcome() {

  char test[] PROGMEM = R"===(<!DOCTYPE html>

<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WaterLevel Controller</title>
    <script>
    
    let dis = 'http://192.168.1.69/test';
    
    document.getElementById("swaterper").value = dis[0];
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
</style>




<!DOCTYPE html>



<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WaterController</title>
    <link rel="stylesheet" href="styles.css">
    <script src="script.js" async></script>

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
            <input type="text" id="swaterper" name="fname" value="%"><br><br>

            <progress id="file" value="%" max="100"> {distance} </progress><br><br>

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
            <input type="text" id="twaterper" name="fname" value="%"><br><br>
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




void config(){

char config [] PROGMEM = R"===(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WiFi Settings</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }
        h1 {
            color: #333;
            text-align: center;
        }
        label {
            display: block;
            margin-bottom: 8px;
        }
        input[type="text"], input[type="password"] {
            width: 100%;
            padding: 8px;
            margin-bottom: 10px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }
        button {
            padding: 10px 15px;
            background-color: #2b4abb;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }
        button:hover {
            background-color: #1258a9;
        }

        #container {
            max-width: 400px;
            margin: auto;
            padding: 20px;
            border: 1px solid #ccc;
            border-radius: 8px;
            background-color: #f9f9f9;
        }
</style>

</head>
<body>
    
<h1>WiFi Configuration</h1>
<div id="container">
<label for="ssid">SSID:</label>
<input type="text" id="ssid" name="ssid"><br><br>
<label for="password">Password:</label>
<input type="password" id="password" name="password"><br><br>
<button id="submit">Submit</button>
<button id="reset">Reset</button>
</div>
</body>
</html>)===";

server.send(200,"text/html",config);


}




void senddata(){

 StaticJsonDocument<300> JSONData; 

  JSONData["FIrst Ultrasonic Sensor"] = distance;
  JSONData["Second Ultrasonic Sensor"] = seconddistance;


  char data[300];
  serializeJson(JSONData,data);
server.send(200,"application/json",data);

}




void setup() {


  Serial.begin(115200);
  
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
  server.on("/", welcome);
  server.on("/config",config);
  server.on("/test",HTTP_GET,senddata);

  server.begin();



  // put your setup code here, to run once:
  pinMode(echo,INPUT);
  pinMode(triggerpin,OUTPUT);
 // pinMode(led, OUTPUT);
  pinMode(Relay, OUTPUT);
  pinMode(second_echo,INPUT);
  pinMode(second_trigger,OUTPUT);


}
void loop() {



//Http Post 

  HTTPClient http;

  http.begin("http:192.168.1.65/config"); 
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
delayMicroseconds(10);
digitalWrite(triggerpin,LOW);

duration = pulseIn(echo,HIGH);
distance =(duration/2)*0.343;


digitalWrite(second_trigger,LOW);
delayMicroseconds(20);

digitalWrite(second_trigger,HIGH);
delayMicroseconds(10);
digitalWrite(second_trigger,LOW);

secondduration = pulseIn(second_echo,HIGH);
seconddistance =(secondduration/2)*0.343;


Serial.print("The distance of the first is \n");
Serial.print(distance);
Serial.print("The distance of the second is \n");
Serial.print(seconddistance);

if(distance <= 160 || seconddistance <= 160 ){

digitalWrite(Relay,HIGH);

} else{
    digitalWrite(Relay,LOW);
}



//Webserver 
  server.handleClient();
  server.send(200, "text/html");
  server.send(404, "text/html");


}
