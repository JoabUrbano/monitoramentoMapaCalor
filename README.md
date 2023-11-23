# Monitoramento de Mapa de calor
Projeto que visa monitorar ambientes e produzir dados para mapas de calor. Os sensores são todos lidos pelo arduino mega e os que estiverem com valores validos são enviados via serial para o esp32, para que ele possa enviar esses dados via MQTT.

Componentes utilizados:
- Esp32 dev module
- Arduino mega
- Protoboard
- Jumpers
- Dois sensores DHT11
- Dois resistores de 1kOhm

<h3>Bibliotecas necessarioas</h3>
Esp32:
- Adafruit Unified Sensor
- Esp32
- Wifi
- NTPClient
- Adafruit MQTT Library
- SPIFFS

Arduino Mega:
- Dht Sensor Library
- SoftwareSerial

<h3>Autores</h3>
<a href="https://github.com/JoabUrbano">Joab Urbano</a><br>
Davi Gibson