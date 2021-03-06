#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
char auth[] = "     ";              //Your Project authentication key 
char ssid[] = "     ";              // Name of your network (HotSpot or Router name)
char pass[] = "     ";
 
#define DHTPIN 14          // D5
 
// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301
 //If you are using any other DHT sensor don't forget
DHT dht(DHTPIN,DHTTYPE);
BlynkTimer timer;
 
// This function sends NodeMCU up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit 
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}
 
void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(D1,OUTPUT); //extend these to D8 if you are using a 8 pin relay or ignore if you are using it for motor driver board
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);

  digitalWrite(D1,LOW); // Make it low if you want everything to go off /*/
  digitalWrite(D2,LOW); // in case of a power cut
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);


  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
 
  dht.begin();
 
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}
 
void loop()
{
  Blynk.run();
  timer.run();
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
