# Quick Start Guide - Biodigestor ESP32

## ⚡ Início Rápido em 5 Minutos

### 1️⃣ Prepare o Hardware

```
Hardware necessário:
✓ Placa ESP32 DevKit
✓ Sensor DHT22
✓ Sensor BMP280
✓ Sensor de pH (Analógico)
✓ LED + Resistor 330Ω
✓ Buzzer Piezo
✓ Cabo USB Micro
✓ Solda e fio
```

### 2️⃣ Instale Arduino IDE

1. Baixe de: https://www.arduino.cc/en/software
2. Instale conforme seu SO
3. Abra Arduino IDE

### 3️⃣ Adicione Suporte ESP32

```
Arduino IDE → Preferences
Adicione em "Additional Boards Manager URLs":
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

### 4️⃣ Instale Bibliotecas

```
Tools → Manage Libraries

Procure e instale:
□ DHT sensor library (Adafruit)
□ Adafruit BMP280 Library
□ Adafruit Unified Sensor
```

### 5️⃣ Faça as Conexões

**Conexões Rápidas:**

| Sensor | Pino ESP32 |
|--------|-----------|
| DHT22 Data | GPIO 4 |
| BMP280 SDA | GPIO 21 |
| BMP280 SCL | GPIO 22 |
| pH Sensor | GPIO 34 |
| LED | GPIO 5 |
| Buzzer | GPIO 12 |
| GND | GND |
| 5V | 5V |

### 6️⃣ Configure o Código

```cpp
// Linha 18-19 do biodigestor_esp32.ino
const char* ssid = "seu_SSID_aqui";
const char* password = "sua_senha_aqui";
```

### 7️⃣ Carregue o Código

```
1. Connect USB
2. Tools → Board → ESP32 Dev Module
3. Tools → Port → COM (seu porto)
4. Sketch → Upload
```

### 8️⃣ Teste

```
Abra Serial Monitor (Ctrl+Shift+M)
Velocidade: 115200

Você deve ver:
✓ Inicialização bem-sucedida
✓ Dados dos sensores
✓ WiFi conectado
```

---

## 📊 Leitura de Dados

### Dados Exibidos Continuamente

```
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

## 🔧 Comandos Serial Rápidos

### Comandos Disponíveis

```
Digitar no Serial Monitor e pressionar Enter:

R → Reseta valores min/max
S → Mostra estatísticas
C → Reconecta WiFi
T → Testa sensores
```

### Teste de Sensores

```
Comando: T

Resultado esperado:
Testando DHT22 (Temperatura)... OK (28.5°C)
Testando BMP280 (Pressão)... OK (1013.25 hPa)
Testando sensor de pH... OK (ADC: 1852)
```

---

## 🚨 Alertas Principais

| Alerta | Condição | Ação |
|--------|----------|------|
| 🔴 Temperatura Baixa | < 20°C | LED pisca + Buzzer |
| 🔴 Temperatura Alta | > 60°C | LED ligado + Buzzer |
| ⚠️ pH Baixo | < 6.5 | Aviso no Serial |
| ⚠️ pH Alto | > 8.0 | Aviso no Serial |
| ⚠️ Pressão Alta | > 1050 hPa | Aviso no Serial |

---

## 📈 Faixa de Valores Esperados

### Temperatura (DHT22)

```
Faixa Normal: 20 - 60°C
Faixa Ideal Biodigestor: 35 - 40°C
```

### Pressão (BMP280)

```
Faixa Normal: 900 - 1100 hPa
Nível do Mar: ~1013 hPa
Alerta: > 1050 hPa
```

### pH (Sensor Analógico)

```
Faixa Possível: 0 - 14
Faixa Ideal Biodigestor: 6.5 - 8.0
Normal: 7.0 (Neutro)
```

---

## 🔌 Troubleshooting Rápido

### Problema: Não vejo dados no Serial

**Solução:**
1. Verifique baud rate: **115200**
2. Confirme USB está conectado
3. Verifique COM port correto
4. Resete ESP32 (botão RST)

### Problema: Sensor não detectado

**Solução:**
1. Execute comando `T` (teste)
2. Verifique conexões
3. Verifique pino GPIO correto
4. Recalibre sensor se necessário

### Problema: WiFi não conecta

**Solução:**
1. Verifique SSID/Senha
2. ESP32 perto do roteador
3. Execute comando `C` (reconecta)
4. Verifique rede 2.4GHz

---

## 📝 Checklist de Funcionamento

- [ ] Código carregado com sucesso
- [ ] Serial Monitor abre a 115200 bps
- [ ] Inicialização bem-sucedida
- [ ] DHT22 lendo temperatura
- [ ] BMP280 lendo pressão
- [ ] Sensor pH enviando sinal
- [ ] WiFi conectado
- [ ] LED acende quando acionado
- [ ] Buzzer soa quando acionado
- [ ] Dados são lidos a cada 10 segundos

---

## 📚 Próximos Passos

1. **Calibrar Sensor de pH**
   - Use soluções pH 4.0 e 7.0
   - Ajuste phVoltageMin e phVoltageMax

2. **Configurar Servidor** (Opcional)
   - ThingSpeak ou Blynk
   - Seu próprio servidor REST

3. **Armazenar Dados** (Opcional)
   - MicroSD Card
   - SPIFFS da ESP32

4. **Criar Dashboard** (Opcional)
   - Página Web
   - App Mobile

---

## 🆘 Recursos Úteis

| Recurso | Link |
|---------|------|
| Docs ESP32 | https://docs.espressif.com/ |
| Arduino | https://www.arduino.cc/ |
| DHT Library | GitHub Adafruit |
| BMP280 Library | GitHub Adafruit |

---

## 📞 Suporte

**Erro no código?**
- Verifique linhas comentadas
- Leia README.md para contexto
- Consulte CHANGELOG.md para detalhes

**Dúvida na conexão?**
- Veja CONEXAO_PINAGEM.md
- Verifique diagrama de circuito

**Sensor não funciona?**
- Execute teste com comando `T`
- Verifique alimentação
- Teste com outro pino GPIO

---

**Versão:** 1.0.0  
**Data:** 12 de Junho de 2026  
**Tempo Estimado de Setup:** 5-10 minutos
