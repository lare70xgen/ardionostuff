/*
TMP36_gnd
TMP36_Vout-->A0 Keskimmäinen pinni
TMP36_Vin

Arduino IDE, Borad: Lolin Wemos D1 mini clone, Port: usb modem
*/
// https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <ESP8266WiFiMulti.h>
  #include <ESP8266mDNS.h> 
#endif

int sensorPin = 0; 
String temperatureC = "";
unsigned long lastTime = 0;  
unsigned long timerDelay = 10000;

//const char* ssid = "iPhone";
//const char* password = "l1a2r3e4";
const char* ssid = "deconet";
const char* password = "l1*a2r3e4t5y6";
//const char* ssid = "Teltonika_Router";
//const char* password = "l1*a2r3e4t5y6";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
String readDSTemperatureC() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  int reading = analogRead(sensorPin);  
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * (3025 / 1024.0);//Ensimmäinen arvo, Suurempi arvo, suurempi lämpö lukema, //esim 3025-3030 = suunnilleen oikea lämpötila.
  // now print out the temperature
  float temperatureC = (voltage - 500) / 10 ;  //converting from 10 mv per degree wit 500 mV offset
  Serial.print(temperatureC); //Serial.println(" degrees C");
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("C");
  return String(temperatureC);
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP TMP36gz Server ESP8266 </h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="ds-labels">Temperature Celsius on ESP8266 </span> 
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
 
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;

</script>
</html>)rawliteral";

String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return temperatureC;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  Serial.println();
   
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureC.c_str());
  });
   
  // Start server
  server.begin();
}
 
void loop(){
   if ((millis() - lastTime) > timerDelay) {
    temperatureC = readDSTemperatureC();
    lastTime = millis();
  }  
}
