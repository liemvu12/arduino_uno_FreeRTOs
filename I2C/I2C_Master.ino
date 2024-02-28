#include <Wire.h>

byte data[10]; // mảng nhận dữ liệu từ slave gửi về
int i;

void setup() {
  Serial.begin(9600);

  Wire.begin(); // Khởi tạo I2C với chế độ master
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(A0) == 0) { // nhấn nút
    Wire.beginTransmission(1); // truyền dữ liệu với địa chỉ là 1 của slave
    Wire.write('b'); // dữ liệu gửi đi
    Wire.endTransmission(); // kết thúc quá trình truyền dữ liệu
  } else {
    Wire.beginTransmission(1);
    Wire.write('a');
    Wire.endTransmission();
  }

  Wire.requestFrom(1, 6); // yêu cầu slave phản hồi về cho bên master với slave địa chỉ 1, num bytes = 6
  if (Wire.available() > 0) {
    for (i = 0; i < 6; i++) {
      data[i] = Wire.read(); // đọc dữ liệu từ slave
      Serial.println(data[i] - 48);
    }
    delay(500);
  }

  // xử lý dữ liệu nhận được từ slave
}