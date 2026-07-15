/*
  Sistema biodigestor com leitura de sensores, display OLED, WiFi e web server.
  Versão: 3.0.0
*/

// Bibliotecas

#include <WiFi.h>                    // WiFi para ESP32
#include <Adafruit_SSD1306.h>        // Display OLED SSD1306
#include <Adafruit_GFX.h>            // Gráficos para OLED
#include <OneWire.h>                 // Comunicação 1-Wire (DS18B20)
#include <DallasTemperature.h>       // Biblioteca DS18B20
#include <WebServer.h>               // Servidor HTTP para web
#include <SPIFFS.h>                  // Armazenamento em Flash
#include <time.h>                    // Timestamp

// Configurações e pinos

// --- WiFi ---
const char* ssid = "seu_SSID_aqui";
const char* password = "sua_senha_aqui";

// --- Pinos ---
#define MQ4_PIN 34                   // GPIO 34 (ADC) - Sensor MQ-4
#define TEMP_PIN 4                   // GPIO 4 (1-Wire) - DS18B20
#define PH_PIN 35                    // GPIO 35 (ADC) - pH BNC
#define PRESSURE_PIN 32              // GPIO 32 (ADC) - Sensor Pressão
#define SCREEN_WIDTH 128             // Largura OLED
#define SCREEN_HEIGHT 64             // Altura OLED

// --- I2C para OLED ---
#define OLED_ADDR 0x3C               // Endereço I2C padrão
#define SDA_PIN 21                   // GPIO 21 (SDA)
#define SCL_PIN 22                   // GPIO 22 (SCL)

// --- Intervalos ---
#define SENSOR_INTERVAL 10000        // Leitura sensores: 10s
#define DISPLAY_INTERVAL 1000        // Atualiza display: 1s
#define LOGGING_INTERVAL 3600000     // Logging em arquivo: 1 hora
#define SERIAL_BAUD 115200

// --- Limites e Calibração ---
#define TEMP_MIN_SAFE 15.0
#define TEMP_MAX_SAFE 55.0
#define pH_MIN 6.0
#define pH_MAX 8.5
#define PRESSURE_MAX 1200.0          // kPa

// Objetos

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
OneWire oneWire(TEMP_PIN);
DallasTemperature sensoresTemp(&oneWire);
WebServer servidor(80);

// Variáveis

// --- Sensores ---
float metano = 0.0;                  // ppm de metano (MQ-4)
float temperatura = 0.0;             // °C (DS18B20)
float pH = 7.0;                      // pH (BNC)
float pressao = 0.0;                 // kPa (Sensor Pressão)

// --- Estatísticas ---
float metanoMin = 999, metanoMax = 0;
float tempMin = 100, tempMax = -40;
float phMin = 14, phMax = 0;
float pressaoMin = 1200, pressaoMax = 0;

// --- Controle ---
unsigned long lastSensorRead = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastLogging = 0;
unsigned long startTime = 0;
int readCounter = 0;
int logCounter = 0;

// --- WiFi ---
bool wifiConnected = false;
String ultimaAtualizacao = "Aguardando...";

// Setup

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(100);
  inicializarSistema();
}

void loop() {
  servidor.handleClient();
  processarTarefas();
}

void inicializarSistema() {
  Serial.println("\n\n");
  Serial.println("=== BIODIGESTOR v3.0 ===");

  Serial.print("Inicializando SPIFFS... ");
  if (!SPIFFS.begin(true)) {
    Serial.println("FALHA!");
  } else {
    Serial.println("OK");
  }

  Serial.print("Inicializando display... ");
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("FALHA!");
  } else {
    Serial.println("OK");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Biodigestor v3.0");
    display.println("Inicializando...");
    display.display();
  }

  Serial.print("Inicializando DS18B20... ");
  sensoresTemp.begin();
  delay(500);
  Serial.println("OK");

  Serial.print("Conectando WiFi: ");
  Serial.println(ssid);
  conectarWiFi();

  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  configurarServidorWeb();
  startTime = millis();

  Serial.println("Sistema pronto!");
}

void processarTarefas() {
  unsigned long timeNow = millis();

  if (timeNow - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = timeNow;
    readCounter++;

    lerSensores();
    verificarAlertas();
    Serial.println(".");
  }

  if (timeNow - lastDisplayUpdate >= DISPLAY_INTERVAL) {
    lastDisplayUpdate = timeNow;
    atualizarDisplay();
  }

  if (timeNow - lastLogging >= LOGGING_INTERVAL) {
    lastLogging = timeNow;
    logarDados();
  }
}

// Leitura dos sensores

void lerSensores() {
  
  // --- MQ-4 (Metano) ---
  lerMetano();
  
  // --- DS18B20 (Temperatura) ---
  lerTemperatura();
  
  // --- pH ---
  lerPH();
  
  // --- Pressão ---
  lerPressao();
  
}

void lerMetano() {
  
  // Lê valor analógico do MQ-4
  int valor = analogRead(MQ4_PIN);
  
  // Converte para voltagem
  float voltagem = valor * (3.3 / 4095.0);
  
  // Calibração para ppm de metano
  // Fórmula aproximada: ppm = (voltagem - 0.9) * 200
  metano = max(0.0f, (voltagem - 0.9f) * 200.0f);
  
  // Atualiza mín/máx
  if (metano < metanoMin) metanoMin = metano;
  if (metano > metanoMax) metanoMax = metano;
  
}

void lerTemperatura() {
  
  sensoresTemp.requestTemperatures();
  temperatura = sensoresTemp.getTempCByIndex(0);
  
  // Verifica leitura válida
  if (temperatura == DEVICE_DISCONNECTED_C) {
    temperatura = 0;
    return;
  }
  
  if (temperatura < tempMin) tempMin = temperatura;
  if (temperatura > tempMax) tempMax = temperatura;
  
}

void lerPH() {
  
  // Lê múltiplas amostras
  float voltageSum = 0;
  for (int i = 0; i < 10; i++) {
    int valor = analogRead(PH_PIN);
    voltageSum += valor * (3.3 / 4095.0);
    delay(10);
  }
  
  float voltage = voltageSum / 10.0;
  
  // Calibração: 2.0V = pH 7.0 (aproximado)
  // 0.6V por unidade de pH
  pH = 7.0 + (2.0 - voltage) / 0.6;
  
  if (pH < phMin) phMin = pH;
  if (pH > phMax) phMax = pH;
  
}

void lerPressao() {
  
  // Lê sensor de pressão
  int valor = analogRead(PRESSURE_PIN);
  
  // Converte para kPa (0-4095 → 0-1200 kPa)
  pressao = (valor / 4095.0) * 1200.0;
  
  if (pressao < pressaoMin) pressaoMin = pressao;
  if (pressao > pressaoMax) pressaoMax = pressao;
  
}

// Alertas

void verificarAlertas() {
  
  if (temperatura < TEMP_MIN_SAFE || temperatura > TEMP_MAX_SAFE) {
    Serial.print("🔴 ALERTA TEMPERATURA: ");
    Serial.println(temperatura);
  }
  
  if (pH < pH_MIN || pH > pH_MAX) {
    Serial.print("⚠️  AVISO pH: ");
    Serial.println(pH);
  }
  
  if (metano > 500) {
    Serial.println("🔴 ALERTA: Nível alto de metano!");
  }
  
}

// Display

void atualizarDisplay() {
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  // Linha 1: Título e WiFi
  display.print("Biodigestor v3.0");
  if (wifiConnected) {
    display.println(" [WIFI]");
  } else {
    display.println(" [OFF]");
  }
  
  // Divisor
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  // Linha 2: Metano
  display.setCursor(0, 12);
  display.print("CH4: ");
  display.print(metano, 1);
  display.println(" ppm");
  
  // Linha 3: Temperatura
  display.print("Temp: ");
  display.print(temperatura, 1);
  display.println(" C");
  
  // Linha 4: pH
  display.print("pH: ");
  display.print(pH, 2);
  display.println();
  
  // Linha 5: Pressão
  display.print("Pres: ");
  display.print(pressao, 0);
  display.println(" kPa");
  
  // Divisor
  display.drawLine(0, 54, 128, 54, SSD1306_WHITE);
  
  // Status
  display.setCursor(0, 56);
  unsigned long tempoDecorrido = (millis() - startTime) / 1000;
  display.print("Tempo: ");
  display.print(tempoDecorrido / 3600);
  display.print("h ");
  display.display();
  
  // Atualiza hora na serial
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
  ultimaAtualizacao = String(buffer);
  
}

// Log

void logarDados() {
  
  logCounter++;
  
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char timestamp[30];
  strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", timeinfo);
  
  // Formata linha de dados
  char linha[256];
  snprintf(linha, sizeof(linha),
    "%d,%s,%.2f,%.2f,%.2f,%.2f,%s",
    logCounter, timestamp, metano, temperatura, pH, pressao,
    (wifiConnected) ? "OK" : "OFFLINE"
  );
  
  // Grava em arquivo
  File file = SPIFFS.open("/dados_biodigestor.csv", "a");
  if (file) {
    file.println(linha);
    file.close();
    Serial.print("✓ Log #");
    Serial.print(logCounter);
    Serial.println(" gravado");
  }
  
}

// WiFi

void conectarWiFi() {
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.print("✓ WiFi OK - IP: ");
    Serial.println(WiFi.localIP());
  } else {
    wifiConnected = false;
    Serial.println("✗ WiFi offline");
  }
  
}

// Web server

void configurarServidorWeb() {
  
  servidor.on("/", HTTP_GET, handleRoot);
  servidor.on("/api/dados", HTTP_GET, handleApiDados);
  servidor.on("/api/stats", HTTP_GET, handleApiStats);
  servidor.on("/api/logs", HTTP_GET, handleApiLogs);
  
  servidor.begin();
  Serial.println("Servidor web iniciado na porta 80");
  
}

void handleRoot() {
  
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width'>";
  html += "<title>Biodigestor</title>";
  html += "<style>";
  html += "body { font-family: Arial; margin: 20px; background: #f0f0f0; }";
  html += ".container { max-width: 600px; margin: 0 auto; background: white; padding: 20px; border-radius: 8px; }";
  html += "h1 { color: #333; }";
  html += ".sensor { background: #f9f9f9; padding: 15px; margin: 10px 0; border-radius: 5px; border-left: 4px solid #4CAF50; }";
  html += ".sensor-value { font-size: 24px; font-weight: bold; color: #4CAF50; }";
  html += ".sensor-label { font-size: 12px; color: #666; }";
  html += ".status { padding: 10px; border-radius: 5px; margin: 10px 0; }";
  html += ".status-ok { background: #d4edda; color: #155724; }";
  html += ".status-warning { background: #fff3cd; color: #856404; }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>🌱 Sistema Biodigestor v3.0</h1>";
  html += "<p>Última atualização: " + ultimaAtualizacao + "</p>";
  html += "<div class='sensor'>";
  html += "<div class='sensor-label'>METANO (ppm)</div>";
  html += "<div class='sensor-value'>" + String(metano, 1) + "</div>";
  html += "</div>";
  html += "<div class='sensor'>";
  html += "<div class='sensor-label'>TEMPERATURA (°C)</div>";
  html += "<div class='sensor-value'>" + String(temperatura, 1) + "</div>";
  html += "</div>";
  html += "<div class='sensor'>";
  html += "<div class='sensor-label'>pH</div>";
  html += "<div class='sensor-value'>" + String(pH, 2) + "</div>";
  html += "</div>";
  html += "<div class='sensor'>";
  html += "<div class='sensor-label'>PRESSÃO (kPa)</div>";
  html += "<div class='sensor-value'>" + String(pressao, 1) + "</div>";
  html += "</div>";
  html += "<div class='status " + String(wifiConnected ? "status-ok" : "status-warning") + "'>";
  html += "WiFi: " + String(wifiConnected ? "Conectado" : "Desconectado");
  html += "</div>";
  html += "<p><small>Leitura #" + String(readCounter) + " | API disponível em /api/dados</small></p>";
  html += "</div>";
  html += "</body>";
  html += "</html>";
  
  servidor.send(200, "text/html", html);
  
}

void handleApiDados() {
  
  String json = "{";
  json += "\"metano\":" + String(metano, 2) + ",";
  json += "\"temperatura\":" + String(temperatura, 2) + ",";
  json += "\"ph\":" + String(pH, 2) + ",";
  json += "\"pressao\":" + String(pressao, 2) + ",";
  json += "\"timestamp\":\"" + ultimaAtualizacao + "\",";
  json += "\"wifi\":" + String(wifiConnected ? "true" : "false");
  json += "}";
  
  servidor.send(200, "application/json", json);
  
}

void handleApiStats() {
  
  String json = "{";
  json += "\"metano\":{\"min\":" + String(metanoMin, 2) + ",\"max\":" + String(metanoMax, 2) + "},";
  json += "\"temperatura\":{\"min\":" + String(tempMin, 2) + ",\"max\":" + String(tempMax, 2) + "},";
  json += "\"ph\":{\"min\":" + String(phMin, 2) + ",\"max\":" + String(phMax, 2) + "},";
  json += "\"pressao\":{\"min\":" + String(pressaoMin, 2) + ",\"max\":" + String(pressaoMax, 2) + "}";
  json += "}";
  
  servidor.send(200, "application/json", json);
  
}

void handleApiLogs() {
  
  if (!SPIFFS.exists("/dados_biodigestor.csv")) {
    servidor.send(404, "text/plain", "Arquivo não encontrado");
    return;
  }
  
  File file = SPIFFS.open("/dados_biodigestor.csv", "r");
  String conteudo = "";
  while (file.available()) {
    conteudo += (char)file.read();
  }
  file.close();
  
  servidor.send(200, "text/csv", conteudo);
  
}

// Fim
