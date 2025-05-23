/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#if PL_CONFIG_USE_OLED
#include "McuUtility.h"
#include "McuSSD1306.h"
#include "McuLog.h"
#include "McuRTOS.h"
#include "McuSSD1306.h"
#include "McuUtility.h"
#include "McuGDisplaySSD1306.h"
#include "McuFontDisplay.h"
#include "McuFontHelv10Normal.h"
#include "McuFontHelv12Normal.h"
#include "McuFontHelv18Bold.h"
#if PL_CONFIG_USE_SENSOR
  #include "sensor.h"
#endif
#if PL_HAS_RADIO
  #include "RNet/Radio.h"
#endif

static QueueHandle_t oledTextQueue;  /* Queue storing pointers to the text to display */
#define OLED_TEXT_QUEUE_LENGTH                 (8)
#define OLED_TEXT_QUEUE_ELEM_SIZE              (sizeof(unsigned char*))

void OLED_SendText(const char *text) {
  size_t len;
  unsigned char *p;

  len = McuUtility_strlen(text)+1; /* plus 1 for the zero byte */
  p = pvPortMalloc(len);

  McuUtility_strcpy(p, len, (unsigned char*)text);
  if (xQueueSendToBack(oledTextQueue, &p, pdMS_TO_TICKS(3000))!= pdPASS) { /* queue pointer to data */
    vPortFree(p); /* failed, give memory free again */
  }
}

static uint8_t OLED_GetText(unsigned char *buf, size_t bufSize) {
  unsigned char *p;

  if (xQueueReceive(oledTextQueue, &p, 0)==pdPASS) {
    McuUtility_strcpy(buf, bufSize, p);
    vPortFree(p);
    return ERR_OK;
  }
  return ERR_IDLE;
}

/*!
 * \brief Shows one line of text centered on the display, with a border around it
 * \param text0 Line one of text
 */
static void Show1Liner(const unsigned char *text0) {
  PGFONT_Callbacks font = McuFontHelv12Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  McuGDisplaySSD1306_Clear();
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text0, font, NULL)/2;
  y = McuGDisplaySSD1306_GetHeight()/2 - charHeight/2; /* 1 line */
  McuFontDisplay_WriteString((unsigned char*)text0, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth()-1-4, McuGDisplaySSD1306_GetHeight()-1-4, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_UpdateFull();
}

/*!
 * \brief Shows two lines of text centered on the display, with a border around it
 * \param text0 Line one of text
 * \param text1 Line two of text
 */
static void Show2Liner(const unsigned char *text0, const unsigned char *text1) {
  PGFONT_Callbacks font = McuFontHelv12Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  McuGDisplaySSD1306_Clear();
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text0, font, NULL)/2;
  y = McuGDisplaySSD1306_GetHeight()/2 - (2*charHeight)/2; /* 2 lines */
  McuFontDisplay_WriteString((unsigned char*)text0, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text1, font, NULL)/2;
  y += McuGDisplaySSD1306_GetHeight()/2 - (2*charHeight)/2;
  McuFontDisplay_WriteString((unsigned char*)text1, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth()-1-4, McuGDisplaySSD1306_GetHeight()-1-4, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_UpdateFull();
}

//static bool volatile wait = false;

#if PL_CONFIG_USE_SENSOR
static void ShowSensorValues(void) {
  float t, h;
  unsigned char buf1[16], buf2[16];

  h = Sensor_GetHumidity();
  t = Sensor_GetTemperature();
  McuUtility_strcpy(buf1, sizeof(buf1), (unsigned char*)"T: ");
  McuUtility_strcatNumFloat(buf1, sizeof(buf1), t, 2);
  McuUtility_strcat(buf1, sizeof(buf1), (unsigned char*)" C");
  McuUtility_strcpy(buf2, sizeof(buf2), (unsigned char*)"H: ");
  McuUtility_strcatNumFloat(buf2, sizeof(buf2), h, 2);
  McuUtility_strcat(buf2, sizeof(buf2), (unsigned char*)" %");
  Show2Liner(buf1, buf2);
}
#endif

static void OledTask(void *pv) {
  unsigned char buf[16];
  uint32_t counter = 0;
  uint32_t ms = 0;

  //while(wait) {
  //  vTaskDelay(pdMS_TO_TICKS(5000));
  //  McuLog_info("waiting for debugger");
  //}
  McuSSD1306_Clear();
#if McuLib_CONFIG_CPU_IS_ESP32
  Show2Liner((unsigned char*)"ADIS", (unsigned char*)"ESP32");
#elif McuLib_CONFIG_CPU_IS_RPxxxx
  Show2Liner((unsigned char*)"TSM", (unsigned char*)"RP2040");
#endif
  vTaskDelay(pdMS_TO_TICKS(2000));
#if PL_CONFIG_USE_SENSOR
  ShowSensorValues();
#endif
  vTaskDelay(pdMS_TO_TICKS(2000));
  for(;;) {
    while (OLED_GetText(buf, sizeof(buf))==ERR_OK) {
      Show1Liner(buf);
      vTaskDelay(pdMS_TO_TICKS(500));
      ms += 500;
    }
#if PL_CONFIG_USE_SENSOR
    if ((ms%10000)==0) { /* every 10 seconds */
      ShowSensorValues();
      vTaskDelay(pdMS_TO_TICKS(1000));
      ms += 1000;
#endif
#if PL_HAS_RADIO
      if (RADIO_IsSane()==ERR_OK) {
        Show1Liner("nRF ok");
      } else {
        Show1Liner("nRF NOT ok");
      }
      vTaskDelay(pdMS_TO_TICKS(1000));
      ms += 1000;
#endif
    } else if ((ms%1000)==0) { /* every second */
      McuUtility_Num32uToStr(buf, sizeof(buf), counter);
      counter++;
    #if McuLib_CONFIG_CPU_IS_ESP32
      Show2Liner((unsigned char*)"ADIS ESP32", buf);
    #elif McuLib_CONFIG_CPU_IS_RPxxxx
      Show2Liner((unsigned char*)"TSM RP2040", buf);
    #else
      #error "???"
    #endif
    }
    vTaskDelay(pdMS_TO_TICKS(100));
    ms += 100;
  }
}

void OLED_Init(void) {
  BaseType_t res;

  res = xTaskCreate(OledTask, "OledTask", (2*1024)/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+4, NULL);
  if (res!=pdPASS) {
    McuLog_fatal("creating OledTask task failed!"); //  GCOVR_EXCL_LINE
  }
  McuSSD1306_Init();
  McuGDisplaySSD1306_Init();

  oledTextQueue = xQueueCreate(OLED_TEXT_QUEUE_LENGTH, OLED_TEXT_QUEUE_ELEM_SIZE);
  if (oledTextQueue==NULL) {
    McuLog_fatal("not able to create oledTextQueue queue"); //  GCOVR_EXCL_LINE
    for(;;){} /* out of memory? */                          //  GCOVR_EXCL_LINE
  }
  vQueueAddToRegistry(oledTextQueue, "oledTextQueue");
}
#endif /* PL_CONFIG_USE_OLED */
