#include <Si4703_Breakout.h>
#include <Wire.h>
#include <EEPROM.h>

int resetPin  = 2;
int SDIO      = A4;
int SCLK      = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);
int channel = 0;
int volume = 1021;

int pinSeekUp       = 6;
int pinSeekDown     = 7;
int pinVolumeUp     = 8;
int pinVolumeDown   = 9;

int addrChannel = 0+1;
int addrVolume = sizeof(int)+1;

void setup()
{
  Serial.begin(9600);
  
  pinMode(pinSeekUp,      INPUT_PULLUP);
  pinMode(pinSeekDown,    INPUT_PULLUP);
  pinMode(pinVolumeUp,    INPUT_PULLUP);
  pinMode(pinVolumeDown,  INPUT_PULLUP);

  byte isNeverLaunched;
  isNeverLaunched = EEPROM.read(0);
  if(isNeverLaunched == 255) {
    EEPROM.write(0, 0);
  } else {
    channel = EEPROM_int_read(addrChannel);
    volume = EEPROM_int_read(addrVolume);
  }
  
  Serial.print("Volume:"); Serial.println(volume);
  Serial.print("Channel:"); Serial.println(channel);
  
  radio.powerOn();
  radio.setChannel(channel);
  radio.setVolume(volume);
}

void loop()
{
  if(!digitalRead(pinSeekUp)) {
    setNewChannel(radio.seekUp());
  } else if(!digitalRead(pinSeekDown)) {
    setNewChannel(radio.seekDown());
  } else if(!digitalRead(pinVolumeUp)) {
    increaseVolume();
  } else if(!digitalRead(pinVolumeDown)) {
    decreaseVolume();
  }
}

int EEPROM_int_read(int addr) {
  byte raw[2];
  for(byte i = 0; i < 2; i++)
    raw[i] = EEPROM.read(addr+i);
  int &num = (int&)raw;
  return num;
}
void EEPROM_int_write(int addr, int num) {
  byte raw[2];
  (int&)raw = num;
  for(byte i = 0; i < 2; i++)
    EEPROM.write(addr+i, raw[i]);
}

void increaseVolume() {
  volume++;
  if (volume == 16)
    volume = 15;
  radio.setVolume(volume);
  EEPROM_int_write(addrVolume, volume);
  Serial.print("Volume:"); Serial.println(volume);

  delay(300);
}

void decreaseVolume() {
  volume--;
  if (volume < 0)
    volume = 0;
  radio.setVolume(volume);
  EEPROM_int_write(addrVolume, volume);
  Serial.print("Volume:"); Serial.println(volume);

  delay(300);
}

void setNewChannel(int ch) {
  channel = ch;
  EEPROM_int_write(addrChannel, channel);
  Serial.print("Channel:"); Serial.println(channel);
  
  delay(300);
}

