#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

const byte ledPin = 13;
const byte interruptPin = 2;

SemaphoreHandle_t interruptSemaphore;

void LedTask(void *pvParameters) {
  pinMode(ledPin, OUTPUT);
  while (1) {
    xSemaphoreTake(interruptSemaphore, portMAX_DELAY); // Chờ tín hiệu từ ngắt ngoại vi
    digitalWrite(ledPin, !digitalRead(ledPin)); // Đảo trạng thái của LED
  }
}

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptHandler, CHANGE);

  interruptSemaphore = xSemaphoreCreateBinary();

  xTaskCreate(LedTask, "LedTask", 100, NULL, 1, NULL);
}

void loop() {
  // Không cần thực hiện gì trong hàm loop vì công việc được thực hiện trong các nhiệm vụ.
}

void interruptHandler() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(interruptSemaphore, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }
}
