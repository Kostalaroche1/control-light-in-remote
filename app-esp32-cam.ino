#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "InfinixHOT11Play";
const char* password = "mbokostagab";

WebServer server(80);

#define LIGHT_PIN1 12   // Safe GPIO for ESP32-CAM
#define LIGHT_PIN2 13   // Safe GPIO for ESP32-CAM
#define LIGHT_PIN3 14   // Safe GPIO for ESP32-CAM
#define LIGHT_PIN4 15   // Safe GPIO for ESP32-CAM

bool lightState1 = false;
bool lightState2 = false;
bool lightState3 = false;
bool lightState4 = false;

// HTML page
String htmlPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>SYSTEME DE CONTROL DE LUMIERE A DISTANCE </title>
  <meta name="viewport" charset="utf-8" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; margin-top: 40px; background-color: #89b1da; }
    button { font-size: 20px; padding: 15px 30px; margin: 10px;  background-color: #b6cce6;border-radius: 5px 5px; border: 1px solid rgba(14, 14, 15, 0.4); }
  </style>
</head>
<body>
  <h2> CONTROLER VOS LAMPES DEPUIS VOTRE TELEPHONE OU ORDINATEUR </h2>
  <p> le systeme vous donne la possibilité de controler 4 lampes</p>
  <p>Light1(Lamp1) is: <b>)rawliteral" + String(lightState1 ? "ON" : "OFF") + R"rawliteral(</b></p>
  <a href="/led1on"><button>ALLUMER LAMPE 1</button></a>
  <a href="/led1off"><button>ETEINDRE LAMPE 1</button></a>
    <p>Light2 (Lamp2)is: <b>)rawliteral" + String(lightState2 ? "ON" : "OFF") + R"rawliteral(</b></p>
 <a href="/led2on"><button>ALLUMER LAMPE 2</button></a>
  <a href="/led2off"><button>ETEINDRE LAMPE 2</button></a>
    <p>Light3(Lamp3) is: <b>)rawliteral" + String(lightState3 ? "ON" : "OFF") + R"rawliteral(</b></p>
  <a href="/led3on"><button>ALLUMER LAMPE 3</button></a>
  <a href="/led3off"><button>ETEINDRE LAMPE 3</button></a>
    <p>Light4(Lamp4) is: <b>)rawliteral" + String(lightState4 ? "ON" : "OFF") + R"rawliteral(</b></p>
  <a href="/led4on"><button>ALLUMER LAMPE 4</button></a>
  <a href="/led4off"><button>ETEINDRE LAMPE 4</button></a>
  <p>cette application web est </p>
    <i>créée par gabriel kosta </i>
</body>
</html>
)rawliteral";
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.println("Connecting to WiFi...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);

  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi reconnected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    if (!MDNS.begin("gabriel")) {
      Serial.println("mDNS failed");
    } else {
      Serial.println("mDNS running: http://gabriel.local");
    }
  } else {
    Serial.println("\nWiFi failed, retry later");
  }
}


void handleOnLamp1() {
  lightState1 = true;
  digitalWrite(LIGHT_PIN1, HIGH);
  handleRoot();
}

void handleOnLamp2() {
  lightState2 = true;
  digitalWrite(LIGHT_PIN2, HIGH);
  handleRoot();
}

void handleOnLamp3() {
  lightState3 = true;
  digitalWrite(LIGHT_PIN3, HIGH);
  handleRoot();
}

void handleOnLamp4() {
  lightState4 = true;
  digitalWrite(LIGHT_PIN4, HIGH);
  handleRoot();
}

void handleOffLamp1() {
  lightState1 = false;
  digitalWrite(LIGHT_PIN1, LOW);
  handleRoot();
}

void handleOffLamp2() {
  lightState2 = false;
  digitalWrite(LIGHT_PIN2, LOW);
  handleRoot();
}
void handleOffLamp3() {
  lightState3 = false;
  digitalWrite(LIGHT_PIN3, LOW);
  handleRoot();
}
void handleOffLamp4() {
  lightState4 = false;
  digitalWrite(LIGHT_PIN4, LOW);
  handleRoot();
}
void setup() {
  Serial.begin(115200);

  pinMode(LIGHT_PIN1, OUTPUT);
  digitalWrite(LIGHT_PIN1, LOW);

  pinMode(LIGHT_PIN2, OUTPUT);
  digitalWrite(LIGHT_PIN2, LOW);

  pinMode(LIGHT_PIN3, OUTPUT);
  digitalWrite(LIGHT_PIN3, LOW);

  pinMode(LIGHT_PIN4, OUTPUT);
  digitalWrite(LIGHT_PIN4, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }

  // Serial.println("\nConnected!");
  // Serial.print("ESP32-CAM IP: ");
  // Serial.println(WiFi.localIP());
  connectWiFi();
  server.on("/", handleRoot);
  server.on("/led1on", handleOnLamp1);
  server.on("/led1off", handleOffLamp1);
  server.on("/led2on", handleOnLamp2);
  server.on("/led2off", handleOffLamp2);
  server.on("/led3on", handleOnLamp3);
  server.on("/led3off", handleOffLamp3);
  server.on("/led4on", handleOnLamp4);
  server.on("/led4off", handleOffLamp4);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  connectWiFi();
  }
