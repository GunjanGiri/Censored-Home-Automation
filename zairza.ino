#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"        //https://github.com/tzapu/WiFiManager


#define dht_dpin 0
DHT dht(dht_dpin, DHTTYPE); 
int fan = 5;

//pir

int Status = 12;  // Digital pin D6 //changed to D4

int sensor = 13; //digital pin D7 //changed to D1


//gas
int count=0,i,m,j,k;

//int outputpin= A0;
int s1,s2;
const int gasPin = A0;//mq6 sensor 



String apiKey = "GMF2JEMOQLBZWYKH"; // Enter your Write API key from ThingSpeak
const char *ssid = "kundu"; // replace with your wifi ssid and wpa2 key
const char *pass = "12345678";
const char* server = "api.thingspeak.com";
WiFiClient client;



void setup(void)
{ 

//humidity
  dht.begin();
  pinMode(fan, OUTPUT);
  Serial.begin(9600);
  Serial.println("Humidity and temperature\n\n");
  delay(700);

  //pir
  Serial.begin(9600);
  pinMode(sensor, INPUT);   // declare sensor as input
  pinMode(Status, OUTPUT);  // declare LED as output

//gas
  Serial.begin(9600);
delay(10);
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");

}

void loop() {

//humidity
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    delay(800);

    if((t > 25)){
      digitalWrite(fan , 200);
    }
    }
    
//pir
    long state = digitalRead(sensor);
  delay(1000);
    if(state == HIGH){
      digitalWrite (Status, HIGH);
      Serial.println("Motion detected!");
    }
    else {
      digitalWrite (Status, LOW);
      Serial.println("Motion absent!");
      }
      
 //gas
 float Th = analogRead(A0);
if (isnan(Th))
{
Serial.println("Failed to read from MQ-5 sensor!");
return;
}

if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
{
String postStr = apiKey;
postStr += "&field1=";
postStr += String(Th/1023*100);
postStr += "r\n";
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
Serial.print("Gas Level: ");
Serial.println(Th/1023*100);
Serial.println("Data Send to Thingspeak");
}
client.stop();
Serial.println("Waiting...");

// thingspeak needs minimum 15 sec delay between updates.
delay(1500);
}
