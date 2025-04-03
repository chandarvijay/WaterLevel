
#include "WiFi.h"
#include "WebServer.h"


int led = 5;
int builtinled = T2;
int Relay = 15;
const char* ssid = "Uma Nelayam 4G";
const char* password = "9980337734";

WebServer server(80);




void welcome() {

  char test[] PROGMEM = R"===(<!DOCTYPE html>



<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test Page</title>
    <script src="script.js" defer></script>
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


</style>

</head>
<body>
    <header>
        <h1>WaterLevel Controller </h1>
    </header>
    <main>
        <section>
            <h2>SUMP Water Level</h2>
            <p>Sump tank that is pumped up to the water tank</p>
            <label for="fname">Water Percentage</label><br>
            <input type="text" id="fname" name="fname"><br><br>
            <progress id="file" value="90" max="100"> 90% </progress><br><br>

            <label for="fname">Motar Manual Operations</label><br>
            <button>ON</button>
            <button>OFF</button>

        </section>
        <section>
            <h2>Sintex Water Tank</h2>
            <p>Water Tank Place on the Builing </p>
            <label for="fname">Water Percentage</label><br>
            <input type="text" id="fname" name="fname"><br><br>
            <progress id="file" value="10" max="100"> 90% </progress><br><br>

            <label for="fname">Motar Manual Operations</label><br>
            <button>ON</button>
            <button>OFF</button>
        </section>
    </main>
    <footer>
        <p>&copy; 2023 WaterLevel Controller</p>
    </footer>






</html>)===";

  //String test = "<!DOCTYPE html>\n";
  //test += " <header><h1 style=background-color: rgb(99, 99, 176);>WaterLevel Controller </h1></header>\n";
  //test += "<title>my webpage</title>";
  //test += "<section><h2>SUMP Water Level</h2>";
  //test+=" <p>Sump tank that is pumped up to the water tank</p>";
  //test +=  "<input type='text'> ";
  //test += " <progress value='90'> 90% </progress><br>";
  //test += " <label>Motar Manual Operations</label><br>";
  //test+= "  <button>ON</button>";
  //test += " <button>OFF</button></section>";
 //test += "<section><h2> Water Tank </h2>";
  //test+=" <p>Sump tank that is pumped up to the water tank</p>";
  ///test +=  "<input type='text'> ";
 // test += " <progress value='10'> 90% </progress><br>";
  //test += " <label>Motar Manual Operations</label><br>";
  //test+= "  <button>ON</button>";
 // test += " <button>OFF</button></section>";



 server.send(200,"text/html",test);

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
  server.begin();



  // put your setup code here, to run once:

  pinMode(led, OUTPUT);
  pinMode(builtinled, OUTPUT);
  pinMode(Relay, OUTPUT);
}
void loop() {

  server.handleClient();
  server.send(200, "text/html");
  server.send(404, "text/html");



  digitalWrite(builtinled, LOW);
  delay(500);
  digitalWrite(builtinled, LOW);
  delay(200);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(2000);
  // put your main code here, to run repeatedly:
}
