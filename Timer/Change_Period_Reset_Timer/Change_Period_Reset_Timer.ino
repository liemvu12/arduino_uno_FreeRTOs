#include <Arduino_FreeRTOS.h>
#include <timers.h>

#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS( 500 )
#define mainAUTO_RELOAD_TIMER_PERIOD2 pdMS_TO_TICKS( 1000 )

TimerHandle_t xAutoReloadTimer;
BaseType_t xTimer2Started;

void setup() {
  Serial.begin(9600); // Enable serial communication library.
  xAutoReloadTimer = xTimerCreate("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvTimerCallback);

  if (xAutoReloadTimer != NULL) {
    xTimer2Started = xTimerStart(xAutoReloadTimer, 0);

    if (xTimer2Started == pdPASS) {
      vTaskStartScheduler();
    }
  }
}

void loop() {}

static void prvTimerCallback(TimerHandle_t xTimer) {
  TickType_t xTimeNow;
  uint32_t ulExecutionCount;

  ulExecutionCount = (uint32_t) pvTimerGetTimerID(xTimer);
  ulExecutionCount++;
  vTimerSetTimerID(xTimer, (void *) ulExecutionCount);

  xTimeNow = xTaskGetTickCount();

  Serial.print("Auto-reload timer callback executing ");
  Serial.println(xTimeNow/31);

  if (ulExecutionCount >= 5) {
    xTimerChangePeriod(xAutoReloadTimer, mainAUTO_RELOAD_TIMER_PERIOD2, 0);
  }
}
