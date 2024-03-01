#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
LiquidCrystal lcd(9, 8, 7, 5, 6, 4);
int Body_Temp = 0 ;
int Body_Temp1 ;
int Body_Temp_Value;
char junk;
String inputString = "";
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup()
{
  Serial.begin(115200);
  //Merial.begin(9600);
  lcd.begin(16, 2);
}
void loop()
{
  if (Serial.available() > 0) {
    int a = Serial.read();
    if (a == '1') {
      Body_Temp1 = analogRead(Body_Temp);
      Body_Temp_Value = (float) Body_Temp1 * (5 / 10.24);
      lcd.setCursor(0, 0);
      lcd.print("Body Temp : ");
      lcd.print(Body_Temp_Value); 
      delay(3000);
      lcd.clear();    
    }
    else if (a == '2') {
      pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
      pox.setOnBeatDetectedCallback(onBeatDetected);
      pox.update();
      if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("BPM : ");
        lcd.print(pox.getHeartRate());
        lcd.setCursor(0, 1);
        lcd.print("SpO2: ");
        lcd.print(pox.getSpO2());
        lcd.print("%");
        tsLastReport = millis();
      }
    }
    else {
      lcd.setCursor (0, 1);
      for (int i = 0; i < 16; ++i)
      {
        lcd.write(' ');
      }
      lcd.setCursor (0, 0);
      lcd.print("Wrong Attemp ");
      delay(3000);
      lcd.clear();
    }
  }
}
