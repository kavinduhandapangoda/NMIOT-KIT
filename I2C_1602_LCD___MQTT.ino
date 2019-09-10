/* 
 *    *| Thingerbits Starter Kit with NodeMCU|*
 * 
 * Subscribe message from remote MQTT client and dispaly the message on the I2C led
 * Tutorial URL http://osoyoo.com/2017/05/21/nodemcu-lesson-18-i2c-1602-lcd-mqtt/
 * CopyRight www.Thingerbits.lk
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
LiquidCrystal_I2C lcd(0x27, 20, 4);/*0x27 means the address of this 1602 I2C LCD display,different lcd may have the different address,
                                     if the LCD do not work,please read below post about how to get the right address for your I2C LCD dispaly:
                                     http://osoyoo.com/2014/12/07/16x2-i2c-liquidcrystal-displaylcd/ */
// Update these with values suitable for your network.
const char* ssid = "******";//put your own wifi ssid here
const char* password = "******";// put your wifi password here
//const char* mqtt_server = "broker.mqttdashboard.com";// choose your mqtt server
const char* mqtt_server = "iot.eclipse.org";

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
  Serial.print("Thingerbits NMCU");
   Serial.println();
  Serial.print(" publish data is:");
  lcd.clear();
  {
  for(int i=0;i<length;i++)
  {
    Serial.print((char)payload[i]);  
    lcd.setCursor(0, 0);
    lcd.print("OSOYOO LCD"); // Start Print text to Line 1
    lcd.setCursor(i, 1);
    lcd.write((char)payload[i]);
    }
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
  lcd.begin();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.setCallback(callback);
  client.loop();

}
