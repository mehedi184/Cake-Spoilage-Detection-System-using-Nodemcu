#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6dQVnRZMt"           // Your Template ID
#define BLYNK_TEMPLATE_NAME "Cake Spoilage Detection" // Your Template Name
#define BLYNK_AUTH_TOKEN "RAJ3n6TQO4mWGVg-qp2vJwhLBVBggR62"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Joy_Turjoy";  // Your WiFi SSID
char pass[] = "16101292";    // Your WiFi password
// Blynk Timer
BlynkTimer timer;

// Function to read pH
void sendPH()
{
  int phValueRaw = analogRead(A0);        // Raw ADC value (0–1023)
  
  // Convert ADC to Voltage (NodeMCU A0 max 1V)
  float voltage = phValueRaw * (1.0 / 1023.0);  

  // Example conversion (depends on calibration of sensor)
  // Assuming ~0 → 0 pH, 1V → ~14 pH
  float phValue = voltage * 14.0;  

  // Send to Blynk Virtual Pin V1
  Blynk.virtualWrite(V1, phValue);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Run every 2 seconds
  timer.setInterval(2000L, sendPH);
}

void loop()
{
  Blynk.run();
  timer.run();
}
