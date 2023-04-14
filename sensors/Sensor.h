/*
 * Sensor.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */
#ifndef SENSOR_H_
#define SENSOR_H_




#include "Arduino.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MqttLogger.h"
//#include "SensorMultiCor.h"
#include <Mqtt.h>
#undef FILE_ID
#define FILE_ID 2

#define MSG_Max_LENGHTS      85
#define NMEA_SEPARATOR ","
#define NMEA_HEADER "$"
#define AIS_HEADER "!"
#define MSG_TAIL "*"
#define SEATALK_HAEDER "#"
#define MQTT_TOPIC_MAX_SIZE 20
#define MQTT_PAYLOAD_MAX_SIZE 20



class Sensor {
public:
	Sensor(
			String sensorName,
			MqttLogger *myLogger,			
			int priorityLevel,
			long intervalTime
			//int priorityState=1
			);
			
	//virtual void queueToMQTT( void * parameter )=0;
	//virtual void  queueMsg( void * parameter)=0;
	virtual String toString()=0;
	virtual void getData()=0;
	boolean publish(char*  msg);
	virtual boolean publish(){
		return true;
	};
	Sensor * testPriority(Sensor * lastSensor);
	String eepromParamtoString() {
		return  String(sensorName+ "as no Eeprom param");
	}
	void checksum(unsigned char* ptr,char* ptChk,int msgLength,int* ptChkInt=NULL);
	static int freeRam () {
		extern int __heap_start, *__brkval; 
		int v; 
		return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
	}
	/*String getSensorName(){
		return sensorName;
	};*/
	 ~Sensor();
protected:
	String sensorName;
	String ErrorMsg="";
	boolean ErrFlag=false;
	MqttLogger *myLogger;
	int priorityState=1;
	int priorityLevel;
	long intervalTime;
	long lastTimeLastSend=millis();
public:
	String getName();
	virtual String getSensorDetail();
	String getErrorMsg(){
		return ErrorMsg;
	}
	boolean getError(){
		return ErrFlag;
	}
	boolean isTimeToSend();
};
extern std::vector<Sensor *>sensorList;
#undef FILE_ID
#endif /* SENSOR_H_ */
