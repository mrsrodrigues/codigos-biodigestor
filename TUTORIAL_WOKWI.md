# Tutorial Completo - Simulador Wokwi com Biodigestor

## 🌐 O que é Wokwi?

Wokwi é um **simulador online gratuito** para Arduino, ESP32 e outros microcontroladores. Permite testar código **sem hardware real**, ideal para desenvolvimento e testes.

**URL:** https://wokwi.com

---

## 🚀 Começar no Wokwi em 5 Minutos

### Passo 1: Criar Conta (Opcional)

```
1. Acesse https://wokwi.com
2. Clique "Sign Up" (canto superior)
3. Use email ou GitHub
4. Confirme email
```

### Passo 2: Criar Novo Projeto

```
1. Clique "+ New Project"
2. Selecione "ESP32"
3. Escolha nome (ex: "Biodigestor")
4. Clique "Create"
```

### Passo 3: Interface do Wokwi

```
┌─────────────────────────────────────┐
│ Editor de Código (Esquerda)        │
│ ┌──────────────────────────────┐   │
│ │ sketch.ino                   │   │
│ │ (Seu código Arduino aqui)    │   │
│ │                              │   │
│ └──────────────────────────────┘   │
├─────────────────────────────────────┤
│ Simulador (Direita)                │
│ ┌──────────────────────────────┐   │
│ │ [Placa ESP32]               │   │
│ │ Componentes conectados      │   │
│ │ [Display OLED]              │   │
│ └──────────────────────────────┘   │
└─────────────────────────────────────┘
```

---

## 🔧 Adicionar Componentes (Passo a Passo)

### 1. OLED SSD1306 (Display)

**Adição Manual:**
```
1. Clique no ícone "+" (Direita)
2. Procure: "SSD1306"
3. Clique em "SSD1306 I2C OLED Display"
4. Clique em area do simulador para colocar
```

**Conexão:**
```
OLED SSD1306:
  GND → GND (ESP32)
  VCC → 3.3V (ESP32)
  SCL → GPIO 22 (ESP32)
  SDA → GPIO 21 (ESP32)

No Wokwi (arrastar fios):
1. Clique no pino GND do OLED
2. Arraste até GND do ESP32
3. Repita para outros pinos
```

### 2. Sensor DS18B20 (Temperatura à prova d'água)

**Adição:**
```
1. Clique "+"
2. Procure: "DS18B20"
3. Selecione "DS18B20 Temperature Sensor"
4. Coloque no simulador
```

**Conexão:**
```
DS18B20 (3 fios):
  Vermelho (VCC) → 3.3V
  Preto (GND) → GND
  Amarelo (DQ) → GPIO 4

Com resistor pull-up:
- Adicione resistor 4.7kΩ entre GPIO 4 e 3.3V
  (Procure por "resistor" e coloque entre os pinos)
```

### 3. Sensor MQ-4 (Metano)

**Adição:**
```
1. Clique "+"
2. Procure: "MQ4" ou "Gas Sensor"
3. Selecione MQ-4 ou similar
4. Coloque no simulador
```

**Conexão:**
```
MQ-4 (4 pinos):
  VCC → 5V (ou 3.3V)
  GND → GND
  A0 (Analógico) → GPIO 34
  D0 (Digital) → (Não usar)
```

### 4. Sensor de Pressão (MPX5700)

**Adição:**
```
1. Clique "+"
2. Procure: "MPX5700" ou "Pressure Sensor"
3. Selecione o sensor
4. Coloque no simulador
```

**Conexão:**
```
MPX5700:
  GND → GND
  VCC → 3.3V
  VOUT → GPIO 32 (Analógico)
```

### 5. Módulo pH BNC PH4502C

**Adição:**
```
1. Clique "+"
2. Procure: "Analog Sensor" ou "pH Module"
3. Use componente genérico de entrada analógica
4. Coloque no simulador
```

**Conexão:**
```
PH4502C:
  GND → GND
  VCC → 5V
  PH → GPIO 35 (Analógico)
```

### 6. Botão de Reset (Opcional)

```
1. Clique "+"
2. Procure: "Button"
3. Coloque no simulador
4. Conecte aos pinos RST e GND
```

---

## 📋 Arquivo de Configuração (diagram.json)

Depois de montar os componentes, o Wokwi cria um arquivo `diagram.json`. Você pode editá-lo para definir conexões precisas:

```json
{
  "version": 1,
  "author": "Seu Nome",
  "editor": "wokwi",
  "parts": [
    { "type": "wokwi-esp32-devkit", "id": "esp" },
    {
      "type": "wokwi-ssd1306",
      "id": "oled",
      "top": 20,
      "left": 300,
      "attrs": { "address": "0x3c" }
    },
    {
      "type": "wokwi-ds18b20",
      "id": "temp",
      "top": 100,
      "left": 300
    },
    {
      "type": "wokwi-resistor",
      "id": "pullup",
      "top": 140,
      "left": 250,
      "attrs": { "resistance": "4700" }
    }
  ],
  "connections": [
    ["esp:GND", "oled:GND", "black", ["0"]],
    ["esp:3V3", "oled:VCC", "red", ["0"]],
    ["esp:22", "oled:SCL", "yellow", ["0"]],
    ["esp:21", "oled:SDA", "green", ["0"]],
    ["esp:4", "temp:DQ", "blue", ["0"]],
    ["esp:3V3", "pullup:1", "red", ["0"]],
    ["pullup:2", "temp:DQ", "blue", ["0"]]
  ]
}
```

---

## 💻 Carregar Código no Wokwi

### Método 1: Copiar e Colar Código

```
1. Abra o arquivo: biodigestor_esp32_v3_completo.ino
2. Selecione TODO o código (Ctrl+A)
3. Copie (Ctrl+C)
4. Abra Wokwi no navegador
5. Na aba "sketch.ino", limpe o código
6. Cole o seu código (Ctrl+V)
7. Clique "Start Simulation" (Play)
```

### Método 2: Importar do GitHub

```
1. Acesse: https://github.com/seu_usuario/biodigestor
2. Copie o link do arquivo .ino
3. No Wokwi, clique "..."
4. Selecione "Import from GitHub"
5. Cole o link
```

### Método 3: Arquivo Local

```
1. Salve seu código como sketch.ino
2. No Wokwi, clique "File"
3. Selecione "Load File"
4. Escolha seu sketch.ino
```

---

## ▶️ Executar a Simulação

### Iniciar Simulação

```
1. Clique no botão Play (▶) superior
2. Aguarde compilação
3. Simulação inicia automaticamente
```

### Monitor Serial

```
1. No lado direito, clique "Serial"
2. Você vê toda a saída do Serial.println()
3. Digite comandos e pressione Enter
4. O código processa sua entrada
```

### Controlar Componentes

```
OLED:
  - Mostra dados em tempo real
  - Atualiza a cada segundo
  
Sensores:
  - Clique no sensor para alterar valor
  - MQ-4: Arraste slider (0-4095)
  - DS18B20: Arraste temperatura (0-100°C)
  - Pressão: Arraste valor (0-1200 kPa)
  - pH: Arraste pH (0-14)
```

---

## 🎮 Interagir com a Simulação

### Simular Aumento de Temperatura

```
1. Clique no sensor DS18B20
2. Arraste o slider para cima
3. Veja no OLED: temperatura aumenta
4. Monitor Serial mostra leitura
```

### Simular Produção de Biogás (MQ-4)

```
1. Clique no sensor MQ-4
2. Arraste slider
3. Valor de ppm aumenta
4. Se > 500 ppm, alerta no serial
```

### Simular Mudança de Pressão

```
1. Clique no sensor de pressão
2. Arraste slider para aumentar/diminuir
3. Valor em kPa muda
```

### Simular Variação de pH

```
1. Clique no sensor de pH
2. Arraste slider
3. Se < 6.0 ou > 8.5, aviso no serial
```

---

## 📊 Testar Componentes

### Teste 1: Display OLED

```
Esperado:
✓ OLED mostra "Biodigestor v3.0"
✓ Mostra valores de cada sensor
✓ Atualiza a cada segundo
✓ Sem erro de conexão I2C
```

### Teste 2: Sensor Temperatura

```
1. Abra monitor serial
2. Veja leitura de temperatura
3. Clique no sensor DS18B20
4. Altere valor
5. Serial mostra novo valor
```

### Teste 3: Sensor Metano

```
1. Clique no MQ-4
2. Arraste slider para 600 ppm
3. Serial mostra: "🔴 ALERTA: Nível alto de metano!"
4. Diminua para 100 ppm
5. Alerta desaparece
```

### Teste 4: Display via HTTP

```
1. Clique em "Simulator"
2. Procure por "5000:80"
3. Clique no link HTTP
4. Página web abre com dados
5. Valores atualizam em tempo real
```

---

## 🔍 Debugar Erros Comuns

### Erro: "Biblioteca não encontrada"

```
Solução:
1. Certifique-se que está usando ESP32
2. Verifique nomes das bibliotecas
3. Se compilar offline, instale via Arduino IDE:
   Tools → Manage Libraries → Pesquise biblioteca
```

### OLED não aparece ou mostra lixo

```
Verificar:
1. Endereço I2C correto (0x3C ou 0x3D)
2. Pinos SDA/SCL corretos (21/22)
3. Resistores pull-up presentes
4. Alimentação 3.3V conectada

Solução no código:
if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Tente 0x3D
  Serial.println("SSD1306 allocation failed");
}
```

### DS18B20 retorna -127 ou erro

```
Verificar:
1. Pino DQ conectado a GPIO 4
2. Resistor pull-up 4.7kΩ presente
3. 1-Wire biblioteca corretamente instalada

Solução:
// Adicione no setup():
sensoresTemp.begin();
sensoresTemp.setResolution(12);
```

### Serial não mostra dados

```
Verificar:
1. Baud rate: 115200
2. No simulador, clique "Serial Monitor"
3. Verifique se está compilando (canto superior)
```

---

## 📤 Exportar Projeto

### Salvar Projeto

```
1. Clique "File"
2. Selecione "Save Project"
3. Nome: "Biodigestor_v3"
4. Salva automaticamente
```

### Compartilhar Projeto

```
1. Clique "Share" (canto superior)
2. Cria link compartilhável
3. Qualquer pessoa pode abrir e simular
4. Link: https://wokwi.com/projects/XXXXX
```

### Exportar para Arduino IDE

```
1. Clique "..."
2. Selecione "Download"
3. Salva sketch.ino
4. Pode carregar em ESP32 real
```

---

## ✅ Checklist de Simulação

- [ ] Conta Wokwi criada
- [ ] Novo projeto ESP32 criado
- [ ] Display OLED SSD1306 adicionado
- [ ] Sensor DS18B20 adicionado
- [ ] Sensor MQ-4 adicionado
- [ ] Sensor de pressão adicionado
- [ ] Sensor de pH adicionado
- [ ] Todas as conexões feitas
- [ ] Código carregado sem erros
- [ ] Simulação iniciada com sucesso
- [ ] OLED mostra dados
- [ ] Serial exibe valores
- [ ] Sensores respondendo a alterações
- [ ] Servidor HTTP respondendo (porta 80)

---

## 🔗 Recursos Úteis

| Recurso | Link |
|---------|------|
| Wokwi | https://wokwi.com |
| Documentação Wokwi | https://docs.wokwi.com |
| Exemplos | https://wokwi.com/projects |
| Componentes | https://docs.wokwi.com/parts |

---

## 💡 Dicas e Truques

### Dica 1: Simular Reinicialização

```
Pressione Ctrl+Shift+R para reiniciar simulação
```

### Dica 2: Acelerar Tempo

```
No canto superior:
- 1x: Velocidade normal
- 2x: Duas vezes mais rápido
- 5x: 5x mais rápido
Útil para testes longos
```

### Dica 3: Breakpoints

```
Clique no número da linha para adicionar breakpoint
Simulação para no ponto
Inspecione variáveis
```

### Dica 4: Gráficos em Tempo Real

```
1. Clique em "Scope"
2. Selecione pinos para monitorar
3. Veja gráfico em tempo real
```

---

## 📝 Código de Teste (Simples)

Se quer começar com código mais simples para testar:

```cpp
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while(1);
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Biodigestor");
  display.setCursor(0, 20);
  display.println("v3.0");
  display.display();
  
  Serial.println("Setup completo!");
}

void loop() {
  Serial.println("Testando...");
  delay(1000);
}
```

---

## 🎓 Próximos Passos

1. ✅ Criar projeto no Wokwi
2. ✅ Adicionar todos os componentes
3. ✅ Conectar pinos corretamente
4. ✅ Carregar código
5. ✅ Testar simulação
6. ✅ Modificar valores de sensores
7. ✅ Ver alterações no OLED
8. ✅ Acessar dados via HTTP
9. ✅ Depois: Carregar em ESP32 real

---

**Versão:** 1.0.0  
**Data:** 12 de Junho de 2026  
**Simulador:** Wokwi (https://wokwi.com)
