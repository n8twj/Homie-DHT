#include <Homie.h>
#include "DHT.h"

#define DHTPIN D1 // NodeMCU specific PIN mapping
#define DHTTYPE DHT22
#define INTERVAL 60

unsigned long lastSent = 0;

DHT dht(DHTPIN, DHTTYPE);

HomieNode temperatureNode("temperature", "temperature");
HomieNode humidityNode("humidity", "humidity");
HomieNode heatIndexNode("heatIndex", "heatIndex");

void setupHandler() {
  Homie.setNodeProperty(temperatureNode, "unit").setRetained(true).send("F");
  Homie.setNodeProperty(humidityNode, "unit").setRetained(true).send("percent");
  Homie.setNodeProperty(heatIndexNode, "unit").setRetained(true).send("F");
}

void loopHandler() {
   if (millis() - lastSent >= INTERVAL * 1000UL || lastSent == 0) {
     float humidity = dht.readHumidity();
     float temperature = dht.readTemperature(true);
     float heatIndex = dht.computeHeatIndex(temperature, humidity);
     Homie.setNodeProperty(temperatureNode, "F").send(String(temperature));
     Homie.setNodeProperty(humidityNode, "percent").send(String(humidity));
     Homie.setNodeProperty(heatIndexNode, "F").send(String(heatIndex));
     lastSent = millis();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Homie_setFirmware("homie-dht", "1.0.0");
  
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);
  
  temperatureNode.advertise("unit");
  temperatureNode.advertise("temperature");
  humidityNode.advertise("unit");
  humidityNode.advertise("percent");
  heatIndexNode.advertise("unit");
  heatIndexNode.advertise("temperature");

  Homie.setup();
}

void loop() {
  Homie.loop();
}
