/* 
 *    *| Thingerbits Starter Kit with NodeMCU|*
 * 
 * Recive the data published by the MQTT client and display it on the 4-digital display
 * Tutorial URL 
 * CopyRight www.Thingerbits.lk
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TM1637Display.h>

// Update these with values suitable for your network.
const char* ssid = "********";//put your wifi ssid here
const char* password = "********";//put your wifi password here
const char* mqtt_server = "broker.mqttdashboard.com";
//const char* mqtt_server = "iot.eclipse.org";

const int CLK = D2; //Set the CLK pin connection to the display
const int DIO = D3; //Set the DIO pin connection to the display

TM1637Display display(CLK, DIO); //set up the 4-Digit Display.
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
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
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
   Serial.println();
  Serial.print(" publish data is:");
  for(int i=0;i<length;i++)
  {
    display.showNumberDec((char)payload[i],false,4,0); //Display the numCounter value;
    Serial.print(payload[i]);
  }
  Serial.println();
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
  client.setServer(mqtt_server, 1883);
  display.setBrightness(0x0a); //set the diplay to maximum brightness
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.setCallback(callback);
  client.loop();

}
