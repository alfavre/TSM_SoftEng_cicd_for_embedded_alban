/*
 * Copyright (c) 2025, Group project
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#if PL_CONFIG_USE_UNIT_TESTS
#include "test_sensor.h"
// I have added all Mcu things from the tests.c file, as I don't know which one I need and which I don't
#include "McuUnity.h"
#include "McuRTOS.h"
#include "McuShell.h"
#include "McuRTT.h"
#include "McuUtility.h"
#include "McuLog.h"
#include "sensor.h" // dont forget the file
#include "unity/unity.h"

void TestSensor_Temperature(void){

    float my_temp = Sensor_GetTemperature();
    TEST_ASSERT_MESSAGE(my_temp > 10, "Temp is below 10 degree celsius, it shouldn't?");

    //float my_temp = Sensor_GetTemperature();
    TEST_ASSERT_MESSAGE(my_temp < 35, "Temp is above 35 degree celsius, it shouldn't?");
    //TEST_ASSERT_MESSAGE(true, "Test, assert is always true"); /* success */
}


void TestSensor_Humidity(void){
    float my_hum = Sensor_GetHumidity();
    TEST_ASSERT_MESSAGE(my_hum > 5, "hum is below 5 percent, it shouldn't?");

    //float my_hum = Sensor_GetHumidity();
    TEST_ASSERT_MESSAGE(my_hum < 90, "hum is above 90 percent, it shouldn't?");
}


void TestSensor_Both(void){
    TestSensor_Temperature();
    TestSensor_Humidity();
}


#endif /* PL_CONFIG_USE_UNIT_TESTS */