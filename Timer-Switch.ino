#include "Wire.h"
#include <RCSwitch.h>
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

RCSwitch mySwitch = RCSwitch();

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  mySwitch.enableTransmit(10);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(30,42,21,4,26,11,14);
}

void loop()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  if(hour == 19 && minute == 0 && second == 0){
    turnOnA();  // turn on lights A at 19:00. Mention seconds here to do only once
  }

  if(hour == 19 && minute == 30 && second == 0){
    turnOnB(); //turn on lights B at 19:30. Mention seconds here to do only once
  } 

  if(hour == 23 && minute == 30 && second == 0) {
    turnOffA(); //turn all lights off at 23:30. Mention seconds here to do only once
    turnOffB();
  }
  
  delay(1000); // check every second
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void turnOnA() 
{
  mySwitch.send(16762196, 24);
  mySwitch.send(16762196, 24);
  mySwitch.send(16762196, 24);
  mySwitch.send(16762196, 24);
  mySwitch.send(16762196, 24);  
}

void turnOffA()
{
  mySwitch.send(16762193, 24);
  mySwitch.send(16762193, 24);
  mySwitch.send(16762193, 24);
  mySwitch.send(16762193, 24);
  mySwitch.send(16762193, 24);
}

void turnOnB() 
{
  mySwitch.send(16765268, 24);
  mySwitch.send(16765268, 24);
  mySwitch.send(16765268, 24);
  mySwitch.send(16765268, 24);
  mySwitch.send(16765268, 24);  
}

void turnOffB()
{
  mySwitch.send(16765264, 24);
  mySwitch.send(16765264, 24);
  mySwitch.send(16765264, 24);
  mySwitch.send(16765264, 24);
  mySwitch.send(16765264, 24);
}
