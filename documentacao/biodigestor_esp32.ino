/*
  ============================================================================
  SISTEMA DE AUTOMAÇÃO PARA BIODIGESTOR COM ESP32
  ============================================================================
  
  Descrição:
    Este código implementa um sistema completo de monitoramento para um 
    biodigestor utilizando placa ESP32. O sistema monitora temperatura,
    pressão e pH continuamente.
  
  Autor: Sistema Biodigestor
  Versão: 1.0.0
  Data: 12 de Junho de 2026
  
  Sensores:
    - DHT22: Sensor de Temperatura
    - BMP280: Sensor de Pressão
    - Analógico: Sensor de pH
    - WiFi: Integrado no ESP32
  
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


// ============================================================================
// DEFINIÇÕES E CONSTANTES
// ============================================================================

// --- Configurações WiFi ---
const char* ssid = "seu_SSID_aqui";              // SSID da rede WiFi
const char* password = "sua_senha_aqui";         // Senha da rede WiFi
const char* serverName = "seu_servidor.com";     // URL do servidor (opcional)

// --- Configurações de Pinos ---
#define DHTPIN 4                    // GPIO 4 - Pino do sensor DHT22
#define DHTTYPE DHT22               // Tipo de sensor DHT (DHT22 ou DHT11)
#define pH_PIN 34                   // GPIO 34 - Entrada analógica do pH
#define SDA_PIN 21                  // GPIO 21 - SDA I2C para BMP280
#define SCL_PIN 22                  // GPIO 22 - SCL I2C para BMP280
#define LED_ALERT 5                 // GPIO 5 - LED de alerta
#define BUZZER_PIN 12               // GPIO 12 - Buzzer de alerta

// --- Configurações de Tempo ---
#define SENSOR_INTERVAL 10000       // Intervalo de leitura dos sensores (ms)
#define WIFI_TIMEOUT 20000          // Timeout para conexão WiFi (ms)
#define SERIAL_BAUD 115200          // Velocidade da Serial (bps)

// --- Configurações de Limites ---
#define TEMP_MIN_SAFE 20.0          // Temperatura mínima segura (°C)
#define TEMP_MAX_SAFE 60.0          // Temperatura máxima segura (°C)
#define pH_MIN_IDEAL 6.5            // pH mínimo ideal
#define pH_MAX_IDEAL 8.0            // pH máximo ideal
#define PRESSURE_MAX_ALERT 1050.0   // Pressão máxima de alerta (hPa)


// ============================================================================
// INSTÂNCIAS DE OBJETOS DOS SENSORES
// ============================================================================

DHT dht(DHTPIN, DHTTYPE);           // Objeto DHT para sensor de temperatura
Adafruit_BMP280 bmp280;             // Objeto BMP280 para sensor de pressão


// ============================================================================
// VARIÁVEIS GLOBAIS
// ============================================================================

// --- Dados de Temperatura ---
float temperatura = 0.0;            // Temperatura atual (°C)
float tempMin = 100.0;              // Temperatura mínima registrada
float tempMax = -40.0;              // Temperatura máxima registrada
bool alertaTempBaixa = false;        // Flag de alerta temperatura baixa
bool alertaTempAlta = false;         // Flag de alerta temperatura alta

// --- Dados de Pressão ---
float pressao = 0.0;                // Pressão atual (hPa)
float pressaoMin = 1100.0;          // Pressão mínima registrada
float pressaoMax = 900.0;           // Pressão máxima registrada

// --- Dados de pH ---
float pH = 7.0;                     // pH atual
float pH_MIN = 6.5;                 // pH mínimo calibrado
float pH_MAX = 8.0;                 // pH máximo calibrado
float phVoltageMin = 0.67;          // Voltagem em pH 2.0
float phVoltageMax = 3.33;          // Voltagem em pH 12.0

// --- Variáveis de Controle ---
unsigned long lastSensorRead = 0;   // Último tempo de leitura dos sensores
unsigned long lastWiFiCheck = 0;    // Último tempo de verificação WiFi
unsigned long wifiConnectionTime = 0; // Tempo de conexão WiFi
bool wifiConnected = false;         // Status de conexão WiFi
int readCounter = 0;                // Contador de leituras realizadas


// ============================================================================
// FUNÇÃO DE SETUP - EXECUTADA UMA ÚNICA VEZ AO INICIAR
// ============================================================================

void setup() {
  
  // Inicializa a comunicação Serial para debug
  Serial.begin(SERIAL_BAUD);
  delay(100);
  
  // Aguarda estabilização
  Serial.println("\n\n");
  Serial.println("=================================================");
  Serial.println("   SISTEMA DE AUTOMAÇÃO PARA BIODIGESTOR ESP32   ");
  Serial.println("=================================================");
  Serial.println("Iniciando...");
  Serial.println("=================================================");
  
  // Inicializa pinos de saída
  pinMode(LED_ALERT, OUTPUT);       // LED como saída
  pinMode(BUZZER_PIN, OUTPUT);      // Buzzer como saída
  digitalWrite(LED_ALERT, LOW);     // LED desligado inicialmente
  digitalWrite(BUZZER_PIN, LOW);    // Buzzer desligado inicialmente
  
  // Inicializa sensor de temperatura DHT22
  Serial.print("Inicializando sensor de temperatura DHT22... ");
  dht.begin();
  delay(500);
  Serial.println("OK");
  
  // Inicializa I2C para BMP280 (especifica SDA e SCL)
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Inicializa sensor de pressão BMP280 via I2C
  Serial.print("Inicializando sensor de pressão BMP280 (I2C)... ");
  if (!bmp280.begin(0x76)) {        // Endereço I2C 0x76
    Serial.println("FALHA!");
    Serial.println("Verifique conexão I2C e endereço do sensor");
    while (1) {
      digitalWrite(LED_ALERT, HIGH);
      delay(200);
      digitalWrite(LED_ALERT, LOW);
      delay(200);
    }
  } else {
    Serial.println("OK");
  }
  
  // Configura BMP280 com valores de sobrecarga de amostragem
  bmp280.setSampling(
    Adafruit_BMP280::MODE_NORMAL,        // Modo de operação
    Adafruit_BMP280::SAMPLING_X2,        // Sobrecarga temperatura
    Adafruit_BMP280::SAMPLING_X16,       // Sobrecarga pressão
    Adafruit_BMP280::FILTER_X16,         // Filtro
    Adafruit_BMP280::STANDBY_MS_500      // Tempo de espera
  );
  
  // Conecta à rede WiFi
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  conectarWiFi();
  
  // Exibe informações de inicialização
  Serial.println();
  Serial.println("=================================================");
  Serial.println("Inicialização concluída com sucesso!");
  Serial.println("Sistema pronto para monitoramento.");
  Serial.println("=================================================");
  Serial.println();
  
}


// ============================================================================
// FUNÇÃO LOOP - EXECUTADA CONTINUAMENTE
// ============================================================================

void loop() {
  
  // Verifica tempo decorrido desde última leitura
  unsigned long timeNow = millis();
  
  if (timeNow - lastSensorRead >= SENSOR_INTERVAL) {
    
    // Atualiza último tempo de leitura
    lastSensorRead = timeNow;
    
    // Incrementa contador de leituras
    readCounter++;
    
    // Realiza leitura de todos os sensores
    lerSensores();
    
    // Verifica condições de alerta
    verificarAlertas();
    
    // Atualiza WiFi se necessário
    if (timeNow - lastWiFiCheck >= 30000) {  // Verifica WiFi a cada 30s
      lastWiFiCheck = timeNow;
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi desconectado! Reconectando...");
        conectarWiFi();
      }
    }
    
    // Exibe dados lidos no Serial Monitor
    exibirDados();
    
    // Envia dados para servidor (opcional)
    if (wifiConnected) {
      enviarDadosServidor();
    }
    
  }
  
  // Verifica se há comandos via Serial
  if (Serial.available()) {
    procesarComandoSerial();
  }
  
}


// ============================================================================
// FUNÇÕES DE LEITURA DE SENSORES
// ============================================================================

/**
 * Função: lerSensores
 * Descrição: Realiza leitura de todos os sensores
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void lerSensores() {
  
  // --- Leitura de Temperatura ---
  lerTemperatura();
  
  // --- Leitura de Pressão ---
  lerPressao();
  
  // --- Leitura de pH ---
  lerPH();
  
}


/**
 * Função: lerTemperatura
 * Descrição: Lê valor do sensor DHT22 e atualiza variáveis globais
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void lerTemperatura() {
  
  // Lê temperatura do sensor DHT22
  temperatura = dht.readTemperature();
  
  // Verifica se leitura é válida
  if (isnan(temperatura)) {
    Serial.println("ERRO: Falha ao ler temperatura!");
    temperatura = 0.0;
    return;
  }
  
  // Atualiza temperatura mínima
  if (temperatura < tempMin) {
    tempMin = temperatura;
  }
  
  // Atualiza temperatura máxima
  if (temperatura > tempMax) {
    tempMax = temperatura;
  }
  
}


/**
 * Função: lerPressao
 * Descrição: Lê valor do sensor BMP280 e atualiza variáveis globais
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void lerPressao() {
  
  // Lê pressão do sensor BMP280 em Pascals
  pressao = bmp280.readPressure() / 100.0;  // Converte para hPa
  
  // Atualiza pressão mínima
  if (pressao < pressaoMin && pressao > 800) {  // Filtra leituras inválidas
    pressaoMin = pressao;
  }
  
  // Atualiza pressão máxima
  if (pressao > pressaoMax && pressao < 1100) {  // Filtra leituras inválidas
    pressaoMax = pressao;
  }
  
}


/**
 * Função: lerPH
 * Descrição: Lê valor do sensor analógico de pH
 *            Nota: Requer calibração prévia com soluções de pH conhecido
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void lerPH() {
  
  // Lê múltiplas amostras para melhor precisão
  float voltageSum = 0;
  const int amostras = 10;
  
  for (int i = 0; i < amostras; i++) {
    int sensorValue = analogRead(pH_PIN);
    float voltage = sensorValue * (3.3 / 4095.0);  // Converte para voltagem
    voltageSum += voltage;
    delay(10);
  }
  
  float voltageMedia = voltageSum / amostras;
  
  // Mapeia voltagem para pH
  // Fórmula: pH = (voltageMedia - phVoltageMin) / (phVoltageMax - phVoltageMin) * 10 + 2
  pH = (voltageMedia - phVoltageMin) / (phVoltageMax - phVoltageMin) * 10.0 + 2.0;
  
  // Limita pH entre 0 e 14
  if (pH < 0) pH = 0;
  if (pH > 14) pH = 14;
  
}


// ============================================================================
// FUNÇÕES DE VERIFICAÇÃO E ALERTAS
// ============================================================================

/**
 * Função: verificarAlertas
 * Descrição: Verifica condições de alerta e dispara avisos
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void verificarAlertas() {
  
  // --- Verificação de Temperatura ---
  if (temperatura < TEMP_MIN_SAFE) {
    if (!alertaTempBaixa) {
      Serial.println("🔴 ALERTA: Temperatura muito baixa!");
      acionarAlerta(2);  // Buzzer intermitente
      alertaTempBaixa = true;
    }
  } else if (temperatura > TEMP_MAX_SAFE) {
    if (!alertaTempAlta) {
      Serial.println("🔴 ALERTA: Temperatura muito alta!");
      acionarAlerta(1);  // Buzzer contínuo
      alertaTempAlta = true;
    }
  } else {
    alertaTempBaixa = false;
    alertaTempAlta = false;
    digitalWrite(LED_ALERT, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  // --- Verificação de pH ---
  if (pH < pH_MIN_IDEAL || pH > pH_MAX_IDEAL) {
    Serial.print("⚠️  AVISO: pH fora da faixa ideal (");
    Serial.print(pH);
    Serial.println(")");
  }
  
  // --- Verificação de Pressão ---
  if (pressao > PRESSURE_MAX_ALERT) {
    Serial.print("⚠️  AVISO: Pressão elevada (");
    Serial.print(pressao);
    Serial.println(" hPa)");
  }
  
}


/**
 * Função: acionarAlerta
 * Descrição: Aciona LED e buzzer em padrão diferente conforme modo
 * Parâmetros: 
 *   - modo: 1 = buzzer contínuo, 2 = buzzer intermitente
 * Retorno: Nenhum
 */
void acionarAlerta(int modo) {
  
  if (modo == 1) {
    // Buzzer contínuo
    digitalWrite(LED_ALERT, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else if (modo == 2) {
    // Buzzer intermitente
    digitalWrite(LED_ALERT, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
  
}


// ============================================================================
// FUNÇÕES DE CONECTIVIDADE WiFi
// ============================================================================

/**
 * Função: conectarWiFi
 * Descrição: Conecta ESP32 à rede WiFi configurada
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void conectarWiFi() {
  
  // Modo WiFi como cliente
  WiFi.mode(WIFI_STA);
  
  // Inicia conexão
  WiFi.begin(ssid, password);
  
  // Tenta conectar com timeout
  unsigned long startTime = millis();
  int pontos = 0;
  
  while (WiFi.status() != WL_CONNECTED && 
         millis() - startTime < WIFI_TIMEOUT) {
    
    delay(500);
    Serial.print(".");
    pontos++;
    
    // Piscada do LED durante conexão
    if (pontos % 2 == 0) {
      digitalWrite(LED_ALERT, HIGH);
    } else {
      digitalWrite(LED_ALERT, LOW);
    }
  }
  
  Serial.println();
  
  // Verifica resultado da conexão
  if (WiFi.status() == WL_CONNECTED) {
    
    wifiConnected = true;
    wifiConnectionTime = millis() - startTime;
    
    Serial.println("✓ WiFi conectado com sucesso!");
    Serial.print("  IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("  RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("  Tempo de conexão: ");
    Serial.print(wifiConnectionTime);
    Serial.println(" ms");
    
    digitalWrite(LED_ALERT, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    
  } else {
    
    wifiConnected = false;
    
    Serial.println("✗ Falha ao conectar WiFi!");
    Serial.println("  Continuando operação em modo local...");
    
    digitalWrite(LED_ALERT, HIGH);
    delay(100);
    digitalWrite(LED_ALERT, LOW);
    
  }
  
}


// ============================================================================
// FUNÇÕES DE ENVIO DE DADOS
// ============================================================================

/**
 * Função: enviarDadosServidor
 * Descrição: Envia dados dos sensores para servidor remoto (opcional)
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void enviarDadosServidor() {
  
  // Esta função é um placeholder
  // Implemente conforme seu servidor
  
  // Exemplo com HTTP GET:
  /*
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    String url = "/api/biodigestor?temp=" + String(temperatura) + 
                 "&pressao=" + String(pressao) + 
                 "&ph=" + String(pH);
    
    // Implementar cliente HTTP
  }
  */
  
}


// ============================================================================
// FUNÇÕES DE EXIBIÇÃO DE DADOS
// ============================================================================

/**
 * Função: exibirDados
 * Descrição: Exibe todos os dados no Serial Monitor formatado
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void exibirDados() {
  
  // Limpa linha anterior (opcional)
  Serial.println("\n=================================================");
  Serial.print("Leitura #");
  Serial.println(readCounter);
  Serial.println("=================================================");
  
  // Exibe dados de temperatura
  Serial.print("📊 TEMPERATURA: ");
  Serial.print(temperatura, 2);
  Serial.print("°C  |  Min: ");
  Serial.print(tempMin, 2);
  Serial.print("°C  |  Max: ");
  Serial.print(tempMax, 2);
  Serial.println("°C");
  
  // Exibe dados de pressão
  Serial.print("📊 PRESSÃO: ");
  Serial.print(pressao, 2);
  Serial.print(" hPa  |  Min: ");
  Serial.print(pressaoMin, 2);
  Serial.print(" hPa  |  Max: ");
  Serial.print(pressaoMax, 2);
  Serial.println(" hPa");
  
  // Exibe dados de pH
  Serial.print("📊 pH: ");
  Serial.print(pH, 2);
  Serial.print("  |  Faixa ideal: ");
  Serial.print(pH_MIN_IDEAL, 1);
  Serial.print(" - ");
  Serial.print(pH_MAX_IDEAL, 1);
  Serial.println();
  
  // Exibe status WiFi
  Serial.print("📡 WiFi: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Conectado (IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(", RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm)");
  } else {
    Serial.println("Desconectado");
  }
  
  // Exibe horário decorrido
  Serial.print("⏱️  Tempo decorrido: ");
  Serial.print(millis() / 1000);
  Serial.println(" segundos");
  
  Serial.println("=================================================\n");
  
}


// ============================================================================
// FUNÇÕES DE COMANDO SERIAL
// ============================================================================

/**
 * Função: procesarComandoSerial
 * Descrição: Processa comandos recebidos via Serial Monitor
 * Comandos suportados:
 *   R - Reseta valores mínimo/máximo
 *   S - Exibe estatísticas
 *   C - Reconecta WiFi
 *   T - Testa todos os sensores
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void procesarComandoSerial() {
  
  // Lê caractere recebido
  char comando = Serial.read();
  
  switch (comando) {
    
    case 'R':
    case 'r':
      // Reseta valores mínimo/máximo
      Serial.println("Resetando valores min/max...");
      tempMin = temperatura;
      tempMax = temperatura;
      pressaoMin = pressao;
      pressaoMax = pressao;
      Serial.println("Valores resetados!");
      break;
      
    case 'S':
    case 's':
      // Exibe estatísticas
      exibirEstatisticas();
      break;
      
    case 'C':
    case 'c':
      // Reconecta WiFi
      Serial.println("Reconectando WiFi...");
      conectarWiFi();
      break;
      
    case 'T':
    case 't':
      // Testa todos os sensores
      testarSensores();
      break;
      
    default:
      if (comando >= 32 && comando <= 126) {  // Caractere imprimível
        Serial.print("Comando desconhecido: ");
        Serial.println(comando);
        Serial.println("Comandos disponíveis:");
        Serial.println("  R - Reseta min/max");
        Serial.println("  S - Estatísticas");
        Serial.println("  C - Reconecta WiFi");
        Serial.println("  T - Testa sensores");
      }
      break;
      
  }
  
}


/**
 * Função: exibirEstatisticas
 * Descrição: Exibe estatísticas gerais do sistema
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void exibirEstatisticas() {
  
  Serial.println("\n=================================================");
  Serial.println("             ESTATÍSTICAS DO SISTEMA              ");
  Serial.println("=================================================");
  
  Serial.print("Total de leituras: ");
  Serial.println(readCounter);
  
  Serial.print("Tempo decorrido: ");
  unsigned long tempoTotal = millis() / 1000;
  Serial.print(tempoTotal / 3600);
  Serial.print("h ");
  Serial.print((tempoTotal % 3600) / 60);
  Serial.print("m ");
  Serial.print(tempoTotal % 60);
  Serial.println("s");
  
  Serial.print("Status WiFi: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado");
  } else {
    Serial.println("Desconectado");
  }
  
  Serial.println("=================================================\n");
  
}


/**
 * Função: testarSensores
 * Descrição: Realiza teste de todos os sensores
 * Parâmetros: Nenhum
 * Retorno: Nenhum
 */
void testarSensores() {
  
  Serial.println("\n=================================================");
  Serial.println("           TESTE DE SENSORES - INICIADO           ");
  Serial.println("=================================================");
  
  // Teste de temperatura
  Serial.print("Testando DHT22 (Temperatura)... ");
  float tempTeste = dht.readTemperature();
  if (!isnan(tempTeste)) {
    Serial.print("OK (");
    Serial.print(tempTeste);
    Serial.println("°C)");
  } else {
    Serial.println("FALHA!");
  }
  
  // Teste de pressão
  Serial.print("Testando BMP280 (Pressão)... ");
  float pressaoTeste = bmp280.readPressure() / 100.0;
  if (pressaoTeste > 0) {
    Serial.print("OK (");
    Serial.print(pressaoTeste);
    Serial.println(" hPa)");
  } else {
    Serial.println("FALHA!");
  }
  
  // Teste de pH
  Serial.print("Testando sensor de pH... ");
  int pHRaw = analogRead(pH_PIN);
  Serial.print("OK (ADC: ");
  Serial.print(pHRaw);
  Serial.println(")");
  
  Serial.println("=================================================\n");
  
}


// ============================================================================
// FIM DO CÓDIGO
// ============================================================================

/*
  NOTAS IMPORTANTES:
  
  1. CALIBRAÇÃO DE pH:
     - Prepare soluções de pH 2.0 e pH 12.0
     - Mergulhe o sensor em cada solução
     - Registre os valores de ADC
     - Ajuste phVoltageMin e phVoltageMax conforme necessário
  
  2. CONECTIVIDADE WIFI:
     - Altere ssid e password com suas credenciais
     - Mantenha ESP32 próximo ao roteador
     - Teste a velocidade baud em 115200
  
  3. ALIMENTAÇÃO:
     - Use fonte de alimentação estável de 5V
     - Adicione capacitor de 100µF próximo ao ESP32
  
  4. MANUTENÇÃO:
     - Limpe regularmente os sensores
     - Calibre pH mensalmente
     - Verifique conexões a cada trimestre
  
  5. TROUBLESHOOTING:
     - Serial não funciona: Verifique baudrate
     - WiFi não conecta: Verifique SSID/senha
     - Sensores inconsistentes: Recalibre e limpe
*/
