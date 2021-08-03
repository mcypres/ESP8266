//Esp8266CompasMqtt.ino
/*
 * lit le senseur HMC5883 ou QMC5883 et envoie l azimut par mqtt
 * 
 */
#define HMC

#include <ESP8266WiFi.h>  //For ESP8266
#include <PubSubClient.h> //For MQTT
#include <ESP8266mDNS.h>  //For OTA
#include <MqttInterfaceEsp32.h>
#include <WiFiUdp.h>      //For OTA
#include <ArduinoOTA.h>   //For OTA

#include"MqttLogger.h"
#include <MqttInterfaceEsp32.h>

#ifdef HMC
#include <SensorHMC5883.h>
#else
#include <SensorQMC5883.h>
#endif
//WIFI configuration

#define WIFI_SSID "BilboNet"
#define WIFI_PASSWORD "marccypres"
#define HOST_NAME "esp8266Compas"

//MQTT configuration

#define mqtt_server "192.168.4.1"
//#define mqtt_user "esp8266"
//#define mqtt_password "esp8266password"
#define mqtt_user ""
#define mqtt_password ""

#define SENSOR_NAME "Compas magnetique"
String mqtt_client_id= HOST_NAME;   //This text is concatenated with ChipId to get unique client_id

//MQTT Topic configuration

//String mqtt_base_topic=WIFI_SSID+"/"+mqtt_client_id+"/data/";

String mqtt_base_topic=WIFI_SSID+String("/")+mqtt_client_id+String("/data/");
#define humidity_topic "/humidity"
#define temperature_topic "/temperature"

//MQTT client
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

//Necesary to make Arduino Software autodetect OTA device
WiFiServer TelnetServer(8266);
 
MqttLogger* myLogger;

MqttInterfaceEsp32* interface;
#ifdef HMC
SensorHMC5883* myCompass;
#else
SensorQMC5883* myCompass;
#endif
/*****************************************************************/
void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  //_LOG_PRINT(M, F("Connecting to "), WIFI_SSID);

  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
  Serial.print("   IP address: ");
  Serial.println(WiFi.localIP());
}

/*********************************************************************/

void setup() { 
  Serial.begin(115200);
  Serial.println("\r\nBooting...");
   setup_wifi();

 Serial.print("Configuring OTA device...");
  TelnetServer.begin();   //Necesary to make Arduino Software autodetect OTA device  
  ArduinoOTA.onStart([]() {Serial.println("OTA starting...");});
  ArduinoOTA.onEnd([]() {Serial.println("OTA update finished!");Serial.println("Rebooting...");});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {Serial.printf("OTA in progress: %u%%\r\n", (progress / (total / 100)));});  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA OK");
  myLogger =new MqttLogger(&mqtt_client,HOST_NAME,D);

  Serial.println("Configuring MQTT server...");

  
  mqtt_client_id=mqtt_client_id+ESP.getChipId();
  //mqtt_base_topic="/sensor/"+mqtt_client_id+"/data";
  mqtt_client.setServer(mqtt_server, 1883);
  Serial.printf("   Server IP: %s\r\n",mqtt_server);  
  Serial.printf("   Username:  %s\r\n",mqtt_user);
  Serial.println("   Cliend Id: "+mqtt_client_id);  
  Serial.println("   MQTT configured!");

// setup sersensors

  interface=new MqttInterfaceEsp32(&Serial,
                (Logger*) myLogger,
                HOST_NAME,
                WIFI_SSID);
 #ifdef HMC               
  myCompass=new   SensorHMC5883(SENSOR_NAME, (Logger*) myLogger,interface); 
  Serial.println("SensorHMC5883 ok");
 #else
  myCompass=new   SensorQMC5883(SENSOR_NAME, (Logger*) myLogger,interface); 
   Serial.println("SensorQMC5883 ok");
#endif
 
}

/*************************************************************************/

boolean mqtt_reconnect() {
  // Loop until we're reconnected
  //while (!mqtt_client.connected()) {
    
  if (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {    
    //if (mqtt_client.connect(mqtt_client_id.c_str())) {
    //if (mqtt_client.connect(mqtt_client_id.c_str(), mqtt_user, mqtt_password)) {
        // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("connected");
      return true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 1 seconds before retrying
      delay(1000);
      return false;
      
    }
  }
}

/*******************************************************************************/
bool checkBound(float newValue, float prevValue, float maxDiff) {
  return(true);
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}



long now =0; //in ms
long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;
int min_timeout=10000; //in ms


/****************************************************************/
void loop() {
  boolean connected;
  ArduinoOTA.handle();
  
  
  if (!mqtt_client.connected()) {
    connected=mqtt_reconnect();
  }
  else {
    connected=true;
  }
  if (connected==true){
    mqtt_client.loop();
    now = millis();
    if (now - lastMsg > min_timeout) {
      Serial.println("test log_print");
       _LOG_PRINT(M, "LOOP", "VIVANT");
       Serial.println("test2 log_print");
       myLogger->publishLog(M,"test",134,"titre","tail","value");
       Serial.println("test3 log_print");
      lastMsg = now;
      now = millis();
      _LOG_PRINT(M, "now", now);
      float newTemp = temp+2;//hdc.readTemperature();
      float newHum = hum+0.5;//hdc.readHumidity();
      _LOG_PRINT(M, "newHum", newHum);
      if (checkBound(newTemp, temp, diff)) {
        temp = newTemp;
        Serial.print("Sent ");
        Serial.print(String(temp).c_str());
        Serial.println(" to "+mqtt_base_topic+temperature_topic);
        mqtt_client.publish((mqtt_base_topic+temperature_topic).c_str(), String(temp).c_str(), true);
        if (myCompass!=NULL){
          myCompass->publish();
        }
        else{
          _LOG_PRINT(M, "myCompass", "null");  
        }
      }
    }
    
  //more code
  }
}
