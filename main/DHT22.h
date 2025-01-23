/*
 * DHT22.h
 *
 *  Created on: Jul 16, 2024
 *      Author: LattePanda
 */


/*
 * DHT22 temperature sensor driver
 */


#ifndef MAIN_DHT22_H_
#define MAIN_DHT22_H_

#define DHT_OK					0
#define DHT_CHECKSUM_ERROR		-1
#define DHT_TIMEOUT_ERROR		-2

#define DHT_GPIO				25

/**
 * Start DHT sensor task
 */
void DHT22_task_start(void);

// == function prototypes ==================================

void 	setDHTgpio(int gpio);
void 	errorHandler(int response);
int 	readDHT();
float	getHumidity();
float	getTemperature();
int 	getSignalLevel( int usTimeOut, bool state );


#endif /* MAIN_DHT22_H_ */
