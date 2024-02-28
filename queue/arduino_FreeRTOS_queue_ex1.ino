#include <Arduino_FreeRTOS.h>
#include <queue.h>
//Hai chân 8,9 có hai đèn led, ban đầu đèn số 8 bật, đèn số 9 tắt.
//Khởi tạo 2 task, một task gửi dữ liệu và một task nhận dữ liệu
//Task gửi:
//Tạo ra một giá trị mỗi giây và thêm giá trị này vào cuối hàng đợi
//Task nhận:
//Đọc một giá trị từ đầu hàng đợi mỗi 500 ms
//Nếu không có giá trị, đợi trong 1 giây
//Nếu giá trị nhận được:
//Lớn hơn hoặc bằng 4 và nhỏ hơn hoặc bằng 10:
//Đèn số 8 tắt
//Đèn số 9 bật
//Nhỏ hơn 4 và lớn hơn 10:
//Đèn số 8 bật

// Define LED pins
#define LED1 8
#define LED2 9

// Task handles
TaskHandle_t SenderTaskHandle, ReceiverTaskHandle;

// Queue handle
QueueHandle_t DataQueue;

// Task functions
void SenderTask(void *pvParameters);
void ReceiverTask(void *pvParameters);

void setup() {
  // Initialize Serial port
  Serial.begin(9600);

  // Set LED pins as output
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Create Data Queue
  DataQueue = xQueueCreate(5, sizeof(int));

  // Create Sender Task
  xTaskCreate(SenderTask, "SenderTask", 128, NULL, 1, &SenderTaskHandle);

  // Create Receiver Task
  xTaskCreate(ReceiverTask, "ReceiverTask", 128, NULL, 1, &ReceiverTaskHandle);

  // Start scheduler
  vTaskStartScheduler();
}

void loop() {
  // This loop should never be reached
}

// Sender Task: Generate data and send to the queue
void SenderTask(void *pvParameters) {
  (void) pvParameters;
  int data = 0;
  for (;;) {
    data++;
    xQueueSend(DataQueue, &data, portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Receiver Task: Receive data from the queue and control LEDs accordingly
void ReceiverTask(void *pvParameters) {
  (void) pvParameters;
  int receivedData;
  for (;;) {
    if (xQueueReceive(DataQueue, &receivedData, 500 / portTICK_PERIOD_MS)) {
      if (receivedData >= 4 && receivedData <= 10) {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
      } else {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
      }
    } else {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}
