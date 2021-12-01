#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <WiFiClient.h>

#define PIN_SENSOR D8 // definição do pino de entrada do sensor PIR
#define PIN_BUZZER D6 // definição do pino de saída do buzzer
#define pin_buzzer_msg D5 // definição do pino de saída do buzzer de mensagem mqtt
#define pin_ledred D4 // definição do pino de saída do led vermelho
#define pin_ledgreen D3 // definição do pino de saída do led verde
#define endereco 0x3F // definição do lcd
#define colunas 16
#define linhas 2

//Wi Fi
const char* nome = "VIVOFIBRA"; //Seu SSID da Rede WIFI
const char* senha = "44853107"; // A Senha da Rede WIFI
int wifiStatus;

//Cliente MQTT
const char* mqtt_server = "broker.mqtt-dashboard.com";
const int mqttPort = 1883;

WiFiClient wifiClient;
PubSubClient client(wifiClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

//instanciando lcd
LiquidCrystal_I2C lcd(endereco, colunas, linhas);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    tone(pin_buzzer_msg, 2000, 3000);
    //digitalWrite(pin_buzzer_msg, HIGH);
    //delay(5000);   
  } else {
    noTone(pin_buzzer_msg);
    //digitalWrite(pin_buzzer_msg, LOW);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  Serial.println("Conectando ao MQTT Broker...");
  while (!client.connected()) {
      Serial.println("Reconectando ao MQTT Broker..");
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      
      if (client.connect(clientId.c_str())) {
        Serial.println("Conectado.");
        // inscreva-se no tópico
        client.publish("Publicação_do_Eder","hello Eder ");
        client.subscribe("Subscrição_do_Eder");
      }else{
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.print(" tente de novo em 5 segundos");
        delay(5000);  
      }   
   }
}  

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();  
  lcd.setCursor(0, 0); 
  lcd.print("DETECTANDO..."); 
  pinMode(PIN_SENSOR, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(pin_ledred, OUTPUT);
  pinMode(pin_ledgreen, OUTPUT);
  Serial.begin(115200);
  delay(200);

  client.setServer(mqtt_server, mqttPort);
  // define a função de retorno de chamada
  client.setCallback(callback); 

  //Fazendo a conexão
  Serial.println("Iniciando a conexão da rede! ");
  Serial.println(nome);
  WiFi.begin(nome, senha);

  while (WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
    } 
  }
  
void loop() {

  wifiStatus = WiFi.status();
  
  if(wifiStatus == WL_CONNECTED){
    Serial.println("O nodeMCU está conectado! ");
    
  }else{
    Serial.println("O nodeMCU não está conectado!");  
  }
  delay(2000);

  //loop mqtt
  if (!client.connected())
    reconnect();
  client.loop();

  //faz a leitura do sensor de presença (retorna HIGH ou LOW)
  boolean sinal = digitalRead(PIN_SENSOR); 

  //HIGH : movimento detectado
  if(sinal == HIGH){
    // aciona o led vermelho
    digitalWrite(pin_ledred, HIGH);
    //desativa o led verde
    digitalWrite(pin_ledgreen, LOW);
    // escreve no lcd
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("MOVIMENTO");
    lcd.setCursor(0, 1); 
    lcd.print("DETECTADO!");
    //aciona o Buzzer
    digitalWrite(PIN_BUZZER, HIGH);
    
    snprintf (msg, MSG_BUFFER_SIZE, "Movimento Detectado! #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Publicação_do_Eder", msg);
    ++value;
  }
  //LOW : nenhum movimento detectado
  else{
    //desativa o buzzer
    digitalWrite(PIN_BUZZER, LOW);
    //desativa o led vermelho
    digitalWrite(pin_ledred, LOW);
    //ativa o led verde
    digitalWrite(pin_ledgreen, HIGH);
    //escreve no lcd
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("DETECTANDO...");

    snprintf (msg, MSG_BUFFER_SIZE, "Monitorando! #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Publicação_do_Eder", msg);
    ++value;
  }

    /*//publicando leituras mqtt
    unsigned long now = millis();
    if (now - lastMsg > 2000) {
      lastMsg = now;
      ++value;
      snprintf (msg, MSG_BUFFER_SIZE, "hello Eder #%ld", value);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("ederTopic", msg);
    }*/
  }
