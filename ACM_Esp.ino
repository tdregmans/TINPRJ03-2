/*
 * ACM voor project 2 Technische Informatica
 * 
 * Bartholomeus Petrus
 * student 1034228
 * Thijs Dregmans
 * student 1024272
 * Periode 2, Ti1H
 * 
 * versie 2.1
 */

// include libs
#include <WiFi.h>

// define pins
#define besturingMetWifiPin 2
#define linksMotorCW 5
#define linksMotorCCW 4
#define rechtsMotorCW 16
#define rechtsMotorCCW 17

// wifi instellingen
const char* ssid     = "Tesla IoT";
const char* password = "fsL6HgjN";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(besturingMetWifiPin, OUTPUT);
  pinMode(linksMotorCW, OUTPUT);
  pinMode(linksMotorCCW, OUTPUT);
  pinMode(rechtsMotorCW, OUTPUT);
  pinMode(rechtsMotorCCW, OUTPUT);
  

  // verbind met wifi
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop(){
// check of er clients zijn
WiFiClient client = server.available();   
if(client) {                            
  Serial.println("New Client.");           
  // make a String to hold incoming data from the client
  String currentLine = "";
  // loop while the client's connected
  while (client.connected()) {            
    // if there's bytes to read from the client
    if (client.available()) {             
      // read a byte, then
      char c = client.read();             
      // print it out the serial monitor
      Serial.write(c);                    
      if (c == '\n') {
        // if the byte is a newline character                    
        
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          
          // the content of the HTTP response follows the header:
          String HTML =
            "<!DOCTYPE html>"
            "<html>"
                "<style>"
                    "body {"
                    "background-color: #7393B3;"
                    "text-align: center;"
                    "}"
                    ".button {"
                    "background-color: #000000;"
                    "border: black 1px solid;"
                    "color: white;"
                    "padding: 15px 32px;"
                    "text-align: center;"
                    "text-decoration: none;"
                    "display: inline-block;"
                    "font-size: 16px;"
                    "margin: 4px 2px;"
                    "width: 100px;"
                    "height: 100px;"
                    "border-radius: 25px;"
                    "}"
                    "div {"
                    "margin: auto;"
                    "text-align: center;"
                    "}"
                "</style>"
                "<body>"
                    "<table style='border: black 1px solid;';>"
                        "<tr>"
                            "<th colspan=6><h1>ACM projectgroep 21 - afstandsbediening</h1></th>"
                        "</tr>"
                        "<tr>"
                            "<td colspan=3><div><a class='button' style='width:150px;' href='/aan'>afstandsbediening aan</a></div></td>"
                            "<td colspan=3><div><a class='button' style='width:150px;' href='/uit'>afstandsbediening uit</a></div></td>"
                        "</tr>"
                        "<tr>"
                            "<td colspan=2></td>"
                            "<td colspan=2><div><a class='button' href='/vooruit'>Vooruit</a></div></td>"
                        "</tr>"
                        "<tr>"
                            "<td colspan=2><div><a class='button' href='/links'>Draaien_Links</a></div></td>"
                            "<td colspan=2><div><a class='button' href='/stop'>Sta stil</a></div></td>"
                            "<td colspan=2><div><a class='button' href='/rechts'>Draaien_Rechts</a></div></td>"
                        "</tr>"
                        "<tr>"
                            "<td colspan=2></td>"
                            "<td colspan=2><div><a class='button' href='/achteruit'>Achteruit</a></div></td>"
                        "</tr>"
                    "</table>"
                "</body>"
            "</html>";
          
          client.print(HTML);
          
          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while loop:

          // hier algoritme plaatsen?
          
          break;
        } 
        else {
          // if you got a newline, then clear currentLine:
          currentLine = "";
        }
      } 
      else if (c != '\r') {
        // if you got anything else but a carriage return character, add it to the end of the currentLine
        currentLine += c;
      }
      
      // Check to see if the client request was "GET /H" or "GET /L":
      if (currentLine.endsWith("GET /vooruit")) {
        digitalWrite(linksMotorCW, HIGH);
        digitalWrite(linksMotorCCW, LOW);
        digitalWrite(rechtsMotorCW, HIGH);
        digitalWrite(rechtsMotorCCW, LOW);
        
      }
      if (currentLine.endsWith("GET /achteruit")) {
        digitalWrite(linksMotorCW, LOW);
        digitalWrite(linksMotorCCW, HIGH);
        digitalWrite(rechtsMotorCW, LOW);
        digitalWrite(rechtsMotorCCW, HIGH);
      }
      if (currentLine.endsWith("GET /rechts")) {
        digitalWrite(linksMotorCW, HIGH);
        digitalWrite(linksMotorCCW, LOW);
        digitalWrite(rechtsMotorCW, LOW);
        digitalWrite(rechtsMotorCCW, HIGH);
      }
      if (currentLine.endsWith("GET /links")) {
        digitalWrite(linksMotorCW, LOW);
        digitalWrite(linksMotorCCW, HIGH);
        digitalWrite(rechtsMotorCW, HIGH);
        digitalWrite(rechtsMotorCCW, LOW);
      }
      if (currentLine.endsWith("GET /stop")) {
        digitalWrite(linksMotorCW, LOW);
        digitalWrite(linksMotorCCW, LOW);
        digitalWrite(rechtsMotorCW, LOW);
        digitalWrite(rechtsMotorCCW, LOW);
      }
      // aan/uit van handmatige besturing
      if (currentLine.endsWith("GET /aan")) {
        digitalWrite(besturingMetWifiPin, HIGH);
        // stop
        digitalWrite(linksMotorCW, LOW);
        digitalWrite(linksMotorCCW, LOW);
        digitalWrite(rechtsMotorCW, LOW);
        digitalWrite(rechtsMotorCCW, LOW);
      }
      if (currentLine.endsWith("GET /uit")) {
        digitalWrite(besturingMetWifiPin, LOW);
        // stop
        digitalWrite(linksMotorCW, LOW);
        digitalWrite(linksMotorCCW, LOW);
        digitalWrite(rechtsMotorCW, LOW);
        digitalWrite(rechtsMotorCCW, LOW);
      }
    }
  }
  // close the connection:
  client.stop();
  Serial.println("Client Disconnected.");
  }
}
