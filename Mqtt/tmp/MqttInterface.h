/*
 * MqttInterface.h
 *
 *  Created on:22/08/2020
 *      Author: marc
 */

#ifndef MQTT_INTERFACE_H_
#define MQTT_INTERFACE_H_
#include "Arduino.h"
#include "Logger.h"

#define START_CHAR '#'
#define END_CHAR '\0'
#define SEPA_CHAR '&'

#define MSG_SIZE 200
#define TOPIC_SIZE 200

#define MQTT_CONNECTED_MSG "$$$Connected$$$"
#define MQTT_DISNECTED_MSG "$$$Disnected$$$"
#define ESP_READY_MSG "$$$EspRady$$$"

typedef struct {
  int sender;
  char *payload;
  char *topic;
} Data;

class MqttInterface{
	public:
		MqttInterface(const char* machineName,const char* netName);
		virtual void publish(const char* topic, const char* payload)=0;
		
		virtual void connect()=0;
		
		boolean isConnected(){ 
			return true ; //connected;
		}
	protected:
		const char* netName;
		const char* machineName;
		//boolean connected=True;
	private:
		//MqttInterface(){}
};

#endif
