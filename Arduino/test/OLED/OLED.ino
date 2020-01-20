#include <Wire.h> //I2C
#include <U8g2lib.h>
String oled_line1;
String oled_line2;
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  oled_line1 = "TEST";
  oled_line2 = "TEST2";
  printScreen();
}

void printScreen() {
  //https://github.com/olikraus/u8g2/wiki/fntlist8x8
  u8g2.setFont(u8g2_font_7x14_mr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 14);
    u8g2.println(oled_line1);  // write something to the internal memory
    u8g2.setCursor(0, 30);
    u8g2.println(oled_line2);  // write something to the internal memory
  }
  while ( u8g2.nextPage() );
}

void loop() {
  // put your main code here, to run repeatedly:

}
