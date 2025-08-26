#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6dQVnRZMt"           // Your Template ID
#define BLYNK_TEMPLATE_NAME "Cake Spoilage Detection" // Your Template Name
#define BLYNK_AUTH_TOKEN "RAJ3n6TQO4mWGVg-qp2vJwhLBVBggR62"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Joy_Turjoy";  // Your WiFi SSID
char pass[] = "16101292";    // Your WiFi password

const int ldrPin = A0;       // LDR connected to A0

void setup() {
  Serial.begin(115200);
  delay(100);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to Blynk!");
}

void loop() {
  Blynk.run();  // Keep Blynk connection alive

  // Read LDR
  int ldrValue = analogRead(ldrPin);
  float lightPercent = ldrValue / 1023.0 * 100; // Convert to 0–100%

  // Print to Serial Monitor
  Serial.print("LDR Raw: ");
  Serial.print(ldrValue);
  Serial.print("\tLight: ");
  Serial.println(lightPercent);

  // Send LDR value to Blynk
  Blynk.virtualWrite(V0, lightPercent);  // For Value Display or Gauge
  Blynk.virtualWrite(V1, ldrValue);      // For Graph widget (raw value)

  delay(1000); // Update every 1 second
}
