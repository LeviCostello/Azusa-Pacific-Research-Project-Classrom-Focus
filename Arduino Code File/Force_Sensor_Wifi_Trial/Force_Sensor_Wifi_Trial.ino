//WiFi Connection
#include <ESP8266WiFiMulti.h>

//MQTT
#include <PubSubClient.h>

//WiFi Connection
ESP8266WiFiMulti wifiMulti;       // Create WiFiMulti object

const char* mqtt_server = "broker.hivemq.com";
WiFiClient espClient;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient client(mqtt_server, 1883, callback, espClient);

char* pubTopic = "apu/cs495/02/Force";
char* clientID = "APU-Costello-ESP1";

//Variables    
int forcePin = A0;
int forceReading;

void setup(void) 
{
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println("Start");
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);  // turn off LED

  //WiFi
  wifiMulti.addAP("FreedomPop_350", "freedompop");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("APU-MYDEVICES");
  wifiMulti.addAP("Costello","Fury-1268");

  //print mac address
  Serial.println("Connecting ...");
  connectWiFi();

  Serial.println("MQTT client started");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

void loop(void) 
{
 if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  //reading the force
  forceReading = analogRead(forcePin);
  Serial.print("Analog reading = ");
  Serial.println(forceReading);
 
  delay(500);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    // Connect MQTT
//    if (client.connect(clientID, username, password)) {
    if (client.connect(clientID)) {
      Serial.println("connected");
      //Subscribe
      //      client.subscribe(subTopic, 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void connectWiFi() {
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
  }
  digitalWrite(BUILTIN_LED, LOW);

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());               // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
}
