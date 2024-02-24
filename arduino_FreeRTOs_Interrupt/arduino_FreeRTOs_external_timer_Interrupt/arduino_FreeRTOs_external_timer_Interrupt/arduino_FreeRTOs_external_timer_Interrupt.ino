#include <Arduino_FreeRTOS.h>
#include <timers.h>
#include <semphr.h>

// viết chương trình sử dụng 2 ngắt lồng nhau thực hiện: sau 1s sẽ in ra Hello World trên màn hình;
// nhận nút sẽ in "hello interrupt from button";

#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS( 1000 )
#define INTERRUPT_PIN 2

TimerHandle_t xAutoReloadTimer;
SemaphoreHandle_t screenSemaphore; // Semaphore để quản lý tài nguyên màn hình

void setup() {
  Serial.begin(9600);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); // Thiết lập chân ngắt là input và kích pull-up
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interruptServiceRoutine, FALLING); // Gắn ngắt vào chân ngắt ngoài, kích hoạt ngắt trên sự kiện falling (khi chân từ HIGH sang LOW)
  
  screenSemaphore = xSemaphoreCreateMutex(); // Khởi tạo semaphore để quản lý tài nguyên màn hình

  xAutoReloadTimer = xTimerCreate(
    "AutoReload",
    mainAUTO_RELOAD_TIMER_PERIOD,
    pdTRUE,
    0,
    prvAutoReloadTimerCallback
  );

  if (xAutoReloadTimer != NULL) {
    xTimerStart(xAutoReloadTimer, 0);
  }
}

void loop() {
  // Không cần thực hiện gì trong hàm loop vì công việc được thực hiện trong trình phục vụ ngắt và callback của timer.
}

static void prvAutoReloadTimerCallback(TimerHandle_t xTimer) {
  if (xSemaphoreTake(screenSemaphore, portMAX_DELAY) == pdTRUE) { // Chờ và lấy semaphore để sử dụng tài nguyên màn hình
    Serial.println("Hello world");
    xSemaphoreGive(screenSemaphore); // Trả lại semaphore sau khi sử dụng xong
  }
}

void interruptServiceRoutine() {
  if (xSemaphoreTake(screenSemaphore, portMAX_DELAY) == pdTRUE) { // Chờ và lấy semaphore để sử dụng tài nguyên màn hình
    Serial.println("Hello interrupt world"); // In ra "Hello interrupt world" khi ngắt xảy ra
    xSemaphoreGive(screenSemaphore); // Trả lại semaphore sau khi sử dụng xong
  }
}
