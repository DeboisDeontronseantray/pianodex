#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "SSID"; // your wifi ssid
const char* password = "password"; //your wifi password
const char* mqtt_server = "MQTT IP"; //your MQTT broker IP
WiFiClient Piano;
PubSubClient client(Piano);
long lastMsg = 0;
char msg[50];
int value = 0;
const char* LTopic = "Lock"; //Topic for lock
const char* OTopic = "OverRide"; //Topic for over ride
const int Key1= D1; //pin key 1 is connected to
const int Key2 = D4; 
const int Key3 = D3; 
const int Key4 = D2; 
const int Key5 = D7;       
int buttonPushCounter = 0;   // counter for the number of button presses
int val = 0;
int bS1 = 0;         // current state of the button
int lBS1 = 0;     // previous state of the button
int bS2 = 0;         // current state of the button
int lBS2 = 0;       // previous state of the button
int bS3 = 0;         // current state of the button
int lBS3 = 0; 
int bS4 = 0;         // current state of the button
int lBS4 = 0; 
int bS5 = 0;         // current state of the button
int lBS5 = 0; 
// configure the pins to the right mode
void setup() {
  pinMode(Key1, INPUT_PULLUP);
  pinMode(Key2, INPUT_PULLUP);
  pinMode(Key3, INPUT_PULLUP);
  pinMode(Key4, INPUT_PULLUP);
  pinMode(Key5, INPUT_PULLUP);
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
    if (client.connect("Piano")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe(OTopic);
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
  // read the state of the button
  static int buttonPushCounter = 0;
  bS1 = digitalRead(Key1);
  bS2 = digitalRead(Key2);
  bS3 = digitalRead(Key3);
  bS4 = digitalRead(Key4);
  bS4 = digitalRead(Key5);
  // check to see if it different than the last time we checked
  if (bS1 != lBS1) {
    // either the button was just pressed or just released
    if (bS1 == HIGH); {
      // it was just pressed
      buttonPushCounter = buttonPushCounter + 1;
      Serial.println(buttonPushCounter);
      if(val == 0)
      {
        val = 1;
      }
    }
  }
  lBS1 = bS1;
if (bS2 != lBS2) {
    // either the button was just pressed or just released
    if (bS2 == HIGH); {
      // it was just pressed
      buttonPushCounter = buttonPushCounter + 1;
      Serial.println(buttonPushCounter);
      if(val == 1)
      {
        val = 2;
      }
    }
  }
  lBS2 = bS2;
if (bS3 != lBS3) {
    // either the button was just pressed or just released
    if (bS3 == HIGH) {
      // it was just pressed
      buttonPushCounter = buttonPushCounter + 1;
      Serial.println(buttonPushCounter);
      if(val == 2);
      {
        val = 3;
      }
    }
  }
  lBS3 = bS3;
if (bS4 != lBS4) {
    // either the button was just pressed or just released
    if (bS4 == HIGH); {
      // it was just pressed
      buttonPushCounter = buttonPushCounter + 1;
      Serial.println(buttonPushCounter);
      if(val == 3)
      {
        val = 4;
      }
    }
  }
  lBS4 = bS4;
if (bS5 != lBS5) {
    // either the button was just pressed or just released
    if (bS5 == HIGH); {
      // it was just pressed
      buttonPushCounter = buttonPushCounter + 1;
      Serial.println(buttonPushCounter);
      if(val == 4)
      {
        val = 5;
      }
    }
  }
  lBS5 = bS5;
if(val == 5 && buttonPushCounter == 5){
  val = 0;
  buttonPushCounter = 0;
  client.publish(LTopic,"open");
}
else if(val != 5 && buttonPushCounter == 5)
{
  val = 0;
    buttonPushCounter = 0;
}
}
