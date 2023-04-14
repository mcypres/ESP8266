// fichier SensorMPU6050.cpp
// auteur mwc
// date 26/12/21

#include <SensorMPU6050.h>



SensorMPU6050::SensorMPU6050(String sensorName,
			MqttLogger* myLogger,
			Mqtt* myMqtt,
			int priorityLevel,
			long intervalTime):
				myMqtt(myMqtt),
				Sensor( sensorName,
						myLogger,
						priorityLevel,
						intervalTime){
	 // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}
void SensorMPU6050::getData(){
	  
  mpu.getEvent(&acceleration, &gyro, &temp);
}
boolean  SensorMPU6050::publish(){
	if(myMqtt==NULL){
		_LOG_PRINT(M,FILE_ID," publish  myMqtt","NULL");
		return false ;	 		
	}
	
	 _LOG_PRINT(M,FILE_ID," publish start","");
	char topic[MQTT_TOPIC_MAX_SIZE];
	char payload[MQTT_PAYLOAD_MAX_SIZE];
	getData();	
	char StrAccX[6],StrAccY[10],StrAccZ[10],StrGyrX[10],StrGyrY[10],StrGyrZ[10],StrTemp[10];
	dtostrf(acceleration.acceleration.x,2, 2,StrAccX);
	dtostrf(acceleration.acceleration.y,2, 2,StrAccY);
	dtostrf(acceleration.acceleration.z,2, 2,StrAccZ);
	sprintf(payload,"%s,%s,%s",StrAccX,StrAccY,StrAccZ);

	_LOG_PRINT(D,FILE_ID," Acceleration",payload);
	sprintf(topic,"%s/Acceleration",sensorName.c_str());
	_LOG_PRINT(M,FILE_ID," topic",topic);
	myMqtt->publish("data",topic,payload);
	
	dtostrf(gyro.gyro.x,2, 2,StrGyrX);
	dtostrf(gyro.gyro.y,2, 2,StrGyrY);
	dtostrf(gyro.gyro.z,2, 2,StrGyrZ);
	sprintf(payload,"%s,%s,%s",StrGyrX,StrGyrY,StrGyrZ);

	_LOG_PRINT(D,FILE_ID," gyroscope",payload);
	sprintf(topic,"%s/gyroscope",sensorName.c_str());
	_LOG_PRINT(M,FILE_ID," topic",topic);
	myMqtt->publish("data",topic,payload);
	
	dtostrf(temp.temperature,2, 2,payload);
	_LOG_PRINT(D,FILE_ID," Temperature",payload);
	sprintf(topic,"%s/Temperature",sensorName.c_str());
	myMqtt->publish("data",topic,payload);
	return true;
}
String SensorMPU6050::toString(){
	return "";
}

