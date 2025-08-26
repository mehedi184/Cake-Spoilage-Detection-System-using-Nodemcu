#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6dQVnRZMt"           // Your Template ID
#define BLYNK_TEMPLATE_NAME "Cake Spoilage Detection" // Your Template Name
#define BLYNK_AUTH_TOKEN "RAJ3n6TQO4mWGVg-qp2vJwhLBVBggR62"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Joy_Turjoy";  // Your WiFi SSID
char pass[] = "16101292";    // Your WiFi password

// Pin setup
int mq3_analog = A0;  // MQ3 AO pin to A0 of NodeMCU
int mq3_digital = D6; // MQ3 DO pin to D6 (optional)

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(mq3_digital, INPUT);
}

void loop()
{
  Blynk.run();

  int analogValue = analogRead(mq3_analog);   // Read alcohol level
  int digitalValue = digitalRead(mq3_digital); // Detect alcohol presence (yes/no)

  // Print to Serial Monitor
  Serial.print("Analog: ");
  Serial.print(analogValue);
  Serial.print("  |  Digital: ");
  Serial.println(digitalValue);

  // Send data to Blynk
  Blynk.virtualWrite(V3, analogValue);  // Graph widget on V1
  Blynk.virtualWrite(V2, digitalValue); // LED or Value widget on V2

  delay(1000); // 1 sec delay
}
