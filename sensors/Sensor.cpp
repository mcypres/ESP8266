/*
 * Sensor.cpp
 *
 *  Created on: 1 août 2015
 *      Author: marc
 */

#include "Sensor.h"

#define FILE_ID 1

Sensor::Sensor(String sensorName,
		MqttLogger *myLogger,			
		int priorityLevel,
		long intervalTime):
			sensorName(sensorName),
			myLogger(myLogger),
			priorityLevel(priorityLevel),
			intervalTime(intervalTime)
			{
				
	_LOG_PRINT(M,FILE_ID, F("CONSTRUCTOR"),this->sensorName);
	_LOG_PRINT(M,FILE_ID, F("intervalTime"),intervalTime);
	//sensorList.push_back (this);
	
}

/*
char* Sensor::toNmea(){
	 
	return (char*)"not implemented for this device";
}
*/
/************************************************************************/
boolean Sensor::publish(char*  msg){
	/*if(interfaceCore1==NULL){
		_LOG_PRINT(E,FILE_ID,"*******ERROR******","interface==NULL");
		return false;		
	}*/
	_LOG_PRINT(E,FILE_ID,"*******ERROR******","publish not implemented for this device");
	return true; 			 	
}

/***********************************************************************/

Sensor * Sensor::testPriority(Sensor * lastSensor){
	this->priorityState+=priorityLevel;
	_LOG_PRINT(D,FILE_ID, F(" priorityState" ), priorityState);
	if(lastSensor==NULL) return this;
	if (this->priorityState> lastSensor->priorityState){
		this->priorityState=1;
		_LOG_PRINT(D,FILE_ID, F(" this->priorityState> lastSensor->priorityState" ), ""); 
		return this;
	}
	//this->priorityState+=priorityLevel;
	_LOG_PRINT(D,FILE_ID, F(" this->priorityState" ), this->priorityState); 
	return lastSensor;
}

/***********************************************************************/

void Sensor::checksum(unsigned char* ptr,char* ptChk,int msgLength,int* ptChkInt){
     unsigned char checksum;
     checksum = 0;
     _LOG_PRINT(V,FILE_ID,"ptr input =",(char *)ptr);
     _LOG_PRINT(V,FILE_ID,"msgLength input =",msgLength);
     if ( *ptr == '$' ) ptr++;
     for (int i=0;i<msgLength;i++){
 
		_LOG_PRINT(V,FILE_ID,"i =",i);
		if(i==0||i==1|| i==msgLength-1)
		_LOG_PRINT(V,FILE_ID,"  *ptr :",(char)*ptr);
		_LOG_PRINT(V,FILE_ID,"  checksum :",checksum);
		
       checksum ^= *ptr;
       //Serial.println(checksum,HEX);
       
       _LOG_PRINT_HEX(V,FILE_ID,"  checksum :",checksum);
       ptr++;
     }
     char tmp[3];
     itoa((int)checksum,tmp,HEX);
     _LOG_PRINT(V,FILE_ID,"  tmp :",tmp);
     strncpy((char*)ptChk,tmp,3);
     if (ptChkInt!=NULL){
     	*ptChkInt=(int)checksum;
     }
     
}

/***********************************************************************/

boolean Sensor::isTimeToSend(){
	if(lastTimeLastSend+intervalTime<millis()){
		lastTimeLastSend=millis();
		return true;
	}
	else{
		//lastTimeLastSend+=intervalTime;
		return false;
	}
}
	

/***********************************************************************/	
String Sensor::getSensorDetail(){
	return sensorName;
}
/***********************************************************************/
Sensor::~Sensor() {
	ErrFlag=false;
	ErrorMsg="";
}

#undef FILE_ID
