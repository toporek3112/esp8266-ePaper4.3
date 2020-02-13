#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "epd.h"


///////////////////////////////////////////////////////////////////////////
// in my case i needed to connect to the internet (ignore it)
///////////////////////////////////////////////////////////////////////////

const char *ssid = "Karol";         //Enter SSID
const char *password = "janina123"; //Enter Password

//////////////////////////////////////////////////////////////////////////

const int wake_up = 5;
const int reset = 4;

int width = 800;
int height = 600;

void setup(void)
{

  epd_init(wake_up, reset);
  epd_wakeup(wake_up);
  epd_set_memory(MEM_NAND);
  epd_clear();

  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output

  Serial.begin(115200);
  Serial.println("Connecting");


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("*");
  }

  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP()); // Print the IP address
  Serial.println("");
}

void loop()
{

  HTTPClient http;
  http.begin("http://192.168.43.247:4001/services/realtime");

  int httpCode = http.GET();

  Serial.print("HTTPCODE: ");
  Serial.println(httpCode);

  String response = http.getString();

  http.end();

  Serial.println(response);

  const size_t capacity = JSON_ARRAY_SIZE(4) + 8 * JSON_OBJECT_SIZE(3) + 4 * JSON_OBJECT_SIZE(5) + 750;
  DynamicJsonBuffer jsonBuffer(capacity);

  JsonArray &root = jsonBuffer.parseArray(response);
  int width = 150;

  delay(3000);

  for (int i = 0; i < 1; i++)
  {
    JsonObject &root_0 = root[i];
    bool endstation = root_0["endstation"];
    const char *lineName = root_0["lineName"];
    const char *stationName = root_0["stationName"];

    JsonObject &steige1 = root_0["steige1"];
    const char *steige1_richtung = steige1["richtung"];
    int steige1_countdown0 = steige1["countdown0"];
    int steige1_countdown1 = steige1["countdown1"];

    JsonObject &seige2 = root_0["seige2"];
    const char *seige2_richtung = seige2["richtung"];
    const char *seige2_countdown0 = seige2["countdown0"];
    const char *seige2_countdown1 = seige2["countdown1"];

    Serial.println();
    Serial.println(lineName);

    epd_clear();

    epd_set_ch_font(GBK64);
    epd_set_en_font(ASCII64);
    epd_set_color(BLACK, WHITE);

    epd_disp_string(stationName, 10, 20);
    epd_disp_string(steige1_richtung, 100, 100);
    epd_disp_string(steige1_richtung, 200, 100);
    
    epd_udpate();
  }

  digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(1000);                     // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  delay(30000);                    // Wait for two seconds (to demonstrate the active low LED)
}
