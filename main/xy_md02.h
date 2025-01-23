/*
 * xy_md02.h
 *
 *  Created on: Jan 18, 2025
 *      Author: LattePanda
 */

#ifndef MAIN_XY_MD02_H_
#define MAIN_XY_MD02_H_

#define XY_MD02_OK              0
#define XY_MD02_TIMEOUT_ERROR  -1
#define XY_MD02_UNKNOWN_ERROR  -2

#define XY_MD02_UART_NUM        UART_NUM_1
#define XY_MD02_RX_PIN          16
#define XY_MD02_TX_PIN          17

/**
 * Start XY_MD02 sensor task
 */
void XY_MD02_task_start(void);

// == function prototypes ==================================

int   readXY_MD02();
float XY_MD02_getHumidity();
float XY_MD02_getTemperature();
void XY_MD02_errorHandler(int response);


#endif /* MAIN_XY_MD02_H_ */
