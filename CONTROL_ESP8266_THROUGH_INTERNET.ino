#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Wifi-1 (ở nhà)
const char *ssid = "VNPT_HUYENTRANG 1";
const char *password = "07070707";

// Wifi-2 (phòng trọ)
// const char *ssid = "VNPT_2.4G";
// const char *password = "nguyen12345";

// Wifi-3 (cơ điện)
//  const char *ssid = "";
//  const char *password = "duoc6789";

// Host-1
//  String host = "lcn113.000webhostapp.com";
//  String path = "/ESP8266 WITH DATABASE/esp_data.php";

// Host-2
String host = "funixproject3.000webhostapp.com";
String path = "/esp_data.php";

String httpResponse;

String apiKeyValue = "kjsjkhjdhfd";
String sensorName = "DS18B20";
String sensorLocation = "BEDROOM";

void setup()
{
  Serial.begin(115200);
  pinMode(16, OUTPUT);

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
    WiFiClient wificlient;
    if (wificlient.connect("funixproject3.000webhostapp.com", 80))
    {
      wificlient.print("POST"); // Bắt đầu gửi request
      wificlient.print(" ");
      wificlient.print("/esp_data.php");
      wificlient.println(" HTTP/1.1");
      wificlient.print("Host: ");
      wificlient.println("funixproject3.000webhostapp.com");
      wificlient.print("Content-Length: ");
      wificlient.println(strlen("api_key=kjsjkhjdhfd&sensor=DS18B20"));
      wificlient.print("Content-Type: ");
      wificlient.println("application/x-www-form-urlencoded");
      wificlient.println();
      wificlient.println("api_key=kjsjkhjdhfd&sensor=DS18B20");
      wificlient.println();
    }

    while (wificlient.available() == 0) // Wait for responsing of Server
    {
    }

    while (wificlient.available())
    {
        httpResponse = wificlient.readString();
        Serial.println(httpResponse);
    }

    // // httpSendToDatabase();
    // char response[1024] = {'\0'};
    // bool connectionStatus;
    // wificlientrequest(response, "funixproject3.000webhostapp.com", 80,
    //                   "POST", true, "/esp_data.php", "api_key=dfgdfgdfgdfg",
    //                   "application/x-www-form-urlencoded", &connectionStatus);
    // // Serial.print(response);
    delay(60000);
  }

  else
  {
    Serial.println("WiFi Disconnected");
    Serial.print(".");
    delay(250);
  }
}

//============================================//

void httpSendToDatabase()
{
  WiFiClient client;
  if (!client.connect(host, 80))
  {
    Serial.println("Connect to server fail!");
    return;
  }

  while (client.available() == 0) // Wait for responsing of Server
  {
  }

  while (client.available())
  {
    httpResponse = client.readString();
    Serial.print(httpResponse);

    delay(60000);

    client.stop();
  }
}

void getDeviceStatus()
{
  WiFiClient client;
  if (!client.connect(host, 80))
  {
    Serial.println("Connect to server fail!");
    return;
  }

  String httpRequest = "GET " + String("/test.json") + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n";

  Serial.println(httpRequest);

  client.print(httpRequest);

  while (client.available() == 0) // Wait for responsing of Server
  {
  }

  while (client.available())
  {
    httpResponse = client.readStringUntil('}');

    String result = httpResponse.substring(httpResponse.length() - 24, httpResponse.length());
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

void wificlientrequest(char result[], const char IPServer[], int Port, const char method[], bool getResponse, const char url[], const char data[], const char contenttype[], bool *connectionStatus)
{
  WiFiClient wificlient;

  if (wificlient.connect(IPServer, Port)) // Nếu kết nối tới host thành công...
  {
    *connectionStatus = true;
    wificlient.print(method); // Bắt đầu gửi request
    wificlient.print(" ");
    wificlient.print(url);
    wificlient.println(" HTTP/1.1");
    wificlient.print("Host: ");
    wificlient.println(IPServer);

    if (strcmp(method, "POST") == 0) // Nếu là POST request
    {
      wificlient.print("Content-Type: ");
      wificlient.println(contenttype);
      wificlient.print("Content-Length: ");
      wificlient.println(strlen(data));
      wificlient.println();
      wificlient.println(data);
      wificlient.println();
    }
    else
    {
      wificlient.println(); // Gửi xong request
    }

    if (getResponse)
    {
      // unsigned long batdaunhanketqua = millis();
      unsigned long start = millis();
      unsigned long timeout = 3000;
      bool currentLineIsBlank = true;

      while (wificlient.connected())
      {
        if (millis() - start > timeout)
        {
          break;
        }
        if (wificlient.available())
        {
          char c = wificlient.read();
          if (currentLineIsBlank && c == '\n')
          {
            break;
          }

          if (c == '\n')
            currentLineIsBlank = true;
          else if (c != '\r')
            currentLineIsBlank = false;
        }

        yield();
      }

      char kq[256] = {'\0'};
      bool batdaukiemtraavailable = false;
      int solannotavailable = 0;

      while (wificlient.connected())
      {
        if (millis() - start > timeout || solannotavailable >= 1000)
        {
          break;
        }

        if (wificlient.available() > 0)
        {
          char c = wificlient.read();
          int len = strlen(kq);
          if (c > 0 && c < 255 && (len <= sizeof(kq)))
          {
            kq[len] = c;
            kq[len + 1] = '\0';

            solannotavailable = 0;
            batdaukiemtraavailable = true;
          }
        }
        else
        {
          if (batdaukiemtraavailable)
          {
            solannotavailable++;
          }
        }

        yield();
      }
      strcpy(result, kq);
    }
    else
    {
      wificlient.stop();
      strcpy(result, "");

      return;
    }
  }
  else
  {
    *connectionStatus = false;
    wificlient.stop();
    strcpy(result, "");
    return;
  }
}
