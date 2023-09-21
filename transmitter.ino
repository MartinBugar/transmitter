// 4 Channel Transmitter
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>
const uint64_t pipeOut = 0xE9E8F0F0E1LL;   //IMPORTANT: The same as in the receiver 0xE9E8F0F0E1LL 
RF24 radio(7, 8); // select CE,CSN pin 
LiquidCrystal_I2C lcd(0x27, 16, 4); // I2C address 0x27, 16 column and 2 rows

struct Signal {
  byte rightTracks;
  byte leftTracks;
  int LED;
  };

Signal data;

void ResetData() {
  data.rightTracks = 135; // Center 
  data.leftTracks = 127; // Center 
 }

void setup() {
  //Start everything up
  lcd.init(); // initialize the lcd
  lcd.backlight();

  pinMode (A0, INPUT);
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.stopListening(); //start the radio comunication for Transmitter 
  ResetData();
}

// Joystick center and its borders
int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse) {
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
  }

void loop()
{



data.LED = 0; // LIGHTS OFF
    if (analogRead(A0) < 200) {
      data.LED = 1;
    } else if (analogRead(A0) > 200) {
      data.LED = 0;
    }

data.rightTracks = mapJoystickValues( analogRead(A1), 12, 524, 1020, true );     
data.leftTracks = mapJoystickValues( analogRead(A4), 12, 524, 1020, true );    
radio.write(&data, sizeof(Signal));
}
