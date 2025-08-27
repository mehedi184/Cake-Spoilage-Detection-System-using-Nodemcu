#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6dQVnRZMt"           // Your Template ID
#define BLYNK_TEMPLATE_NAME "Cake Spoilage Detection" // Your Template Name
#define BLYNK_AUTH_TOKEN "RAJ3n6TQO4mWGVg-qp2vJwhLBVBggR62"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

char auth[] = BLYNK_AUTH_TOKEN;   // Auth token
char ssid[] = "Joy_Turjoy";  // Your WiFi SSID
char pass[] = "16101292";    // Your WiFi password

#define DHTPIN D4          // Connect DHT11 Data pin to NodeMCU D4 (GPIO2)
#define DHTTYPE DHT11      // DHT11 Sensor type

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Function to send sensor data to Blynk
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, t);  // Send Temperature to Virtual Pin V0
  Blynk.virtualWrite(V6, h);  // Send Humidity to Virtual Pin V1

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C  | Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // Send data every 2 seconds
  timer.setInterval(2000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
