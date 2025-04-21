#include <WiFi.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiServer server(80);
int ledPin = 2; // Built-in LED

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
            client.println("<style>body{font-family:Arial;text-align:center;}</style></head>");
            client.println("<body><h2>ESP32 Smart Light Control</h2>");
            client.println("<form><button name='LED' value='ON' type='submit'>Turn ON</button>");
            client.println("<button name='LED' value='OFF' type='submit'>Turn OFF</button></form>");
            client.println("</body></html>");

            client.println();
            break;
          } else {
            if (currentLine.indexOf("GET /?LED=ON") >= 0) {
              digitalWrite(ledPin, HIGH);
            }
            if (currentLine.indexOf("GET /?LED=OFF") >= 0) {
              digitalWrite(ledPin, LOW);
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
