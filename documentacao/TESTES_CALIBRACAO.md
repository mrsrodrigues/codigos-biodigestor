# Guia de Testes e Calibração - Biodigestor ESP32

## 📋 Índice

1. [Testes Iniciais](#testes-iniciais)
2. [Calibração de Sensores](#calibração-de-sensores)
3. [Procedimentos de Teste](#procedimentos-de-teste)
4. [Validação de Dados](#validação-de-dados)
5. [Troubleshooting](#troubleshooting)

---

## Testes Iniciais

### ✅ Teste 1: Inicialização do Sistema

**Procedimento:**
1. Conecte USB na ESP32
2. Abra Serial Monitor (115200 bps)
3. Verifique inicialização

**Resultado Esperado:**
```
=================================================
   SISTEMA DE AUTOMAÇÃO PARA BIODIGESTOR ESP32   
=================================================
Iniciando...
Inicializando sensor de temperatura DHT22... OK
Inicializando sensor de pressão BMP280 (I2C)... OK
Conectando ao WiFi: seu_SSID_aqui
.....
✓ WiFi conectado com sucesso!
```

**✗ Se falhar:**
- Reinicie ESP32
- Verifique conexão USB
- Tente outra porta COM

---

### ✅ Teste 2: Sensor de Temperatura

**Procedimento:**
1. No Serial, digite: `T` (Teste de sensores)
2. Verifique temperatura exibida

**Resultado Esperado:**
```
Testando DHT22 (Temperatura)... OK (28.5°C)
```

**Valores Típicos:**
- Temperatura Ambiente: 20-30°C
- Temperatura Biodigestor: 35-40°C

**✗ Se retornar NaN ou valor 0:**
- Verifique conexão GPIO 4
- Adicione resistor pull-up 10kΩ
- Tente outro GPIO

---

### ✅ Teste 3: Sensor de Pressão

**Procedimento:**
1. No Serial, digite: `T` (Teste de sensores)
2. Verifique pressão exibida

**Resultado Esperado:**
```
Testando BMP280 (Pressão)... OK (1013.25 hPa)
```

**Valores Típicos:**
- Nível do Mar: 1013 hPa
- -500m de altitude: 1050 hPa
- +500m de altitude: 950 hPa

**✗ Se não encontrar sensor:**
- Verifique conexão I2C (GPIO 21/22)
- Tente endereço 0x77
- Verifique pull-ups 4.7kΩ

---

### ✅ Teste 4: Sensor de pH

**Procedimento:**
1. No Serial, digite: `T` (Teste de sensores)
2. Verifique leitura ADC

**Resultado Esperado:**
```
Testando sensor de pH... OK (ADC: 1852)
```

**Valores ADC Típicos:**
- pH 0: 200-300
- pH 7: 1800-2000
- pH 14: 3500-3800

**✗ Se retornar 0 ou 4095:**
- Verifique conexão GPIO 34
- Teste alimentação do sensor
- Verifique fio amarelo

---

### ✅ Teste 5: Conectividade WiFi

**Procedimento:**
1. No Serial, verifique "WiFi: Conectado"
2. Copie o IP exibido

**Resultado Esperado:**
```
📡 WiFi: Conectado (IP: 192.168.1.100, RSSI: -45 dBm)
```

**Intensidade do Sinal (RSSI):**
- -30 dBm: Excelente
- -50 dBm: Bom
- -70 dBm: Fraco
- -90 dBm: Muito fraco

**✗ Se desconectar frequentemente:**
- Aproxime do roteador
- Verifique SSID/senha
- Aumente potência WiFi

---

### ✅ Teste 6: LED de Alerta

**Procedimento:**
1. Altere código temporariamente para forçar alerta:
   ```cpp
   // No loop(), adicione:
   if (temperatura > 50) {
     acionarAlerta(1);  // LED contínuo
   }
   ```
2. Verifique se LED acende

**Resultado Esperado:**
- LED acende quando acionado

**✗ Se não acender:**
- Verifique GPIO 5
- Teste resistor 330Ω
- Verifique orientação LED

---

### ✅ Teste 7: Buzzer

**Procedimento:**
1. Altere código temporariamente:
   ```cpp
   digitalWrite(BUZZER_PIN, HIGH);
   delay(1000);
   digitalWrite(BUZZER_PIN, LOW);
   ```
2. Verifique se buzzer soa

**Resultado Esperado:**
- Som por 1 segundo

**✗ Se não soar:**
- Verifique GPIO 12
- Teste alimentação 5V
- Meça voltagem no pino

---

## Calibração de Sensores

### 🔧 Calibração de Temperatura (DHT22)

**Método 1: Comparação com Termômetro**

```
1. Mergulhe DHT22 em água quente (50°C)
2. Simultaneamente meça com termômetro de referência
3. Registre valores: ESP32 vs Termômetro
4. Calcule diferença

Se ESP32 ler 50.5°C e termômetro 50.0°C:
   Erro = 0.5°C (aceitável, < 2°C)
```

**Método 2: Ponto de Congelamento**

```
1. Coloque DHT22 em uma mistura de gelo+água
2. Valor esperado: 0°C
3. Registre valor lido

Aceitável: -2°C a +2°C
```

**Ajuste no Código (se necessário):**

```cpp
// Adicione no início de lerTemperatura()
temperatura = dht.readTemperature() + 1.0;  // +1.0°C de compensação
```

---

### 🔧 Calibração de Pressão (BMP280)

**Método 1: Comparação com Barômetro**

```
1. Obtenha pressão local em tempo real:
   - INMET (Instituto Nacional de Meteorologia)
   - ou App de previsão do tempo
   
2. Compare com sensor BMP280
3. Se diferença > 5 hPa, sensor precisa revisão
```

**Método 2: Mudança de Altitude**

```
1. Meça em local A (p.ex. nível do mar: 1013 hPa)
2. Suba para local B (p.ex. 100m: ~1013 - 12 = 1001 hPa)
3. Verifique coerência da mudança
```

**Ajuste no Código (se necessário):**

```cpp
// Adicione no início de lerPressao()
pressao = (bmp280.readPressure() / 100.0) + 2.0;  // +2 hPa compensação
```

---

### 🔧 Calibração de pH (Sensor Analógico)

**⚠️ MAIS IMPORTANTE - Deve ser feita obrigatoriamente!**

**Materiais Necessários:**
- Soluções de pH 4.0 e 7.0 (Calibração 2 pontos)
- OU Soluções de pH 2.0, 7.0 e 12.0 (3 pontos)
- Recipiente pequeno
- Papel secante

**Procedimento 1 - Calibração 2 Pontos:**

```
PASSO 1: Preparar Ambiente
├─ Abra serial para monitorar ADC em tempo real
├─ Anote valores de ADC conforme mergulha sensor

PASSO 2: Merging em pH 7.0
├─ Prepare solução pH 7.0 em recipiente
├─ Mergulhe sensor completamente
├─ Aguarde 30 segundos para estabilizar
├─ Registre valor de ADC na serial
├─ Ex: "pH 7.0 → ADC: 2048"

PASSO 3: Merging em pH 4.0
├─ Prepare solução pH 4.0 em OUTRO recipiente
├─ Lave sensor com água destilada
├─ Mergulhe sensor completamente
├─ Aguarde 30 segundos
├─ Registre valor de ADC
├─ Ex: "pH 4.0 → ADC: 2800"

PASSO 4: Calcular Constantes
├─ ADC_pH7 = 2048
├─ ADC_pH4 = 2800
├─
├─ phVoltageMin = (ADC_pH4 * 3.3 / 4095)  # ~2.27V (pH 4)
├─ phVoltageMax = (ADC_pH7 * 3.3 / 4095)  # ~1.66V (pH 7)

PASSO 5: Atualizar Código
├─ biodigestor_esp32.ino linhas ~35-36
├─ phVoltageMin = 2.27  // Seu valor calculado
├─ phVoltageMax = 1.66  // Seu valor calculado
└─ Carregue código novamente
```

**Procedimento 2 - Verificação Rápida:**

```
1. Mergulhe em pH 7.0
2. Valor deve estar entre 6.5 - 7.5
3. Se fora desta faixa, recalibre
```

**Código para Calibração (Teste Rápido):**

```cpp
// Adicione esta função em biodigestor_esp32.ino
void calibrarPH() {
  Serial.println("\n=== CALIBRAÇÃO DE PH ===");
  Serial.println("Mergulhe sensor em pH 7.0");
  Serial.println("Valores ADC (20 leituras):");
  
  for (int i = 0; i < 20; i++) {
    int sensorValue = analogRead(pH_PIN);
    float voltage = sensorValue * (3.3 / 4095.0);
    
    Serial.print("Leitura ");
    Serial.print(i + 1);
    Serial.print(": ADC=");
    Serial.print(sensorValue);
    Serial.print(" Voltage=");
    Serial.println(voltage, 2);
    
    delay(500);
  }
}

// Chame com comando 'L' (calibration):
case 'L':
case 'l':
  calibrarPH();
  break;
```

---

## Procedimentos de Teste

### 🧪 Teste de Precisão - Temperatura

**Objetivo:** Validar estabilidade da temperatura

```
PROCEDIMENTO:
1. Deixe sistema ligado por 2 horas
2. Anote temperatura a cada 5 minutos
3. Calcule média, desvio padrão
4. Verifique variação máxima

CRITÉRIO DE ACEITAÇÃO:
✓ Variação < 1°C por hora
✓ Desvio padrão < 0.5°C
```

**Dados de Teste Exemplo:**

```
Tempo | Temperatura | Nota
  0min   28.5°C      Início
  5min   28.6°C      Estável
 10min   28.4°C      Normal
 15min   28.7°C      Normal
 20min   28.5°C      Normal

Média: 28.54°C
Máximo: 28.7°C
Mínimo: 28.4°C
Variação: 0.3°C ✓ OK
```

---

### 🧪 Teste de Precisão - Pressão

**Objetivo:** Validar estabilidade da pressão

```
PROCEDIMENTO:
1. Deixe sistema ligado por 2 horas
2. Anote pressão a cada 5 minutos
3. Calcule média
4. Verifique variação

CRITÉRIO DE ACEITAÇÃO:
✓ Variação < 2 hPa por hora
✓ Sem saltos abruptos > 5 hPa
```

---

### 🧪 Teste de Precisão - pH

**Objetivo:** Validar resposta rápida do sensor

```
PROCEDIMENTO:
1. Mergulhe em pH 4.0 → Registre valor
2. Lave com água destilada
3. Mergulhe em pH 7.0 → Registre valor
4. Lave com água destilada
5. Mergulhe em pH 12.0 → Registre valor

CRITÉRIO DE ACEITAÇÃO:
✓ pH 4.0: Leitura entre 3.8 - 4.2
✓ pH 7.0: Leitura entre 6.8 - 7.2
✓ pH 12.0: Leitura entre 11.8 - 12.2
✓ Tempo de resposta < 5 segundos
```

---

## Validação de Dados

### 📊 Planilha de Validação

Use para registrar testes:

```
DATA: 12/06/2026
HORA INÍCIO: 10:30
CONDIÇÕES: Ambiente controlado

┌─────────┬──────────┬──────────┬────────┬────────────┐
│ Tempo   │ Temp(°C) │ Press(hPa)│ pH     │ Status WiFi│
├─────────┼──────────┼──────────┼────────┼────────────┤
│ 10:30   │   28.5   │  1013.25 │  7.20  │ Conectado  │
│ 10:35   │   28.6   │  1013.30 │  7.19  │ Conectado  │
│ 10:40   │   28.4   │  1013.20 │  7.21  │ Conectado  │
│ 10:45   │   28.7   │  1013.35 │  7.20  │ Conectado  │
│ 10:50   │   28.5   │  1013.25 │  7.22  │ Conectado  │
└─────────┴──────────┴──────────┴────────┴────────────┘

RESUMO:
Temp Média: 28.54°C
Press Média: 1013.27 hPa
pH Médio: 7.20

CONCLUSÃO: ✓ Sistema operacional
```

---

### ✅ Checklist de Validação Final

- [ ] Todos os sensores detectados
- [ ] Valores dentro de faixa esperada
- [ ] WiFi conectado e estável
- [ ] LED acende corretamente
- [ ] Buzzer soa corretamente
- [ ] Dados são atualizados a cada 10s
- [ ] Nenhuma mensagem de erro
- [ ] Serial comunica a 115200 bps
- [ ] pH calibrado e validado
- [ ] Sistema pronto para produção

---

## Troubleshooting

### ❌ Erro: "DHT Sensor not found"

**Soluções:**
1. Verifique GPIO 4 → DHT22
2. Adicione resistor 10kΩ pull-up
3. Tente outro GPIO (ex: GPIO 5)
4. Recoloque biblioteca DHT
5. Teste sensor com outro ESP32

---

### ❌ Erro: "BMP280 I2C not responding"

**Soluções:**
1. Verifique I2C: GPIO 21 (SDA), GPIO 22 (SCL)
2. Adicione resistores 4.7kΩ pull-up
3. Teste endereço 0x77 também
4. Use Scanner I2C para confirmar

**Scanner I2C (Código teste):**
```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
}

void loop() {
  for (int i = 0; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Device found at 0x");
      Serial.println(i, HEX);
    }
  }
  delay(1000);
}
```

---

### ❌ Erro: "pH leitura 0 ou 4095"

**Soluções:**
1. Verifique GPIO 34 é entrada analógica
2. Teste sem sensor (deve retornar ~2048)
3. Verifique alimentação 5V
4. Adicione filtro capacitor 100nF

---

### ❌ Erro: "WiFi constantemente desconectado"

**Soluções:**
1. Aproxime do roteador
2. Verifique SSID/Senha corretos
3. Reinicie roteador WiFi
4. Use outra rede para testar
5. Aumente timeout WiFi para 30s

---

**Versão:** 1.0.0  
**Data:** 12 de Junho de 2026  
**Mantido por:** Sistema Biodigestor
