/*
 * MqttInterfaceEsp32.h
 *
 *  Created on:22/08/2020
 *      Author: marc
 */

#ifndef MQTT_INTERFACE_ESP32_H_
#define MQTT_INTERFACE_ESP32_H_
#ifndef __AVR_ATmega2560__
#ifndef MQTT_INTERFACE_ESP8266_H_
#include "Arduino.h"
#include <MqttInterface.h>
#include <PubSubClient.h>
//#include <WiFiUdp.h> 
#include <ESP8266WiFi.h>
#define MSG_SIZE 200
#define TOPIC_SIZE 200


enum StateWaiting {
  start,
  commande,
  endTopic,
  endMessage
  
};

	
class MqttInterfaceEsp32 :public MqttInterface{
public:
	MqttInterfaceEsp32(HardwareSerial* ptEspSer,
								Logger* myLogger,
								const char* machineName,
								const char* netName);
	void publish(const char* topic, const char* payload);
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
	Logger* myLogger;
	StateWaiting state;
	char buffMsg[MSG_SIZE];
	char buffTopic[TOPIC_SIZE];
	int posTopic,posMsg;
	char cmd;
};
#endif
#endif
#endif
