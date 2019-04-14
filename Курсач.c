#include <Wire.h>
#include <BH1750.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define CURTAIN_PIN 12

BH1750 lightMeter;
RTC_DS1307 RTC;
Servo shtorki;

int nightStart = 0;
int nightEnd = 9;

float lowLum = 4.5;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){

Serial.begin(9600);

shtorki.attach(CURTAIN_PIN);

Wire.begin(D1,D2);

RTC.begin();

lightMeter.begin();

lcd.begin(16,2);
lcd.init();
lcd.backlight();
lcd.setCursor(2,0);
lcd.home();

lcd.print("Wake up , Fedya");
lcd.setCursor(0,1);
lcd.print("You obosralsya..");
delay(5000);

if (! RTC.isrunning()) {
Serial.println("RTC is NOT running!");

RTC.adjust(DateTime(__DATE__, __TIME__));
}
//Serial.println(F("BH1750 Test begin"));

}
bool isNight(int date){

if (date >= nightStart && date <= nightEnd){
Serial.print("Night - ");
Serial.println(date);
Serial.println("------------");
return true;
} else {
Serial.print("Day - ");
Serial.print(date);
Serial.println("------------");
return false;
}

}

void loop() {

DateTime now = RTC.now(); 

int tempH = now.hour();

delay(1000);

float lux = lightMeter.readLightLevel();
Serial.print("Light: ");
Serial.print(lux);
Serial.println(" lx");


lcd.clear();

LcdPrintDate(now , isNight(tempH));
LcdPrintLuminocity(lux);

lcd.setCursor(10,1);
if(lux < lowLum && !isNight(tempH)){
//send open sig
lcd.print("Open");
shtorki.write(180);
} else {
lcd.print("Closed");
shtorki.write(0);
}
// delay(1000);

}

void LcdPrintLuminocity(float lux){
lcd.setCursor(0, 1);
lcd.print(lux);
lcd.setCursor(6, 1);
lcd.print(" LX"); 

}

void LcdPrintDate(DateTime date, bool night){

lcd.setCursor(0, 0);
lcd.print(date.hour(), DEC);
lcd.setCursor(2,0);
lcd.print(':');

lcd.setCursor(3, 0);
lcd.print(date.minute(), DEC);
lcd.setCursor(5,0);
lcd.print(':');

lcd.setCursor(6, 0);
lcd.print(date.second(), DEC);



lcd.setCursor(11,0);
if(night){
lcd.print("Night");
} else {
lcd.print("Day");
}


}