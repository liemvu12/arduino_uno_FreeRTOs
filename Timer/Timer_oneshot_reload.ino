#include <Arduino_FreeRTOS.h>
#include <timers.h>
#include <task.h>

#define mainONE_SHOT_TIMER_PERIOD pdMS_TO_TICKS( 3333 )
#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS( 500 )

TimerHandle_t xAutoReloadTimer, xOneShotTimer;
BaseType_t xTimer1Started, xTimer2Started;

void setup() {
  Serial.begin(9600);
  xOneShotTimer = xTimerCreate("OneShot", mainONE_SHOT_TIMER_PERIOD, pdFALSE, 0, prvOneShotTimerCallback);
  xAutoReloadTimer = xTimerCreate("AutoReload", mainAUTO_RELOAD_TIMER_PERIOD, pdTRUE, 0, prvAutoReloadTimerCallback);
  
  if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL)) {
    xTimer1Started = xTimerStart(xOneShotTimer, 0);
    xTimer2Started = xTimerStart(xAutoReloadTimer, 0);
    
    if ((xTimer1Started == pdPASS) && (xTimer2Started == pdPASS)) {
      vTaskStartScheduler();
    }
  }
}

void loop() {}

static void prvOneShotTimerCallback(TimerHandle_t xTimer) {
  TickType_t xTimeNow;
  xTimeNow = xTaskGetTickCount();
  Serial.print("One-shot timer callback executing ");
  Serial.println(xTimeNow/31);
}

static void prvAutoReloadTimerCallback(TimerHandle_t xTimer) {
  TickType_t xTimeNow;
  xTimeNow = xTaskGetTickCount();
  Serial.print("Auto-reload timer callback executing ");
  Serial.println(xTimeNow/31);
}
