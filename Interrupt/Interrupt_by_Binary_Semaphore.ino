#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Sử dụng FreeRTOS trên Arduino để đồng bộ hóa giữa các nhiệm vụ và ngắt, và điều khiển một LED để nhấp nháy với tần số 1Hz.

SemaphoreHandle_t semaphore;
TaskHandle_t toggleTaskHandle;
const int ledPin = 13; // Pin của LED trên Arduino

// Task 1: Chờ tín hiệu từ ngắt trước khi tiếp tục thực thi
void Task1(void *pvParameters) {
  while(1) {
    if (xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE) {
      // Thực hiện công việc cần thực hiện sau khi nhận tín hiệu từ ngắt
      digitalWrite(ledPin, HIGH); // Bật LED
      vTaskDelay(pdMS_TO_TICKS(500)); // Chờ 500ms
      digitalWrite(ledPin, LOW); // Tắt LED
    }
  }
}

// Task 2: Ngắt, phát tín hiệu cho Task1 khi có sự kiện xảy ra
void Interrpt_Sevire() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(semaphore, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }
}

void setup() {
  // Khởi tạo Semaphore
  semaphore = xSemaphoreCreateBinary();

  // Khởi tạo Task1
  xTaskCreate(Task1, "Task1", 100, NULL, 1, NULL);

  // Thiết lập ngắt và liên kết với ISR
  attachInterrupt(0, Interrpt_Sevire, RISING);
}

void loop() {
  // Không có gì để làm ở hàm loop vì công việc đã được thực hiện trong các nhiệm vụ.
}
