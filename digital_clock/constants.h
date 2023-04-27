#define CLINT 2       //Interrup PIN
#define HOUR_PIN 3    // Hour data LEDS
#define MINUTE_PIN 6  // Minute data LEDS

#define NUMPIXELS 15  // Number of LEDs per side

#define alarmBits 0b00001110  // Alarm settings per minute
#define H12MODE false         // 24h settings
#define alarmDayIsDay false   // Alarm settings
#define alarmPM false         // PM Alarm settings

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

#define OFF_LIGHT 0x000000

#define BAUD_RATE 9600

#define COLOR_COMMAND "C"
#define TCOLOR_COMMAND "T"
#define BRIGHTNESS_COMMAND "B"
#define SYN_HOURE_COMMAND "H"


