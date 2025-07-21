#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <LittleFS.h>

#define DigPin1 D1
#define DigPin2 D2
#define DigPin3 D3
#define DigPin4 D4
#define AnalPin1 A0
#define AnalPin2 A1

WiFiClient espClient;
PubSubClient client(espClient);

const int csPin = 15;
const int resetPin = -1;
const int irqPin = 10;

String outgoing;
byte localAddress = 0xF0;
byte destination = 0;

char P_STATUS;
const char* ssid;
const char* password;
const char* mqtt_server;
const char* mqtt_username;
const char* mqtt_password;
unsigned long LB;
unsigned long SignalBandwidth;
unsigned long SpreadingFactor;
unsigned long SyncWord;
unsigned long CodingRate4;
unsigned long TxPower;
unsigned long Gain;

String clientId = "MEHREGAN\\GW6";

void setup_CONFIG() {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  File file = LittleFS.open("/example.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  // Read text from file
  String text = file.readString();
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, text);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  ssid = doc["ssid"];
  password = doc["password"];
  mqtt_server = doc["mqtt_server"];
  mqtt_username = doc["mqtt_username"];
  mqtt_password = doc["mqtt_password"];
  LB = doc["LB"];
  SignalBandwidth = doc["SignalBandwidth"];
  SpreadingFactor = doc["SpreadingFactor"];
  SyncWord = doc["SyncWord"];
  CodingRate4 = doc["CodingRate4"];
  TxPower = doc["TxPower"];
  Gain = doc["Gain"];
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  setup_lora();
  file.close();

  delay(100);
}

void setup_JSON() {
  while (!Serial.available());
  String input = Serial.readStringUntil('\n');
  input.trim();
  File file = LittleFS.open("/example.txt", "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  // Truncate file to clear previous content
  file.truncate(0);
  // Write text to file
  file.println(input);
  // Close the file
  file.close();
  delay(1000);
  setup_CONFIG();
}

void setup_wifi() {
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (millis() - startTime > 30000) {
      Serial.println("Connection timed out.");
      delay(1000);
      ESP.restart();
    }
  }
  Serial.println("ssid:");
  Serial.print(ssid);
  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Mac address: ");
  Serial.println(WiFi.macAddress());
}

void setup_lora() {
  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(LB)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSignalBandwidth(SignalBandwidth);
  LoRa.setSpreadingFactor(SpreadingFactor);
  LoRa.setSyncWord(SyncWord);
  LoRa.setCodingRate4(CodingRate4);
  LoRa.setTxPower(TxPower);
  LoRa.setGain(Gain);

  Serial.println("LoRa initialization successful!");
}

void PIN_STATUS(){

  if (Serial.available() > 0) {
P_STATUS = Serial.read();
    if (P_STATUS == 'T'){  // Print the temperature on the LCD;
  StaticJsonDocument<200> doc;
  doc["pin1"] = digitalRead(DigPin1);
  doc["pin2"] = digitalRead(DigPin2);
  doc["pin3"] = digitalRead(DigPin3);
  doc["pin4"] = digitalRead(DigPin4);
  doc["pin5"] = analogRead(AnalPin1);
  doc["pin6"] = analogRead(AnalPin2);
  char PinStatus[512];
  serializeJson(doc, PinStatus);
  Serial.print(PinStatus);
    }
  delay(5000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();

  // Handle message here
}

void setup() {
  Serial.begin(9600);

  setup_CONFIG();
//  setup_JSON();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available() > 0) {
    Serial.print("Received New Config ");
    setup_JSON();
    delay(1000);
    Serial.println("New config applied!");
  }
}

void reconnect() {
  unsigned long startTime = millis();
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("CONNECT", clientId.c_str());
      // client.subscribe(control_topic);
      // client.subscribe(config_topic);
      // client.subscribe(check_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }

    if (millis() - startTime > 20000) {
      Serial.println("Connection timed out. ESP restart");
      delay(1000);
      ESP.restart;
    }
  }
}
