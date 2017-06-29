#include <ESP8266WiFi.h>      
#include <PubSubClient.h>         

#define DOOR_STATE_PIN 4
#define DOOR_RELAY_PIN 5
#define LIGHT_RELAY_PIN 6

const char* host = "GarageESP";
const char* ssid = "Pirate Radio_2G";
const char* password = "123456";
const char* mqtt_server "192.168.1.232"
const char* mqtt_user = "garagedoor"; 
const char* mqtt_pass = "123456";
const char* door_topic "garage/door"
const char* button_topic "garage/button"
const char* light_topic "garage/light"

String door;
String strTopic;
String strPayload;
char* door_state = "UNDEFINED";
char* last_state = "";
long lastMsg = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  
  pinMode(DOOR_RELAY_PIN, OUTPUT);
  pinMode(DOOR_RELAY_PIN, LOW);
  pinMode(DOOR_STATE_PIN, INPUT);

  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  checkDoorState();
  client.loop()
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  strTopic = String((char*)topic);
  if (strTopic == button_topic)
  {
    door = String((char*)payload);
    if (door == "OPEN")
    {
      Serial.println("ON");
      pinMode(DOOR_RELAY_PIN, HIGH);
      delay(600);
      pinMode(DOOR_RELAY_PIN, LOW);
    }
  }
}

void checkDoorState() {
  
  last_state = door_state;
  
  if (digitalRead(DOOR_STATE_PIN) == 0) 
    door_state = "OPENED";
  else if (digitalRead(DOOR_STATE_PIN) == 1)
    door_state = "CLOSED"; 

  if (last_state != door_state) { e
    client.publish(door_topic, door_state);
    Serial.println(door_state);
  }
  
  long now = millis();
  
  if (now - lastMsg > 60000) {
    lastMsg = now;
    client.publish(door_topic, door_state);
  }
}

void reconnect() {
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.print("Attempting MQTT connection...");
  
  if (client.connect(host, mqtt_user, mqtt_pass)) {
    Serial.println("connected");
    client.subscribe("garage/#");
  } 
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
}
