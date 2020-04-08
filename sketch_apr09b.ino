//#include ESP32_Servo.h

 // Include library to connect the ESP32 to the internet
 
 #include <WiFi.h>

#include <WiFiMulti.h>
#include <DHT.h>

WiFiMulti WiFiMulti;
float h,t;
const char* ssid     = "Ashok"; // Your SSID (Name of your WiFi)
const char* password = "Project2020"; //Your Wifi password

const char* host = "api.thingspeak.com";
String api_key = "O8ZZ2MX4GW2Z7F3Y"; // Your API Key provied by thingspeak
#define DHTPIN 13          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);
  


void setup(){
  
  Serial.begin(115200);
         delay(10);
       dht.begin();
 
  

     Connect_to_Wifi();
}

void loop(){
  float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
Send_Data();

  delay(5000);
 
  

}

void Connect_to_Wifi()
{

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void Send_Data()
{

  // map the moist to 0 and 100% for a nice overview in thingspeak.
  
  

  Serial.println("Prepare to send data");

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  { 
    float h = dht.readHumidity();
      float t = dht.readTemperature();
      Serial.print(t);
      Serial.print(h);
    String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += String(t);
    data_to_send += "&field2=";
    data_to_send += String(h);
    // data_to_send += "&field3=";
    // data_to_send += String(pressure);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    delay(1000);
  }

  client.stop();

}
