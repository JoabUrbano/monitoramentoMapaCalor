#include <stdlib.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include "SPIFFS.h"
#include <FS.h>
#include <queue>

std::queue<String> filaDeStrings;

/* Variáveis para conexão MQTT */

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

#define wifi_ssid "SSID"
#define wifi_password "PASSWORD"
int wifi_timeout = 100000;

#define mqtt_broker "io.adafruit.com"
const int mqtt_port = 1883;
int mqtt_timeout = 10000;

#define mqtt_usernameAdafruitIO "USERNAME"
#define mqtt_keyAdafruitIO "KEY"

int nowTime, oldTime = 0;

/* Variáveis de texto */
String state, path;
int maxLines = 10;

/* Configuração dos feeds */

Adafruit_MQTT_Client mqtt(&wifi_client, mqtt_broker, mqtt_port, mqtt_usernameAdafruitIO, mqtt_usernameAdafruitIO, mqtt_keyAdafruitIO);
Adafruit_MQTT_Publish temperaturaTopico01 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura01");
Adafruit_MQTT_Publish temperaturaTopico02 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura02");
Adafruit_MQTT_Publish temperaturaTopico03 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura03");
Adafruit_MQTT_Publish temperaturaTopico04 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura04");
Adafruit_MQTT_Publish temperaturaTopico05 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura05");
Adafruit_MQTT_Publish temperaturaTopico06 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura06");
Adafruit_MQTT_Publish temperaturaTopico07 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura07");
Adafruit_MQTT_Publish temperaturaTopico08 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura08");
Adafruit_MQTT_Publish temperaturaTopico09 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/temperatura09");
/*
Adafruit_MQTT_Publish humidadeTopico01 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade01");
Adafruit_MQTT_Publish humidadeTopico02 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade02");
Adafruit_MQTT_Publish humidadeTopico03 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade03");
Adafruit_MQTT_Publish humidadeTopico04 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade04");
Adafruit_MQTT_Publish humidadeTopico05 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade05");
Adafruit_MQTT_Publish humidadeTopico06 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade06");
Adafruit_MQTT_Publish humidadeTopico07 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade07");
Adafruit_MQTT_Publish humidadeTopico08 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade08");
Adafruit_MQTT_Publish humidadeTopico09 = Adafruit_MQTT_Publish(&mqtt, mqtt_usernameAdafruitIO "/feeds/humidade09");
*/

/* Funções utilizadas */
void connectWiFi();
void connectMQTT();

/* Funções de interação com arquivos */
void writeFile(String strQuantSensores, String path);
void readFile(String path);
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

  //formatFile(); // Apenas a primeira vez que gravar o código

  Serial.println("Abrir arquivo");
  openFS();

  Serial2.begin(9600, SERIAL_8N1, 16, -1);

}

void loop()
{
  // Verificando conexão
  if (!mqtt_client.connected()) // Se MQTT não estiver conectado
    connectMQTT();              // Tente se conectar ao broker

  if (mqtt_client.connected())
    mqtt_client.loop();

  /* Começar a comunicação serial aqui */
  if (Serial2.available() > 0) {
    char receivedChar = Serial2.read();

    if (receivedChar == '-') {
      String strQuantSensores = readNextValue();
      String strNumSensor = readNextValue();
      String strTemperatura = readNextValue();
      String strHumidade = readNextValue();

      int quantSensores = strQuantSensores.toInt();
      int numSensor = strNumSensor.toInt();
      float temperatura = strTemperatura.toFloat();
      float humidade = strHumidade.toFloat();

      Serial.print("Número do senssor: ");
      Serial.println(numSensor);
      Serial.print("Temperatura: ");
      Serial.println(temperatura, 2);  // 2 casas decimais
      Serial.print("Humidade: ");
      Serial.println(humidade, 2);  // 2 casas decimais
      writeFile(strQuantSensores, "/sensoresLogs.txt");

      if(numSensor == 1)
      {
        temperaturaTopico01.publish(temperatura);
        //humidadeTopico01.publish(humidade);
      }
      else if(numSensor == 2)
      {
        temperaturaTopico02.publish(temperatura);
        //humidadeTopico02.publish(humidade);
      }
      else if(numSensor == 3)
      {
        temperaturaTopico03.publish(temperatura);
        //humidadeTopico03.publish(humidade);
      }
      else if(numSensor == 4)
      {
        temperaturaTopico04.publish(temperatura);
        //humidadeTopico04.publish(humidade);
      }
      else if(numSensor == 5)
      {
        temperaturaTopico05.publish(temperatura);
        //humidadeTopico05.publish(humidade);
      }
      else if(numSensor == 6)
      {
        temperaturaTopico06.publish(temperatura);
        //humidadeTopico06.publish(humidade);
      }
      else if(numSensor == 7)
      {
        temperaturaTopico07.publish(temperatura);
        //humidadeTopico07.publish(humidade);
      }
      else if(numSensor == 8)
      {
        temperaturaTopico08.publish(temperatura);
        //humidadeTopico08.publish(humidade);
      }
      else if(numSensor == 9)
      {
        temperaturaTopico09.publish(temperatura);
        //humidadeTopico09.publish(humidade);
      }
    }
  }
  
  delay(5000);
}

String readNextValue() {
  String result = "";
  while (Serial2.available() > 0) {
    char nextChar = Serial2.read();
    if (nextChar == ',') {
      break;  // Finalizou a leitura do valor
    }
    result += nextChar;
  }
  return result;
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

/* Funções de interação arquivos ESP */

void writeFile(String strQuantSensores, String path)
{
  while (!filaDeStrings.empty())
  {
    filaDeStrings.pop();
  }

  readFile(path);
  File rFile = SPIFFS.open(path, "w");

  if (!rFile)
  {
    Serial.println("Erro ao abrir arquivo!");
    return;
  }
  filaDeStrings.push(strQuantSensores + " funcionando");

  // Escrever no arquivo
  while (filaDeStrings.size() > maxLines)
  {
    filaDeStrings.pop();
  }
  while (!filaDeStrings.empty())
  {
    rFile.println(filaDeStrings.front());
    filaDeStrings.pop();
  }

  rFile.close();
}

void readFile(String path)
{
  Serial.println("Read file");

  File rFile = SPIFFS.open(path, "r"); // r+ leitura e escrita

  if (!rFile)
  {
    Serial.println("Erro ao abrir arquivo!");

    return;
  }

  else
  {
    Serial.print("---------- Lendo arquivo ");

    Serial.print(path);

    Serial.println("  ---------");
    while (rFile.position() < rFile.size())
    {
      String line = rFile.readStringUntil('\n'); // Lê uma linha do arquivo
      Serial.println(line);
      filaDeStrings.push(line);
    }

    rFile.close();
  }
}

void formatFile()
{
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}

void openFS(void)
{
  if (!SPIFFS.begin())
    Serial.println("\nErro ao abrir o sistema de arquivos");
  else
    Serial.println("\nSistema de arquivos aberto com sucesso!");
}
