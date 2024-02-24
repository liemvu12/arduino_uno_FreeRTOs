#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Viết một chương trình sử dụng Arduino và FreeRTOS để quản lý việc nhận dữ liệu từ cổng UART và xử lý ngắt từ một chân ngoại vi.
// Chương trình cũng điều khiển một LED để nhấp nháy theo các sự kiện xảy ra.

SemaphoreHandle_t uartSemaphore;
SemaphoreHandle_t externalInterruptSemaphore;
TaskHandle_t ledTaskHandle;

// Task: Đợi tín hiệu từ Semaphore và xử lý dữ liệu nhận được từ UART
void UartTask(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(uartSemaphore, portMAX_DELAY) == pdTRUE) {
      // Xử lý dữ liệu nhận được từ UART ở đây
      while (Serial.available() > 0) {
        char receivedChar = Serial.read(); // Đọc byte dữ liệu từ UART
        Serial.print("Received character: ");
        Serial.println(receivedChar);
      }
      // Gửi tín hiệu cho LedTask để bật LED
      xTaskNotifyGive(ledTaskHandle);
    }
  }
}

// Task: Điều khiển LED sáng và tắt
void LedTask(void *pvParameters) {
  const int ledPin = 13; // Pin của LED trên Arduino
  pinMode(ledPin, OUTPUT);
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Chờ tín hiệu từ UartTask hoặc externalInterruptTask
    digitalWrite(ledPin, HIGH); // Bật LED
    vTaskDelay(pdMS_TO_TICKS(500)); // Chờ 500ms
    digitalWrite(ledPin, LOW); // Tắt LED
  }
}

// Ngắt UART, phát tín hiệu Semaphore khi có dữ liệu được nhận
void serialEvent() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(uartSemaphore, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }
}

// Ngắt từ pin 2
void externalInterruptTask() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(externalInterruptSemaphore, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }
}

void setup() {
  Serial.begin(9600); // khởi tạo ngắt UART
  while (!Serial) {
    ; // Chờ khi cổng Serial không sẵn sàng
  }

  // Khởi tạo Semaphore
  uartSemaphore = xSemaphoreCreateBinary();
  externalInterruptSemaphore = xSemaphoreCreateBinary();

  // Tạo Task cho UART
  xTaskCreate(UartTask, "UartTask", 100, NULL, 1, NULL);

  // Tạo Task cho LED
  xTaskCreate(LedTask, "LedTask", 100, NULL, 1, &ledTaskHandle);

  // Cài đặt ngắt từ pin 2
  attachInterrupt(digitalPinToInterrupt(2), externalInterruptTask, LOW);
}

void loop() {
  // Không cần thực hiện gì trong hàm loop vì công việc được thực hiện trong các nhiệm vụ.
}
