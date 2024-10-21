#include <WiFi.h>

#define PORT 80
WiFiServer server(PORT);

const char* ssid     = "CESJT";
const char* password = "itisjtsmg";

const int segmentos[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 0, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1}
};
int e = 0, d = 0, f = 0;
const int display1 [7] = {13, 12, 14, 27, 26, 25, 33};
const int display2 [7] = {15, 5, 18, 19, 21, 22, 23};

int contconexion = 0;

String palabra;

String pagina = 

  "<!DOCTYPE html>"
  "<html>"
    "<head>"
      "<meta charset='utf-8' />"
      "<title>Actividades ESP32 - Punto 2</title>"
    "</head>"
    "<body>"
      "<center>"
        "<h1>Control dos 7 segmentos</h1>"
        "<p><a href='/ascendente'><button style='height:50px;width:100px'>ASCENDENTE</button></a></p>"
        "<p><a href='/descendente'><button style='height:50px;width:100px'>DESCENDENTE</button></a></p>"
        "<p><a href='/parar'><button style='height:50px;width:100px'>PARAR CONTEO</button></a></p>"
        "<p><a href='/reiniciar'><button style='height:50px;width:100px'>REINICIAR CONTEO</button></a></p>"
      "</center>"
    "</body>"
  "</html>";


void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 7; i++) {
    pinMode(display1[i], OUTPUT);
    pinMode(display2[i], OUTPUT);
  }
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
        palabra += c;
        if (c == '\n') {    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (palabra.indexOf("GET /ascendente") >= 0) {
              for (e; e < 10;e++) {
                for (d; d < 10;d++) {
                  for (f = 0; f < 7;f++) {
                    digitalWrite(display1[f], segmentos[d][f]);
                    digitalWrite(display1[f], segmentos[e][f]);
                    delay(1000);
                  }
                  if (d == 9) {
                    d = 0;
                  }
                  if (e == 9) {
                    e = 0;
                  }
                }
              }
            } else if (palabra.indexOf("GET /descendente") >= 0) {
              for (e; e > 0;e++) {
                for (d; d > 10;d++) {
                  for (f = 0; f < 7;f++) {
                    digitalWrite(display1[f], segmentos[d][f]);
                    digitalWrite(display1[f], segmentos[e][f]);
                    delay(1000);
                  }
                  if (d == 0) {
                    d = 9;
                  }
                  if (e == 0) {
                    e = 9;
                  }
                }
              }
            }else if (palabra.indexOf("GET /parar") >= 0) {
              for (f = 0; f > 7;f++) {
                digitalWrite(display1[f], segmentos[d][f]);
                digitalWrite(display1[f], segmentos[e][f]);
              }
            }else if (palabra.indexOf("GET /reiniciar") >= 0) {
              for (f = 0; f > 7;f++) {
                digitalWrite(display1[f], segmentos[0][f]);
                digitalWrite(display1[f], segmentos[0][f]);
              }
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
    palabra = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
