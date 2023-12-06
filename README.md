# Monitoramento de Mapa de calor

<h1 id="usage" > 💻 Descrição </h1>
Projeto que visa monitorar ambientes e produzir dados para mapas de calor. Os sensores são todos lidos pelo arduino mega e os que estiverem com valores validos são enviados via serial para o esp32, para que ele possa enviar esses dados via MQTT.

<h1 id="usage" > 🧰 Componentes utilizados </h1>
- Esp32 dev module<br>
- Arduino mega<br>
- Protoboard<br>
- Jumpers<br>
- Dois sensores DHT11<br>
- Dois resistores de 1kOhm<br>


<h1 id="usage" > 📚 Bibliotecas </h1>
<h2>Esp32</h2>
- Adafruit Unified Sensor<br>
- Esp32<br>
- Wifi<br>
- NTPClient<br>
- Adafruit MQTT Library<br>
- SPIFFS<br>
- Fs<br>
- queue<br>

<h2>Arduino Mega</h2>
- Dht Sensor Library<br>
- SoftwareSerial<br>
- TXOnlySerial<br>

<h3>Autores</h3>
<a href="https://github.com/JoabUrbano">Joab Urbano</a><br>
<a href="https://github.com/DaviGibson">Davi Gibson</a>