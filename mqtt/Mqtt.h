
/*SensorMulMqtt.h
 * Marc Cypres
 * 10/10/2020
 * */
 
 /*
 * topic structure commune :
 * 	SSID/HOSTNAME/msgtype
 * si msgtype == log
 * 	/FileName/Level/lineNb
 * si msgtype==data
 * 	/sensorName/xxxx
 */
 
#ifndef MQTT_H
#define MQTT_H


//#include "SensorMultiCor.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
//#include <WiFi.h>

#include <MqttLogger.h>
//#include <Sensor.h>

#define NB_FIELD 11
#define NB_MAX_CHAR_TOPIC 12
#define MSG_SIZE 200
#define TOPIC_SIZE 200
#define TOPIC "BilboNet/cmd/raspBilbo/#"

extern char mqtt_server[];
extern const char ssid[];
extern const char machineName[];
extern PubSubClient* MQTTclient;
extern  WiFiClient espClient;

void callback(char* topic, byte* payload, unsigned int length);

class Mqtt   {
public:
      
      Mqtt(String subScribeTopic);
      void publish(const char* msgtype,
			      const char* topic,
			      const char* payload); 
      
      void connect(); 
      void loop();               
protected:
      String subScribeTopic;
      //String getTopicName(int i);
      int getNbField(){ return NB_FIELD ;};
      //int getNbTopics();      
      String* topicNames;
};
#endif
