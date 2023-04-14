
// fichier SensorMPU6050.h
// auteur mwc
// date 26/12/21

#ifndef SENSOR_MPU_6050_H
#define SENSOR_MPU_6050_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Sensor.h"
//#include"Logger.h"
#include "MqttLogger.h"
#include  "MqttInterface.h"
#undef FILE_ID
#define FILE_ID 11
class SensorMPU6050 : public Sensor {
public:
	SensorMPU6050(String sensorName,
			MqttLogger* myLogger,
			Mqtt* myMqtt,
			int priorityLevel,
			long intervalTime);
			//MqttInterface* interface);
	virtual void getData();
	boolean  publish();
	String toString();
private:
	Adafruit_MPU6050 mpu;
	sensors_event_t acceleration, gyro, temp;
	Mqtt* myMqtt;
};
#endif
