#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PZEM004Tv30.h>


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Dados para conexão Wi-Fi
const char* ssid = "CLARO_2G40595C"; // Inserir nome da rede
const char* password = "33322585"; // Inserir senha da rede
WiFiServer server(80);
// Dados para definição de IP Fixo
//IPAddress local_IP(192, 168, 0, 108);

// Variable to store the HTTP request
String header;

/* Use software serial for the PZEM
 * Pin 4 Rx (Connects to the Tx pin on the PZEM)
 * Pin 5 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(4, 5);
float voltage;
float current;
float power;
float energy;
float frequency;
float pf;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
   
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println("Refresh: 5");  // refresh the page automatically every 5 sec
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<body>");
            voltage = pzem.voltage();
            if( !isnan(voltage) ){
                Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
                client.print("Voltage: "); client.print(voltage); client.print("V");
                client.println("<br />");
            } else {
                Serial.println("Error reading voltage");
                client.print("Error reading voltage");
                client.println("<br />");
            }
            current = pzem.current();
            if( !isnan(current) ){
                Serial.print("Current: "); Serial.print(current); Serial.println("A");
                client.print("Current: "); client.print(current); client.print("A");
                client.println("<br />");
            } else {
                Serial.println("Error reading current");
                client.print("Error reading current");
                client.println("<br />");
            }
            power = pzem.power();
            if( !isnan(power) ){
                Serial.print("Power: "); Serial.print(power); Serial.println("W");
                client.print("Power: "); client.print(power); client.print("W");
                client.println("<br />");
            } else {
                Serial.println("Error reading power");
                client.print("Error reading power");
                client.println("<br />");
            }
            energy = pzem.energy();
            if( !isnan(energy) ){
                Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
                client.print("Energy: "); client.print(energy); client.print("kWh");
                client.println("<br />");
            } else {
                Serial.println("Error reading energy");
                client.println("Error reading energy");
                client.println("<br />");
            }
            frequency = pzem.frequency();
            if( !isnan(frequency) ){
                Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
                client.print("Frequency: "); client.print(frequency, 1); client.print("Hz");
                client.println("<br />");
            } else {
                Serial.println("Error reading frequency");
                client.print("Error reading frequency");
                client.println("<br />");
            }
            pf = pzem.pf();
            if( !isnan(pf) ){
                Serial.print("PF: "); Serial.println(pf);
                client.print("PF: "); client.print(pf);
                client.println("<br />");
            } else {
                Serial.println("Error reading power factor");
                client.print("Error reading power factor");
                client.println("<br />");
            }
            client.println("</body></html>");
          
           // The HTTP response ends with another blank line
           client.println();
           // Break out of the while loop
           break;
         } else { // if you got a newline, then clear currentLine
            currentLine = "";
         }
       } else if (c != '\r') {  // if you got anything else but a carriage return character,
         currentLine += c;      // add it to the end of the currentLine
       }
      }
     }
     // Clear the header variable
     header = "";
     // Close the connection
     client.stop();
     Serial.println("Client disconnected.");
     Serial.println("");
  }
}
