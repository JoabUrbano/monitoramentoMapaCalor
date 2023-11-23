#include <stdlib.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <NTPClient.h>
#include "SPIFFS.h"

#define RXpin 16
#define TXpin 17
HardwareSerial transmissor(1);

/* Variáveis para conexão MQTT */

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

#define wifi_ssid "NPITI-IoT"
#define wifi_password "NPITI-IoT"
int wifi_timeout = 100000;

#define mqtt_broker "io.adafruit.com"
const int mqtt_port = 1883;
int mqtt_timeout = 10000;
#define mqtt_usernameAdafruitIO "USER"
#define mqtt_keyAdafruitIO "KEY"

/* NTP */
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000); // Cria um objeto "NTP" com as configurações utilizadas no Brasil

int nowTime, oldTime = 0;

/* Variáveis de texto */
String state, path;
int maxLines = 10;

/* Configuração dos feeds */
/*
Adafruit_MQTT_Client mqtt(&wifi_client, mqtt_broker, mqtt_port, mqtt_usernameAdafruitIO, mqtt_usernameAdafruitIO, mqtt_keyAdafruitIO);
*/

/* Funções utilizadas */
void connectWiFi();
void connectMQTT();

/* Funções de interação com arquivos */
void writeFile(String state, String path);
String readFile(String path);
void formatFile();
void openFS(void);



void setup()
{
    Serial.begin(9600);

    /* Variáveis para conexão MQTT */

    WiFi.mode(WIFI_STA); //"station mode": permite o ESP32 ser um cliente da rede WiFi
    WiFi.begin(wifi_ssid, wifi_password);
    connectWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);

    transmissor.begin(9600);

    // formatFile(); // Apenas a primeira vez que gravar o código

    Serial.println("Abrir arquivo");
    openFS();

    ntp.begin();       // Inicia o protocolo
    ntp.forceUpdate(); // Atualização
}



void loop()
{
    // Verificando conexão
    if (!mqtt_client.connected()) // Se MQTT não estiver conectado
        connectMQTT();            // Tente se conectar ao broker

    if (mqtt_client.connected())
        mqtt_client.loop();

    // Faz a publicação dos valores de temperatura e umidade dos sensores e suas médias
    // temperaturaTopico01.publish(temperatura01);

    // Começar a comunicação serial aqui
    if (transmissor.available())
        Serial.write(transmissor.read());

    if (Serial.available())
        transmissor.write(Serial.read());
    delay(1000);
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
        Serial.println("Conexão com WiFi falhou!");
    else
    {
        Serial.print("Conectado com o IP: ");
        Serial.println(WiFi.localIP());
    }
}

/* Função para conexão no broker MQTT */
void connectMQTT()
{
    unsigned long tempoInicial = millis();
    while (!mqtt_client.connected() && (millis() - tempoInicial < mqtt_timeout))
    {
        if (WiFi.status() != WL_CONNECTED)
            connectWiFi();

        Serial.println("Conectando ao MQTT Broker..");

        if (mqtt_client.connect("ESP32Client", mqtt_usernameAdafruitIO, mqtt_keyAdafruitIO))
            Serial.print("Conectado ao broker MQTT!");
        else
        {
            Serial.print("Conexão com o broker MQTT falhou!");
            delay(500);
        }
    }
    Serial.println();
}

/* Funções de interação com arquivo */
void writeFile(String state, String path)
{
    File rFile = SPIFFS.open(path, "r+"); // 'r+' para leitura e escrita
    if (!rFile)
    {
        Serial.println("Erro ao abrir arquivo!");
        return;
    }

    rFile.seek(0); // Mover o cursor para o início do arquivo

    int lineCount = 0;
    String lines[maxLines];

    // Ler linhas do arquivo
    while (rFile.position() < rFile.size())
    {
        lines[lineCount] = rFile.readStringUntil('\n');
        lineCount++;
    }

    rFile.seek(0); // Mover o cursor para o início do arquivo

    if (lineCount / 2 >= maxLines)                             // Se atingir o máximo de elementos, sobrescrever a partir da segunda entrada
        rFile.seek(lines[2].length() + lines[3].length() + 4); // 4 é a quantidade de caracteres adicionados por println

    // Escrever no arquivo
    rFile.println(state);
    Serial.println("Gravou!");

    rFile.close();
}

String readFile(String path)
{
    Serial.println("Read file");
    File rFile = SPIFFS.open(path, "r"); // r+ leitura e escrita
    if (!rFile)
    {
        Serial.println("Erro ao abrir arquivo!");
        String s = "";
        return s;
    }
    else
    {
        Serial.print("---------- Lendo arquivo " + path + "  ---------");
        String s = "";
        while (rFile.position() < rFile.size())
        {
            String line = rFile.readStringUntil('\n'); // Lê uma linha do arquivo
            Serial.println(line);
            s = line;
        }
        rFile.close();
        return s;
    }
}

void formatFile()
{
    SPIFFS.format();
    Serial.println("Formatou SPIFFS!");
}

void openFS(void)
{
    if (!SPIFFS.begin())
        Serial.println("\nErro ao abrir o sistema de arquivos");
    else
        Serial.println("\nSistema de arquivos aberto com sucesso!");
}
