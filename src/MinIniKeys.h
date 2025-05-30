/*
 * Copyright (c) 2022-2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MININIKEYS_H_
#define MININIKEYS_H_

/* strings and names used in minINI configuration file */

#include "platform.h"

#define NVMC_MININI_FILE_NAME       "settings.ini" /* 'file' name used */

#if PL_CONFIG_USE_WIFI
  /* section for WiFi settings
   * configure them using the shell and the following commands:
   * McuMinINI write settings.ini WiFi ssid YOUR_SSID
   * McuMinINI write settings.ini WiFi pass YOUR_PASSWORD
   */
  #define NVMC_MININI_SECTION_WIFI              "WiFi"
  #define NVMC_MININI_KEY_WIFI_SSID                "ssid"     /* string, SSID of network */
  #define NVMC_MININI_KEY_WIFI_PASS                "pass"     /* string, password */
  #define NVMC_MININI_KEY_WIFI_HOSTNAME            "hostname" /* string, hostname to be used */
  #define NVMC_MININI_KEY_WIFI_ENABLE              "enable"   /* bool, if WiFi is enabled */
#endif

#if PL_CONFIG_USE_MQTT_CLIENT
  /* section for MQTT client settings */
  #define NVMC_MININI_SECTION_MQTT              "MQTT"
  #define NVMC_MININI_KEY_MQTT_BROKER              "broker"   /* string, broker name or IP */
  #define NVMC_MININI_KEY_MQTT_CLIENT              "client"   /* string, client ID */
  #define NVMC_MININI_KEY_MQTT_USER                "user"     /* string, username */
  #define NVMC_MININI_KEY_MQTT_PASS                "pass"     /* string, password */
  #define NVMC_MININI_KEY_MQTT_PUBLISH             "publish"  /* bool, if publishing */
#endif

#if PL_CONFIG_USE_NTP_CLIENT
  /* section for NTP client settings */
  #define NVMC_MININI_SECTION_NTP               "NTP"
  #define NVMC_MININI_KEY_NTP_START                "start"   /* bool, if start ntp or not after reset */
#endif

#endif /* MININIKEYS_H_ */
