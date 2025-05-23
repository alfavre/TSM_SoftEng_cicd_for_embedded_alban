/*
 * Copyright (c) 2023-2024, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#if PL_CONFIG_USE_PICO_W
  #include "pico/cyw43_arch.h"
  #include "PicoWiFi.h"
#endif
#include "application.h"
#include "McuRTOS.h"
#include "McuLog.h"
#include "McuUtility.h"
#include "McuLED.h"
#include "leds.h"
#if PL_CONFIG_USE_BUTTONS && !PL_CONFIG_USE_BUTTONS_IRQ
  #include "buttons.h"
  #include "debounce.h"
#endif
#if PL_CONFIG_USE_OLED
  #include "oled.h"
#endif
#if PL_CONFIG_USE_GCOV
  #include "McuCoverage.h"
#endif
#if PL_CONFIG_USE_GPROF
  #include "profile_test.h"
#endif
#include <stdio.h>
#if PL_CONFIG_USE_MQTT_CLIENT
  #include "mqtt_client.h"
  #include "sensor.h"
#endif

#if PL_CONFIG_USE_BUTTONS
void App_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds kind) {
  const char *p = NULL;

  if (kind==MCUDBNC_EVENT_PRESSED) {
    switch(button) {
      case BTN_NAV_UP:
        p = "up";
        break;
      case BTN_NAV_DOWN:
         p = "down";
         break;
      case BTN_NAV_LEFT:
         p = "left";
         break;
      case BTN_NAV_RIGHT:
         p = "right";
         break;
      case BTN_NAV_CENTER:
         p = "center";
         break;
      default:
        p = NULL;
        break;
    }
    if (p!=NULL) {
      McuLog_info(p);
    #if PL_CONFIG_USE_OLED
      OLED_SendText(p);
    #endif
    }
  }
}
#endif

static void AppTask(void *pv) {
  uint32_t ms = 0;
  volatile bool dumpCoverage = true;

#if PL_CONFIG_USE_PICO_W && PL_CONFIG_USE_LEDS
  Leds_InitFromTask(); /* needed for the on-board Pico-W LED */
#endif
  for(;;) {
  #if PL_CONFIG_USE_BUTTONS && !PL_CONFIG_USE_BUTTONS_IRQ
    /*! \TODO if enabled WiFi, it triggers GPIO button interrupts? Doing polling instead */
    uint32_t buttons;

    buttons = BTN_GetButtons();
    if (buttons!=0) { /* poll buttons */
      Debounce_StartDebounce(buttons); 
    }
    vTaskDelay(pdMS_TO_TICKS(20));
    ms += 20;
  #else
    vTaskDelay(pdMS_TO_TICKS(1000));
    ms += 1000;
  #endif
  #if PL_CONFIG_USE_GCOV
    if (dumpCoverage && ms>5*1000) {
      vTaskEndScheduler(); /* exit scheduler to write coverage information */
    }
  #endif
  #if PL_CONFIG_USE_GPROF
    gprof_test();
    if (ms>5*1000) {
        vTaskEndScheduler();
    }
  #endif
  }
}

#if PL_CONFIG_USE_MQTT_CLIENT

static TaskHandle_t mqttTaskHandle = NULL;

void App_MqttTaskResume(void) {
  if (mqttTaskHandle!=NULL) {
    vTaskResume(mqttTaskHandle);
  }
}

void App_MqttTaskSuspend(void) {
  if (mqttTaskHandle!=NULL) {
    vTaskSuspend(mqttTaskHandle);
  }
}

static void MqttTask(void *pv) {
  for(;;) {
    float t, h;

    h = Sensor_GetHumidity();
    t = Sensor_GetTemperature();
    if (MqttClient_GetDoPublish()) {
      if (MqttClient_Publish_SensorValues(t, h)!=ERR_OK) {
        McuLog_error("failed publishing sensor values");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  } /* for */
}

#endif

void App_Init(void) {
#if PL_CONFIG_USE_APP_TASK
  if (xTaskCreate(
      AppTask,  /* pointer to the task */
      "App", /* task name for kernel awareness debugging */
      1500/sizeof(StackType_t), /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY+2,  /* initial priority */
      (TaskHandle_t*)NULL /* optional task handle to create */
    ) != pdPASS)
  {
    McuLog_fatal("Failed creating task");         // GCOVR_EXCL_LINE
    for(;;){} /* error! probably out of memory */ // GCOVR_EXCL_LINE
  }
#endif
#if PL_CONFIG_USE_MQTT_CLIENT
  if (xTaskCreate(
      MqttTask,  /* pointer to the task */
      "mqtt", /* task name for kernel awareness debugging */
      1500/sizeof(StackType_t), /* task stack size */
      (void*)NULL, /* optional task startup argument */
      tskIDLE_PRIORITY+2,  /* initial priority */
      &mqttTaskHandle /* optional task handle to create */
    ) != pdPASS)
  {
    McuLog_fatal("Failed creating myqtt task");
    for(;;){} /* error! probably out of memory */
  }
#endif
}
