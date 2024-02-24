#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>

const byte ledPin1 = 11; // Pin của LED 1
const byte ledPin2 = 12; // Pin của LED 2

QueueHandle_t interruptQueue; // Hàng đợi để nhận các giá trị từ ngắt

void LedTask(void *pvParameters) {
  int interruptValue;
  while (1) {
    if (xQueueReceive(interruptQueue, &interruptValue, portMAX_DELAY) == pdTRUE) {
      // Nhận giá trị từ hàng đợi và thực hiện các hành động điều khiển LED tương ứng
      digitalWrite(interruptValue, !digitalRead(interruptValue));
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

void interruptHandler1() {
  xQueueSendFromISR(interruptQueue, &ledPin1, NULL); // Gửi giá trị tương ứng vào hàng đợi từ ngắt
}

void interruptHandler2() {
  xQueueSendFromISR(interruptQueue, &ledPin2, NULL); // Gửi giá trị tương ứng vào hàng đợi từ ngắt
}

void setup() {
  pinMode(ledPin1, OUTPUT); // Khởi tạo LED 1
  pinMode(ledPin2, OUTPUT); // Khởi tạo LED 2

  attachInterrupt(digitalPinToInterrupt(2), interruptHandler1, CHANGE); // Ngắt từ digital pin 2
  attachInterrupt(digitalPinToInterrupt(3), interruptHandler2, CHANGE); // Ngắt từ digital pin 3

  interruptQueue = xQueueCreate(3, sizeof(int)); // Khởi tạo hàng đợi với 3 phần tử, mỗi phần tử có kích thước là int

  // Tạo Task cho mỗi LED
  xTaskCreate(LedTask, "LedTask", 100, NULL, 1, NULL);
}

void loop() {
  // Không cần thực hiện gì trong hàm loop vì công việc được thực hiện trong các nhiệm vụ.
}
