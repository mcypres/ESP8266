/*
 * MqttInterface.h
 *
 *  Created on:22/08/2020
 *      Author: marc
 */

#ifndef MQTT_INTERFACE_H_
#define MQTT_INTERFACE_H_
//#ifndef __AVR_ATmega2560__

#include "Arduino.h"
#include "Interface.h"
#include <PubSubClient.h>
#include "MqttLogger.h"
//#include <WiFi.h>
#include <ESP8266WiFi.h>
#undef FILE_ID
#define FILE_ID 14
#define MSG_SIZE 200
#define TOPIC_SIZE 200


enum StateWaiting {
  start,
  commande,
  endTopic,
  endMessage  
};

	
class MqttInterface :public Interface{
public:
	MqttInterface(HardwareSerial* ptEspSer,
				MqttLogger* myLogger,
                const char* mqtt_server,
				const char* machineName,
				const char* netName);
	boolean publish(const char* topic, const char* payload);
	void publishSerial(const char* ptMsg);
	void readSerial();
	void connect();
	void loop();
	boolean  connected();
	void setServer(const char* mqtt_server,const int port );
	void setCallback(void callback(char* topic, byte* payload, unsigned int length));
private:
	WiFiClient espClient;
	PubSubClient* MQTTclient;
	HardwareSerial* ptOutputSer;
	MqttLogger* myLogger;
	StateWaiting state;
	char buffMsg[MSG_SIZE];
	char buffTopic[TOPIC_SIZE];
	int posTopic,posMsg;
	char cmd;
};
//#endif
//#endif
#endif
