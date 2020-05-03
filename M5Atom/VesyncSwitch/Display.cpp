#include "Display.h"

#include "FastLEDConfig.h"
#include <M5Atom.h>

uint8_t dispBuf[2 + 5 * 5 * 3];

static void fillDisplay(uint8_t r, uint8_t g, uint8_t b)
{
  dispBuf[0] = 0x05;
  dispBuf[1] = 0x05;
  for (int i = 0; i < 25; i++)
  {
    dispBuf[2 + i * 3 + 0] = r;
    dispBuf[2 + i * 3 + 1] = g;
    dispBuf[2 + i * 3 + 2] = b;
  }
  M5.dis.displaybuff(dispBuf);
}

static void stopDisplay()
{
  M5.dis.clear();
  delay(500);
  // M5.dis.stop();
  // delay(100);
}

void displayInit() {
  fillDisplay(0x00, 0x00, 0x40);
}

void displayError() {
  fillDisplay(0x40, 0x40, 0x00);
}

void displaySwitch(bool state) {
  if (state) {
    fillDisplay(0x00, 0x40, 0x00);
  } else {
    fillDisplay(0x40, 0x00, 0x00);
  }
}

void displayOff() {
  fillDisplay(0x00, 0x00, 0x00);
}
