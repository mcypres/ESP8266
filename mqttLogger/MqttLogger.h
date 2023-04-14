#ifndef MQTT_LOGGER_H
#define MQTT_LOGGER_H

//#include "HardwareSerial.h"
//#include "Print.h"

#include <PubSubClient.h>
#include <Mqtt.h>
//#include <WiFi.h>
#include <stdlib.h>

//#define WITH_PATH
#define SEP1 "-->\t"
#define SEP2 "-->\t***"


#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#define LOG_USB
#define LOG_WIFI
#define BUFF_TOPIC_SIZE 200
#define BUFF_PAYLOAD_SIZE 200
#define LOG_QUEUE_SISE 20
enum LevelValue{N=0,M=1,E=2,W=3,D=4,V=5};

//extern Mqtt* myMqtt;
struct stLogMsg{
	char msgTopic[BUFF_TOPIC_SIZE];
	char msgPayload[BUFF_PAYLOAD_SIZE];
};
class MqttLogger{
private:

public:
	MqttLogger(HardwareSerial* ptLogSer,
					const char* machineName,
					LevelValue requieredLevel);
	//void queueToMQTT( void * parameter );
	void setRequieredLevel(char c);
	LevelValue getRequieredLevel(){
		 return requieredLevel;
	 }
	
//	void printLog(int level,char* sender,char* title,const __FlashStringHelper *value);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],const String &value);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],const char* value);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],char value);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned char value, int type);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],int value, int type= DEC);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned int value, int type= DEC);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],long value, int type= DEC);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned long value, int type= DEC);

	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],float value, int type=  2);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[],double value, int type=  2);
	void printLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title,const char tail[], Printable& value);

private: 
	void printSerialLog(LevelValue level,
								const int fileId,
								const char* sender,
								const int lineNb ,
								const char* title,
								const char tail[],
								const char* value);
	void publishLog( char* buffTopic,const char* filleName,const int lineNb ,const char* title);
	void headerLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title);
	void topicLog(const char* filleName,const int lineNb ,const char* title);
	
	//xQueueHandle xQueue;
	String ssId,hostName;
	HardwareSerial* ptLogSer;
	LevelValue requieredLevel=M;
	const char* machineName;
	int locked=0;
};
#ifndef MAIN
extern MqttLogger* myLogger;
#endif
//#define _LOG_  printLog(2,(char*)  __FILE__ ,__LINE__,
#ifndef NO_LOG
	
	#undef F
	#define F(x) x
	#define _LOG_PRINT(LEVEL,FILE_ID_VAR,TITRE,VALUE) myLogger->printLog(LEVEL,(int) FILE_ID_VAR,(char*)  __FILE__ ,__LINE__,(char*)(TITRE),"***\n",(VALUE))
	#define _LOG_PRINT_TAIL(LEVEL,FILE_ID_VAR,TITRE,VALUE,TAIL) myLogger->printLog(LEVEL,(int) FILE_ID_VAR,(char*)  __FILE__ ,__LINE__,(char*)(TITRE),(TAIL),(VALUE))
	#define _LOG_PRINT_HEX(LEVEL,FILE_ID_VAR,TITRE,VALUE) myLogger->printLog(LEVEL,(int) FILE_ID_VAR,(char*)  __FILE__ ,__LINE__,(char*)(TITRE),"***\n",(VALUE))
#endif
#ifdef NO_LOG
	#define _LOG_PRINT(LEVEL,FILE_ID_VAR,TITRE,VALUE) ((void)0)
	#define _LOG_PRINT_TAIL(LEVEL,FILE_ID_VAR,TITRE,VALUE,TAIL) ((void)0)
	#define _LOG_PRINT_HEX(LEVEL,FILE_ID_VAR,TITRE,VALUE) ((void)0)
#endif
#endif
