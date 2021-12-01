# Projeto-Objetos-Inteligentes-Conectados

O funcionamento do modelo apresentado consiste em realizar a detecção de movimento por meio do sensor PIR, que enviará ao controlador a informação, sendo que, em caso negativo, o led verde permanecerá acesso e em caso positivo, acenderá o led vermelho, acionará o sinal sonoro do buzzer e exibirá mensagem de movimento detectado no display LCD, sendo, portanto, estes quatro dispositivos atuadores deste protótipo. Serão registradas no monitor serial do Arduino IDE as mensagens de detecção ou monitoramento de movimento.
O protótipo possuirá conexão à internet por meio de WI-FI, a partir da qual realizará conexão com o broker MQTT HiveMQ e encaminhará mensagens e receberá comandos remotamente pela aplicação MQTTBox. Serão registradas no monitor serial do Arduino IDE as mensagens de conexão à internet e ao broker, que serão exibidas também pelo MQTTBox.
Por meio do MQTTBox, além do recebimento das mensagens produzidas pelo protótipo, será possível o envio de comando para o acionamento remoto do buzzer.

Os componentes utilizados são:
- Nodemcu Esp8266;
- Sensor de presença e movimento PIR;
- Resistor de 220 amperes;
- Leds verde e vermelho;
- Buzzer;
- Protoboard 400 pontos;
- Jumpers macho-femea, macho-macho e femea-femea;
- Display LCD 16x2 com adaptador I2C.

Os softwares utilizados são:
- Arduino IDE;
- Broker HiveMQ;
- MQTTBox. 
