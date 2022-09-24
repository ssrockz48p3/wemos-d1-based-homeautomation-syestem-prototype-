#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#ifndef STASSID
#define STASSID "RAVIKUMAR"
#define STAPSK  "ravikrishna2u"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80);
const int led = 13;
const int led0 = D6;
const int led1 = D3;
const int led2 = D4;
const int led3 = D5;
const int button = A0;
String page = "";
String text = "";
double data;

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri(); 
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";}
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");}
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");}
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/data.txt", []() {
    text = (String)data +"%";
    server.send(200, "text/html", text); });

  server.on("/", []() {
    page = "<h1> Wemos D1 Web server</h1><h1>Button-state:</h1> <h1 id=\"data\">""</h1>\r\n";
    page += "<script>\r\n";
    page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 1000);\r\n";
    page += "function loadData(url, callback){\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function(){\r\n";
    page += " if(this.readyState == 4 && this.status == 200){\r\n";
    page += " callback.apply(xhttp);\r\n";
    page += " }\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", url, true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}\r\n";
    page += "function updateData(){\r\n";
    page += " document.getElementById(\"data\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
    page += "</script>\r\n";
    server.send(200, "text/html", page); });

  server.on("/led0_on", []() {
    Serial.println("led0_on");
    server.send(200, "text/plain", "BOOM");
    digitalWrite(led0, HIGH);});
  server.on("/led0_off", []() {
    Serial.println("led0_off");
    server.send(200, "text/plain", "BAMM");
    digitalWrite(led0, LOW);});
  server.on("/led1_on", []() {
    Serial.println("led1_on");
    server.send(200, "text/plain", "BOOM");
    digitalWrite(led1, HIGH);});
  server.on("/led1_off", []() {
    Serial.println("led1_off");
    server.send(200, "text/plain", "BAMM");
    digitalWrite(led1, LOW);});
  server.on("/led2_on", []() {
    Serial.println("led2_on");
    server.send(200, "text/plain", "BOOM");
    digitalWrite(led2, HIGH);});
  server.on("/led2_off", []() {
    Serial.println("led2_off");
    server.send(200, "text/plain", "BAMM");
    digitalWrite(led2, LOW);});
  server.on("/led3_on", []() {
    Serial.println("led3_on");
    server.send(200, "text/plain", "BOOM");
    digitalWrite(led3, HIGH);});
  server.on("/led3_off", []() {
    Serial.println("led3_off");
    server.send(200, "text/plain", "BAMM");
    digitalWrite(led3, LOW);});

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));});

  server.onNotFound(handleNotFound);

  server.addHook([](const String & method, const String & url, WiFiClient * client, ESP8266WebServer::ContentTypeFunction contentType) {
    (void)method;      // GET, PUT, ...
    (void)url;         // example: /root/myfile.html
    (void)client;      // the webserver tcp client connection
    (void)contentType; // contentType(".html") => "text/html"
    Serial.printf("A useless web hook has passed\n");
    Serial.printf("(this hook is in 0x%08x area (401x=IRAM 402x=FLASH))\n", esp_get_program_counter());
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;});
  server.addHook([](const String&, const String & url, WiFiClient*, ESP8266WebServer::ContentTypeFunction) {
    if (url.startsWith("/fail")) {
      Serial.printf("An always failing web hook has been triggered\n");
      return ESP8266WebServer::CLIENT_MUST_STOP;}
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;});
  server.addHook([](const String&, const String & url, WiFiClient * client, ESP8266WebServer::ContentTypeFunction) {
    if (url.startsWith("/dump")) {
      Serial.printf("The dumper web hook is on the run\n");

#ifdef STREAMSEND_API
      // we are lucky
      client->sendAll(Serial, 500);
#else
      auto last = millis();
      while ((millis() - last) < 500) {
        char buf[32];
        size_t len = client->read((uint8_t*)buf, sizeof(buf));
        if (len > 0) {
          Serial.printf("(<%d> chars)", (int)len);
          Serial.write(buf, len);
          last = millis();}}
#endif
      Serial.printf("\nTelling server to forget this connection\n");
      static WiFiClient forgetme = *client;
      return ESP8266WebServer::CLIENT_IS_GIVEN;}
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;});
  server.begin();
  Serial.println("HTTP server started");}

void loop(void) {
  server.handleClient();
  MDNS.update();}
