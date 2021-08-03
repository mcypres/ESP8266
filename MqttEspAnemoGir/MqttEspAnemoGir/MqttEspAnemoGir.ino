#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <SensorMLX90393.h>
//WiFi Connection configuration
char ssid[] = "BilboNet";     //  le nom du reseau WIFI
char password[] = "marccypres";  // le mot de passe WIFI
//mqtt server
//char mqtt_server[] = "127.0.0.1";  //adresse IP serveur
char mqtt_server[] = "192.168.4.1";  //adresse IP serveur

#define MQTT_USER ""
#define MQTT_PASS ""

WiFiClient espClient;
PubSubClient MQTTclient(espClient);
static boolean led;
SensorMLX90393* girouette;

//Fonctions mqtt
void MQTTsend() {
  static int cpt = 0;
  cpt++;
  String reponse = "test Mqtt n°" + (String)cpt;
  Serial.println(reponse);
  MQTTclient.publish("Bilbo/msg", reponse.c_str());
}

void MQTTconnect() {

  while (!MQTTclient.connected()) {
    Serial.print("Attente  MQTT connection...");
    String clientId = "TestClient-";
    clientId += String(random(0xffff), HEX);

    // test connexion
    if (MQTTclient.connect(clientId.c_str(), "", "")) {
      Serial.println("connected");

    } else {  // si echec affichage erreur
      Serial.print("ECHEC, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  led = true;
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  Serial.println("setup Start");
  // Conexion WIFI
  WiFi.disconnect();
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  MQTTclient.setServer(mqtt_server, 1883);
  girouette=new SensorMLX90393();
  if(girouette->getStatus()){
    Serial.println("MLX90393 OK");    
  }
  else{    
    Serial.println("MLX90393 not found");
  }
  Serial.println("******setup end*****");
}

void loop() {
  static uint32_t  lastTimeMqtt = 0;
  // connect serveur MQTT
  if (!MQTTclient.connected()) {
    
    MQTTconnect();
  }

  if (millis() - lastTimeMqtt >= 1000)  // toutes les secondes
  {
    lastTimeMqtt = millis();
    if (led == true) {
      digitalWrite(LED_BUILTIN, LOW);
      led = false;
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
      led = true;
    }

    String reponse = (String)girouette->getHeadingDeg();
    MQTTclient.publish("Bilbo/girouette", reponse.c_str());
    Serial.print("Bilbo/girouette : ");
    Serial.println(reponse);
    reponse = (String)girouette->getHeadingRad();
    MQTTclient.publish("Bilbo/girouette/rad", reponse.c_str());
   /* Serial.print("Bilbo/girouette/rad : ");
    Serial.println(reponse);*/
    reponse = (String)girouette->getBx();
    MQTTclient.publish("Bilbo/Bx", reponse.c_str());
    /*Serial.print("Bilbo/Bx : ");
    Serial.println(reponse);*/

    reponse = (String)girouette->getBy();
    MQTTclient.publish("Bilbo/By", reponse.c_str());
   /* Serial.print("Bilbo/By : ");
    Serial.println(reponse);*/
  }
}
