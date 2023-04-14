/*
 * MqttInterfaceEsp32.cpp
 *
 *  Created on:22/08/2020
 *      Author: marc
 */
//#define NO_LOG
#include<MqttInterfaceEsp32.h>
#ifndef __AVR_ATmega2560__



MqttInterfaceEsp32::MqttInterfaceEsp32( HardwareSerial* ptOutputSer,
											Logger* myLogger,
											const char* machineName,
											const char* netName)
												:MqttInterface( machineName, netName){
	this->ptOutputSer=ptOutputSer;
	this->myLogger=myLogger;
  state=start;
	MQTTclient= new PubSubClient(espClient) ;
  if (MQTTclient==NULL) _LOG_PRINT(M," MQTTclient ","NULL");
  _LOG_PRINT(M," EndConstructor MqttInterfaceEsp8266 ","");
  
}
void MqttInterfaceEsp32::publish(const char* topic, const char* payload){
    _LOG_PRINT(M,"publish topic ",topic);
    _LOG_PRINT(M,"publish payload ",payload);
    sprintf(buffTopic,"%s/%s/%s",netName,machineName,topic);
    _LOG_PRINT(M,"buffTopic: ",buffTopic);
    if (!MQTTclient->connected()) connect();
    _LOG_PRINT(M,"MQTTclient->connected(): ",MQTTclient->connected());
    MQTTclient->publish(buffTopic,payload);
}
void MqttInterfaceEsp32::readSerial(){
	if(ptOutputSer==NULL) return;
  _LOG_PRINT(M,"readSerial Start ","");

  if (ptOutputSer->available() > 0) {
    _LOG_PRINT(D,"***************state ",state);
    char car=ptOutputSer->read();/*
    //_LOG_PRINT(D,"caractere lu  ",(int)car);
    //Serial.println(car);
   // _LOG_PRINT(D,"***************car ","");
    //Serial.print(car,HEX);
    */
    switch  (state){
      
      case start:
        if (car==START_CHAR){
          _LOG_PRINT(D,"decript","start");
          state=commande;
          posTopic=0;
          posMsg=0;
          for (int i=0;i<MSG_SIZE;i++)buffMsg[i]='\0';
          for (int i=0;i<TOPIC_SIZE;i++)buffTopic[i]='\0';
        }
        break;

      case commande:
      //_LOG_PRINT(D,"commande",car);
        if(car=='p'||car=='s'||car=='u'){
          cmd=car;
          state=endTopic;
          posTopic=0;  
         // Serial.print(cmd);        
          //_LOG_PRINT(D," commnande= ","cmd"); 
        }
        else{
              state=start; 
              _LOG_PRINT(E,"commnande inconnue (p,s,u) ",cmd);  
        }
        break;
        
      case endTopic:   //on lit topic
        _LOG_PRINT(D,"endTopic buffTopic ",buffTopic);
        _LOG_PRINT(D,"endTopic PosTopic ",posTopic);
        if (car==SEPA_CHAR){                    // fin de topic
          buffTopic[posTopic++]='\0';           //
          state=endMessage;
          posMsg=0;                              // par defaut on publie
          _LOG_PRINT(D,"fin topic ",buffTopic); //
          _LOG_PRINT(D,"fin postopic ",posTopic);
                                              //
          if (cmd=='s'){ //suscribe              //  
            _LOG_PRINT(W,"subscribe ",buffTopic);// 
            MQTTclient->subscribe(buffTopic);  //
            state=start;                        //
            break;                              //
          }                                     //
          if (cmd=='u'){ // unsubscribe          //
            _LOG_PRINT(W,"unsubscribe ",buffTopic);
            MQTTclient->unsubscribe(buffTopic);//
            state=start;                        //
            break;                              //
          }                                     //
                                         //
        }
        else{
        buffTopic[posTopic++]=car;
          if (posTopic>=TOPIC_SIZE){ 
              buffTopic[(TOPIC_SIZE-1)]='\0';
              _LOG_PRINT(E," topic overflow ",buffTopic);//
              state=start;                        // cas d erreur on ignore la suite
          }
        } 
        break;
        
      case endMessage:   // on lit message
        
        if (car==END_CHAR||car==SEPA_CHAR){ 
          _LOG_PRINT(D,"endMessage",car);                        // fin message
          buffMsg[posMsg++]='\0';                   //
          _LOG_PRINT(W,"publish topic ",buffTopic); //
          _LOG_PRINT(W,"publish msg ",buffMsg);     //
          publish(buffTopic, buffMsg);              //
          state=start;                              //
          break;                                    //
        } 
        
        buffMsg[posMsg++]=car;
        
        if(posMsg>=MSG_SIZE){
          buffMsg[MSG_SIZE-1]='\0';
          _LOG_PRINT(E," message overflow ",buffMsg);
          state=start;
        }
        _LOG_PRINT(D,"endMessage ",buffMsg);
        break;
       default:
        
          _LOG_PRINT(E," case default posfMsg ",posMsg);
          _LOG_PRINT(E," case default buffMsg ",buffMsg);
          _LOG_PRINT(E," case default posTopic ",posTopic);
          _LOG_PRINT(E," case default buffTopic ",buffTopic);
        break;
    }       
  }
  yield();
}
void MqttInterfaceEsp32::connect(){

  // Loop until we're reconnected
  while (!MQTTclient->connected()) {
    _LOG_PRINT(M,"Attempting MQTT connection...","");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (MQTTclient->connect(clientId.c_str())) {
      _LOG_PRINT(M,"connected","");

    } else {
      _LOG_PRINT(M,"failed, rc= ",MQTTclient->state());
      _LOG_PRINT(M," try again in 5 seconds","");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    yield;
  }
}


void MqttInterfaceEsp32::publishSerial(const char* ptMsg){
    String cmd,topic,payload;
    String msg=String(*ptMsg);
    /*char cmd[MSG_SIZE];
    char topic[TOPIC_SIZE];
    char msg[MSG_SIZE];*/
    // find start char
    int startPos,endPos;
    startPos=msg.indexOf(START_CHAR);
    if (startPos<0){
      _LOG_PRINT(D,"no start char ",msg);
       return;
     }
    // find commande
    endPos=msg.indexOf(SEPA_CHAR,startPos+1);
    if (endPos<0) {
      _LOG_PRINT(M,"no SEPA_CHAR1 ",msg);
       return;
     }
    cmd=msg.substring(startPos+1,endPos);
    _LOG_PRINT(D,"cmd ",cmd);
    // find topic
    startPos=endPos;
    endPos=msg.indexOf(SEPA_CHAR,startPos+1);
    if (endPos<0) {
      _LOG_PRINT(D,"no SEPA_CHAR2 ",*ptMsg);
       return;
     }
    topic=msg.substring(startPos+1,endPos);
    _LOG_PRINT(M,"topic ",topic);
    if (cmd=="s"){
      _LOG_PRINT(W,"subscribe ",msg);
      MQTTclient->subscribe(topic.c_str());
      return;
    }
       if (cmd=="u"){
         _LOG_PRINT(W,"unsubscribe ",msg);
         MQTTclient->unsubscribe(topic.c_str());
      return;
    }
   if (cmd=="p"){
     _LOG_PRINT(W,"pulish ",msg);
      // publish find payload
      startPos=endPos;
      endPos=msg.indexOf(SEPA_CHAR,startPos+1);
      if (endPos<0) return;
      payload=msg.substring(startPos+1,endPos);
      publish(topic.c_str(),payload.c_str()); 
      return;
    }
    _LOG_PRINT(E,"commande inconnue ",cmd);
        
  }
  
  
void MqttInterfaceEsp32::setServer(const char* mqtt_server,const int port ){
	
	MQTTclient->setServer(mqtt_server, port );
}

void MqttInterfaceEsp32::loop(){
	MQTTclient->loop();
}
void MqttInterfaceEsp32::setCallback(void callback(char* topic, byte* payload, unsigned int length)){
	MQTTclient->setCallback(callback);
}
boolean MqttInterfaceEsp32::connected(){
	return MQTTclient->connected();
}
#endif

#undef NO_LOG
