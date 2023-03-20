#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"
#define BLYNK_TEMPLATE_ID "TMPLSS83weTC"
#define BLYNK_DEVICE_NAME "ESP8266"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
//#include <PZEM004Tv30.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "AuQTV_eNE2YXzVbnLEZXPNxiNC8GlXNp";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "dlink-E470";
char pass[] = "Bruno156";

/*
PZEM004Tv30 pzem(4, 5); //D2, D1
float voltage;
float current;
float power;
float energy;
float frequency;
float pf;
*/

unsigned long currentMillis, readMillis = 0;
#define DHTPIN 14          // Define o pino digital que o DHT11 está conectado
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE); //Inicializando o objeto dht do tipo DHT passando como parâmetro o pino (DHTPIN) e o tipo do sensor (DHTTYPE)

void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin(); //Inicializa o sensor
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  currentMillis = millis();
  Blynk.run();
  if (currentMillis - readMillis >= 2000) {
    //ReadPzem();
    sendSensorDHT();
    SetBlynk();
  }
}

/*

void ReadPzem() {
  voltage = pzem.voltage();
  if( !isnan(voltage) ){
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
  } else {
    Serial.println("Error reading voltage");
  }
  current = pzem.current();
  if( !isnan(current) ){
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
  } else {
    Serial.println("Error reading current");
  }
  power = pzem.power();
  if( !isnan(power) ){
    Serial.print("Power: "); Serial.print(power); Serial.println("W");
  } else {
    Serial.println("Error reading power");
  }
  energy = pzem.energy();
  if( !isnan(energy) ){
    Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
  } else {
   Serial.println("Error reading energy");
  }
  frequency = pzem.frequency();
  if( !isnan(frequency) ){
    Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
  } else {
    Serial.println("Error reading frequency");
  }
  pf = pzem.pf();
  if( !isnan(pf) ){
    Serial.print("PF: "); Serial.println(pf);
  } else {
    Serial.println("Error reading power factor");
  }
}
 */

void sendSensorDHT(){
  float umidade = dht.readHumidity(); //Realiza a leitura da umidade e armazena na variável h
  float temp = dht.readTemperature(); //Realiza a leitura da temperatura e armazena na variável t

  if (isnan(umidade) || isnan(temp)) { //Verifica se houve falha na leitura do sensor
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V6, temp); //Escreve no pino virtual v6 o valor de temperatura
  Blynk.virtualWrite(V7, umidade); //Escreve no pino virtual V5 o valor de umidade
}

void SetBlynk() {
  /*Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, current);
  Blynk.virtualWrite(V2, energy);
  Blynk.virtualWrite(V3, power);
  Blynk.virtualWrite(V4, frequency);
  Blynk.virtualWrite(V5, pf);*/
}
