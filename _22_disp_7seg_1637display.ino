#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)

#define CLK_1637  18    // clk
#define DIO_1637  5     // dio

TM1637Display disp_7seg(CLK_1637, DIO_1637);

void dispOFF(){
  disp_7seg.clear();
}
void setup_disp(){
  disp_7seg.setBrightness(0x0f);
  disp_7seg.clear();
}
void showScore(){
    disp_7seg.showNumberDec(score, false);
    delay(10);
}
// void loop_disp(int tt) {
//   disp_7seg.showNumberDecEx(tt, (0x80 >> 1), true);
//   delay(500);
//   disp_7seg.showNumberDecEx(tt, (0x80 >> 0), true);
//   delay(500);
// }

