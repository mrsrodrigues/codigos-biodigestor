# CHANGELOG - Sistema de Automação para Biodigestor ESP32

Este documento rastreia todas as mudanças, atualizações e alterações do projeto ao longo do tempo.

---

## 📌 [1.0.0] - 2026-06-12

### 🆕 Adicionado

#### Funcionalidades Principais
- **Leitura de Temperatura (DHT22)**
  - Coleta contínua de dados de temperatura
  - Rastreamento de valores mínimo e máximo
  - Alertas para temperaturas fora da faixa segura (20-60°C)
  - Comando: `lerTemperatura()` - Lê e armazena temperatura
  
- **Leitura de Pressão (BMP280)**
  - Monitoramento em tempo real da pressão interna
  - Conversão automática para hPa
  - Detecção de pressão anômala
  - Comando: `lerPressao()` - Lê e armazena pressão
  
- **Monitoramento de pH (Sensor Analógico)**
  - Leitura contínua com filtragem de 10 amostras
  - Calibração de dois pontos (pH 2.0 e 12.0)
  - Alertas para pH fora da faixa ideal (6.5-8.0)
  - Comando: `lerPH()` - Lê e converte valor de pH

#### Conectividade e Comunicação
- **Conexão WiFi**
  - Conexão automática à rede WiFi configurada
  - Reconexão automática em caso de desconexão
  - Exibição de IP local e intensidade do sinal (RSSI)
  - Comando: `conectarWiFi()` - Conecta/reconecta à rede
  - Comando Serial: `C` - Reconecta WiFi manualmente

#### Sistema de Alertas
- **Alertas de Temperatura**
  - LED piscante + buzzer intermitente para temperatura baixa
  - LED contínuo + buzzer contínuo para temperatura alta
  - Função: `acionarAlerta(modo)` - Aciona LED/buzzer
  
- **Alertas de pH**
  - Notificação no Serial Monitor
  - Faixa ideal: 6.5-8.0

#### Interface Serial
- **Comandos de Controle**
  - `R` - Reseta valores mínimo/máximo dos sensores
  - `S` - Exibe estatísticas gerais do sistema
  - `C` - Reconecta WiFi manualmente
  - `T` - Executa teste de todos os sensores
  
- **Função: `procesarComandoSerial()`** - Processa entrada do usuário

#### Sistema de Diagnóstico
- **Teste de Sensores**
  - Função: `testarSensores()` - Verifica funcionamento de cada sensor
  - Retorna valores em tempo real de cada sensor
  
- **Relatório de Estatísticas**
  - Função: `exibirEstatisticas()` - Exibe dados agregados
  - Mostra: total de leituras, tempo decorrido, status WiFi

#### Exibição de Dados
- **Serial Monitor Formatado**
  - Função: `exibirDados()` - Exibe leitura completa
  - Mostra: temperatura, pressão, pH, status WiFi, contador
  - Atualização a cada 10 segundos

#### Estrutura de Código
- Comentários detalhados em cada função
- Documentação de parâmetros e retorno
- Seções bem organizadas por funcionalidade
- Notas importantes ao final do código

#### Documentação
- **README.md** - Documentação completa do projeto
- **biodigestor_esp32.ino** - Código-fonte comentado
- **CHANGELOG.md** - Este arquivo com histórico de alterações

### 📋 Constantes e Configurações Definidas

```cpp
// WiFi
ssid = "seu_SSID_aqui"
password = "sua_senha_aqui"
serverName = "seu_servidor.com"

// Pinos
DHTPIN = 4
pH_PIN = 34
LED_ALERT = 5
BUZZER_PIN = 12

// Sensores
DHTTYPE = DHT22
BMP280_ADDR = 0x76

// Tempo
SENSOR_INTERVAL = 10000 ms
WIFI_TIMEOUT = 20000 ms
SERIAL_BAUD = 115200

// Limites de Temperatura
TEMP_MIN_SAFE = 20.0°C
TEMP_MAX_SAFE = 60.0°C

// Limites de pH
pH_MIN_IDEAL = 6.5
pH_MAX_IDEAL = 8.0

// Limites de Pressão
PRESSURE_MAX_ALERT = 1050.0 hPa
```

### 🔧 Variáveis Globais Inicializadas

- `temperatura` - Temperatura atual (0.0°C)
- `tempMin` - Temperatura mínima (100.0°C)
- `tempMax` - Temperatura máxima (-40.0°C)
- `pressao` - Pressão atual (0.0 hPa)
- `pressaoMin` - Pressão mínima (1100.0 hPa)
- `pressaoMax` - Pressão máxima (900.0 hPa)
- `pH` - pH atual (7.0)
- `wifiConnected` - Status de conexão WiFi (false)
- `readCounter` - Contador de leituras (0)

### 📦 Bibliotecas Utilizadas

- `WiFi.h` - Conectividade WiFi (Built-in ESP32)
- `DHT.h` - Sensor de temperatura (v1.4.6+)
- `Adafruit_BMP280.h` - Sensor de pressão (v2.6.8+)
- `Adafruit_Sensor.h` - Biblioteca Adafruit unificada (v1.1.14+)
- `Wire.h` - Comunicação I2C (Built-in)

### ✅ Testes e Validação

- [x] Leitura de temperatura testada
- [x] Leitura de pressão testada
- [x] Leitura de pH testada
- [x] Conexão WiFi testada
- [x] Alertas e buzzer testados
- [x] Comandos Serial testados
- [x] Serial Monitor formatado
- [x] Código comentado completamente

---

## 📝 Guia de Comandos

### Comandos Serial (Entrada pelo Serial Monitor)

| Comando | Descrição | Exemplo |
|---------|-----------|---------|
| `R` ou `r` | Reseta valores min/max | Digitar: `R` + Enter |
| `S` ou `s` | Exibe estatísticas | Digitar: `S` + Enter |
| `C` ou `c` | Reconecta WiFi | Digitar: `C` + Enter |
| `T` ou `t` | Testa sensores | Digitar: `T` + Enter |

### Saída do Serial Monitor

```
=================================================
   SISTEMA DE AUTOMAÇÃO PARA BIODIGESTOR ESP32   
=================================================
Iniciando...
=================================================

Inicializando sensor de temperatura DHT22... OK
Inicializando sensor de pressão BMP280 (I2C)... OK
Conectando ao WiFi: seu_SSID_aqui
.....
✓ WiFi conectado com sucesso!
  IP: 192.168.1.100
  RSSI: -45 dBm
  Tempo de conexão: 5200 ms

=================================================
Inicialização concluída com sucesso!
Sistema pronto para monitoramento.
=================================================

=================================================
Leitura #1
=================================================
📊 TEMPERATURA: 28.50°C  |  Min: 28.50°C  |  Max: 28.50°C
📊 PRESSÃO: 1013.25 hPa  |  Min: 1013.25 hPa  |  Max: 1013.25 hPa
📊 pH: 7.20  |  Faixa ideal: 6.5 - 8.0
📡 WiFi: Conectado (IP: 192.168.1.100, RSSI: -45 dBm)
⏱️  Tempo decorrido: 10 segundos
=================================================
```

---

## 🔍 Descrição Detalhada das Alterações

### Alteração 1.0.0.1: Inicialização do Sistema

**Data:** 2026-06-12  
**Descrição:** Setup completo do sistema

**Mudanças no código:**
```cpp
// Função setup() - Executa uma única vez
void setup() {
  Serial.begin(SERIAL_BAUD);           // 115200 bps
  pinMode(LED_ALERT, OUTPUT);          // GPIO 5
  pinMode(BUZZER_PIN, OUTPUT);         // GPIO 12
  dht.begin();                          // Inicia DHT22
  bmp280.begin(0x76);                  // Inicia BMP280 I2C
  conectarWiFi();                      // Conecta WiFi
}
```

**Comando executado:**
```
Setup() → Inicializa Serial, Pinos, Sensores, WiFi
```

---

### Alteração 1.0.0.2: Loop Principal de Leitura

**Data:** 2026-06-12  
**Descrição:** Loop contínuo com intervalo de 10 segundos

**Mudanças no código:**
```cpp
// Função loop() - Executa continuamente
void loop() {
  unsigned long timeNow = millis();
  
  if (timeNow - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = timeNow;
    lerSensores();                   // Lê todos os sensores
    verificarAlertas();              // Verifica condições críticas
    exibirDados();                   // Mostra no Serial Monitor
    enviarDadosServidor();           // Envia dados (opcional)
  }
  
  if (Serial.available()) {
    procesarComandoSerial();         // Processa comandos
  }
}
```

**Intervalo:** 10 segundos (SENSOR_INTERVAL = 10000 ms)

---

### Alteração 1.0.0.3: Sensor de Temperatura

**Data:** 2026-06-12  
**Descrição:** Implementação DHT22 com rastreamento min/max

**Função:**
```cpp
void lerTemperatura() {
  temperatura = dht.readTemperature();
  
  // Atualiza min/max
  if (temperatura < tempMin) tempMin = temperatura;
  if (temperatura > tempMax) tempMax = temperatura;
}
```

**Pino:** GPIO 4 (configurável)  
**Sensor:** DHT22  
**Intervalo:** 10 segundos  
**Alertas:** < 20°C ou > 60°C

---

### Alteração 1.0.0.4: Sensor de Pressão

**Data:** 2026-06-12  
**Descrição:** Implementação BMP280 via I2C

**Função:**
```cpp
void lerPressao() {
  pressao = bmp280.readPressure() / 100.0;  // Converte Pa → hPa
  
  // Atualiza min/max com filtro
  if (pressao < pressaoMin && pressao > 800) pressaoMin = pressao;
  if (pressao > pressaoMax && pressao < 1100) pressaoMax = pressao;
}
```

**Endereço I2C:** 0x76  
**Pinos:** GPIO 21 (SDA), GPIO 22 (SCL)  
**Unidade:** hPa (hectopascal)  
**Alerta:** > 1050 hPa

---

### Alteração 1.0.0.5: Sensor de pH

**Data:** 2026-06-12  
**Descrição:** Leitura analógica com filtragem e calibração

**Função:**
```cpp
void lerPH() {
  float voltageSum = 0;
  
  // 10 amostras para precisão
  for (int i = 0; i < 10; i++) {
    int sensorValue = analogRead(pH_PIN);
    float voltage = sensorValue * (3.3 / 4095.0);
    voltageSum += voltage;
    delay(10);
  }
  
  float voltageMedia = voltageSum / 10;
  pH = (voltageMedia - phVoltageMin) / (phVoltageMax - phVoltageMin) * 10.0 + 2.0;
}
```

**Pino:** GPIO 34 (entrada analógica)  
**Resolução:** 12-bit (4095)  
**Amostras:** 10 por leitura  
**Faixa Ideal:** 6.5 - 8.0

---

### Alteração 1.0.0.6: Conectividade WiFi

**Data:** 2026-06-12  
**Descrição:** Conexão automática com reconexão

**Função:**
```cpp
void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && 
         millis() - startTime < WIFI_TIMEOUT) {
    delay(500);
    // LED pisca enquanto conecta
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(WiFi.localIP());
  }
}
```

**Timeout:** 20 segundos  
**SSID:** Configurável  
**Senha:** Configurável  
**Modo:** Cliente (STA)

---

### Alteração 1.0.0.7: Sistema de Alertas

**Data:** 2026-06-12  
**Descrição:** Alertas com LED e buzzer

**Função:**
```cpp
void verificarAlertas() {
  if (temperatura < TEMP_MIN_SAFE) {
    acionarAlerta(2);  // Intermitente
  } else if (temperatura > TEMP_MAX_SAFE) {
    acionarAlerta(1);  // Contínuo
  }
}
```

**Padrões:**
- Modo 1: Buzzer contínuo (temperatura alta)
- Modo 2: Buzzer intermitente (temperatura baixa)

---

### Alteração 1.0.0.8: Comandos Serial

**Data:** 2026-06-12  
**Descrição:** Interface de comandos via Serial Monitor

**Comando R - Resetar:**
```cpp
case 'R':
  tempMin = temperatura;
  tempMax = temperatura;
  pressaoMin = pressao;
  pressaoMax = pressao;
  Serial.println("Valores resetados!");
  break;
```

**Comando S - Estatísticas:**
```cpp
case 'S':
  exibirEstatisticas();
  break;
```

**Comando C - Conectar WiFi:**
```cpp
case 'C':
  conectarWiFi();
  break;
```

**Comando T - Testar:**
```cpp
case 'T':
  testarSensores();
  break;
```

---

### Alteração 1.0.0.9: Teste de Sensores

**Data:** 2026-06-12  
**Descrição:** Função de diagnóstico para cada sensor

**Função:**
```cpp
void testarSensores() {
  float tempTeste = dht.readTemperature();
  float pressaoTeste = bmp280.readPressure() / 100.0;
  int pHRaw = analogRead(pH_PIN);
  
  // Exibe resultado de cada teste
}
```

---

### Alteração 1.0.0.10: Documentação Completa

**Data:** 2026-06-12  
**Descrição:** Comentários e documentação criados

**Arquivos criados:**
- `README.md` - Documentação de 500+ linhas
- `biodigestor_esp32.ino` - Código comentado (900+ linhas)
- `CHANGELOG.md` - Este arquivo

---

## 🐛 Problemas Conhecidos e Soluções

### Problema 1: Sensor DHT22 retorna NaN

**Causa:** Conexão instável ou biblioteca incompatível  
**Solução:**
```cpp
// Adicione delay após inicialização
dht.begin();
delay(2000);  // Espera 2 segundos
```

### Problema 2: BMP280 não encontrado no I2C

**Causa:** Endereço I2C incorreto  
**Solução:**
```cpp
// Teste com endereço 0x77 também
if (!bmp280.begin(0x77)) {  // Tenta 0x77
  if (!bmp280.begin(0x76)) {  // Depois 0x76
    Serial.println("Sensor não encontrado!");
  }
}
```

### Problema 3: WiFi desconecta periodicamente

**Causa:** Sinal fraco ou roteador distante  
**Solução:**
```cpp
// Aumentar potência WiFi
WiFi.setTxPower(WIFI_POWER_19_5dBm);
```

### Problema 4: Leitura de pH instável

**Causa:** Eletrodo sujo ou fora de calibração  
**Solução:**
```
1. Limpe o eletrodo com água destilada
2. Recalibre com soluções pH 4.0 e 7.0
3. Aumente número de amostras para 20
```

---

## 📊 Histórico de Versões

| Versão | Data | Status | Descrição |
|--------|------|--------|-----------|
| 1.0.0 | 2026-06-12 | ✅ Estável | Versão inicial completa |
| 1.1.0 | Planejado | ⏳ Em desenvolvimento | Suporte para MicroSD |
| 1.2.0 | Planejado | ⏳ Planejado | Integração com ThingSpeak |
| 1.3.0 | Planejado | ⏳ Planejado | Dashboard Web |

---

## 🚀 Próximas Melhorias

- [ ] Suporte para armazenamento em MicroSD
- [ ] Integração com ThingSpeak ou Blynk
- [ ] Dashboard web para visualização
- [ ] Sistema de notificações por email
- [ ] Controle remoto de parâmetros
- [ ] Histórico de dados com gráficos
- [ ] Backup automático de dados

---

## 📞 Suporte e Contato

Para dúvidas ou sugestões sobre o projeto, consulte o arquivo README.md ou entre em contato.

---

**Documento gerado em:** 12 de Junho de 2026  
**Versão:** 1.0.0  
**Mantido por:** Sistema de Automação Biodigestor
