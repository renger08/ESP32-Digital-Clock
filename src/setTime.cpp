/*
You have to run this Code just once to get time from NTP server and update RTC Module 
*/
#include <DS1307.h> // RealTimeClock Library
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h> 
// Wifi information
const char* ssid = "WIFI_SSID";     // WIFI Name
const char* password = "wifi_password";    // WIFI Password
// Time Offset: Tehran GMT +03:30
const long utcOffsetInSeconds = 12600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; // Weekdays
// NTPClient Init
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
// RTC Init
DS1307 rtc;
// SETUP
void setup(void){
  Serial.begin(115200);       /*Set the baudrate to 115200*/
  WiFi.begin(ssid, password); // Connecting to WIFI
  Serial.println("intializing WIFI...");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." ); // Waiting...
  }
  Serial.println(" ");
  Serial.println("timeClient begin...");
  timeClient.begin();
  delay(5000);
  Serial.println("timeClient updating...");
  timeClient.update();

  Serial.println("Getting time...");
  Serial.print(timeClient.getDay());
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  Serial.println("\n\nWaiting 10s..."); 

  delay(10000);
  Serial.println("RTC begin..."); 
  rtc.begin(); // Starting RTC Module
  delay(500);
  /*03:47:00 13.05.2022 //sec, min, hour, day, month, year*/
  /*HH:MM:SS DD.MM.YYYY - Time Format*/
  Serial.println("timeClient updating...");
  timeClient.update();// Important: you have to update timeClient again!
  rtc.set(
  timeClient.getSeconds(), // Secounds
  timeClient.getMinutes(), // Minutes
  timeClient.getHours(), // Hours
  4, 8, 2025 // Please Set Days, Months and Years Manually (Because of Epoch time and converting it to human read)
//D, M,  Y
);
/* 
Epoch Time started since January 1, 1970 midnight GMT
NTPClient Documentation:
getEpochTime() – returns an unsigned long with the epoch time (number of seconds that have elapsed since January 1, 1970 (midnight GMT)
*/
  rtc.stop();                 /*stop/pause RTC*/
  // Restarting TRC Module
  rtc.start();                /*start RTC*/
  delay(1000);                /*Wait for 1000mS*/
  Serial.print("You have set: "); // reading time from NTP server
  Serial.print("\nTime: ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  // You can read time directly from RTC module with -> rtc.get();
  Serial.print("\nDate: ");
  Serial.print(4);
  Serial.print(".");
  Serial.print(8);
  Serial.print(".");
  Serial.print(2025);
  Serial.println("");
}

void loop(void){/*No code required for loop function*/}
