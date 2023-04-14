/*
 * SensorQMC5883.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef SensorQMC5883_H_
#define SensorQMC5883_H_
#include "Arduino.h"
#include <Wire.h>
#include<MqttInterface.h>
#include <QMC5883L.h>
#include <Adafruit_Sensor.h>
#include <SensorMagneto.h>
#include "Sensor.h"
#include <stdio.h>
#include"MqttLogger.h" 


#undef FILE_ID
#define FILE_ID 12

#define LSB800 0.012207031f
#define LSB200 0,003051758f



class SensorQMC5883: public SensorMagneto ,QMC5883L{
public:
	
	float xOffset, yOffset,zOffset,speedCoef;
	SensorQMC5883(String sensorName,
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
						//MqttInterface* interface);
		/*String sensorName,
							MqttLogger *myLogger,
							Mqtt* myMqtt,
							String *topicNames,
							const int topicSize,
							int priorityLevel,
							long intervalTime);	*/			
/*	SensorQMC5883(String sensorName,
						MqttLogger* myLogger,
						int declinationAngleDegres,
						float declinationAnglemin,
						float xOffset,
						float yOffset,
						float zOffset,
						int id);
	SensorQMC5883(String sensorName,
						MqttLogger* myLogger,
						MqttInterface* interface);*/
											
/*	SensorQMC5883(HardwareSerial *ptOutputSer,
					Logger *myLogger,
					int declinationAngleDegres,
					float declinationAnglemin,
					int id,
					float xOffset=0,
					float yOffset=0,
					float zOffset=0,
					float speedCoef=1);*/
	String toString();
	 
	String toString(sensors_event_t *event);
	sensors_event_t  getEvent();
	void getData();
	String getSensorDetail();	
	boolean publish();
//	void setRange( int range );
	void init(uint8_t range );
	virtual ~SensorQMC5883();
protected:
	Mqtt* myMqtt;	
	//Adafruit_HMC5883_Unified mag;
	//QMC5883L mag;
/*	sensors_event_t event;	
	float magnetudexy,magnetude;
	float declinationAngle ;
	float headingDegrees;
	float heading;
	float speed;
	unsigned long mLastTime;
	float minx,miny,minz,maxx,maxy,maxz;
*/	


};

#endif /* SensorQMC5883 */
