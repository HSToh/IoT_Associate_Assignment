#define S0 D0                             /* Assign Multiplexer pin S0 connect to pin D0 of NodeMCU */
#define S1 D1                             /* Assign Multiplexer pin S1 connect to pin D1 of NodeMCU */
#define S2 D2                             /* Assign Multiplexer pin S2 connect to pin D2 of NodeMCU */
#define S3 D3                             /* Assign Multiplexer pin S3 connect to pin D3 of NodeMCU */
#define SIG A0                            /* Assign SIG pin as Analog output for all 16 channels of Multiplexer to pin A0 of NodeMCU */

String readString;
float Temp1, Temp2, Temp3, Temp4, Temp5, Temp6, Temp7, Temp8;
float Vibr1, Vibr2, Vibr3, Vibr4, Vibr5, Vibr6, Vibr7, Vibr8;


//MQTT INIT
#ifdef ESP8266
#include <ESP8266WiFi.h>  // Pins for board ESP8266 Wemos-NodeMCU
#else
#include <WiFi.h>
#endif

#include <PubSubClient.h>
#include <WiFiClientSecure.h>


//---- WiFi settings
const char* ssid = "XXX";
const char* password = "YYY";


//---- MQTT Broker settings
const char* mqtt_server = "9917008b67b043eea295f801a047fc87.s1.eu.hivemq.cloud"; // replace with your broker url
const char* mqtt_username = "IoT123";
const char* mqtt_password = "IoTAssignment123";
const int mqtt_port = 8883;



WiFiClientSecure espClient;   // for no secure connection use WiFiClient instead of WiFiClientSecure
//WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

//MQTT PUBLISH TOPICS
const char*  M1_Temp = "Temp/1";
const char*  M2_Temp = "Temp/2";
const char*  M3_Temp = "Temp/3";
const char*  M4_Temp = "Temp/4";
const char*  M5_Temp = "Temp/5";
const char*  M6_Temp = "Temp/6";
const char*  M7_Temp = "Temp/7";
const char*  M8_Temp = "Temp/8";

const char*  M1_Vibr = "Vibr/1";
const char*  M2_Vibr = "Vibr/2";
const char*  M3_Vibr = "Vibr/3";
const char*  M4_Vibr = "Vibr/4";
const char*  M5_Vibr = "Vibr/5";
const char*  M6_Vibr = "Vibr/6";
const char*  M7_Vibr = "Vibr/7";
const char*  M8_Vibr = "Vibr/8";


//WiFi setup
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}


//MQTT connection
void reconnect() {
  //Loop connected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   //Create random client ID
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      //Insert subscribe to topics

    }

    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");   //Wait 3 seconds before retrying
      delay(3000);
    }
  }
}


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(S0, OUTPUT);                      // Define digital signal pin as output to the Multiplexer pin SO
  pinMode(S1, OUTPUT);                      // "                                                        " S1
  pinMode(S2, OUTPUT);                      // "                                                        " S2
  pinMode(S3, OUTPUT);                      // "                                                        " S3
  pinMode(SIG, INPUT);                      // Define analog signal pin as input or receiver from the Multiplexer pin SIG

  while (!Serial) delay(1);
  setup_wifi();

#ifdef ESP8266
  espClient.setInsecure();
#else   // for the ESP32
  espClient.setCACert(root_ca);      //enable with the "certificate" code for secure connection
#endif

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}


void loop()
{
  if (!client.connected()) reconnect();
  client.loop();
  Read_Publish();
}


//Serial read for data from Arduino Mega
void Read_Publish()
{
  // Channel 0 (C0 pin - binary output 0,0,0,0)
  digitalWrite(S0, LOW); digitalWrite(S1, LOW); digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  Temp1 = analogRead(SIG);
  publishMessage(M1_Temp, String(Temp1), true);

  // Channel 1 (C1 pin - binary output 1,0,0,0)
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  Vibr1 = analogRead(SIG);
  publishMessage(M1_Vibr, String(Vibr1), true);

  // Channel 2 (C2 pin - binary output 0,1,0,0)
  digitalWrite(S0, LOW); digitalWrite(S1, HIGH); digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  Temp2 = analogRead(SIG);
  publishMessage(M2_Temp, String(Temp2), true);

  // Channel 3 (C3 pin - binary output 1,1,0,0)
  digitalWrite(S0, HIGH); digitalWrite(S1, HIGH); digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  Vibr2 = analogRead(SIG);
  publishMessage(M2_Vibr, String(Vibr2), true);

  // Channel 4 (C4 pin - binary output 0,0,1,0)
  digitalWrite(S0, LOW); digitalWrite(S1, LOW); digitalWrite(S2, HIGH); digitalWrite(S3, LOW);
  Temp3 = analogRead(SIG);
  publishMessage(M3_Temp, String(Temp3), true);

  // Channel 5 (C5 pin - binary output 1,0,1,0)
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); digitalWrite(S2, HIGH); digitalWrite(S3, LOW);
  Vibr3 = analogRead(SIG);
  publishMessage(M3_Vibr, String(Vibr3), true);

  // Channel 6 (C6 pin - binary output 0,1,1,0)
  digitalWrite(S0, LOW); digitalWrite(S1, HIGH); digitalWrite(S2, HIGH); digitalWrite(S3, LOW);
  Temp4 = analogRead(SIG);
  publishMessage(M4_Temp, String(Temp4), true);

  // Channel 7 (C7 pin - binary output 1,1,1,0)
  digitalWrite(S0, HIGH); digitalWrite(S1, HIGH); digitalWrite(S2, HIGH); digitalWrite(S3, LOW);
  Vibr4 = analogRead(SIG);
  publishMessage(M4_Vibr, String(Vibr4), true);

  // Channel 8 (C8 pin - binary output 0,0,0,1)
  digitalWrite(S0, LOW); digitalWrite(S1, LOW); digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  Temp5 = analogRead(SIG);
  publishMessage(M5_Temp, String(Temp5), true);

  // Channel 9 (C9 pin - binary output 1,0,0,1)
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  Vibr5 = analogRead(SIG);
  publishMessage(M5_Vibr, String(Vibr5), true);

  // Channel 10 (C10 pin - binary output 0,1,0,1)
  digitalWrite(S0, LOW); digitalWrite(S1, HIGH); digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  Temp6 = analogRead(SIG);
  publishMessage(M6_Temp, String(Temp6), true);

  // Channel 11 (C11 pin - binary output 1,1,0,1)
  digitalWrite(S0, HIGH); digitalWrite(S1, HIGH); digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  Vibr6 = analogRead(SIG);
  publishMessage(M6_Vibr, String(Vibr6), true);

  // Channel 12 (C12 pin - binary output 0,0,1,1)
  digitalWrite(S0, LOW); digitalWrite(S1, LOW); digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  Temp7 = analogRead(SIG);
  publishMessage(M7_Temp, String(Temp7), true);

  // Channel 13 (C13 pin - binary output 1,0,1,1)
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  Vibr7 = analogRead(SIG);
  publishMessage(M7_Vibr, String(Vibr7), true);

  // Channel 14 (C14 pin - binary output 0,1,1,1)
  digitalWrite(S0, LOW); digitalWrite(S1, HIGH); digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  Temp8 = analogRead(SIG);
  publishMessage(M8_Temp, String(Temp8), true);

  // Channel 15 (C15 pin - binary output 1,1,1,1)
  digitalWrite(S0, HIGH); digitalWrite(S1, HIGH); digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  Vibr8 = analogRead(SIG);
  publishMessage(M8_Vibr, String(Vibr8), true);

}


//Publish functions for data as strings
void publishMessage(const char* topic, String payload , boolean retained) {
  client.publish(topic, (byte*) payload.c_str(), 7, true);
  //  Serial.println("Message publised [" + String(topic) + "]: " + payload);
}
