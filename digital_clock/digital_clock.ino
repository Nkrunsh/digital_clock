#include <Adafruit_NeoPixel.h>
#include <DS3231.h>
#include <Wire.h>
#include <string.h>
#include <EEPROM.h>

#define CLINT 2       //Interrup PIN
#define HOUR_PIN 3    // Hour data LEDS
#define MINUTE_PIN 6  // Minute data LEDS

#define NUMPIXELS 15  // Number of LEDs per side

#define alarmBits 0b00001110  // Alarm settings per minute
#define H12MODE false         // 24h settings
#define alarmDayIsDay false   // Alarm settings
#define alarmPM false         // PM Alarm settings
int addr = 0;

// Digital sequences for 7-segment display
#define ZERO "1110111"
#define ONE "0010001"
#define TWO "1101011"
#define THREE "0111011"
#define FOUR "0011101"
#define FIVE "0111110"
#define SIX "1111110"
#define SEVEN "0010011"
#define EIGTH "1111111"
#define NINE "0111111"
#define NONE "0000000"


byte lightBrightness = 75;  // Brightness intensity
uint32_t lightColor = 0x009696;

DS3231 RTC;
RTClib RTC_CLOCK;
DateTime now;

Adafruit_NeoPixel hour_pixels(NUMPIXELS, HOUR_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minute_pixels(NUMPIXELS, MINUTE_PIN, NEO_GRB + NEO_KHZ800);

volatile byte tick = 1;  // Interruption handling variable

void setup() {

  //**** PIXEL CONFIGURATION ****
  hour_pixels.begin();
  hour_pixels.setBrightness(lightBrightness);

  minute_pixels.begin();
  minute_pixels.setBrightness(lightBrightness);

  //**** RTC CONFIGURATION ****
  Wire.begin();
  RTC.setClockMode(H12MODE);
  /*  // SOLO PARA CARGA FINAL
  if (EEPROM.read(addr) != 1) {
    DateTime setCompileTime(__DATE__, __TIME__);
    RTC.setYear(setCompileTime.year());
    RTC.setMonth(setCompileTime.month());
    RTC.setDate(setCompileTime.day());
    RTC.setHour(setCompileTime.hour());
    RTC.setMinute(setCompileTime.minute());
    RTC.setSecond(setCompileTime.second());

    EEPROM.write(addr, 1);
  }
 */
  DateTime setCompileTime(__DATE__, __TIME__);
  RTC.setYear(setCompileTime.year());
  RTC.setMonth(setCompileTime.month());
  RTC.setDate(setCompileTime.day());
  RTC.setHour(setCompileTime.hour());
  RTC.setMinute(setCompileTime.minute());
  RTC.setSecond(setCompileTime.second());

  RTC.turnOffAlarm(1);
  RTC.setA1Time(0x00, 0x00, 0x00, 0x00, alarmBits, alarmDayIsDay, H12MODE, alarmPM);
  RTC.turnOnAlarm(1);
  RTC.checkIfAlarm(1);

  RTC.setA2Time(0x00, 0x00, 0xFF, 0b01100000, alarmDayIsDay, H12MODE, alarmPM);
  RTC.turnOffAlarm(2);
  RTC.checkIfAlarm(2);

  //**** INTERRUP CONFIGURATION ****
  pinMode(CLINT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLINT), isr_clockUpdate, FALLING);

  //Serial.begin(9600);  // DEBUG LINE
  minute_pixels.clear();
updateClockDot();
}

void loop() {


  if (tick) {
    now = RTC_CLOCK.now();
    setHourDigit(getNumberSequence(now.hour()));
    setMinuteDigit(getNumberSequence(now.minute()));
    showLed();


    tick = 0;
    RTC.checkIfAlarm(1);
  }
}

void isr_clockUpdate() {
  tick = 1;
}

/*
  encodeNumber - receives a character (char) as input, which represents a numeric digit 
  (between '0' and '9'), and returns a string (String) that represents the code for that 
  digit on a 7-segment display.
*/
String encodeNumber(char digit) {
  String encode;
  if (digit == '0') {
    encode = ZERO;
  } else if (digit == '1') {
    encode = ONE;
  } else if (digit == '2') {
    encode = TWO;
  } else if (digit == '3') {
    encode = THREE;
  } else if (digit == '4') {
    encode = FOUR;
  } else if (digit == '5') {
    encode = FIVE;
  } else if (digit == '6') {
    encode = SIX;
  } else if (digit == '7') {
    encode = SEVEN;
  } else if (digit == '8') {
    encode = EIGTH;
  } else if (digit == '9') {
    encode = NINE;
  } else {
    encode = NONE;
  }

  return encode;
}

/*
    getNumberSequence receives a number (byte) from 0-99 as input and returns a string (String)
    that represents the sequence of lighting up the digits on a 7-segment display for that number.
*/
String getNumberSequence(byte number) {
  char descomposed[3];
  sprintf(descomposed, "%2d", number);

  String encode;
  char encodeAux[8];
  char strAux[14];

  encode = encodeNumber(descomposed[0]);
  encode.toCharArray(encodeAux, 8);
  strcpy(strAux, encodeAux);

  encode = encodeNumber(descomposed[1]);
  encode.toCharArray(encodeAux, 8);
  strcat(strAux, encodeAux);
  return String(strAux);
}

/* setHourDigit takes the sequence string as input and lights up the corresponding LEDs 
  on a numeric panel to display the time. The code loops through the characters of the 
  sequence string and lights up the corresponding LEDs using the hour_pixels library.
*/
void setHourDigit(String num) {
  for (int i = 13; i >= 0; i--) {
    if (num[13 - i] == '0') {
      hour_pixels.setPixelColor(i + 1, 0x000000);
    } else {
      hour_pixels.setPixelColor(i + 1, lightColor);
    }
  }
}

/* setMinuteDigit takes the sequence string as input and lights up the corresponding LEDs 
  on a numeric panel to display the time. The code loops through the characters of the 
  sequence string and lights up the corresponding LEDs using the minute_pixels library.
*/
void setMinuteDigit(String num) {
  for (int i = 0; i < 14; i++) {
    Serial.println(num[i]);
    if (num[i] == '0') {
      minute_pixels.setPixelColor(i + 1, 0x000000);
    } else {
      minute_pixels.setPixelColor(i + 1, lightColor);
    }
  }
}

void showLed() {
  hour_pixels.show();
  minute_pixels.show();
}

void updateClockDot(){
  hour_pixels.setPixelColor(0, lightColor);
  minute_pixels.setPixelColor(0, lightColor);
  showLed();
}

void setClockColor(uint32_t color_l) {
  lightColor = color_l;
}

void setClocksetBrightness(uint8_t Brightness) {
  hour_pixels.setBrightness(Brightness);
  minute_pixels.setBrightness(Brightness);
  showLed();
}
