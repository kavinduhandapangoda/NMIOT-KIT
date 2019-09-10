/* 
 *    *| Thingerbits Starter Kit with NodeMCU|*
 * 
 * Value of the potentiometer will send to MQTT server
 * Tutorial URL 
 * CopyRight www.Thingerbits.lk
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//  
#define Potentiometer A0

// Update these with values suitable for your network.
const char* ssid = "********";//put your hotspot ssid here
const char* password ="********";// put your hotspot password here
const char* mqtt_server = "broker.mqttdashboard.com";
//const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int threhold=50; // you might need to adjust this value to define light on/off status
void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
} //end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("OsoyooCommand");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);//the default mqqt port is 1883.
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  //send data every 6 second
  if (now - lastMsg > 500) {
     lastMsg = now;
    int val=analogRead(Potentiometer);
     String msg="The resistance value is: ";
     msg= msg+ val;
   if (val>threhold)
      msg="0: "+msg;//when the resistance value>50
    else
      msg="1: "+msg;//when the resistance value<50
     char message[58];
     msg.toCharArray(message,58);
     Serial.println(message);
  
     //publish sensor data to MQTT broker 

      client.publish("OsoyooData", message);
  }
}
