/*
 * SensorQMC5883.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#include "SensorQMC5883.h"
#include <Adafruit_HMC5883_U.h>
#define GIRO_TIME_INTERVAL 1000
// anemotetre param definition
#define MAX_VALUE 10000
#define MIN_VALUE -10000

SensorQMC5883::SensorQMC5883(String sensorName,
								Logger* myLogger,
								int declinationAngleDegres,
								float declinationAnglemin,
								float xOffset,
								float yOffset,
								float zOffset,
								int id,
								MqttInterface* interfaceL)
								 :SensorMagneto(sensorName,
											myLogger,
											declinationAngleDegres,
											declinationAnglemin,
											xOffset,
											yOffset,
											zOffset,
											id,
											interface)
											{

	
}

SensorQMC5883::SensorQMC5883(String sensorName,
								Logger* myLogger,
								int declinationAngleDegres,
								float declinationAnglemin,
								float xOffset,
								float yOffset,
								float zOffset,
								int id
								)
								 :SensorMagneto(sensorName,
											myLogger,
											declinationAngleDegres,
											declinationAnglemin,
											xOffset,
											yOffset,
											zOffset,
											id)
											{

 	
}

SensorQMC5883::SensorQMC5883(String sensorName,
								Logger* myLogger,
								MqttInterface* interface)
								 :SensorMagneto(sensorName,
											myLogger,
											interface)
											{
 	
}

void SensorQMC5883::init(uint8_t range ){
	_LOG_PRINT(M," init","start");
	
	QMC5883L::init();
	QMC5883L::range=range;
	QMC5883L::reconfig();
 	_LOG_PRINT(M," init","SensorQMC5883 ok");
}

/*****************************************************************/

String SensorQMC5883::toString(){
	String retVal;
	
	getData();
	//mag.getEvent(&event);
	retVal="\n\tX:\t"+(String)event.magnetic.x;
  	retVal+="\tY:\t"+(String)event.magnetic.y;
  	retVal+="\tZ:\t"+(String)event.magnetic.z+"uT";
  	                                             
  	retVal+="\n\tmagxy(uT):\t"+(String)magnetudexy;
  	retVal+="\tmag(uT)  :\t"+(String)magnetude;
	retVal+="\tcap (°)  :\t"+(String)headingDegrees;
	return retVal;
}

/*********************************************************************/

void SensorQMC5883::publish(){
	if(interface==NULL){
		return ;	 		
	}
	
	 _LOG_PRINT(M," publish start","");
	char topic[MQTT_TOPIC_MAX_SIZE];
	char payload[MQTT_PAYLOAD_MAX_SIZE];
	getData();	
	dtostrf(headingDegrees,7, 2,payload);
	_LOG_PRINT(D," headingDegrees",payload);
	sprintf(topic,"%s/Cap",sensorName.c_str());
	_LOG_PRINT(M," topic",topic);
	interface->publish(topic,payload);	
	/*
	dtostrf(SensorBmp183::temperature,5, 2,payload);
	sprintf(topic,"%s/temperature",sensorName.c_str());
	interface->publish(topic,payload);		
	*/
}

/************************************************************************/

String SensorQMC5883::getSensorDetail(){
 
 	return "no data";
}


/************************************************************************/

sensors_event_t  SensorQMC5883::getEvent(){
	getData();
	return event;
}

/***********************************************************************/

 void SensorQMC5883::getData(){
	
	_LOG_PRINT(D,"getData getEvent :","Start");
	int16_t x=0;
	int16_t y=0;
	int16_t z=0;
	int16_t t=0;
	int16_t result;
	int16_t i=0;
	result=readRaw(&x,&y,&z,&t);
	if(result!=1){
		_LOG_PRINT(M,"result:",result);
	}
	_LOG_PRINT(D,"x:",x);	
	_LOG_PRINT(D,"y:",y);
	_LOG_PRINT(D,"z:",z);

	event.magnetic.x=x;
	event.magnetic.y=y;
	event.magnetic.z=z;
	_LOG_PRINT(D,"event.magnetic.x:",event.magnetic.x);
	_LOG_PRINT(D,"event.magnetic.y:",event.magnetic.y);
	_LOG_PRINT(D,"event.magnetic.z:",event.magnetic.z);
	magnetudexy = sqrt(sq(event.magnetic.y-yOffset) + sq(event.magnetic.x-xOffset));
  	magnetude = sqrt(sq(event.magnetic.y-yOffset) + sq(event.magnetic.x-xOffset) + sq(event.magnetic.z-zOffset));
	headingDegrees = getHeadingDegrees( event,
					 declinationAngle,
					 xOffset,
					 yOffset);
  	if(minx>event.magnetic.x){
		minx=event.magnetic.x;
	}
	if(miny>event.magnetic.y){
		miny=event.magnetic.y;
	}
	if(minz>event.magnetic.z){
		minz=event.magnetic.z;
	}
	if(maxx<event.magnetic.x){
		maxx=event.magnetic.x;
	}
	if(maxy<event.magnetic.y){
		maxy=event.magnetic.y;
	}
	if(maxz<event.magnetic.z){
		maxz=event.magnetic.z;
	}
}

/****************************************************************/
/*
  void SensorQMC5883::setRange( int range ){
  	QMC5883L::setRange( int range );
  }
*/

/*****************************************************************/

SensorQMC5883::~SensorQMC5883() {
	// TODO Auto-generated destructor stub
}

