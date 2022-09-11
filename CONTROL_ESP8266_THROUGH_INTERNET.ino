#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char *ssid = "VNPT_HUYENTRANG 1";
const char *password = "07070707";

//  const char* ssid     = "VNPT_2.4G";
//  const char* password = "nguyen12345";

const char *serverName = "http://lcn113.000webhostapp.com/ESP8266 WITH DATABASE/esp_data.php";

String host = "lcn113.000webhostapp.com";
String path = "/CONTROL ESP8266 THROUGH INTERNET/test.json";
String line;

// const char* serverName = "http://funixproject3.000webhostapp.com/esp_data.php";
String apiKeyValue = "kjsjkhjdhfd";
String sensorName = "DS18B20";
String sensorLocation = "BEDROOM";

void setup()
{
  Serial.begin(115200);

  pinMode(16, OUTPUT);

  digitalWrite(16, HIGH);
  delay(3000);
  digitalWrite(16, LOW);
  delay(3000);

  // Connect to Wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  delay(1000);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {

    WiFiClient client;
    if (!client.connect(host, 80))
    {
      Serial.println("Connect to server fail!");
      return;
    }

    client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: keep-alive\r\n\r\n");

    while (client.available() == 0)
    {
    }

    while (client.available())
    {
      //    String line = client.readStringUntil('\r');
      //    Serial.print(line);
      line = client.readStringUntil('}');
      //    Serial.println(line + "}");

      String result = line.substring(line.length() - 24, line.length());
      Serial.println(result);

      if (result == "\"led1\":\"on\",\"led2\":\"off\"")
      {
        digitalWrite(16, LOW);
      }
      else
      {
        digitalWrite(16, HIGH);
      }
      client.stop();
    }
  }

  else
  {
    Serial.println("WiFi Disconnected");
    Serial.print(".");
    delay(250);
  }
}