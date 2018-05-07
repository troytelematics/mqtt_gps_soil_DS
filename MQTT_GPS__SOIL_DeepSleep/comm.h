void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  //Serial.println(SSID);
WiFi.begin();
  //WiFi.begin(SSID, PASSWORD);
  delay(500);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// The callback is for receiving messages back from other sources
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

// Reconnect to the network if connection is lost
void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // mqttClient.publish("TT", "Connected to Mqtt");
      // ... and resubscribe
      mqttClient.subscribe("TT");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Setup Mqtt
void setupMqtt()
{
  mqttClient.setServer(MQTT_SERVER, 1883);
  mqttClient.setCallback(callback);
  while (!mqttClient.connected()) {
    reconnect();
  }
  //Serial.println();
  //Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
}

void sendJS(gps_info  gps_data ,int n, String sType[], float sValue[], String sUom[]) {
  String msg;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["macid"] = cMacId;
  root["utime"] = gps_data.utime;
  root["lat"] = gps_data.lat;
  root["lon"] = gps_data.lon;
Serial.println(gps_data.utime);
  //Create the array of info
  JsonArray& readings = root.createNestedArray("readings");

  for (int i = 0; i < n; i++) {
    JsonObject& reading = readings.createNestedObject();
    reading["type"] = sType[i];
    reading["value"] = sValue[i];
    reading["uom"] = sUom[i];
  }
  root.printTo(msg);
  String strs = "TT/" + String(cMacId);
  //Publish the data
  mqttClient.publish(strs.c_str(), msg.c_str());
}

/*
// Format the message and send via MQTT
void sendJS( ulong uTime, float lat, float lon ) {
  String msg;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //Get The MAC id
  // uint64_t chipid = ESP.getEfuseMac();
  // char cMsg[16];
  // snprintf (cMacId, 16, "%04X%8x", (uint16_t)(chipid >> 32), (uint32_t)chipid);
  root["macid"] = cMacId;
  root["utime"] = uTime;
  root["lat"] = lat;
  root["lon"] = lon;

  root.printTo(msg);
  String strs = "TT/" + String(cMacId);
  Serial.println(strs);
  mqttClient.publish(strs.c_str(), msg.c_str());
}
*/

