
/* 
* Mqtt.cpp  
* Marc Cypres
* 10/05/2022
* */


#include <Mqtt.h>

Mqtt::Mqtt(String subScribeTopic):  
	      subScribeTopic( subScribeTopic){
    Serial.print("subScribe to Topic ");
    Serial.println(subScribeTopic);
    connect();
}
/***********************************************************************/


void Mqtt::loop(){
  MQTTclient->loop();
}     

void Mqtt::connect(){
  int nbtest=0;
  while (MQTTclient==NULL){
    nbtest++;
    //_LOG_PRINT(M," MQTTclient ","NULL");
    MQTTclient= new PubSubClient(espClient) ;
    MQTTclient->setServer(mqtt_server, 1883);
    MQTTclient->setCallback(callback);		
  }	
  Serial.print("!MQTTclient!=NULL after ");
  Serial.println(nbtest);
  // Loop until we're reconnected
  int nbConnect=0;
  while (!MQTTclient->connected()) {
    Serial.println("!MQTTclient->connected");
    //MQTTclient->disconnect ();		
    //Serial.println("MQTTclient->disconnect ");		
    // Create a random client ID
    String clientId =machineName;;
    clientId += String(random(0xffff), HEX);		
    // Attempt to connect
    if (MQTTclient->connect(clientId.c_str())) { //connected
      MQTTclient->subscribe((subScribeTopic+(String)"/#").c_str());
      Serial.print("\n connected with clientId ");
      Serial.println(clientId.c_str());
      Serial.print("\t reconnected subscribe to ");
      Serial.println(subScribeTopic+"/#");
      
      break;
    }
    else { //connection failed 
      nbConnect++;
      if (nbConnect> 10){
		ESP.restart();
		break;
      }
      else{
		Serial.print("not connected code ");
		Serial.println(MQTTclient->state());
		delay(2000);
      }
    }
    yield;
  }
	//Serial.print("connected ");
}

	
void Mqtt::publish(const char* msgtype,
		      const char* topic,
		      const char* payload){
	
    char buffTopic[TOPIC_SIZE];
    sprintf(buffTopic,"%s/%s/%s/%s",ssid,msgtype,machineName,topic);
    if(MQTTclient==NULL){
      connect();
    }
    else{
      if (!MQTTclient->connected()) {
	connect();
      }
    }
    //_LOG_PRINT(D,"MQTTclient->connected(): ",MQTTclient->connected());
   // Serial.print(buffTopic);
    //Serial.print("\t");
    //Serial.println(payload);
    MQTTclient->publish(buffTopic,payload);
}
	
/***********************************************************************/	  
	
