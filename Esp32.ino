#include <stdlib.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

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
/*
Adafruit_MQTT_Client mqtt(&wifi_client, mqtt_broker, mqtt_port, mqtt_usernameAdafruitIO, mqtt_usernameAdafruitIO, mqtt_keyAdafruitIO);
Adafruit_MQTT_Publish estadoSensores = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/estadoSensores");
Adafruit_MQTT_Publish estadoSensoresHumidade = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/estadoSensoresHumidade");
Adafruit_MQTT_Publish temperaturaMediaTopico = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperaturaMedia");
Adafruit_MQTT_Publish humidadeMediaTopico = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidadeMedia");
Adafruit_MQTT_Publish temperaturaTopico01 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura01");
Adafruit_MQTT_Publish temperaturaTopico02 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura02");
Adafruit_MQTT_Publish humidadeTopico01 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade01");
Adafruit_MQTT_Publish humidadeTopico02 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade02");
*/

/***** Funções utilizadas *****/
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
}

void loop()
{
    // Verificando conexão
    if (!mqtt_client.connected()) // Se MQTT não estiver conectado
        connectMQTT();            // Tente se conectar ao broker

    if (mqtt_client.connected())
        mqtt_client.loop();

    // Faz a publicação dos valores de temperatura e humidade dos sensores e suas médias
    /*temperaturaMediaTopico.publish(temperaturaMedia);
    temperaturaTopico01.publish(temperatura01);
    temperaturaTopico02.publish(temperatura02);
    humidadeMediaTopico.publish(humidadeMedia);
    humidadeTopico01.publish(humidade01);
    humidadeTopico02.publish(humidade02);
    */
    delay(20000);
}

// Função para conectar o Esp ao Wifi
void connectWiFi()
{
    Serial.print("Conectando à rede WiFi .. ");

    unsigned long tempoInicial = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - tempoInicial < wifi_timeout))
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Conexão com WiFi falhou!");
    }
    else
    {
        Serial.print("Conectado com o IP: ");
        Serial.println(WiFi.localIP());
    }
}

// Função para conexão no broker MQTT
void connectMQTT()
{
    unsigned long tempoInicial = millis();
    while (!mqtt_client.connected() && (millis() - tempoInicial < mqtt_timeout))
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            connectWiFi();
        }
        Serial.print("Conectando ao MQTT Broker..");

        if (mqtt_client.connect("ESP32Client", mqtt_usernameAdafruitIO, mqtt_keyAdafruitIO))
        {
            Serial.println();
            Serial.print("Conectado ao broker MQTT!");
        }
        else
        {
            Serial.println();
            Serial.print("Conexão com o broker MQTT falhou!");
            delay(500);
        }
    }
    Serial.println();
}
