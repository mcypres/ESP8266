#include"MqttLogger.h" 

extern Mqtt* myMqtt;

/**********************************************************************/


MqttLogger::MqttLogger(HardwareSerial* ptLogSer,
						const char* machineName,
						LevelValue requieredLevel):																																																	
						ptLogSer(ptLogSer),
						machineName(machineName),
						requieredLevel(requieredLevel){
	//xQueue = xQueueCreate(LOG_QUEUE_SISE, sizeof(stLogMsg));
	if (ptLogSer==NULL){
		Serial.println("ptLogSer==NULL");
	}
}

/***********************************************************************/

void MqttLogger::printLog(LevelValue level,
						const int fileId,
						const char sender[],
						const int lineNb,
						const char title[],
						const char tail[],
						const String &value){
	printSerialLog(level, fileId,sender,lineNb , title, tail, value.c_str());

}

/**********************************************************************/


void MqttLogger::printLog(LevelValue level,
							const int fileId,
							const char* sender,
							const int lineNb ,
							const char* title,
							const char tail[],
							const char* value){

	printSerialLog(level, fileId,sender,lineNb , title, tail, value);

}

/**********************************************************************/


void MqttLogger::printLog(LevelValue level,const int fileId,
							const char* sender,
							const int lineNb ,
							const char* title,
							const char tail[],
							char value){
	char vOut []={value,0};							
	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);

}

/**********************************************************************/


void MqttLogger::printLog(LevelValue level,const int fileId,
							const char* sender,
							const int lineNb ,
							const char* title,
							const char tail[],
							unsigned char value,
							int type ){
	char vOut [11];
	sprintf(vOut,"%d",value);
	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);

}

/**********************************************************************/


void MqttLogger::printLog(LevelValue level,const int fileId,
							const char* sender,
							const int lineNb ,
							const char* title,
							const char tail[],
							int value,
							int type){
	char vOut [12];
	itoa(value,vOut,10);
	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);
}

/**********************************************************************/

		
void MqttLogger::printLog(LevelValue level,const int fileId,
							const char* sender,
							const int lineNb ,
							const char* title,
							const char tail[],
							unsigned int value,
							int type){
	char vOut [21];
	sprintf(vOut,"%d",value);
	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);
}

/**********************************************************************/

void MqttLogger::printLog(LevelValue level,const int fileId,
							const char* sender,
							const int lineNb ,
							const char* title,
							const char tail[],
							long value, 
							int type){
	char vOut [12];
	
	sprintf(vOut,"%d",value);
	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);
	}

/***********************************************************************/

void MqttLogger::printLog(LevelValue level,const int fileId,
							const char* sender,
							const int lineNb ,
							const char* title,
							const char tail[],
							unsigned long value, 
							int type){
	char vOut [12];
	sprintf(vOut,"%d",value);
	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);
	}
	
/*************************************************************************/
	
void MqttLogger::printLog(LevelValue level,const int fileId,
							const char* sender,
							const int lineNb ,
							const char* title,
							const char tail[],
							float value, 
							int type){
	char vOut [17];
	dtostrf(value,15,4,vOut);
	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);
}

/**********************************************************************/

void MqttLogger::printLog(LevelValue level,const int fileId,
								const char* sender,
								const int lineNb ,
								const char* title,
								const char tail[],
								double value, 
								int type){
	char vOut [26];
	dtostrf(value,25,6,vOut);
	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);
	}
/*
void MqttLogger::printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[], Printable& value){

	printSerialLog(level, fileId,sender,lineNb , title, tail, vOut);
	}*/
	
/******************************************************************************************************/

void MqttLogger::printSerialLog(LevelValue level,
								const int fileId,
								const char* sender,
								const int lineNb ,
								const char* title,
								const char tail[],
								const char* value){
									
	if (myLogger==NULL) return;
	
	if (level>requieredLevel){
		return;	
	}	
	if (ptLogSer==NULL){
		return;	
	}	
	if (locked==1){
		return;	
	}		
#ifdef LOG_WIFI
	//Serial.println("LOG_WIFI");
	char*buffTopic1=(char*)malloc(BUFF_TOPIC_SIZE*sizeof(char));
	stLogMsg mystLogMsg;
	stLogMsg *ptStLogMsg=&mystLogMsg;
	char buffTopic[BUFF_TOPIC_SIZE];
	char* ptr = strrchr( sender, '/' );	
	sprintf(ptStLogMsg->msgTopic,"%d/%d/%s/%d/%s",level,fileId,ptr+1,lineNb,title);	strcpy(ptStLogMsg->msgPayload,value);

#endif
#ifdef LOG_USB	
		headerLog(level,fileId,sender,lineNb,title);
		ptLogSer->print(value);
		ptLogSer->print(tail);
#endif	
}

/***********************************************************************/


/***********************************************************************/	
void MqttLogger::headerLog(LevelValue level,const int fileId,const char* sender,const int lineNb ,const char* title){
		ptLogSer->println();
		ptLogSer->print(SEP1);
		ptLogSer->print(level);
		ptLogSer->print(SEP1);
			//#define WITH_PATH
#ifdef WITH_PATH
		ptLogSer->print(sender);
#endif
#ifndef WITH_PATH
		{
			ptLogSer->print(fileId);
			ptLogSer->print(SEP1);
			String sTmp=(String) sender;
			int pos=sTmp.lastIndexOf('/');
			//	ptLogSer->print("pos =");
			//	ptLogSer->print(pos);
			if(pos>=0){
				ptLogSer->print(sTmp.substring(pos+1));
			}
			else{
				ptLogSer->print(sender);
			}
		}

#endif
		ptLogSer->print(" line : ");
		ptLogSer->print(lineNb);
		ptLogSer->print(SEP1);
		ptLogSer->print(title);
		ptLogSer->print(SEP2);
}
/**********************************************************************/
//enum LevelValue{N=0,M=1,E=2,W=3,D=4,V=5};


#define CHAR_TO_ENUM(ch,ch1) if(c==ch1){ \
			requieredLevel=ch;\
			return;\
		}
void MqttLogger::setRequieredLevel(char c){
	
	CHAR_TO_ENUM(N,'N')
	CHAR_TO_ENUM(M,'M')
	CHAR_TO_ENUM(E,'E')
	CHAR_TO_ENUM(W,'W')
	CHAR_TO_ENUM(D,'D')
	CHAR_TO_ENUM(V,'V')
	
}

