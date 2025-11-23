#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

const char* ssid = "Your wifi";
const char* password = "Your wifi password";

#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

String writeAPIKey = "12BANIxxxxCY";
String channelID = "3166926";

WiFiServer server(80);

unsigned long lastPush = 0;
const unsigned long pushInterval = 15000;

unsigned long lastTempRead = 0;
const unsigned long tempReadInterval = 5000;
float currentTemperature = 0.0;

float readTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT11, keeping last value");
    return currentTemperature;
  }
  return temp;
}

void serveDashboard(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html>");
  client.println("<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<title>Sensor Dashboard</title>");
  client.println("<link href='https://fonts.googleapis.com/css2?family=Inter:wght@300;400;600&family=Space+Mono:wght@400;700&display=swap' rel='stylesheet'>");
  client.println("<style>");
  client.println("body{margin:0;padding:20px;background:linear-gradient(135deg,#0f0c29,#302b63,#24243e);min-height:100vh;color:#e8eaed;font-family:'Inter',sans-serif;font-weight:300;}");
  client.println(".container{max-width:900px;margin:0 auto;}");
  client.println("h1{text-align:center;margin-bottom:40px;color:#ffffff;font-weight:600;font-family:'Space Mono',monospace;letter-spacing:4px;font-size:1.8em;text-shadow:0 0 20px rgba(138,180,248,0.3);}");
  client.println(".cards{display:flex;flex-wrap:wrap;gap:20px;justify-content:center;margin-bottom:30px;}");
  client.println(".card{background:rgba(255,255,255,0.05);backdrop-filter:blur(10px);padding:30px;border-radius:16px;min-width:200px;text-align:center;border:1px solid rgba(255,255,255,0.1);transition:all 0.3s ease;box-shadow:0 8px 32px rgba(0,0,0,0.3);}");
  client.println(".card.active{background:rgba(255,255,255,0.08);border-color:rgba(138,180,248,0.4);box-shadow:0 8px 32px rgba(138,180,248,0.2);}");
  client.println(".card>div:first-child{font-size:0.75em;letter-spacing:2px;font-weight:600;opacity:0.7;margin-bottom:8px;}");
  client.println(".value{font-size:2.8em;font-weight:700;margin:15px 0;letter-spacing:2px;font-family:'Space Mono',monospace;}");
  client.println(".temp{color:#8ab4f8;text-shadow:0 0 20px rgba(138,180,248,0.4);} .flame{color:#f28b82;text-shadow:0 0 20px rgba(242,139,130,0.4);} .light{color:#81c995;text-shadow:0 0 20px rgba(129,201,149,0.4);}");
  client.println(".graph{background:rgba(255,255,255,0.05);backdrop-filter:blur(10px);padding:25px;border-radius:16px;margin-top:20px;border:1px solid rgba(255,255,255,0.1);box-shadow:0 8px 32px rgba(0,0,0,0.3);}");
  client.println("iframe{width:100%;height:320px;border:none;border-radius:8px;}");
  client.println(".status{text-align:center;color:rgba(232,234,237,0.6);margin-top:20px;font-size:0.85em;letter-spacing:1px;font-family:'Space Mono',monospace;}");
  client.println(".disabled{opacity:0.4;}");
  client.println(".card.disabled .value{text-shadow:none;}");
  client.println("</style>");

  client.println("<meta http-equiv='refresh' content='5'>");
  client.println("</head><body>");
  client.println("<div class='container'>");
  client.println("<h1>SENSOR DASHBOARD</h1>");

  client.println("<div class='cards'>");

  client.println("<div class='card active'>");
  client.println("<div>TEMPERATURE</div>");
  client.println("<div class='value temp'>" + String(currentTemperature, 1) + "</div>");
  client.println("<div>CELSIUS</div>");

  if(currentTemperature > 35.0) {
    client.println("<div style='color:#f28b82;margin-top:10px;font-size:0.9em;font-weight:600;'>HIGH</div>");
  } else if(currentTemperature > 28.0) {
    client.println("<div style='color:#fdd663;margin-top:10px;font-size:0.9em;font-weight:600;'>WARM</div>");
  } else if(currentTemperature < 15.0) {
    client.println("<div style='color:#8ab4f8;margin-top:10px;font-size:0.9em;font-weight:600;'>COOL</div>");
  } else {
    client.println("<div style='color:#81c995;margin-top:10px;font-size:0.9em;font-weight:600;'>NORMAL</div>");
  }

  client.println("</div>");

  client.println("<div class='card disabled'>");
  client.println("<div>FLAME</div>");
  client.println("<div class='value flame'>NIL</div>");
  client.println("<div>ANALOG</div>");
  client.println("</div>");

  client.println("<div class='card disabled'>");
  client.println("<div>LIGHT</div>");
  client.println("<div class='value light'>NIL</div>");
  client.println("<div>LUX</div>");
  client.println("</div>");

  client.println("</div>");

  client.println("<div class='graph'>");
  client.println("<iframe src='https://thingspeak.com/channels/3166926/charts/1?api_key=WJX75SCXXXXXE5&bgcolor=%232c2c3f&color=%23e0e0e0&width=700&height=300'></iframe>");
  client.println("</div>");

  client.println("<div class='status'>");
  client.println("UPDATED: " + String(millis()/1000) + "s AGO | AUTO-REFRESH: 5s");
  client.println("</div>");
  client.println("</div>");
  client.println("</body></html>");
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
  currentTemperature = readTemperature();
  Serial.println("Initial Temperature: " + String(currentTemperature, 1));
}

void loop() {
  if (millis() - lastTempRead >= tempReadInterval) {
    currentTemperature = readTemperature();
    Serial.println("Temperature: " + String(currentTemperature, 1));
    lastTempRead = millis();
  }

  WiFiClient clientDashboard = server.available();
  if (clientDashboard) {
    String request = clientDashboard.readStringUntil('\r');
    clientDashboard.flush();
    
    serveDashboard(clientDashboard);
    clientDashboard.stop();
    Serial.println("Dashboard served");
  }

  if (WiFi.status() == WL_CONNECTED && millis() - lastPush >= pushInterval) {
    HTTPClient http;
    WiFiClient clientThingSpeak;

    String url = "http://api.thingspeak.com/update?api_key=" + writeAPIKey + "&field1=" + String(currentTemperature);

    http.begin(clientThingSpeak, url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      Serial.print("ThingSpeak updated. Response code: ");
      Serial.println(httpCode);
    } else {
      Serial.print("ThingSpeak update failed. Error: ");
      Serial.println(http.errorToString(httpCode));
    }
    
    http.end();
    lastPush = millis();
  }
}