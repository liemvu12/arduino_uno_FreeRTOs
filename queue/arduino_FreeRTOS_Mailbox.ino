#include <Arduino_FreeRTOS.h>
#include <queue.h>

QueueHandle_t xMailbox;
TaskHandle_t TaskHandle_1; // handler for Task1
TaskHandle_t TaskHandle_2; // handler for Task2

void vUpdateMailbox(void *Parameters);
void vReadMailbox(void *Parameters);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  xMailbox = xQueueCreate(1, sizeof(int32_t)); 
  xTaskCreate(vUpdateMailbox, "Sender", 1000, NULL, 1, &TaskHandle_1);
  xTaskCreate(vReadMailbox, "Receiver", 1000, NULL, 1, &TaskHandle_2);
}

void vUpdateMailbox(void *Parameters)
{
  int32_t ulNewValue = 1;
  while (1)
  {
    xQueueOverwrite(xMailbox, &ulNewValue);   //ghi đè giá trị ulNewValue vào Queue 
    Serial.println("Ghi giá trị vào Mailbox");
    ulNewValue++;
    if (ulNewValue > 100) ulNewValue = 0;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void vReadMailbox(void *Parameters)
{
  int32_t value_received;
  while (1)
  {
    xQueueReceive(xMailbox, &value_received, portMAX_DELAY);
    Serial.print("Giá trị đọc được từ mailbox = ");
    Serial.println(value_received);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  vTaskDelay(portMAX_DELAY); // Delay indefinitely
}
