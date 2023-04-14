/*
 * SensorHMC5883.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef SensorHMC5883_H_
#define SensorHMC5883_H_
#include "Arduino.h"
//#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "Sensor.h"
#include <stdio.h>
#include <SensorMagneto.h>

#undef FILE_ID
#define FILE_ID 13

/*#include <avr/interrupt.h>

extern unsigned long lastTime;
extern volatile unsigned long GanemoCount;
extern void anemoInterupt();
*/

class SensorHMC5883: public SensorMagneto {
public:
	SensorHMC5883(String sensorName,
					MqttLogger* myLogger,
					Mqtt* myMqtt,
					String *topicNames,
					const int topicSize,
					int declinationAngleDegres,
					float declinationAnglemin,
					float xOffset,
					float yOffset,
					float zOffset,
					int id,
					int priorityLevel,
					long intervalTime);
	
/*		
	SensorHMC5883(String sensorName,
						MqttLogger* myLogger,
						int declinationAngleDegres,
						float declinationAnglemin,
						float xOffset,
						float yOffset,
						float zOffset,
						int id,
						MqttInterface* interface);

	SensorHMC5883(String sensorName,
						MqttLogger* myLogger,
						MqttInterface* interface);
*/					
	//String toNmea();
//	float getSpeed();
	void getData();
	String toString();
//	String getSensorDetail();
	boolean publish();
	virtual ~SensorHMC5883();
private:
	Adafruit_HMC5883_Unified mag;
	//void init();
	Mqtt* myMqtt;
	String *topicNames;
	const int topicSize;
	void init(uint8_t range );


};

#endif /* SensorHMC5883 */
