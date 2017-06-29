#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RELAY_ONE 4
#define RELAY_TWO 5
#define mqtt_server "192.168.1.232"
#define light_topic "office/light"
#define fan_topic "office/fan"

const char* host = "OfficeESP";
const char* ssid = "Pirate Radio_2G";
const char* password = "!QAZ2wsx";
const char* mqtt_user = "office"; 
const char* mqtt_pass = "Evelynmayschmitz2010";
const char compile_date[] = __DATE__ " " __TIME__;

String light;
String fan;
String strTopic;
String strPayload;
char* last_state = "";
long lastMsg = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  pinMode(RELAY_ONE, OUTPUT);
  pinMode(RELAY_ONE, LOW);
  pinMode(RELAY_TWO, OUTPUT);
  pinMode(RELAY_TWO, LOW);

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
  
  client.loop();
}
  
void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  strTopic = String((char*)topic);
  if (strTopic == light_topic)
  {
    light = String((char*)payload);
    if (light == "ON")
    {
      Serial.println("ON");
      pinMode(RELAY_ONE, HIGH);
    }
    else if (light == "OFF")
    {
      Serial.println("OFF");
      pinMode(RELAY_ONE, LOW);
    }
  }
  else if (strTopic == fan_topic)
  {
    fan = String((char*)payload);
    if (fan == "ON")
    {
      Serial.println("ON");
      pinMode(RELAY_TWO, HIGH);
    }
    else if (fan == "OFF")
    {
      Serial.println("OFF");
      pinMode(RELAY_TWO, LOW);
    }
  }
}

void reconnect() {
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.print("Attempting MQTT connection...");
  if (client.connect(host, mqtt_user, mqtt_pass)) {
    Serial.println("connected");
    client.subscribe("office/#");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
  }
}
