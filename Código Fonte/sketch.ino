// Antônio Biasotti Amorim 32114141
// Augusto Koshiyama Bento 32150741
// Benjamim Yong Jin Kim 32169388
// Gabriel Moreira Cabral 32153074

#include "EspMQTTClient.h"
#include "DHTesp.h"
#include <AccelStepper.h>

AccelStepper stepper1(AccelStepper::FULL4WIRE, 13, 12, 14, 27);

EspMQTTClient client(
  "Wokwi-GUEST",         
  "",                    
  "test.mosquitto.org",  

  "mqtt-mack-pub-sub",         
  1883                   
);

const int DHT_PIN = 15;

DHTesp dhtSensor;

void setup()
{
  stepper1.setMaxSpeed(8000);
  Serial.begin(115200);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);


  client.enableDebuggingMessages();
}

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 2000;

void lerEnviarDados() {
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= sendInterval) {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    float temperature = String(data.temperature, 2).toFloat();
    client.publish("chad/temp", String(temperature) + "°C");  
    lastSendTime = currentTime;
  }
}

void onConnectionEstablished()
{

}

void loop()
{
  client.loop(); // Executa em loop
  float temperature = dhtSensor.getTemperature();
  
  if (temperature <= 20) {
    stepper1.setSpeed(0);
    stepper1.runSpeed();
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(14, LOW);
    digitalWrite(27, LOW);
    
  } else if (temperature >= 20 && temperature <25) {
    stepper1.setSpeed(500);
    stepper1.runSpeed();
    
  } else if (temperature >=25 && temperature <30) {
    stepper1.setSpeed(1000);
    stepper1.runSpeed();
    
  } else if (temperature >=30 && temperature <35) {
    stepper1.setSpeed(1500);
    stepper1.runSpeed();
    
  } else if (temperature >=35) {
    stepper1.setSpeed(2000);
    stepper1.runSpeed();
    
  }
  
  lerEnviarDados();
}