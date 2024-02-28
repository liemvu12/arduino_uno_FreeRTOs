unsigned char val;

void setup() {
  // put your etup code here, to run once:
  Serial.Begin(9600)
}

void loop() {
  // put your main code here, to run repeatedly:
  val = 123;
  Serial.print('A'); // Master gửi 0x41
  Serial.print(val); // Master gửi lần lượt các giá trị vì val kiểu char

  delay(500);

  val = 345;
  Serial.print('B');
  Serial.print(val);
  
  delay(500);

}
// Bản tin 1: 'A' (Ký tự 'A' được gửi từ Master)
// Bản tin 2: '1' (Ký tự '1' được gửi từ Master, nếu val có giá trị là 123)
// Bản tin 3: '2' (Ký tự '2' được gửi từ Master, nếu val có giá trị là 123)
// Bản tin 4: '3' (Ký tự '3' được gửi từ Master, nếu val có giá trị là 123)
// Bản tin 5: 'B' (Ký tự 'B' được gửi từ Master)
// Bản tin 6: '3' (Ký tự '3' được gửi từ Master, nếu val có giá trị là 345)
// Bản tin 7: '4' (Ký tự '4' được gửi từ Master, nếu val có giá trị là 345)
// Bản tin 8: '5' (Ký tự '5' được gửi từ Master, nếu val có giá trị là 345)