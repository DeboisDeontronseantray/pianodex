#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "SSID";// your wifi ssid
const char* password = "password";//your wifi password
const char* mqtt_server = "MQTT IP";//your MQTT broker IP
WiFiClient Bench;
PubSubClient client(Bench);
long lastMsg = 0;
char msg[50];
int value = 0;
const char* LTopic = "Lock";//Topic for lock
const char* OTopic = "OverRide";//Topic for over ride
const int Lock = D1; //pin lock is connected to
void setup() {
  pinMode(Lock, OUTPUT);
  Serial.begin(9600);
  setup_wifi();                   // Connect to wifi 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    for(int i = 0; i<500; i++){
    }
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length){
  int i = 0;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
 for(i=0; i<length; i++) {
    msg[i] = payload[i];
  }
  msg[i] = '\0';
  
  String msgString = String(msg);
 
  // Switch on the LED if an 1 was received as first character
  if (strcmp(topic,LTopic)==0){  //if open is publushed to lock topic then:
    if (String(msg)=="open"){
    digitalWrite(Lock, HIGH);
    Serial.println("Lock Open"); //serial print device status 
  }
  else(String(msg)=="0");  // if 0 is publushed to lock topic then:
    digitalWrite(Lock, LOW);
    Serial.println("Lock Closed"); //serial print device status 
}
   if (strcmp(topic,OTopic)==0){  //if 1 is publushed to over ride topic then:
    if (String(msg)=="1"){  
    Serial.println("Lock Open"); //serial print device status 
  }
  else(String(msg)=="0");  // if 0 is publushed to over ride topic then:
   Serial.println("Lock Closed"); //serial print device status 
}  
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Bench")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe(LTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      for(int i = 0; i<5000; i++){
        delay(10);
      }
    }
  }
}
// the main loop will constantly check to see if the button has been pressed
// if it has, a counter is incremented, and then some action can be taken
void loop() {
if (!client.connected()) {
    reconnect();
  }
  client.loop();
}  
