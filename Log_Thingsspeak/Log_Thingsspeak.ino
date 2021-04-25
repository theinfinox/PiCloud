#include <DHT.h>                                      //Declarate DHT Library
#include <ThingSpeak.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

static const int RXPin = 4, TXPin = 5;   // GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800

TinyGPSPlus gps; // The TinyGPS++ object
WidgetMap myMap(V0);  // V0 for virtual pin of Map Widget

SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device

BlynkTimer timer;

long myChannelNumber =         *      ;
const char myWriteAPIKey[] = " *     ";

#define DHTPIN 2          //pin where the dht11 is connected

DHT dht(DHTPIN, DHT11);
WiFiClient client;


float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS


char auth[] = "   *    ";              //Your Project authentication key
char ssid[] = "   *    ";                                       // Name of your network (HotSpot or Router name)
char pass[] = "   *    ";

//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now


void setup()
{
  Serial.begin(115200);
  Serial.println();
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
  dht.begin();                                        //Start DHT sensor


  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void checkGPS() {
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
}

void loop()
{
  while (ss.available() > 0)
  {
    // sketch displays information every time a new sentence is correctly encoded.
    if (gps.encode(ss.read()))
      displayInfo();
  }
  Blynk.run();
  timer.run();



  float h = dht.readHumidity();                       //Read humidity and put it in h variable
  float t = dht.readTemperature();                    //Read temperature and put it in t variable
  float q = analogRead(A0)/10;                           //Read air quality and put it into q variable
  Blynk.virtualWrite(V6, t);                          //Send t value to blynk in V5 virtual pin
  Blynk.virtualWrite(V7, h);                          //Send h value to blynk in V6 virtual pin
  Blynk.virtualWrite(V8, q);                          //Send q value to blynk in V0 virtual pin
  if (q > 100) {
    Blynk.notify("The Quailty of air is bad !!");     //Blynk will send notify if q>70
  }
  if (t > 40) {
    Blynk.notify("The Temperature is too high");      //Blynk will send notify if t>40
  }


  Serial.println("Temperature: " + (String) t);
  Serial.println("Humidity: " + (String) h);
  Serial.println("Air quality: " + (String) q);
  ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, q, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 4, latitude, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 5, longitude, myWriteAPIKey);
  delay(2000);



}

void displayInfo()
{
  if (gps.location.isValid() )
  {
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon.
    float longitude = (gps.location.lng());

    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));
    Blynk.virtualWrite(V2, String(longitude, 6));
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
    Blynk.virtualWrite(V3, spd);

    sats = gps.satellites.value();    //get number of satellites
    Blynk.virtualWrite(V4, sats);

    bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
    Blynk.virtualWrite(V5, bearing);
  }

  Serial.println();
}
