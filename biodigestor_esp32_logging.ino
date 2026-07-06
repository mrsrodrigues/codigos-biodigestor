/*
  ============================================================================
  SISTEMA DE AUTOMAÇÃO PARA BIODIGESTOR COM ESP32 - VERSÃO COM LOGGING
  ============================================================================
  
  Descrição:
    Sistema de monitoramento com armazenamento de dados em SPIFFS (memória interna)
    para experimento de 10 dias com leituras a cada hora.
  
  Autor: Sistema Biodigestor
  Versão: 2.0.0 (Com Logging de Dados)
  Data: 12 de Junho de 2026
  
  Novidades v2.0.0:
    - Logging de dados em arquivo CSV a cada hora
    - Suporte para experimentos longos (até 10+ dias)
    - Comandos para consultar dados armazenados
    - Formatos estruturados para análise
  
  ============================================================================
*/

// ============================================================================
// INCLUSÃO DE BIBLIOTECAS
// ============================================================================

#include <WiFi.h>                    // Biblioteca WiFi para ESP32
#include <DHT.h>                     // Biblioteca para sensor DHT22
#include <Adafruit_BMP280.h>         // Biblioteca para sensor BMP280
#include <Adafruit_Sensor.h>         // Biblioteca Adafruit Sensor unificada
#include <Wire.h>                    // Biblioteca para comunicação I2C
#include <SPIFFS.h>                  // Biblioteca para armazenamento em Flash
#include <time.h>                    // Biblioteca para timestamp

// ============================================================================
// DEFINIÇÕES E CONSTANTES
// ============================================================================

// --- Configurações WiFi ---
const char* ssid = "seu_SSID_aqui";
const char* password = "sua_senha_aqui";

// --- Configurações de Pinos ---
#define DHTPIN 4
#define DHTTYPE DHT22
#define pH_PIN 34
#define SDA_PIN 21
#define SCL_PIN 22
#define LED_ALERT 5
#define BUZZER_PIN 12

// --- Configurações de Tempo ---
#define SENSOR_INTERVAL 10000           // Intervalo leitura rápida: 10 segundos
#define LOGGING_INTERVAL 3600000        // Intervalo logging: 1 hora (3600000 ms)
#define WIFI_TIMEOUT 20000
#define SERIAL_BAUD 115200

// --- Limites ---
#define TEMP_MIN_SAFE 20.0
#define TEMP_MAX_SAFE 60.0
#define pH_MIN_IDEAL 6.5
#define pH_MAX_IDEAL 8.0
#define PRESSURE_MAX_ALERT 1050.0

// --- Configurações de Arquivo ---
#define DATA_FILE "/dados_biodigestor.csv"
#define HEADER_FILE "/header_escrito"
#define MAX_ENTRIES 300                 // Máximo de entradas (300 = 12.5 dias com leituras horárias)

// ============================================================================
// INSTÂNCIAS DE OBJETOS
// ============================================================================

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp280;

// ============================================================================
// VARIÁVEIS GLOBAIS
// ============================================================================

// --- Dados de Sensores ---
float temperatura = 0.0;
float tempMin = 100.0;
float tempMax = -40.0;

float pressao = 0.0;
float pressaoMin = 1100.0;
float pressaoMax = 900.0;

float pH = 7.0;
float phVoltageMin = 0.67;
float phVoltageMax = 3.33;

// --- Controle de Tempo ---
unsigned long lastSensorRead = 0;
unsigned long lastLoggingTime = 0;      // Controla logging horário
unsigned long startTime = 0;            // Tempo de início do experimento
int loggingCounter = 0;                 // Contador de entradas registradas

// --- WiFi ---
bool wifiConnected = false;
unsigned long lastWiFiCheck = 0;

// --- Outros ---
int readCounter = 0;
bool alertaTempBaixa = false;
bool alertaTempAlta = false;

// ============================================================================
// FUNÇÃO DE SETUP
// ============================================================================

void setup() {
  
  Serial.begin(SERIAL_BAUD);
  delay(100);
  
  Serial.println("\n\n");
  Serial.println("=================================================");
  Serial.println("   BIODIGESTOR ESP32 - v2.0.0 COM LOGGING       ");
  Serial.println("=================================================");
  Serial.println("Modo: Experimento com Armazenamento de Dados");
  Serial.println("Duração: Até 10 dias com leituras horárias");
  Serial.println("=================================================");
  
  // Inicializa pinos
  pinMode(LED_ALERT, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_ALERT, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Inicializa SPIFFS
  Serial.print("Inicializando SPIFFS... ");
  if (!SPIFFS.begin(true)) {
    Serial.println("FALHA!");
    while (1) {
      digitalWrite(LED_ALERT, HIGH);
      delay(200);
      digitalWrite(LED_ALERT, LOW);
      delay(200);
    }
  }
  Serial.println("OK");
  
  // Verifica espaço disponível
  exibirInfoSPIFFS();
  
  // Inicializa sensores
  Serial.print("Inicializando DHT22... ");
  dht.begin();
  delay(500);
  Serial.println("OK");
    // Inicializa I2C para BMP280 (especifica SDA e SCL)
  Wire.begin(SDA_PIN, SCL_PIN);
    Serial.print("Inicializando BMP280 (I2C)... ");
  if (!bmp280.begin(0x76)) {
    Serial.println("FALHA!");
    while (1) {
      digitalWrite(LED_ALERT, HIGH);
      delay(200);
      digitalWrite(LED_ALERT, LOW);
      delay(200);
    }
  }
  Serial.println("OK");
  
  bmp280.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );
  
  // Conecta WiFi
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  conectarWiFi();
  
  // Configura Timezone (Brasil: UTC-3)
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  
  // Marca início do experimento
  startTime = millis();
  lastLoggingTime = millis();
  
  // Cria arquivo com cabeçalho se não existir
  if (!SPIFFS.exists(HEADER_FILE)) {
    criarArquivoComCabecalho();
  }
  
  Serial.println();
  Serial.println("=================================================");
  Serial.println("Sistema pronto para experimento!");
  Serial.println("Dados serão registrados a cada 1 hora");
  Serial.println("=================================================");
  Serial.println();
  
}

// ============================================================================
// FUNÇÃO LOOP
// ============================================================================

void loop() {
  
  unsigned long timeNow = millis();
  
  // --- Leitura Rápida de Sensores (a cada 10s) ---
  if (timeNow - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = timeNow;
    readCounter++;
    
    lerSensores();
    verificarAlertas();
    
    // Reconecta WiFi se necessário
    if (timeNow - lastWiFiCheck >= 30000) {
      lastWiFiCheck = timeNow;
      if (WiFi.status() != WL_CONNECTED) {
        conectarWiFi();
      }
    }
    
    exibirDados();
  }
  
  // --- Logging de Dados (a cada 1 hora) ---
  if (timeNow - lastLoggingTime >= LOGGING_INTERVAL) {
    lastLoggingTime = timeNow;
    logarDados();  // Grava dados no arquivo
  }
  
  // Processa comandos Serial
  if (Serial.available()) {
    procesarComandoSerial();
  }
  
}

// ============================================================================
// FUNÇÕES DE LEITURA DE SENSORES
// ============================================================================

void lerSensores() {
  lerTemperatura();
  lerPressao();
  lerPH();
}

void lerTemperatura() {
  temperatura = dht.readTemperature();
  
  if (isnan(temperatura)) {
    Serial.println("ERRO: Falha ao ler temperatura!");
    temperatura = 0.0;
    return;
  }
  
  if (temperatura < tempMin) tempMin = temperatura;
  if (temperatura > tempMax) tempMax = temperatura;
}

void lerPressao() {
  pressao = bmp280.readPressure() / 100.0;
  
  if (pressao < pressaoMin && pressao > 800) pressaoMin = pressao;
  if (pressao > pressaoMax && pressao < 1100) pressaoMax = pressao;
}

void lerPH() {
  float voltageSum = 0;
  const int amostras = 10;
  
  for (int i = 0; i < amostras; i++) {
    int sensorValue = analogRead(pH_PIN);
    float voltage = sensorValue * (3.3 / 4095.0);
    voltageSum += voltage;
    delay(10);
  }
  
  float voltageMedia = voltageSum / amostras;
  pH = (voltageMedia - phVoltageMin) / (phVoltageMax - phVoltageMin) * 10.0 + 2.0;
  
  if (pH < 0) pH = 0;
  if (pH > 14) pH = 14;
}

// ============================================================================
// FUNÇÕES DE LOGGING EM ARQUIVO
// ============================================================================

/**
 * Função: criarArquivoComCabecalho
 * Descrição: Cria arquivo CSV com cabeçalho
 */
void criarArquivoComCabecalho() {
  
  File file = SPIFFS.open(DATA_FILE, "w");
  
  if (!file) {
    Serial.println("ERRO: Não foi possível criar arquivo de dados!");
    return;
  }
  
  // Escreve cabeçalho CSV
  file.println("LEITURA,DATA_HORA,TEMP_C,TEMP_MIN,TEMP_MAX,PRESSAO_HPA,PH,WIFI_STATUS");
  
  file.close();
  
  // Marca que cabeçalho foi escrito
  File marker = SPIFFS.open(HEADER_FILE, "w");
  marker.println("1");
  marker.close();
  
  Serial.println("✓ Arquivo de dados criado com sucesso!");
  
}

/**
 * Função: logarDados
 * Descrição: Grava leitura atual no arquivo CSV a cada hora
 */
void logarDados() {
  
  // Verifica se atingiu limite de entradas
  if (loggingCounter >= MAX_ENTRIES) {
    Serial.println("⚠️  AVISO: Limite de registros atingido!");
    Serial.println("    Arquivo contém 300 entradas (12.5 dias)");
    Serial.println("    Comando 'D' para consultar ou 'L' para limpar");
    return;
  }
  
  File file = SPIFFS.open(DATA_FILE, "a");  // Append mode
  
  if (!file) {
    Serial.println("ERRO: Não foi possível abrir arquivo para gravação!");
    return;
  }
  
  loggingCounter++;
  
  // Obtém hora atual
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char timestamp[30];
  strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", timeinfo);
  
  // Monta linha de dados
  char linha[256];
  snprintf(linha, sizeof(linha), 
    "%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%s",
    loggingCounter,
    timestamp,
    temperatura,
    tempMin,
    tempMax,
    pressao,
    pH,
    (WiFi.status() == WL_CONNECTED) ? "OK" : "OFFLINE"
  );
  
  // Escreve linha no arquivo
  file.println(linha);
  file.close();
  
  // Exibe confirmação
  Serial.println();
  Serial.println("=================================================");
  Serial.print("✓ Dados gravados - Entrada #");
  Serial.print(loggingCounter);
  Serial.print(" em ");
  Serial.println(timestamp);
  Serial.println("=================================================");
  
}

/**
 * Função: exibirInfoSPIFFS
 * Descrição: Exibe informações sobre armazenamento SPIFFS
 */
void exibirInfoSPIFFS() {
  
  size_t totalBytes = SPIFFS.totalBytes();
  size_t usedBytes = SPIFFS.usedBytes();
  size_t freeBytes = totalBytes - usedBytes;
  
  Serial.print("  Total: ");
  Serial.print(totalBytes / 1024);
  Serial.print(" KB | Usado: ");
  Serial.print(usedBytes / 1024);
  Serial.print(" KB | Livre: ");
  Serial.print(freeBytes / 1024);
  Serial.println(" KB");
  
}

// ============================================================================
// FUNÇÕES DE CONSULTA E GERENCIAMENTO DE DADOS
// ============================================================================

/**
 * Função: exibirDadosArmazenados
 * Descrição: Exibe todos os dados armazenados
 */
void exibirDadosArmazenados() {
  
  if (!SPIFFS.exists(DATA_FILE)) {
    Serial.println("Arquivo de dados não existe!");
    return;
  }
  
  Serial.println("\n=================================================");
  Serial.println("        DADOS ARMAZENADOS NO EXPERIMENTO         ");
  Serial.println("=================================================");
  
  File file = SPIFFS.open(DATA_FILE, "r");
  
  if (!file) {
    Serial.println("Erro ao abrir arquivo!");
    return;
  }
  
  int linhas = 0;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    Serial.println(line);
    linhas++;
  }
  
  file.close();
  
  Serial.println("=================================================");
  Serial.print("Total de linhas: ");
  Serial.println(linhas);
  Serial.println("=================================================\n");
  
}

/**
 * Função: exibirStatisticasExperimento
 * Descrição: Exibe estatísticas calculadas dos dados
 */
void exibirStatisticasExperimento() {
  
  if (!SPIFFS.exists(DATA_FILE)) {
    Serial.println("Arquivo de dados não existe!");
    return;
  }
  
  Serial.println("\n=================================================");
  Serial.println("      ESTATÍSTICAS DO EXPERIMENTO               ");
  Serial.println("=================================================");
  
  File file = SPIFFS.open(DATA_FILE, "r");
  
  if (!file) {
    Serial.println("Erro ao abrir arquivo!");
    return;
  }
  
  float tempMédia = 0, tempMín = 100, tempMáx = -40;
  float pressaoMédia = 0, phMédio = 0;
  int contagem = 0;
  
  // Pula cabeçalho
  file.readStringUntil('\n');
  
  while (file.available()) {
    String line = file.readStringUntil('\n');
    
    if (line.length() == 0) continue;
    
    // Parse CSV simples
    int idx1 = line.indexOf(',');
    int idx2 = line.indexOf(',', idx1 + 1);
    int idx3 = line.indexOf(',', idx2 + 1);
    int idx4 = line.indexOf(',', idx3 + 1);
    int idx5 = line.indexOf(',', idx4 + 1);
    int idx6 = line.indexOf(',', idx5 + 1);
    int idx7 = line.indexOf(',', idx6 + 1);
    
    float temp = line.substring(idx2 + 1, idx3).toFloat();
    float tempMn = line.substring(idx3 + 1, idx4).toFloat();
    float tempMx = line.substring(idx4 + 1, idx5).toFloat();
    float pres = line.substring(idx5 + 1, idx6).toFloat();
    float ph = line.substring(idx6 + 1, idx7).toFloat();
    
    tempMédia += temp;
    if (temp < tempMín) tempMín = temp;
    if (temp > tempMáx) tempMáx = temp;
    pressaoMédia += pres;
    phMédio += ph;
    
    contagem++;
  }
  
  file.close();
  
  if (contagem > 0) {
    tempMédia /= contagem;
    pressaoMédia /= contagem;
    phMédio /= contagem;
    
    Serial.println("\nTEMPERATURA:");
    Serial.print("  Mínima: ");
    Serial.print(tempMín, 2);
    Serial.println("°C");
    Serial.print("  Máxima: ");
    Serial.print(tempMáx, 2);
    Serial.println("°C");
    Serial.print("  Média: ");
    Serial.print(tempMédia, 2);
    Serial.println("°C");
    Serial.print("  Amplitude: ");
    Serial.print(tempMáx - tempMín, 2);
    Serial.println("°C");
    
    Serial.println("\nPRESSÃO:");
    Serial.print("  Média: ");
    Serial.print(pressaoMédia, 2);
    Serial.println(" hPa");
    
    Serial.println("\npH:");
    Serial.print("  Média: ");
    Serial.print(phMédio, 2);
    Serial.println();
    
    Serial.print("\nTotal de Leituras: ");
    Serial.println(contagem);
    
    Serial.print("Duração Estimada: ");
    Serial.print((contagem - 1));
    Serial.println(" horas");
    
    unsigned long horasDecorridas = (contagem - 1);
    unsigned long dias = horasDecorridas / 24;
    unsigned long horas = horasDecorridas % 24;
    Serial.print("Que corresponde a: ");
    Serial.print(dias);
    Serial.print(" dias e ");
    Serial.print(horas);
    Serial.println(" horas");
  }
  
  Serial.println("=================================================\n");
  
}

/**
 * Função: limparDadosArmazenados
 * Descrição: Limpa arquivo de dados (para novo experimento)
 */
void limparDadosArmazenados() {
  
  Serial.println("Limpando dados do experimento anterior...");
  
  if (SPIFFS.remove(DATA_FILE)) {
    Serial.println("✓ Arquivo de dados removido");
  }
  
  if (SPIFFS.remove(HEADER_FILE)) {
    Serial.println("✓ Marcador removido");
  }
  
  loggingCounter = 0;
  
  // Recria arquivo com cabeçalho
  criarArquivoComCabecalho();
  
  Serial.println("✓ Sistema pronto para novo experimento!");
  
}

// ============================================================================
// FUNÇÕES DE ALERTA
// ============================================================================

void verificarAlertas() {
  
  if (temperatura < TEMP_MIN_SAFE) {
    if (!alertaTempBaixa) {
      Serial.println("🔴 ALERTA: Temperatura muito baixa!");
      acionarAlerta(2);
      alertaTempBaixa = true;
    }
  } else if (temperatura > TEMP_MAX_SAFE) {
    if (!alertaTempAlta) {
      Serial.println("🔴 ALERTA: Temperatura muito alta!");
      acionarAlerta(1);
      alertaTempAlta = true;
    }
  } else {
    alertaTempBaixa = false;
    alertaTempAlta = false;
    digitalWrite(LED_ALERT, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  if (pH < pH_MIN_IDEAL || pH > pH_MAX_IDEAL) {
    Serial.print("⚠️  AVISO: pH fora da faixa ideal (");
    Serial.print(pH);
    Serial.println(")");
  }
  
}

void acionarAlerta(int modo) {
  
  if (modo == 1) {
    digitalWrite(LED_ALERT, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (modo == 2) {
    digitalWrite(LED_ALERT, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
  
}

// ============================================================================
// FUNÇÕES DE CONECTIVIDADE
// ============================================================================

void conectarWiFi() {
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && 
         millis() - startTime < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("✓ WiFi conectado!");
    Serial.print("  IP: ");
    Serial.println(WiFi.localIP());
  } else {
    wifiConnected = false;
    Serial.println("✗ WiFi offline - continuando...");
  }
  
}

// ============================================================================
// FUNÇÕES DE EXIBIÇÃO
// ============================================================================

void exibirDados() {
  
  Serial.println();
  Serial.print("LEITURA #");
  Serial.print(readCounter);
  Serial.print(" | Entrada Log #");
  Serial.print(loggingCounter);
  Serial.print(" | Tempo: ");
  
  unsigned long tempoDecorrido = (millis() - startTime) / 1000;
  unsigned long dias = tempoDecorrido / 86400;
  unsigned long horas = (tempoDecorrido % 86400) / 3600;
  unsigned long minutos = (tempoDecorrido % 3600) / 60;
  
  Serial.print(dias);
  Serial.print("d ");
  Serial.print(horas);
  Serial.print("h ");
  Serial.print(minutos);
  Serial.println("m");
  
  Serial.print("📊 TEMP: ");
  Serial.print(temperatura, 1);
  Serial.print("°C | PRES: ");
  Serial.print(pressao, 1);
  Serial.print(" hPa | pH: ");
  Serial.println(pH, 2);
  
  Serial.print("📡 WiFi: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("OK");
  } else {
    Serial.println("OFFLINE");
  }
  
}

// ============================================================================
// FUNÇÕES DE COMANDO SERIAL
// ============================================================================

void procesarComandoSerial() {
  
  char comando = Serial.read();
  
  switch (comando) {
    
    case 'R':
    case 'r':
      Serial.println("Resetando valores min/max...");
      tempMin = temperatura;
      tempMax = temperatura;
      pressaoMin = pressao;
      pressaoMax = pressao;
      Serial.println("✓ Valores resetados!");
      break;
      
    case 'S':
    case 's':
      exibirEstatisticas();
      break;
      
    case 'C':
    case 'c':
      Serial.println("Reconectando WiFi...");
      conectarWiFi();
      break;
      
    case 'T':
    case 't':
      testarSensores();
      break;
      
    case 'D':
    case 'd':
      exibirDadosArmazenados();
      break;
      
    case 'E':
    case 'e':
      exibirStatisticasExperimento();
      break;
      
    case 'L':
    case 'L': {
      Serial.println("\n⚠️  Tem certeza que deseja limpar dados?");
      Serial.println("Confirme digitando 'Y' ou 'N':");
      delay(100);
      while (!Serial.available()) delay(10);
      char confirmacao = Serial.read();
      if (confirmacao == 'Y' || confirmacao == 'y') {
        limparDadosArmazenados();
      } else {
        Serial.println("Operação cancelada!");
      }
      break;
    }
      
    case 'H':
    case 'h':
      exibirAjuda();
      break;
      
    default:
      if (comando >= 32 && comando <= 126) {
        Serial.print("Comando desconhecido: ");
        Serial.println(comando);
        Serial.println("Digite 'H' para ajuda");
      }
      break;
      
  }
  
}

void exibirEstatisticas() {
  
  Serial.println("\n=================================================");
  Serial.println("            ESTATÍSTICAS DO SISTEMA              ");
  Serial.println("=================================================");
  
  Serial.print("Total de leituras rápidas: ");
  Serial.println(readCounter);
  
  Serial.print("Total de registros gravados: ");
  Serial.println(loggingCounter);
  
  unsigned long tempoDecorrido = (millis() - startTime) / 1000;
  Serial.print("Tempo decorrido: ");
  Serial.print(tempoDecorrido / 3600);
  Serial.print("h ");
  Serial.print((tempoDecorrido % 3600) / 60);
  Serial.println("m");
  
  Serial.print("WiFi: ");
  Serial.println((WiFi.status() == WL_CONNECTED) ? "Conectado" : "Desconectado");
  
  Serial.println("=================================================\n");
  
}

void exibirAjuda() {
  
  Serial.println("\n=================================================");
  Serial.println("           COMANDOS DISPONÍVEIS                  ");
  Serial.println("=================================================");
  Serial.println("R - Reseta valores min/max dos sensores");
  Serial.println("S - Exibe estatísticas do sistema");
  Serial.println("C - Reconecta WiFi");
  Serial.println("T - Testa todos os sensores");
  Serial.println("D - Exibe dados armazenados");
  Serial.println("E - Exibe estatísticas do experimento");
  Serial.println("L - Limpa dados (novo experimento)");
  Serial.println("H - Exibe esta ajuda");
  Serial.println("=================================================\n");
  
}

void testarSensores() {
  
  Serial.println("\n=================================================");
  Serial.println("         TESTE DE SENSORES - INICIADO            ");
  Serial.println("=================================================");
  
  Serial.print("DHT22... ");
  float tempTeste = dht.readTemperature();
  if (!isnan(tempTeste)) {
    Serial.print("OK (");
    Serial.print(tempTeste);
    Serial.println("°C)");
  } else {
    Serial.println("FALHA!");
  }
  
  Serial.print("BMP280... ");
  float pressaoTeste = bmp280.readPressure() / 100.0;
  if (pressaoTeste > 0) {
    Serial.print("OK (");
    Serial.print(pressaoTeste);
    Serial.println(" hPa)");
  } else {
    Serial.println("FALHA!");
  }
  
  Serial.print("pH Sensor... ");
  int pHRaw = analogRead(pH_PIN);
  Serial.print("OK (ADC: ");
  Serial.print(pHRaw);
  Serial.println(")");
  
  Serial.println("=================================================\n");
  
}

// ============================================================================
// FIM DO CÓDIGO
// ============================================================================
