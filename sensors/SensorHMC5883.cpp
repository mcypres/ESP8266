/*
 * SensorHMC5883.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#include <SensorHMC5883.h>

// anemotetre param definition
#define MAX_VALUE 10000
#define MIN_VALUE -10000

SensorHMC5883::SensorHMC5883(String sensorName,
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
								long intervalTime):
									myMqtt(myMqtt),
									topicNames(topicNames),
									topicSize(topicSize),
									
									SensorMagneto( sensorName,
												myLogger,
												declinationAngleDegres,
												declinationAnglemin,
												xOffset,
												yOffset,
												zOffset,
												id,
												priorityLevel,
												intervalTime){
											
	init(id);
}
/*
SensorHMC5883::SensorHMC5883(String sensorName,
								MqttLogger* myLogger,
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

	init(id);	
}*/


 	

/*
SensorHMC5883::SensorHMC5883(String sensorName,
								MqttLogger* myLogger,
								MqttInterface* interface)
								 :SensorMagneto(sensorName,
											myLogger,
											interface)
											{
	init(1);	
}

*/


void SensorHMC5883::init(uint8_t id ){
	//Serial.println("HMC5883 start");
	Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(id);
	//Serial.println("HMC5883 created");
	//declinationAngle = (declinationAngleDegres + (declinationAnglemin / 60)) * M_PI / 180;
	//lastTime=0;
	//sensorName="HMC5883";
	ErrFlag=false;
	if (!mag.begin()) {
		ErrorMsg="Could not find a valid HMC5883 sensor, check wiring!";
		Serial.println(ErrorMsg);
		ErrFlag=true;
		return;
	  }
}

	
boolean SensorHMC5883::publish(){
	if(myMqtt==NULL){
		_LOG_PRINT(E,FILE_ID," interface","NULL");
		Serial.println("SensorHMC5883::publish myMqtt NULL");
		return false ;	 		
	}
	
	 _LOG_PRINT(M,FILE_ID," publish start","");
	 //Serial.println("publish start");
	char topic[MQTT_TOPIC_MAX_SIZE];
	char payload[MQTT_PAYLOAD_MAX_SIZE];
	//getData();	
	dtostrf(headingDegrees,7, 2,payload);
	//Serial.println("headingDegrees");
	_LOG_PRINT(D,FILE_ID," headingDegrees",payload);
	sprintf(topic,"%s/Cap",sensorName.c_str());
	//Serial.println(topic);
	_LOG_PRINT(M,FILE_ID," topic",topic);
	myMqtt->publish("data",topic,payload);
	return true;
}
String SensorHMC5883::toString(){
	
	String retVal;
	mag.getEvent(&event);

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


	retVal="\n\tX:\t"+(String)event.magnetic.x;
  	retVal+="\n\tY:\t"+(String)event.magnetic.y;
  	retVal+="\n\tZ:\t"+(String)event.magnetic.z+"uT";
	retVal+="\n\tminx:\t "+(String)minx+"uT";
	retVal+="\n\tminy:\t "+(String)miny+"uT";
	retVal+="\n\tminz:\t "+(String)minz+"uT";
	retVal+="\n\tmaxx:\t "+(String)maxx+"uT";
	retVal+="\n\tmaxy:\t "+(String)maxy+"uT";
	retVal+="\n\tmaxz:\t "+(String)maxz+"uT";
	retVal+="\n\tx offset:\t "+(String)((maxx+minx)/2)+"uT";
	retVal+="\n\ty offset:\t "+(String)((maxy+miny)/2)+"uT";
	retVal+="\n\tz offset:\t "+(String)((maxz+minz)/2)+"uT";



  	retVal+="\n\tmagxy(uT):\t"+(String)magnetudexy;
  	retVal+="\n\tmag(uT)  :\t"+(String)magnetude;
	retVal+="\n\tcap (°)  :\t"+(String)headingDegrees;
	//retVal+="\n\tvitesse (N)  :\t"+(String)speed;
	return retVal;
}



 void SensorHMC5883::getData(){
	mag.getEvent(&event);
	_LOG_PRINT(D,FILE_ID, "event x :", event.magnetic.x);
	_LOG_PRINT(D,FILE_ID, "event y :", event.magnetic.y);

	magnetudexy = sqrt(sq(event.magnetic.y) + sq(event.magnetic.x));
  	magnetude = sqrt(sq(event.magnetic.y) + sq(event.magnetic.x) + sq(event.magnetic.z));
	heading = atan2(event.magnetic.y, event.magnetic.x) + declinationAngle;
	// Correct for when signs are reversed.
	if (heading < 0)
		heading += 2 * PI;
	// Check for wrap due to addition of declination.
	if (heading > 2 * PI)
    		heading -= 2 * PI;
	// Convert radians to degrees for readability.
  	headingDegrees = heading * 180 / M_PI;
 }


SensorHMC5883::~SensorHMC5883() {
	// TODO Auto-generated destructor stub
}

