#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
byte data[4]; // mảng chứa dữ liệu nhận về
unsigned char val = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >4 ) {
    data[0] = Serial.read();
    if(data[0] = 'B'){
      data[1] = Serial.read() -48;
      data[2] = Serial.read() -48;
      data[3] = Serial.read() -48;
      val = (data[1]*100)+(data[2]*10) +data[3];
      lcd.setCursor(0,0);
      lcd.print(val);
    }
  }
}
