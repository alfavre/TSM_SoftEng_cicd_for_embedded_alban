/*
 * Copyright (c) 2025, Group project
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TEST_SENSOR_H_
#define _TEST_SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Test sensor temperature
 */
void TestSensor_Temperature(void);

/*!
 * \brief Test sensor humidity
 */
void TestSensor_Humidity(void);

/*!
 * \brief Test both sensor temperature and humidity
 */
void TestSensor_Both(void);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* _TEST_SENSOR_H_ */
