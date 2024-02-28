#include <SPI.h> 
#include <pins_arduino.h>

const int button = A0;
int spi_receiver;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  pinMode(button, INPUT_PULLUP);
  digitalWrite(SS, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(button) == 0){
    digitalWrite(SS, LOW);
    spi_receiver = SPI.transfer(125);
    digitalWrite(SS, HIGH);
    Serial.println(spi_receiver); // hiển thị giá trị nhập từ Slave
  }
}
