# Referência de API - Biodigestor ESP32

## 📋 Índice

1. [Constantes e Configurações](#constantes-e-configurações)
2. [Variáveis Globais](#variáveis-globais)
3. [Funções de Leitura](#funções-de-leitura)
4. [Funções de Controle](#funções-de-controle)
5. [Comandos Serial](#comandos-serial)
6. [Estrutura de Dados](#estrutura-de-dados)
7. [Valores e Limites](#valores-e-limites)

---

## Constantes e Configurações

### WiFi

```cpp
const char* ssid = "seu_SSID_aqui"
// Descrição: Nome da rede WiFi
// Tipo: String
// Modificação: Altere conforme sua rede

const char* password = "sua_senha_aqui"
// Descrição: Senha da rede WiFi
// Tipo: String
// Modificação: Altere conforme sua senha

const char* serverName = "seu_servidor.com"
// Descrição: URL do servidor para envio de dados (opcional)
// Tipo: String
// Modificação: Configure conforme seu servidor
```

### Pinos GPIO

```cpp
#define DHTPIN 4
// Sensor de Temperatura (DHT22)
// GPIO 4 (Pino D4)
// Protocolo: 1-Wire Digital
// Mutável: Sim, reconfigure conforme necessário

#define DHTTYPE DHT22
// Tipo de sensor DHT
// Valores: DHT22 ou DHT11
// Padrão: DHT22 (mais preciso)

#define pH_PIN 34
// Sensor de pH (Entrada Analógica)
// GPIO 34 (ADC1_CH6)
// Intervalo: 0-3.3V → 0-4095 ADC
// Mutável: Sim, use outro pino ADC se necessário

#define LED_ALERT 5
// LED Indicador de Alerta
// GPIO 5
// Saída Digital (3.3V, ~40mA)
// Mutável: Sim

#define BUZZER_PIN 12
// Buzzer Piezo
// GPIO 12
// Saída Digital (3.3V)
// Mutável: Sim, use GPIO com suporte PWM
```

### Configurações de Tempo

```cpp
#define SENSOR_INTERVAL 10000
// Intervalo de leitura dos sensores
// Valor: 10000 ms (10 segundos)
// Unidade: Milissegundos
// Range: 5000 - 60000 ms recomendado
// Efeito: Aumentar economiza energia; diminuir = mais dados

#define WIFI_TIMEOUT 20000
// Timeout para tentativa de conexão WiFi
// Valor: 20000 ms (20 segundos)
// Unidade: Milissegundos
// Efeito: Se WiFi não conecta em 20s, continua offline

#define SERIAL_BAUD 115200
// Velocidade da comunicação Serial
// Valor: 115200 bps
// Padrão: Padrão para ESP32
// ⚠️ CRÍTICO: Deve corresponder a Serial Monitor
```

### Limites de Temperatura

```cpp
#define TEMP_MIN_SAFE 20.0
// Temperatura mínima segura
// Valor: 20.0°C
// Alerta: Se < 20°C, aciona buzzer intermitente

#define TEMP_MAX_SAFE 60.0
// Temperatura máxima segura
// Valor: 60.0°C
// Alerta: Se > 60°C, aciona buzzer contínuo
```

### Limites de pH

```cpp
#define pH_MIN_IDEAL 6.5
// pH mínimo ideal para biodigestor
// Valor: 6.5
// Alerta: Se pH < 6.5, aviso no Serial

#define pH_MAX_IDEAL 8.0
// pH máximo ideal para biodigestor
// Valor: 8.0
// Alerta: Se pH > 8.0, aviso no Serial
```

### Limites de Pressão

```cpp
#define PRESSURE_MAX_ALERT 1050.0
// Pressão máxima antes de alerta
// Valor: 1050.0 hPa
// Alerta: Se pressão > 1050 hPa, aviso no Serial
// Nota: Ajuste conforme seu biodigestor
```

---

## Variáveis Globais

### Sensor de Temperatura

```cpp
float temperatura
// Descrição: Temperatura atual em °C
// Tipo: float
// Range: -40 a 125°C (DHT22)
// Atualização: A cada 10 segundos

float tempMin
// Descrição: Temperatura mínima registrada
// Tipo: float
// Inicial: 100.0°C
// Atualização: Quando temperatura < tempMin
// Reset: Comando 'R' no Serial

float tempMax
// Descrição: Temperatura máxima registrada
// Tipo: float
// Inicial: -40.0°C
// Atualização: Quando temperatura > tempMax
// Reset: Comando 'R' no Serial

bool alertaTempBaixa
// Descrição: Flag de alerta temperatura baixa
// Tipo: boolean
// Estados: true = Alerta ativo, false = Normal
// Uso: Evita repetição de alertas

bool alertaTempAlta
// Descrição: Flag de alerta temperatura alta
// Tipo: boolean
// Estados: true = Alerta ativo, false = Normal
// Uso: Evita repetição de alertas
```

### Sensor de Pressão

```cpp
float pressao
// Descrição: Pressão atual em hPa
// Tipo: float
// Range: 300 a 1100 hPa (BMP280)
// Unidade: hectopascal (hPa)
// Atualização: A cada 10 segundos

float pressaoMin
// Descrição: Pressão mínima registrada
// Tipo: float
// Inicial: 1100.0 hPa
// Atualização: Quando pressão < pressaoMin
// Reset: Comando 'R' no Serial

float pressaoMax
// Descrição: Pressão máxima registrada
// Tipo: float
// Inicial: 900.0 hPa
// Atualização: Quando pressão > pressaoMax
// Reset: Comando 'R' no Serial
```

### Sensor de pH

```cpp
float pH
// Descrição: Valor de pH atual
// Tipo: float
// Range: 0 - 14
// Precisão: ±0.5 (depende de calibração)
// Atualização: A cada 10 segundos

float phVoltageMin
// Descrição: Voltagem em pH mínimo (pH 2.0)
// Tipo: float
// Padrão: 0.67V
// Modificação: Ajuste durante calibração

float phVoltageMax
// Descrição: Voltagem em pH máximo (pH 12.0)
// Tipo: float
// Padrão: 3.33V
// Modificação: Ajuste durante calibração
```

### Controle e Status

```cpp
unsigned long lastSensorRead
// Descrição: Timestamp da última leitura
// Tipo: unsigned long
// Unidade: milissegundos
// Uso: Controla intervalo de 10s entre leituras

unsigned long lastWiFiCheck
// Descrição: Timestamp da última verificação WiFi
// Tipo: unsigned long
// Unidade: milissegundos
// Uso: Reconecta WiFi a cada 30s

unsigned long wifiConnectionTime
// Descrição: Tempo gasto na última conexão WiFi
// Tipo: unsigned long
// Unidade: milissegundos
// Informacional: Exibido no Serial

bool wifiConnected
// Descrição: Status da conexão WiFi
// Tipo: boolean
// Valores: true = Conectado, false = Desconectado
// Uso: Determina se envia dados ao servidor

int readCounter
// Descrição: Contador total de leituras realizadas
// Tipo: int
// Incremento: +1 a cada leitura
// Máximo: ~2 bilhões antes de overflow
// Exibição: Mostrado a cada leitura no Serial
```

---

## Funções de Leitura

### lerSensores()

```cpp
void lerSensores(void)
// Descrição: Realiza leitura de TODOS os sensores
// Parâmetros: Nenhum
// Retorno: Nenhum
// Frequência: A cada 10 segundos (SENSOR_INTERVAL)
// Chamada por: loop()
// 
// Operação:
//   1. Chama lerTemperatura()
//   2. Chama lerPressao()
//   3. Chama lerPH()
//
// Exemplo de uso:
//   lerSensores();  // Lê todos os três sensores
```

### lerTemperatura()

```cpp
void lerTemperatura(void)
// Descrição: Lê sensor DHT22 e atualiza variáveis
// Parâmetros: Nenhum
// Retorno: Nenhum
// Chamada por: lerSensores()
//
// Operação:
//   1. Lê temperatura do DHT22
//   2. Valida se não é NaN
//   3. Atualiza tempMin se menor
//   4. Atualiza tempMax se maior
//
// Variáveis modificadas:
//   - temperatura (float)
//   - tempMin (float)
//   - tempMax (float)
//
// Tempo de execução: ~30ms
// Exemplo:
//   lerTemperatura();
//   Serial.println(temperatura);  // 28.5
```

### lerPressao()

```cpp
void lerPressao(void)
// Descrição: Lê sensor BMP280 via I2C
// Parâmetros: Nenhum
// Retorno: Nenhum
// Chamada por: lerSensores()
//
// Operação:
//   1. Lê pressão em Pa do BMP280
//   2. Converte para hPa (÷ 100)
//   3. Filtra leituras inválidas (< 800 ou > 1100)
//   4. Atualiza pressaoMin se menor
//   5. Atualiza pressaoMax se maior
//
// Variáveis modificadas:
//   - pressao (float)
//   - pressaoMin (float)
//   - pressaoMax (float)
//
// Tempo de execução: ~50ms
// Unidade de saída: hPa (hectopascal)
// Exemplo:
//   lerPressao();
//   Serial.println(pressao);  // 1013.25
```

### lerPH()

```cpp
void lerPH(void)
// Descrição: Lê sensor de pH com filtragem
// Parâmetros: Nenhum
// Retorno: Nenhum
// Chamada por: lerSensores()
//
// Operação:
//   1. Coleta 10 amostras do ADC (GPIO 34)
//   2. Converte ADC (0-4095) em voltagem (0-3.3V)
//   3. Calcula média das voltagens
//   4. Mapeia voltagem para pH (0-14)
//   5. Limita resultado entre 0-14
//
// Variáveis modificadas:
//   - pH (float)
//
// Configurações usadas:
//   - phVoltageMin (calibração pH 2.0)
//   - phVoltageMax (calibração pH 12.0)
//
// Tempo de execução: ~150ms (10 amostras × 10ms)
// Precisão: ±0.5 após calibração
// Exemplo:
//   lerPH();
//   Serial.println(pH);  // 7.2
```

---

## Funções de Controle

### verificarAlertas()

```cpp
void verificarAlertas(void)
// Descrição: Verifica condições de alerta
// Parâmetros: Nenhum
// Retorno: Nenhum
// Frequência: A cada 10 segundos (em loop())
//
// Condições verificadas:
//   1. Temperatura < 20°C → Buzzer intermitente
//   2. Temperatura > 60°C → Buzzer contínuo
//   3. pH < 6.5 → Aviso Serial
//   4. pH > 8.0 → Aviso Serial
//   5. Pressão > 1050 hPa → Aviso Serial
//
// Ações:
//   - Aciona acionarAlerta(modo)
//   - Exibe mensagem no Serial
//   - Modifica LED e Buzzer
//
// Exemplo de saída:
//   🔴 ALERTA: Temperatura muito baixa!
//   ⚠️  AVISO: pH fora da faixa ideal (8.5)
```

### acionarAlerta(int modo)

```cpp
void acionarAlerta(int modo)
// Descrição: Aciona LED e Buzzer com padrão específico
// Parâmetros:
//   - modo (int): 1 = Contínuo, 2 = Intermitente
// Retorno: Nenhum
//
// Comportamento:
//   modo == 1: LED LIGADO, Buzzer LIGADO
//   modo == 2: LED PISCANTE, Buzzer PISCANTE (200ms on/off)
//
// Variáveis modificadas:
//   - LED_ALERT (GPIO 5)
//   - BUZZER_PIN (GPIO 12)
//
// Tempo de execução:
//   modo 1: Imediato
//   modo 2: ~400ms (piscada completa)
//
// Exemplos:
//   acionarAlerta(1);  // Buzzer contínuo
//   acionarAlerta(2);  // Buzzer intermitente
```

### conectarWiFi()

```cpp
void conectarWiFi(void)
// Descrição: Conecta/reconecta à rede WiFi
// Parâmetros: Nenhum
// Retorno: Nenhum
// Timeout: 20 segundos (WIFI_TIMEOUT)
//
// Operação:
//   1. Define modo WiFi como cliente (STA)
//   2. Inicia conexão com SSID/Senha
//   3. Aguarda até 20 segundos
//   4. LED pisca durante conexão
//   5. Define wifiConnected true/false
//   6. Exibe resultado no Serial
//
// Saída Serial (Sucesso):
//   ✓ WiFi conectado com sucesso!
//   IP: 192.168.1.100
//   RSSI: -45 dBm
//
// Saída Serial (Falha):
//   ✗ Falha ao conectar WiFi!
//   Continuando operação em modo local...
//
// Variáveis modificadas:
//   - wifiConnected (bool)
//   - wifiConnectionTime (unsigned long)
//
// Chamada por:
//   - setup() durante inicialização
//   - loop() a cada 30 segundos
//   - Comando 'C' do usuário
```

### enviarDadosServidor()

```cpp
void enviarDadosServidor(void)
// Descrição: Envia dados para servidor remoto (STUB)
// Parâmetros: Nenhum
// Retorno: Nenhum
// Status: Não implementado (placeholder)
//
// Propósito: Facilita integração futura com:
//   - ThingSpeak
//   - Blynk
//   - Ubidots
//   - Servidor customizado
//
// Exemplo implementação HTTP GET:
//   String url = "/api/biodigestor?temp=" + String(temperatura) +
//                "&pressao=" + String(pressao) +
//                "&ph=" + String(pH);
//   // WiFiClient client
//   // client.connect(serverName, 80)
//   // client.print("GET " + url)
```

---

## Comandos Serial

### Entrada de Comandos

Todos os comandos são **case-insensitive** (maiúsculas/minúsculas).

```
Serial Monitor → Digitar Comando + Enter
```

### 'R' - Resetar Valores Min/Max

```cpp
case 'R':
case 'r':
  // Ação executada
  tempMin = temperatura;
  tempMax = temperatura;
  pressaoMin = pressao;
  pressaoMax = pressao;
  Serial.println("Valores resetados!");

// Entrada: R
// Saída:
//   Resetando valores min/max...
//   Valores resetados!
//
// Uso: Após ajustar sensores ou para novo ciclo
```

### 'S' - Exibir Estatísticas

```cpp
case 'S':
case 's':
  exibirEstatisticas();

// Entrada: S
// Saída:
//   =================================================
//   ESTATÍSTICAS DO SISTEMA
//   =================================================
//   Total de leituras: 125
//   Tempo decorrido: 0h 20m 50s
//   Status WiFi: Conectado
//   =================================================
//
// Uso: Monitorar operação geral do sistema
```

### 'C' - Reconectar WiFi

```cpp
case 'C':
case 'c':
  Serial.println("Reconectando WiFi...");
  conectarWiFi();

// Entrada: C
// Saída:
//   Reconectando WiFi...
//   Conectando ao WiFi: seu_SSID_aqui
//   .....
//   ✓ WiFi conectado com sucesso!
//   IP: 192.168.1.100
//
// Uso: Reconectar manualmente se WiFi instável
```

### 'T' - Testar Sensores

```cpp
case 'T':
case 't':
  testarSensores();

// Entrada: T
// Saída:
//   =================================================
//   TESTE DE SENSORES - INICIADO
//   =================================================
//   Testando DHT22 (Temperatura)... OK (28.5°C)
//   Testando BMP280 (Pressão)... OK (1013.25 hPa)
//   Testando sensor de pH... OK (ADC: 2048)
//   =================================================
//
// Uso: Diagnóstico de funcionamento dos sensores
```

---

## Estrutura de Dados

### Objeto DHT

```cpp
DHT dht(DHTPIN, DHTTYPE)
// Biblioteca: DHT.h (Adafruit)
// Parâmetros:
//   - DHTPIN: GPIO 4
//   - DHTTYPE: DHT22
//
// Métodos disponíveis:
//   dht.begin()
//   dht.readTemperature()
//   dht.readHumidity()
//
// Exemplo:
//   float temp = dht.readTemperature();
```

### Objeto BMP280

```cpp
Adafruit_BMP280 bmp280
// Biblioteca: Adafruit_BMP280.h
// Interface: I2C (0x76 ou 0x77)
//
// Métodos disponíveis:
//   bmp280.begin(0x76)
//   bmp280.readTemperature()
//   bmp280.readPressure()
//   bmp280.readAltitude()
//   bmp280.setSampling(...)
//
// Exemplo:
//   float pressure_pa = bmp280.readPressure();
//   float pressure_hpa = pressure_pa / 100.0;
```

---

## Valores e Limites

### Faixa de Temperatura

```
Sensor DHT22:
├─ Faixa de Medida: -40 a 125°C
├─ Precisão: ±0.5°C
├─ Resolução: 0.1°C
├─ Tempo de resposta: ~2 segundos
└─ Consumo: 1-2 mA

Biodigestor:
├─ Faixa Normal: 20 - 60°C
├─ Faixa Ideal: 35 - 40°C
├─ Alerta Baixo: < 20°C
└─ Alerta Alto: > 60°C
```

### Faixa de Pressão

```
Sensor BMP280:
├─ Faixa de Medida: 300 - 1100 hPa
├─ Precisão: ±1 hPa
├─ Resolução: 0.01 hPa
├─ Consumo: 0.7 mA
└─ Interface: I2C (0x76/0x77)

Valores de Referência:
├─ Nível do Mar: 1013.25 hPa
├─ 500m altitude: ~950 hPa
├─ -500m profundidade: ~1050 hPa
└─ Alerta: > 1050 hPa (biodigestor)
```

### Faixa de pH

```
Sensor Analógico:
├─ Faixa de Medida: 0 - 14
├─ Resolução ADC: 12-bit (4095 níveis)
├─ Voltagem: 0 - 3.3V
├─ Precisão: ±0.5 (após calibração)
└─ Tempo de resposta: 1-5 segundos

Biodigestor:
├─ Faixa Ideal: 6.5 - 8.0
├─ Neutro: 7.0
├─ Ácido: < 7.0
└─ Alcalino: > 7.0

Calibração:
├─ pH 4.0: ~2.27V
├─ pH 7.0: ~1.66V
└─ pH 12.0: ~0.67V (invertido)
```

### ADC (Entrada Analógica)

```
GPIO 34 (ADC1_CH6):
├─ Resolução: 12-bit
├─ Valores: 0 - 4095
├─ Voltagem Máxima: 3.3V
├─ Fórmula: V = ADC × (3.3 / 4095)
├─ Exemplo: ADC 2048 = 1.65V
└─ Tempo de Conversão: ~1µs
```

---

## Fluxo de Execução

```
setup() → Inicializa todo o sistema
  ├─ Serial.begin()
  ├─ pinMode() para LED e Buzzer
  ├─ DHT22.begin()
  ├─ BMP280.begin()
  ├─ conectarWiFi()
  └─ Pronto para operação

loop() → Executa continuamente
  ├─ Verifica tempo decorrido
  ├─ Se ≥ 10s:
  │  ├─ lerSensores()
  │  │  ├─ lerTemperatura()
  │  │  ├─ lerPressao()
  │  │  └─ lerPH()
  │  ├─ verificarAlertas()
  │  ├─ exibirDados()
  │  └─ enviarDadosServidor()
  ├─ Se 30s desde última verificação WiFi:
  │  └─ Verifica/reconecta WiFi
  ├─ Se Serial disponível:
  │  └─ procesarComandoSerial()
  └─ Volta para verificação de tempo
```

---

**Versão:** 1.0.0  
**Data:** 12 de Junho de 2026  
**Última Atualização:** 12 de Junho de 2026
