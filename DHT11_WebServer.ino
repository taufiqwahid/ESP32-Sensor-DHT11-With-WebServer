#include "DHT.h"
#include <WiFi.h>
#include <WebServer.h>

#define DHTPIN 2  //Digital pin connected to the DHT sensor

const char* ssid = "66";
const char* password = "11111111";

WebServer server(80);

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
  
void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT11 TEST!!")); 
  dht.begin();
  Serial.println("Terhubung Ke WIFI "); Serial.print(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Terhubung..!");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());

  server.on("/",handle_OnConnect);
  server.onNotFound(handle_NotFoundConnect);
  server.begin();
  Serial.println("HTTP SERVER DIMULAI..");
 
}

void loop() {
  server.handleClient();
  float suhu = dht.readTemperature();  
  float kelembaban = dht.readHumidity();
  Serial.print("Suhu : ");
  Serial.println(suhu);
  Serial.print("Kelembaban : ");
  Serial.println(kelembaban);
  delay(5000);
}

void handle_OnConnect(){
  float suhu = dht.readTemperature();  
  float kelembaban = dht.readHumidity();
  server.send(200,"text/html",SendHTML(suhu,kelembaban));
}

void handle_NotFoundConnect(){
  server.send(404,"tetx/plan","Halaman Tidak ditemukan");
}

String SendHTML(float temperaturestat, float humiditystat){
  String ptr ="<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<meta charset=\"utf-8\">\n";
  ptr +="<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  ptr +="<meta name=\"viewport\" content=\"width=\"device-width,initial-scale=1.0,user-scalable=no\">\n";
  ptr +="<title>Suhu dan Kelembaban</title>\n";
  ptr +="<style>html{font-family: Helveltica; display: inline-block;margin:0px auto;text-align: center;}\n";
  ptr +="body{margin-top: 50px;}h1{color: #444;margin: 50px auto 30px;}\n";
  ptr +="p{font-size: 24px;color: #444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";

ptr +="<script>\n";
ptr +="setInterval(loadDoc,200);\n";
ptr +="setTime(function(){location=''},200)\n";
ptr +="setInterval('window.location.reload()',600)\n";
ptr +="function loadDoc(){\n";
ptr +="var xhttp = XMLHttpRequest();\n";
ptr +="xhttp.onreadstatechange = function(){\n";
ptr +="if (this.readyState == 4 && this.status == 200) {\n";
ptr +="document.getElementById(\"webpage\").innerHTML = this.responseText};\n";
ptr +="};\n";
ptr +="xhttp.open(\"GET\",\"/\",true);\n";
ptr +="xhttp.send();\n";
ptr +="}\n";
ptr +="</script>";

ptr +="</head>\n"; 
ptr +="<body>\n";

ptr +="<div id=\"webpage\">\n";
ptr +="<h1>Laporan Suhu dan Kelembaban</h1>\n";
ptr +="<p>Suhu : \n";
ptr +=(int)temperaturestat;
ptr +="*C</p>\n";
ptr +="<p>Kelembaban : \n";
ptr +=(int)humiditystat;
ptr +="%</p>\n";
ptr +=temperaturestat;
ptr +=" || ";
ptr +=humiditystat;
ptr +="</div>\n";

ptr +="</body>\n";
ptr +="</html>\n";
return ptr;  
}
