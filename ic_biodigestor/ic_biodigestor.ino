// Bibliotecas
#include <WiFi.h>                    
#include <Wire.h>                    
#include <Adafruit_SSD1306.h>        
#include <OneWire.h>                 
#include <DallasTemperature.h>       
#include <SPIFFS.h>                  
#include <time.h>                    
#include <WebServer.h>               // Adicionado para exportar via rede local

// --- Configurações da Rede ---
const char* ssid = "Studio Home";
const char* password = "123sandroEKI";

// --- Pinos ---
#define MQ4_PIN 1                  
#define TEMP_PIN 2                 
#define PH_PIN 6                   
#define PRESSURE_PIN 7             

// --- I2C para OLED ---
#define OLED_ADDR 0x3C               
#define SDA_PIN 17                   
#define SCL_PIN 18                   

// --- Intervalos ---
#define SENSOR_INTERVAL 10000        // Leitura: 10s
#define DISPLAY_INTERVAL 1000        // Display: 1s
#define LOGGING_INTERVAL 3600000     // Salvar na memória: 1 hora

Adafruit_SSD1306 display(128, 64, &Wire, -1);
OneWire oneWire(TEMP_PIN);
DallasTemperature sensoresTemp(&oneWire);

// Micro-servidor apenas para despejar o texto CSV
WebServer server(80);

// Variáveis
float metano = 0.0;                  
float temperatura = 0.0;             
float pH = 7.0;                      
float pressao = 0.0;                 

unsigned long lastSensorRead = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastLogging = 0;

bool wifiConnected = false;

void setup() {
  SPIFFS.begin(true);

  Wire.begin(SDA_PIN, SCL_PIN);
  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Iniciando...");
    display.display();
  }

  sensoresTemp.begin();
  
  conectarWiFi();
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // ROTA 1: Baixar os dados (À prova de travamentos para arquivos gigantes)
  server.on("/", []() {
    File file = SPIFFS.open("/dados_biodigestor.csv", "r");
    if (!file) {
      server.send(200, "text/plain", "Nenhum dado gravado na memoria ainda.");
      return;
    }
    
    // Diz ao navegador para fazer o download do arquivo como CSV
    server.sendHeader("Content-Disposition", "attachment; filename=dados_biodigestor.csv");
    
    // Faz o streaming direto sem usar a RAM da placa
    server.streamFile(file, "text/csv");
    file.close();
  });

  // ROTA 2: Apagar a memória para um novo experimento
  server.on("/limpar", []() {
    SPIFFS.remove("/dados_biodigestor.csv");
    server.send(200, "text/plain", "Memoria apagada com sucesso! O sistema esta zerado para a proxima coleta.");
  });
  
  server.begin();
}

void loop() {
  unsigned long timeNow = millis();

  // Mantém o servidor aguardando você acessar pelo navegador
  server.handleClient();

  // Leitura dos sensores
  if (timeNow - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = timeNow;
    lerSensores();
  }

  // Atualiza OLED
  if (timeNow - lastDisplayUpdate >= DISPLAY_INTERVAL) {
    lastDisplayUpdate = timeNow;
    atualizarDisplay();
  }

  // Grava na Memória Interna (1 hora)
  if (timeNow - lastLogging >= LOGGING_INTERVAL) {
    lastLogging = timeNow;
    logarDados();
  }
}

void lerSensores() {
  int valorMQ4 = analogRead(MQ4_PIN);
  float voltagemMQ4 = valorMQ4 * (3.3 / 4095.0);
  metano = max(0.0f, (voltagemMQ4 - 0.9f) * 200.0f); 
  
  sensoresTemp.requestTemperatures();
  temperatura = sensoresTemp.getTempCByIndex(0);
  if (temperatura == DEVICE_DISCONNECTED_C) temperatura = 0.0;
  
  float voltageSum = 0;
  for (int i = 0; i < 10; i++) {
    voltageSum += analogRead(PH_PIN) * (3.3 / 4095.0);
    delay(10);
  }
  float voltagemPH = voltageSum / 10.0;
  pH = 7.0 + (2.0 - voltagemPH) / 0.6; 
  
  int valorPressao = analogRead(PRESSURE_PIN);
  float voltagemPressao = valorPressao * (3.3 / 4095.0);
  pressao = max(0.0f, (voltagemPressao - 0.5f) * 300.0f); 
}

void atualizarDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  // Alterna o topo do OLED entre o IP e o Relógio a cada 3 segundos
  if ((millis() / 3000) % 2 == 0 && wifiConnected) {
    display.print("IP: ");
    display.print(WiFi.localIP());
  } else {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      display.print("Sincronizando...");
    } else {
      display.print(&timeinfo, "%d/%m %H:%M:%S"); 
    }
  }
  
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.setCursor(0, 12);
  display.print("CH4:  "); display.print(metano, 1); display.println(" ppm");
  display.print("Temp: "); display.print(temperatura, 1); display.println(" C");
  display.print("pH:   "); display.print(pH, 2); display.println();
  display.print("Pres: "); display.print(pressao, 0); display.println(" kPa");
  display.display();
}

void logarDados() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return; // Se não tem hora certa, não salva
  }

  char dataHora[30];
  strftime(dataHora, sizeof(dataHora), "%d/%m/%Y,%H:%M:%S", &timeinfo);
  
  char linha[256];
  snprintf(linha, sizeof(linha), "%s,%.2f,%.2f,%.2f,%.2f", dataHora, temperatura, pH, pressao, metano);
  
  File file = SPIFFS.open("/dados_biodigestor.csv", "a");
  if (file) {
    file.println(linha);
    file.close();
  }
}

void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
  } else {
    wifiConnected = false;
  }
}