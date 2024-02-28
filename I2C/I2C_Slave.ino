#include <Wire.h>

#define LED_PIN 13
int x, i;

char data[] = {'0', '1', '2', '3', '4', '5'}; // mảng để gửi dữ liệu cho master

void setup() {
  Serial.begin(9600);
  Wire.begin(1); // Khởi tạo I2C với chế độ slave với 1 là giá trị địa chỉ
  Wire.onReceive(receiveEvent); // Khởi tạo hàm nhận dữ liệu
  Wire.onRequest(requestEvent); // Khởi tạo hàm phản hồi tức là khi master gửi xuống slave thì
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if (x == 'a') {
    digitalWrite(LED_PIN, HIGH);
  } else if (x == 'b') {
    digitalWrite(LED_PIN, LOW);
  }
}

void receiveEvent(int howMany) { // hàm ngắt nhận
  x = Wire.read(); // sau khi đọc xong thì kiểm tra nếu bằng a thì led bật
}

void requestEvent() { // hàm ngắt phản hồi
  Wire.write(data); // gửi dữ liệu thứ i trong mảng data cho master
}
