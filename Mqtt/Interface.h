/*
 * MqttInterface.h
 *
 *  Created on:22/08/2020
 *      Author: marc
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_
#include "Arduino.h"
#include "MqttLogger.h"

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

class Interface{
	public:
		Interface(const char* machineName,const char* netName);
		virtual boolean publish(const char* topic, const char* payload)=0;
		
		virtual void connect()=0;
		
		boolean isConnected(){ 
			return true ; //connected;
		}
	protected:
		const char* netName;
		const char* machineName;
		//boolean connected=True;
	private:
		
};

#endif
