#include <TM1637Display.h>
#include <DS1307.h> // 7-segment Display Module
#include <Arduino.h>
// Display Init:
TM1637Display display(17, 16);
uint8_t sec, minute, hour, day, month;
uint16_t year;
// RTC Init:
DS1307 rtc;

struct Time {
  int hour;
  int minute;
};

Time getTime(){
  rtc.get(&sec, &minute, &hour, &day, &month, &year);
  Time t;
  t.hour = hour;
  t.minute = minute;
  return t;
}

// متغیرهای وضعیت برای کنترل به‌روزرسانی
Time lastDisplayedTime = {-1, -1};
bool dotsVisible = true;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 1000; // 1 ثانیه

void setup(void){
  Serial.begin(115200); /*Set the baudrate to 115200*/
  rtc.begin(); // Starting RTC Module
  rtc.start();
  delay(1000);
  display.setBrightness(3); // set display brightness
}

void loop(){
  Time currentTime = getTime(); // get current time from getTime() function based on Time Structure
  unsigned long now = millis();

  // اگر زمان (ساعت یا دقیقه) تغییر کرد => به‌روزرسانی نمایشگر
  if (currentTime.hour != lastDisplayedTime.hour || currentTime.minute != lastDisplayedTime.minute) {
    display.showNumberDecEx(
      currentTime.hour * 100 + currentTime.minute,
      dotsVisible ? 0b01000000 : 0, // دو نقطه بین ساعت و دقیقه
      true, 4, 0
    );
    lastDisplayedTime = currentTime;
  }

  // هر 1 ثانیه یک بار، وضعیت دو نقطه وسط رو تغییر بده
  if (now - lastBlinkTime >= blinkInterval) {
    dotsVisible = !dotsVisible; // چشمک زدن دو نقطه وسط
    display.showNumberDecEx(
      currentTime.hour * 100 + currentTime.minute,
      dotsVisible ? 0b01000000 : 0,
      true, 4, 0
    );
    lastBlinkTime = now;
  }
}
