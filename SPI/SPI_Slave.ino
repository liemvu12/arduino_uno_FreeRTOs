#include <SPI.h> 

int spi_receiver;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE); //Bật SPI (SPI Enable) bằng cách set bit SPE trong thanh ghi SPCR (SPI Control Register).
  SPI.attachInterrupt();
}

ISR(SPI_STC_vect){
  spi_receiver = SPDR;  //Lấy dữ liệu nhận được từ thanh ghi SPDR (SPI Data Register) và lưu vào biến spi_receiver.
  SPDR = 11; // Gán giá trị 255 cho thanh ghi SPDR, điều này có thể dùng để gửi dữ liệu từ Master đến Slave trong quá trình truyền nhận SPI.
}
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(spi_receiver);
}
