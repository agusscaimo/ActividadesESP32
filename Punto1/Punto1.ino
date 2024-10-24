#include <WiFi.h>

#define PORT 80
WiFiServer server(PORT);

const char* ssid     = "CESJT";
const char* password = "itisjtsmg";

const uint8_t leds [5] = {13, 12, 14, 27, 26};

int contconexion = 0;

String header;

String pagina = 

  "<!DOCTYPE html>"
  "<html>"
    "<head>"
      "<meta charset='utf-8' />"
      "<title>Actividades ESP32 - Punto 1</title>"
    "</head>"
    "<body>"
      "<center>"
        "<h1>Control animacion de leds</h1>"
        "<p><a href='/animacion1'><button style='height:50px;width:100px'>ANIMACION1</button></a></p>"
        "<p><a href='/animacion2'><button style='height:50px;width:100px'>ANIMACION2</button></a></p>"
        "<p><a href='/apagar'><button style='height:50px;width:100px'>APAGAR</button></a></p>"
      "</center>"
    "</body>"
  "</html>";


void setup() {
  Serial.begin(115200);
  pinMode(leds[0], OUTPUT); 
  digitalWrite(leds[0], LOW);
  pinMode(leds[1], OUTPUT); 
  digitalWrite(leds[1], LOW);
  pinMode(leds[2], OUTPUT); 
  digitalWrite(leds[2], LOW);
  pinMode(leds[3], OUTPUT);
  digitalWrite(leds[3], LOW);
  pinMode(leds[4], OUTPUT); 
  digitalWrite(leds[4], LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      server.begin();
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

void loop(){
  WiFiClient client = server.available();

  if (client) {   
    String currentLine = "";           
    while (client.connected()) {      
      if (client.available()) {       
        char c = client.read();                    
        header += c;
        if (c == '\n') {    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /animacion1") >= 0) {
              //Oruga ida solo
              digitalWrite(leds[0], HIGH);
              digitalWrite(leds[1], LOW);
              digitalWrite(leds[2], LOW);
              digitalWrite(leds[3], LOW);
              digitalWrite(leds[4], LOW);
              delay(1000);
              digitalWrite(leds[0], LOW);
              digitalWrite(leds[1], HIGH);
              delay(1000);
              digitalWrite(leds[1], LOW);
              digitalWrite(leds[2], HIGH);
              delay(1000);
              digitalWrite(leds[2], LOW);
              digitalWrite(leds[3], HIGH);
              delay(1000);
              digitalWrite(leds[3], LOW);
              digitalWrite(leds[4], HIGH);
              delay(1000);
            } else if (header.indexOf("GET /animacion2") >= 0) {
              //Se prenden intercaladamente
              digitalWrite(leds[0], HIGH);
              digitalWrite(leds[1], LOW);
              digitalWrite(leds[2], HIGH);
              digitalWrite(leds[3], LOW);
              digitalWrite(leds[4], HIGH);
              delay(1000);
              digitalWrite(leds[0], LOW);
              digitalWrite(leds[1], HIGH);
              digitalWrite(leds[2], LOW);
              digitalWrite(leds[3], HIGH);
              digitalWrite(leds[4], LOW);
            }else if (header.indexOf("GET /apagar") >= 0) {
              digitalWrite(leds[0], LOW);
              digitalWrite(leds[1], LOW);
              digitalWrite(leds[2], LOW);
              digitalWrite(leds[3], LOW);
              digitalWrite(leds[4], LOW);
            }  
            client.println(pagina);
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
