#include"MqttLogger.h" 

	MqttLogger::MqttLogger(PubSubClient* mqtt_client,char* machinName,LevelValue level):
							Logger(NULL,machinName, level){																																																	
		this->mqtt_client=mqtt_client;
		if (this->mqtt_client==NULL){
			Serial.println("this->mqtt_client==NULL");
		}
		//mMachineName=machinName;
		//mRequieredLevel=level;
		//mqtt_client->println("logger onstructeur argument");
		//this->mqtt_client->println("logger onstructeur");
		
	}
	
/*
	void MqttLogger::publishLog(LevelValue level,char* sender,char* title,const __FlashStringHelper *value){
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			mqtt_client->print(*value);
			mqtt_client->print(tail);
		}*/
		//int level,char* sender,char* title, String &value

	void MqttLogger::publishLog(LevelValue level,const char sender[],const int lineNb,const char title[],const char tail[], String &value){
		Serial.println("publishLog start");
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			mqtt_client->publish(buffTopic,value.c_str());
		}
	}
	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[], char* value){
		Serial.println("publishLog start");

		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			mqtt_client->publish(buffTopic,value);
		}
	}

	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],char value){
		Serial.println("publishLog start");

		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			mqtt_client->publish(buffTopic,&value);
		}
	}
	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned char value, int type ){
		Serial.println("publishLog start");
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			sprintf(buffMsg,"%c",value);
			mqtt_client->publish(buffTopic,buffMsg);
		}
	}
	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],int value, int type){
	Serial.println("publishLog start");
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			sprintf(buffMsg,"%d",value);
			mqtt_client->publish(buffTopic,buffMsg);
		}
	}
		
	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned int value, int type){
	Serial.println("publishLog start");
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			sprintf(buffMsg,"%d",value);
			mqtt_client->publish(buffTopic,buffMsg);		}
	}
	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],long value, int type){
	Serial.println("publishLog start");
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			sprintf(buffMsg,"%d",value);
			mqtt_client->publish(buffTopic,buffMsg);		}
	}
	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned long value, int type){
	Serial.println("publishLog start");
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			sprintf(buffMsg,"%d",value);
			mqtt_client->publish(buffTopic,buffMsg);		}
	}
		void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],float value, int type){
	Serial.println("publishLog start");
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			dtostrf(value,6,2,buffMsg);
			mqtt_client->publish(buffTopic,buffMsg);	
		}
	}
	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],double value, int type){
	Serial.println("publishLog start");
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			dtostrf(value,6,2,buffMsg);
			mqtt_client->publish(buffTopic,buffMsg);
		}
	}

/*	void MqttLogger::publishLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[], Printable& value){
		if (level<=mRequieredLevel&&mqtt_client!=NULL){
			topicLog(sender,lineNb,title);
			mqtt_client->publish(buffTopic,
			mqtt_client->print(value);
			mqtt_client->print(tail);
		}
	}*/

	void MqttLogger::topicLog(const char* sender,const int lineNb ,const char* title){
	
		//mqtt_client->println();
			//mqtt_client->print(SEP1);
			//#define WITH_PATH
#ifdef WITH_PATH
			//mqtt_client->print(sender);
			sprintf(buffTopic,"Log%s%s%s line : %d%s%s",SEP1,sender,SEP1,lineNb,SEP1,title);
#endif
#ifndef  WITH_PATH
			{
				String sTmp=(String) sender;
				int pos=sTmp.lastIndexOf('/');
			//	mqtt_client->print("pos =");
			//	mqtt_client->print(pos);
				if(pos>=0){
				
					sprintf(buffTopic,"Log%s%s%s line : %d%s%s",SEP1,sTmp.substring(pos+1).c_str(),SEP1,lineNb,SEP1,title);
					//mqtt_client->print(sTmp.substring(pos+1));
				}
				else{
					//mqtt_client->print(sender);
					sprintf(buffTopic,"Log%s%s%s line : %d%s%s",SEP1,sender,SEP1,lineNb,SEP1,title);
				}
			}
#endif
			Serial.print("MqttLogger:: buffTopic ");
			Serial.println(buffTopic);
		/*	mqtt_client->print(" line : ");
			mqtt_client->print(lineNb);
			mqtt_client->print(SEP1);
			mqtt_client->print(title);
			mqtt_client->print(SEP2);*/
	}
