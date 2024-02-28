#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <string.h> 
//Viết chương trình thực hiện tạo hai tác vụ có tên "Task1", "Task2" sao cho:

//Cả 2 tác vụ có:
//Stack 250 từ máy (word), mức ưu tiên (tskIDLE_PRIORITY + 1), Handle lần lượt là HTask1, HTask2
//Task 1 thực hiện:
//Tạo hàng đợi có tên “Queue" có 6 phần tử kiểu chuỗi 20 ký tự
//Gửi lần lượt 3 chuỗi ký tự ("CT03-Hello"; "DT02-Hi"; "AT-Welcome") vào đầu hàng đợi
//Task2 thực hiện:
//Đọc từng phần tử từ hàng đợi và lần lượt in ra màn hình sau mỗi lần đọc:
//Giá trị của phần tử
//Số phần tử có thể đọc ra từ hàng đợi
//Không gian khả dụng còn lại của hàng đợi

// Khai báo handles cho tasks
// Khai báo handles cho tasks
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;

// Khai báo Handle cho queue
QueueHandle_t xQueue;

// Prototype cho các tasks
void Task1(void *pvParameters);
void Task2(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // Tạo các tasks
  xTaskCreate(Task1, "Task1", 250, NULL, tskIDLE_PRIORITY + 1, &Task1_Handle);
  xTaskCreate(Task2, "Task2", 250, NULL, tskIDLE_PRIORITY + 1, &Task2_Handle);

  // Khởi tạo FreeRTOS scheduler
  vTaskStartScheduler();
}

void loop() {
  // Không có gì cần thực hiện trong loop() khi sử dụng FreeRTOS
}

// Task 1
void Task1(void *pvParameters) {
  (void)pvParameters;

  // Tạo queue với kích thước là 6 phần tử kiểu mảng char[20]
  xQueue = xQueueCreate(6, sizeof(char[20]));

  if (xQueue == NULL) {
    Serial.println("khong the tao hang doi");
    vTaskDelete(NULL); // Xóa task hiện tại
  }
  else {
    Serial.println("Tao hang doi thanh cong");
  }

  // Mảng chứa dữ liệu để gửi vào hàng đợi
  const char *data[3] = {"CT03-Hello", "DT02-Hi", "AT-Welcome"};

  // Gửi lần lượt 3 chuỗi vào hàng đợi
  for (int i = 0; i < 3; i++) {
    char receivedData[20];
    strcpy(receivedData, data[i]); // Sao chép chuỗi từ data[i] vào receivedData
    xQueueSend(xQueue, (void *)receivedData, portMAX_DELAY);
    Serial.print("Da gui: ");
    Serial.println(receivedData);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
  }

  // Xóa task 1 sau khi gửi xong
  vTaskDelete(NULL);
}

// Task 2
void Task2(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    // Đọc từng phần tử từ hàng đợi và in ra thông tin
    char receivedData[20];
    if (xQueueReceive(xQueue, (void *)receivedData, portMAX_DELAY) == pdPASS) {
      Serial.print("Gia tri cua phan tu: ");
      Serial.println(receivedData);

      // Số phần tử có thể đọc ra từ hàng đợi
      unsigned uxQueueSpaces = uxQueueSpacesAvailable(xQueue);
      Serial.print("So phan tu co the doc tu hang doi: ");
      Serial.println(uxQueueSpaces);

      // Không gian khả dụng còn lại của hàng đợi
      unsigned uxQueueSpacesWaiting = uxQueueMessagesWaiting(xQueue);
      Serial.print("Khong gian kha dung cua hang doi: ");
      Serial.println(uxQueueSpacesWaiting);
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
  }
}