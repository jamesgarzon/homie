#include <Homie.h>
#include <DHT.h>

const int TEMPERATURE_INTERVAL = 60;
unsigned long lastTemperatureSent = 0;

const int PIN_DHT22 = 2;    // Broche - Pin DHT22
DHT dht(PIN_DHT22, DHT22);

HomieNode recordNode("record", "record");
HomieNode temperatureNode("temperature", "temperature");
HomieNode humidityNode("humidity", "humidity");

void loopHandler() {
  if (millis() - lastTemperatureSent >= TEMPERATURE_INTERVAL * 1000UL || lastTemperatureSent == 0) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    // Affiche les mesures dans le journal - send values to the logger
    Homie.getLogger() << "Temperature: " << t << " °C | humidity: " << h << "%" << endl;
    if ( !isnan(t) ) {
      temperatureNode.setProperty("temperature").send(String(t));
      temperatureNode.setProperty("json").send("{\"t\":" + String(t) + ",\"unit\":\"c\"}");
      humidityNode.setProperty("humidity").send(String(h));
      humidityNode.setProperty("json").send("{\"h\":" + String(t) + ",\"unit\":\"%\"}");
      recordNode.setProperty("json")
      .send("{\"deviceId\":\"5c6a3593309ec7fcc0224816\",\"temp\":"+ String(t) +",\"tempUnit\":\"C\",\"hum\":"+ String(h) +",\"humUnit\":\"%\"}");

      lastTemperatureSent = millis();
    }
    lastTemperatureSent = millis();
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  Homie_setFirmware("demo-dht22", "1.0.0");
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop() {
  Homie.loop();
}