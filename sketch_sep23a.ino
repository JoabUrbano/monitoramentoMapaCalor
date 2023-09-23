#include"DHT.h"


/***** Definições de variaveis *****/

#defineDHT_PIN0115

//#define DHT_PIN01 4

#defineDHTTYPE DHT11



DHT dht01(DHT_PIN01, DHTTYPE);

//DHT dht02(DHT_PIN01, DHTTYPE);



voidsetup(){

  Serial.begin(9600);

  Serial.println(F("DHTxx test!"));



  dht01.begin();

  //dht02.begin();

}



voidloop(){

  delay(2000);

  //Leitura da humidade dos sensores

  float humidade01 = dht01.readHumidity();

  //float humidade02 = dht02.readHumidity();



  // Leitura da temperatura em Celcius

  float temperatura01 = dht01.readTemperature();

  //float temperatura02 = dht02.readTemperature();



  /*

  float variacaoTemperaturaSensores = temperatura01 - temperatura02;

  float variacaoHumidadeSensores = humidade01 - humidade02;

  float temperaturaMedia = (temperatura01 + temperatura02) / 2;

  float humidadeMedia = (humidade01 + humidade02) / 2;

  */




  // Checa se alguma das leituras do sensor 01 falhou e tenta novamente

  if(isnan(humidade01) || isnan(temperatura01)){

    Serial.println(F("Falha ao ler as informações do sensor 01"));

    return;

  }

  /*

  // Checa se alguma das leituras do sensor 02 falhou e tenta novamente

  if (isnan(humidade02) || isnan(temperatura02)) {

    Serial.println(F("Falha ao ler as informações do sensor 02"));

    return;

  }

  */

  // Capta a sensação termica

  float hic01 = dht01.computeHeatIndex(temperatura01, humidade01, false);

  //float hic02 = dht01.computeHeatIndex(temperatura02, humidity02, false);



  Serial.print(F("Humidade: "));

  Serial.print(humidade01);

  Serial.print(F("%  Temperaturas: "));

  Serial.print(temperatura01);

  Serial.print(F("°C ,"));

  Serial.print(hic01);

  Serial.println(F("°C "));



  /*

  Serial.print(F("Humidade: "));

  Serial.print(humidade02);

  Serial.print(F("%  Temperaturas: "));

  Serial.print(temperatura02);

  Serial.print(F("°C ,"));

  Serial.print(hic02);

  Serial.println(F("°C "));

  */

}
