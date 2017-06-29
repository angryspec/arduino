#include <SPI.h>
#include <EthernetV2_0.h>
#include <PubSubClient.h>

////**********START CUSTOM PARAMS******************//

//Setup network values
const char* host = "sensorcontroller";
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress server(192, 168, 1, 232);

#define W5200_CS  10
#define SDCARD_CS 4

//Define the pins
const int LIVINGROOM_PIN = 3;
const int FRONTDOOR_PIN = 5;
const int KITCHEN_PIN = 6;
const int BASEMENT_PIN = 7;

//const MQTT Params
const char* mqtt_server = "192.168.1.232";
const char* livingroom_topic = "sensor/livingroom";
const char* frontdoor_topic = "sensor/frontdoor";
const char* kitchen_topic = "sensor/kitchen";
const char* basement_topic = "sensor/basement";
const char* mqtt_user = "sensorcontroller"; 
const char* mqtt_pass = "1234546";

//Setup Variables
String livingroom;
String frontdoor;
String kitchen;
String basement;
String light;
String aux;
String strTopic;
String strPayload;
char* livingroom_state = "UNDEFINED";
char* frontdoor_state = "UNDEFINED";
char* kitchen_state = "UNDEFINED";
char* basement_state = "UNDEFINED";
char* last_state = "";
long lastMsg = 0;

//This can be used to output the date the code was compiled
const char compile_date[] = __DATE__ " " __TIME__;

//Initialize MQTT and Ethernet
EthernetClient ethclient;
PubSubClient client(ethclient);

void setup() {
  
  pinMode(LIVINGROOM_PIN, INPUT);
  pinMode(FRONTDOOR_PIN, INPUT);
  pinMode(KITCHEN_PIN, INPUT);
  pinMode(BASEMENT_PIN, INPUT);


  Serial.begin(9600);
  pinMode(SDCARD_CS,OUTPUT);
  digitalWrite(SDCARD_CS,HIGH);//Deselect the SD card
  delay(500);
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  // give the Ethernet shield a second to initialize:
  delay(500);
  Serial.println("connecting...");
  // if you get a connection, report back via serial:
  Serial.println("IP address: ");
  Serial.println(Ethernet.localIP());
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); 
}
  
void loop() {
  //If MQTT client can't connect to broker, then reconnect
  if (!client.connected()) {
    reconnect();
  }
  
  checkLivingRoomState();
  checkFrontDoorState();
  checkKitchenState();
  checkBasementState();
  
  client.loop(); //the mqtt function that processes MQTT messages
}

void checkLivingRoomState() {
  //Checks if the sensor state has changed, and MQTT pub the change
  last_state = livingroom_state; //get previous state of basement
  if (digitalRead(LIVINGROOM_PIN) == 0) // get new state of sensor
    livingroom_state = "ON";
  else if (digitalRead(LIVINGROOM_PIN) == 1)
    livingroom_state = "OFF"; 

  if (last_state != livingroom_state) { // if the state has changed then publish the change
    client.publish(livingroom_topic, livingroom_state);
    Serial.println(livingroom_state);
  }
  //pub every minute, regardless of a change.
  long now = millis();
  if (now - lastMsg > 60000) {
    lastMsg = now;
    client.publish(livingroom_topic, livingroom_state);
  }
}

void checkFrontDoorState() {
  //Checks if the sensor state has changed, and MQTT pub the change
  last_state = frontdoor_state; //get previous state of sensor
  if (digitalRead(FRONTDOOR_PIN) == 0) // get new state of sensor
    frontdoor_state = "ON";
  else if (digitalRead(FRONTDOOR_PIN) == 1)
    frontdoor_state = "OFF"; 

  if (last_state != frontdoor_state) { // if the state has changed then publish the change
    client.publish(frontdoor_topic, frontdoor_state);
    Serial.println(frontdoor_state);
  }
  //pub every minute, regardless of a change.
  long now = millis();
  if (now - lastMsg > 60000) {
    lastMsg = now;
    client.publish(frontdoor_topic, frontdoor_state);
  }
}

void checkKitchenState() {
  //Checks if the sensor state has changed, and MQTT pub the change
  last_state = kitchen_state; //get previous state of sensor
  if (digitalRead(KITCHEN_PIN) == 0) // get new state of sensor
    kitchen_state = "ON";
  else if (digitalRead(KITCHEN_PIN) == 1)
    kitchen_state = "OFF"; 

  if (last_state != kitchen_state) { // if the state has changed then publish the change
    client.publish(kitchen_topic, kitchen_state);
    Serial.println(kitchen_state);
  }
  //pub every minute, regardless of a change.
  long now = millis();
  if (now - lastMsg > 60000) {
    lastMsg = now;
    client.publish(kitchen_topic, kitchen_state);
  }
}

void checkBasementState() {
  //Checks if the sensor state has changed, and MQTT pub the change
  last_state = basement_state; //get previous state of sensor
  if (digitalRead(BASEMENT_PIN) == 0) // get new state of sensor
    basement_state = "ON";
  else if (digitalRead(BASEMENT_PIN) == 1)
    basement_state = "OFF"; 

  if (last_state != basement_state) { // if the state has changed then publish the change
    client.publish(basement_topic, basement_state);
    Serial.println(basement_state);
  }
  //pub every minute, regardless of a change.
  long now = millis();
  if (now - lastMsg > 60000) {
    lastMsg = now;
    client.publish(basement_topic, basement_state);
  }
}

void reconnect() {
  Serial.println();
  Serial.print("Lost connection to server!");
  Serial.println();
  Serial.print("Attempting MQTT connection...");
  if (client.connect(host, mqtt_user, mqtt_pass)) {
    Serial.println("connected");
    client.subscribe("sensor/#");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
}

