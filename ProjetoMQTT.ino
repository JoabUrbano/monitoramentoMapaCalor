#include "DHT.h"
#include <stdlib.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/***** Definições de variaveis *****/

#define DHT_PIN01 15
#define DHT_PIN02 18

#define DHTTYPE DHT11

DHT dht01(DHT_PIN01, DHTTYPE);

DHT dht02(DHT_PIN02, DHTTYPE);

/***** Variaveis para conexão MQTT *****/

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

#define wifi_ssid "imd0902"
#define wifi_password "imd0902iot"
int wifi_timeout = 100000;

#define mqtt_broker "io.adafruit.com"
const int mqtt_port = 1883;
int mqtt_timeout = 10000;

#define mqtt_usernameAdafruitIO "USER"
#define mqtt_keyAdafruitIO "KEY"

/***** Configuração dos feeds *****/
Adafruit_MQTT_Client mqtt(&wifi_client, mqtt_broker, mqtt_port, mqtt_usernameAdafruitIO, mqtt_usernameAdafruitIO, mqtt_keyAdafruitIO);
Adafruit_MQTT_Publish estadoSensores = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/estadoSensores");
Adafruit_MQTT_Publish estadoSensoresHumidade = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/estadoSensoresHumidade");
Adafruit_MQTT_Publish temperaturaMediaTopico = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperaturaMedia");
Adafruit_MQTT_Publish humidadeMediaTopico = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidadeMedia");
Adafruit_MQTT_Publish temperaturaTopico01 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura01");
Adafruit_MQTT_Publish temperaturaTopico02 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura02");
Adafruit_MQTT_Publish humidadeTopico01 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade01");
Adafruit_MQTT_Publish humidadeTopico02 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade02");

/***** Funções utilizadas *****/
void imprimeSerial(float temperatura01, float temperatura02, float humidade01, float humidade02, float variacaoTemperaturaSensores, float variacaoHumidadeSensores, float temperaturaMedia, float humidadeMedia, float hic01, float hic02, bool temperaturaDiscrepante, bool humidadeDiscrepante);
void connectWiFi();
void connectMQTT();

void setup()
{
  Serial.begin(9600);

  /***** Variaveis para conexão MQTT *****/

  WiFi.mode(WIFI_STA); //"station mode": permite o ESP32 ser um cliente da rede WiFi
  WiFi.begin(wifi_ssid, wifi_password);
  connectWiFi();
  mqtt_client.setServer(mqtt_broker, mqtt_port);

  // Iniciando sensores DHT
  dht01.begin();
  dht02.begin();
}

void loop()
{
  // Verificando conexão
  if (!mqtt_client.connected()){ // Se MQTT não estiver conectado
    connectMQTT(); // Tente se conectar ao broker
  }

  if (mqtt_client.connected()){
    mqtt_client.loop();

  // Leitura da humidade dos sensores

  float humidade01 = dht01.readHumidity();

  float humidade02 = dht02.readHumidity(); //31.0

  // Leitura da temperatura em Celcius

  float temperatura01 = dht01.readTemperature();

  float temperatura02 = dht02.readTemperature(); //27.0

  // Calculo da media das netricas e a variação entre sensires
  float variacaoTemperaturaSensores = abs(temperatura01 - temperatura02);

  float variacaoHumidadeSensores = abs(humidade01 - humidade02);

  float temperaturaMedia = (temperatura01 + temperatura02) / 2;

  float humidadeMedia = (humidade01 + humidade02) / 2;

  // Checa se alguma das leituras do sensor 01 falhou e tenta novamente

  if (isnan(humidade01) || isnan(temperatura01))
  {
    Serial.println(F("Falha ao ler as informações do sensor 01!"));
    return;
  }

  // Checa se alguma das leituras do sensor 02 falhou e tenta novamente

  if (isnan(humidade02) || isnan(temperatura02))
  {
    Serial.println(F("Falha ao ler as informações do sensor 02!"));
    return;
  }

  // Capta a sensação termica
  float hic01 = dht01.computeHeatIndex(temperatura01, humidade01, false);
  float hic02 = dht01.computeHeatIndex(temperatura02, humidade02, false);

  // Checa se temperatura ou humidade estão discrepantes
  bool temperaturaDiscrepante = variacaoTemperaturaSensores > 5;
  bool humidadeDiscrepante = variacaoHumidadeSensores > 10;

  imprimeSerial(temperatura01, temperatura02, humidade01, humidade02, variacaoTemperaturaSensores, variacaoHumidadeSensores, temperaturaMedia, humidadeMedia, hic01, hic02, temperaturaDiscrepante, humidadeDiscrepante);

  // Publica uma mensagem indicando se a temperatura é discrepante ou não
  if(temperaturaDiscrepante) 
    estadoSensores.publish("Temperatura discrepante!");
  else
    estadoSensores.publish("Valores de temperatura semelhantes!");
  
  // Publica uma mensagem indicando se a humidade é discrepante ou não
  if(humidadeDiscrepante) 
    estadoSensoresHumidade.publish("Humidade discrepante!");
  else
    estadoSensoresHumidade.publish("Valores de humidade semelhantes!");
  
  // Faz a publicação dos valores de temperatura e humidade dos sensores e suas médias
  temperaturaMediaTopico.publish(temperaturaMedia);
  temperaturaTopico01.publish(temperatura01);
  temperaturaTopico02.publish(temperatura02);
  humidadeMediaTopico.publish(humidadeMedia);
  humidadeTopico01.publish(humidade01);
  humidadeTopico02.publish(humidade02);

  delay(20000);
  }
}

// Função para conectar o Esp ao Wifi
void connectWiFi() {
  Serial.print("Conectando à rede WiFi .. ");

  unsigned long tempoInicial = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - tempoInicial < wifi_timeout)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conexão com WiFi falhou!");
  } else {
    Serial.print("Conectado com o IP: ");
    Serial.println(WiFi.localIP());
  }
}

// Função para conexão no broker MQTT
void connectMQTT() {
  unsigned long tempoInicial = millis();
  while (!mqtt_client.connected() && (millis() - tempoInicial < mqtt_timeout)) {
    if (WiFi.status() != WL_CONNECTED) {
      connectWiFi();
    }
    Serial.print("Conectando ao MQTT Broker..");

    if (mqtt_client.connect("ESP32Client", mqtt_usernameAdafruitIO, mqtt_keyAdafruitIO)) {
      Serial.println();
      Serial.print("Conectado ao broker MQTT!");
    } else {
      Serial.println();
      Serial.print("Conexão com o broker MQTT falhou!");
      delay(500);
    }
  }
  Serial.println();
}

// Função para imprimir na serial os dados coletados e suas medias
void imprimeSerial(float temperatura01, float temperatura02, float humidade01, float humidade02, float variacaoTemperaturaSensores, float variacaoHumidadeSensores, float temperaturaMedia, float humidadeMedia, float hic01, float hic02, bool temperaturaDiscrepante, bool humidadeDiscrepante)
{
  Serial.println("Sensor 01: ");
  Serial.print(F("Humidade: "));
  Serial.print(humidade01);

  Serial.print(F("%  Temperatura: "));
  Serial.print(temperatura01);
  Serial.print(F("°C"));

  Serial.print(F("Sensação termica: "));
  Serial.print(hic01);
  Serial.println(F("°C "));
  Serial.println("");

  Serial.println("Sensor 02: ");
  Serial.print(F("Humidade: "));
  Serial.print(humidade02);

  Serial.print(F("%  Temperatura: "));
  Serial.print(temperatura02);
  Serial.print(F("°C"));

  Serial.print(F("Sensação termica: "));
  Serial.print(hic02);
  Serial.println(F("°C "));
  Serial.println("");

  Serial.println("******* Variação das metricas ********");
  Serial.print(F("Variação de temperatura dos sensores: "));
  Serial.print(variacaoTemperaturaSensores);
  Serial.println(F("°C ,"));
  if (temperaturaDiscrepante)
    Serial.println(F("Sensores com temperaturas muito discrepantes!"));
  Serial.println("");

  Serial.print(F("Variação da humidade dos sensores: "));
  Serial.println(variacaoHumidadeSensores);
  if (humidadeDiscrepante)
    Serial.println(F("Sensores com humidades muito discrepantes!"));
  Serial.println("");

  Serial.println("******* Temperatura e humidade media ********");
  Serial.print(F("Temperatura media da sala: "));
  Serial.println(temperaturaMedia);
  Serial.println("");

  Serial.print(F("Humidade media da sala: "));
  Serial.println(humidadeMedia);
  Serial.println("------------------------------------------------------------");
}
