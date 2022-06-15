
#include "Keyboard.h"

#define LED 17
#define ROW0 6
#define ROW1 7
#define COL0 19
#define COL1 20

void selectRow(uint8_t row)
{
  switch (row) {
  case 0:
    digitalWrite(ROW0, LOW); // select this line
    digitalWrite(ROW1, HIGH);
    break;
  case 1:
    digitalWrite(ROW0, HIGH);
    digitalWrite(ROW1, LOW); // select this line
    break;
  }
}

uint8_t readBits()
{
  uint8_t bits = 0;
  if (!digitalRead(COL0)) {
    bits |= 0x01;
  }
  if (!digitalRead(COL1)) {
    bits |= 0x02;
  }
  return bits;
}

uint8_t scanKeys()
{
  uint8_t bits;
  for (uint8_t row = 0; row < 2; row++) {
    selectRow(row);
    bits  = (bits  << 2) | readBits();
  }
  return bits;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ROW0, OUTPUT);
  pinMode(ROW1, OUTPUT);
  digitalWrite(ROW0, HIGH);
  digitalWrite(ROW1, HIGH);
  pinMode(COL0, INPUT_PULLUP);
  pinMode(COL1, INPUT_PULLUP);
  Keyboard.begin();
}

void pressKey(uint8_t code, bool pressed)
{
    if (pressed) {
      Keyboard.press(code);
    } else {
      Keyboard.release(code);
    }
}

void led(bool on)
{
  digitalWrite(LED, !on);   // active low
}

void loop()
{
  static uint8_t bits_last = 0;
  uint8_t bits_curr = scanKeys();
  if (bits_curr != bits_last) {
    pressKey('q', bits_curr & 0x08);
    pressKey('w', bits_curr & 0x04);
    pressKey('e', bits_curr & 0x02);
    pressKey('r', bits_curr & 0x01);
    bits_last = bits_curr;
  }
  led(bits_curr != 0);
  delay(10);
}
