#include "DHT.h"
#include <stdlib.h>
#include <SoftwareSerial.h>

SoftwareSerial coletorDados(A9, A10); // A9 receptor e A10 transmissor

/* Struct que armazenará os dados */

struct dados
{
  float temperatura;
  float humidade;
  bool funcionando = 0;
};

typedef struct dados Dados;

/***** Definições dos sensores DHTs *****/

#define DHT_PIN01 A0
#define DHT_PIN02 A1
#define DHT_PIN03 A2
#define DHT_PIN04 A3
#define DHT_PIN05 A4
#define DHT_PIN06 A5
#define DHT_PIN07 A6
#define DHT_PIN08 A7
#define DHT_PIN09 A8

#define DHTTYPE DHT11

DHT dht01(DHT_PIN01, DHTTYPE);
DHT dht02(DHT_PIN02, DHTTYPE);
DHT dht03(DHT_PIN03, DHTTYPE);
DHT dht04(DHT_PIN04, DHTTYPE);
DHT dht05(DHT_PIN05, DHTTYPE);
DHT dht06(DHT_PIN06, DHTTYPE);
DHT dht07(DHT_PIN07, DHTTYPE);
DHT dht08(DHT_PIN08, DHTTYPE);
DHT dht09(DHT_PIN09, DHTTYPE);

void setup()
{
  Serial.begin(9600);

  // Iniciando sensores DHT
  dht01.begin();
  dht02.begin();
  dht03.begin();
  dht04.begin();
  dht05.begin();
  dht06.begin();
  dht07.begin();
  dht08.begin();
  dht09.begin();

  Serial.println("Sensores iniciados!");

  coletorDados.begin(9600);
}

void loop()
{
  Dados dados01, dados02, dados03, dados04, dados05, dados06, dados07, dados08, dados09;

  // Leitura da humidade dos sensores

  dados01.humidade = dht01.readHumidity();
  dados02.humidade = dht02.readHumidity();
  dados03.humidade = dht03.readHumidity();
  dados04.humidade = dht04.readHumidity();
  dados05.humidade = dht05.readHumidity();
  dados06.humidade = dht06.readHumidity();
  dados07.humidade = dht07.readHumidity();
  dados08.humidade = dht08.readHumidity();
  dados09.humidade = dht09.readHumidity();

  // Leitura da temperatura em Celcius

  dados01.temperatura = dht01.readTemperature();
  dados02.temperatura = dht02.readTemperature();
  dados03.temperatura = dht03.readTemperature();
  dados04.temperatura = dht04.readTemperature();
  dados05.temperatura = dht05.readTemperature();
  dados06.temperatura = dht06.readTemperature();
  dados07.temperatura = dht07.readTemperature();
  dados08.temperatura = dht08.readTemperature();
  dados09.temperatura = dht09.readTemperature();

  // Checa se alguma das leituras dos sensores falharam
  if (!isnan(dados01.humidade) && !isnan(dados01.temperatura))
  {
    Serial.println(F("Sensor 01 lido com sucesso!"));
    if (dados01.temperatura >= 0 && dados01.temperatura <= 50 && dados01.humidade >= 20 && dados01.humidade <= 80)
      dados01.funcionando = 1;
  }
  if (!isnan(dados02.humidade) && !isnan(dados02.temperatura))
  {
    Serial.println(F("Sensor 02 lido com sucesso!"));
    if (dados02.temperatura >= 0 && dados02.temperatura <= 50 && dados02.humidade >= 20 && dados02.humidade <= 80)
      dados02.funcionando = 1;
  }
  if (!isnan(dados03.humidade) && !isnan(dados03.temperatura))
  {
    Serial.println(F("Sensor 03 lido com sucesso!"));
    if (dados03.temperatura >= 0 && dados03.temperatura <= 50 && dados03.humidade >= 20 && dados03.humidade <= 80)
      dados03.funcionando = 1;
  }
  if (!isnan(dados04.humidade) && !isnan(dados04.temperatura))
  {
    Serial.println(F("Sensor 04 lido com sucesso!"));
    if (dados04.temperatura >= 0 && dados04.temperatura <= 50 && dados04.humidade >= 20 && dados04.humidade <= 80)
      dados04.funcionando = 1;
  }
  if (!isnan(dados05.humidade) && !isnan(dados05.temperatura))
  {
    Serial.println(F("Sensor 05 lido com sucesso!"));
    if (dados05.temperatura >= 0 && dados05.temperatura <= 50 && dados05.humidade >= 20 && dados05.humidade <= 80)
      dados05.funcionando = 1;
  }
  if (!isnan(dados06.humidade) && !isnan(dados06.temperatura))
  {
    Serial.println(F("Sensor 06 lido com sucesso!"));
    if (dados06.temperatura >= 0 && dados06.temperatura <= 50 && dados06.humidade >= 20 && dados06.humidade <= 80)
      dados06.funcionando = 1;
  }
  if (!isnan(dados07.humidade) && !isnan(dados07.temperatura))
  {
    Serial.println(F("Sensor 07 lido com sucesso!"));
    if (dados07.temperatura >= 0 && dados07.temperatura <= 50 && dados07.humidade >= 20 && dados07.humidade <= 80)
      dados07.funcionando = 1;
  }
  if (!isnan(dados08.humidade) && !isnan(dados08.temperatura))
  {
    Serial.println(F("Sensor 08 lido com sucesso!"));
    if (dados08.temperatura >= 0 && dados08.temperatura <= 50 && dados08.humidade >= 20 && dados08.humidade <= 80)
      dados08.funcionando = 1;
  }
  if (!isnan(dados09.humidade) && !isnan(dados09.temperatura))
  {
    Serial.println(F("Sensor 09 lido com sucesso!"));
    if (dados09.temperatura >= 0 && dados09.temperatura <= 50 && dados09.humidade >= 20 && dados09.humidade <= 80)
      dados09.funcionando = 1;
  }

  // Começar a comunicação serial aqui
  if(coletorDados.avaliable())
  {
    Serial.write(coletorDados.read());
  }
  if(Serial.avaliable())
  {
    coletorDados.write("a");
  }

  delay(1000);
}
