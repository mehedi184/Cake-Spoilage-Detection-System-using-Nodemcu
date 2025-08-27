#define BLYNK_PRINT Serial
// ---------- BLYNK / WIFI CONFIG ----------
#define BLYNK_TEMPLATE_ID "TMPL6dQVnRZMt"
#define BLYNK_TEMPLATE_NAME "Cake Spoilage Detection" // Your Template Name
#define BLYNK_AUTH_TOKEN "RAJ3n6TQO4mWGVg-qp2vJwhLBVBggR62"


char ssid[] = "@Mehedi_5G@";
char pass[] = "suraiya184";
// ----------------------------------------

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <LiquidCrystal.h>

// ---- Sensor pin map ----
#define DHTPIN D4
#define DHTTYPE DHT11
#define LDR_PIN A0
#define MQ_DO_PIN D3      // via 10k/10k divider if MQ powered at 5V
#define PH_DO_PIN D8      // via 10k/10k divider if pH board at 5V

// ---- LCD parallel pins ----
const int LCD_RS = D5;   // GPIO14
const int LCD_EN = D6;   // GPIO12
const int LCD_D4 = D7;   // GPIO13
const int LCD_D5 = D1;   // GPIO5
const int LCD_D6 = D2;   // GPIO4
const int LCD_D7 = D0;   // GPIO16

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// ---- Thresholds from your spec ----
const float LDR_THRESHOLD_PERCENT = 80.0;
const float TEMP_LOW   = 20.0;
const float TEMP_HIGH  = 40.0;
const float HUM_THRESH = 70.0;

// ---- State ----
int   ldrRaw=0, mqState=0, phState=0;
float ldrPct=0, tempC=0, hum=0;
bool  warningFlag=false, gasDetected=false, rotten=false, safeFlag=false;

void readAndDecide() {
  // LDR
  ldrRaw = analogRead(LDR_PIN);
  ldrPct = (ldrRaw / 1023.0) * 100.0;

  // DHT
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t)) tempC = t;
  if (!isnan(h)) hum   = h;

  // MQ3 and pH (digital thresholds)
  mqState = digitalRead(MQ_DO_PIN);   // HIGH = over threshold (module dependent)
  phState = digitalRead(PH_DO_PIN);   // HIGH = out-of-range (module pot)

  // Priority: WARNING -> GAS -> ROTTEN -> SAFE
  warningFlag = gasDetected = rotten = safeFlag = false;
  if ( (ldrPct > LDR_THRESHOLD_PERCENT) || (tempC < TEMP_LOW) || (tempC > TEMP_HIGH) || (hum > HUM_THRESH) ) {
    warningFlag = true;
  } else if (mqState == HIGH) {
    gasDetected = true;
  } else if (phState == HIGH) {
    rotten = true;
  } else {
    safeFlag = true;
  }

  // LCD
  String status;
  if (warningFlag)      status = "Warning: Check!";
  else if (gasDetected) status = "Gas Detected!";
  else if (rotten)      status = "Spoilage Alert";
  else                  status = "Status: SAFE";

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(status.substring(0,16));
  lcd.setCursor(0,1);
  String line2 = String(tempC,1) + "C " + String(hum,0) + "% L:" + String((int)ldrPct);
  lcd.print(line2.substring(0,16));

  // Blynk values
  Blynk.virtualWrite(V0, ldrPct);
  Blynk.virtualWrite(V1, ldrRaw);
  Blynk.virtualWrite(V2, tempC);
  Blynk.virtualWrite(V3, hum);
  Blynk.virtualWrite(V4, mqState);
  Blynk.virtualWrite(V5, phState);
  Blynk.virtualWrite(V6, warningFlag ? 1 : 0);
  Blynk.virtualWrite(V7, gasDetected ? 1 : 0);
  Blynk.virtualWrite(V8, rotten ? 1 : 0);
  Blynk.virtualWrite(V9, safeFlag ? 1 : 0);
  Blynk.virtualWrite(V10, status);

  // debug
  Serial.print("LDR: "); Serial.print(ldrRaw); Serial.print(" (");
  Serial.print(ldrPct); Serial.print("%)  T: "); Serial.print(tempC);
  Serial.print("C  H: "); Serial.print(hum);
  Serial.print("%  MQ: "); Serial.print(mqState);
  Serial.print("  pH: "); Serial.print(phState);
  Serial.print("  -> "); Serial.println(status);
}

void setup() {
  Serial.begin(115200);
  pinMode(MQ_DO_PIN, INPUT);
  pinMode(PH_DO_PIN, INPUT);

  lcd.begin(16,2);
  lcd.print("Booting...");

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, readAndDecide);
}

void loop() {
  Blynk.run();
  timer.run();
}
