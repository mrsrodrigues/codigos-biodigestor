# ⚡ Início Rápido - Biodigestor v3.0

## 🎯 3 Formas de Começar

---

## ✅ OPÇÃO 1: Simular no Wokwi (MAIS FÁCIL - 30 min)

**Sem comprar nada. Teste online grátis.**

### Passo 1: Ir ao Wokwi
```
1. Abra navegador
2. Digite: https://wokwi.com
3. Clique "Start Free"
```

### Passo 2: Criar Projeto
```
1. Clique "+ New Project"
2. Selecione "ESP32"
3. Nome: "Biodigestor"
4. Clique "Create"
```

### Passo 3: Carregar Código
```
1. Abra arquivo: biodigestor_esp32_v3_completo.ino
2. Selecione TUDO (Ctrl+A)
3. Copie (Ctrl+C)
4. Na aba sketch.ino do Wokwi
5. Limpe e cole (Ctrl+V)
```

### Passo 4: Adicionar Componentes

**Clique "+" no lado direito e pesquise:**

```
1. SSD1306 OLED     → Coloque no simulador
2. DS18B20         → Coloque no simulador
3. MQ4 Gas Sensor  → Coloque no simulador
4. Pressure Sensor → Coloque no simulador
5. Resistor 4.7k   → Coloque no simulador
```

### Passo 5: Conectar Pinos

**Arraste "fios" (linhas) entre:**

```
OLED:
  GND → GND
  VCC → 3.3V
  SCL → GPIO 22
  SDA → GPIO 21

DS18B20:
  GND → GND
  VCC → 3.3V
  DQ → GPIO 4

MQ-4:
  GND → GND
  VCC → 3.3V
  A0 → GPIO 34

Pressão:
  GND → GND
  VCC → 3.3V
  OUT → GPIO 32

Resistor:
  Pino 1 → 3.3V
  Pino 2 → GPIO 4
```

### Passo 6: Rodar Simulação
```
1. Clique ▶ (Play) no topo
2. Aguarde compilação (~10 seg)
3. Simulação começa
4. Veja Serial Monitor à direita
5. OLED mostra dados na esquerda
```

### Resultado ✅
```
✓ OLED mostra: Biodigestor v3.0
✓ Serial mostra: Setup completo!
✓ Sensores respondendo
✓ Dados atualizando
```

---

## ✅ OPÇÃO 2: Usar Local (Recomendado - 2 horas)

**Compre componentes e monte no ESP32 real.**

### Passo 1: Comprar Componentes
```
Tempo: 5-7 dias (envio)
Custo: ~R$150

1. ESP32 DevKit (R$40)
2. OLED SSD1306 (R$25)
3. DS18B20 (R$10)
4. MQ-4 (R$30)
5. pH4502C (R$25)
6. Sensor Pressão (R$20)
7. Resistor 4.7kΩ (R$1)
8. Cabo Micro USB (R$10)
9. Jumpers + Protoboard (R$10)
```

### Passo 2: Instalar Arduino IDE
```
1. Acesse: https://www.arduino.cc/en/software
2. Baixe para seu SO (Windows/Mac/Linux)
3. Instale normalmente
4. Abra Arduino IDE
```

### Passo 3: Configurar ESP32
```
1. Arduino IDE → Arquivo → Preferências
2. Em "URLs Adicionais de Gerenciadores de Placas"
3. Cole: https://dl.espressif.com/dl/package_esp32_index.json
4. OK
5. Ferramentas → Placa → Gerenciador de Placas
6. Pesquise "ESP32"
7. Instale "esp32 by Espressif"
8. Aguarde ~5 minutos
```

### Passo 4: Carregar Código
```
1. Abra Arduino IDE
2. Arquivo → Abrir
3. Selecione: biodigestor_esp32_v3_completo.ino
4. ANTES DE CARREGAR, edite:
   - Linha ~32: seu_SSID_aqui → seu WiFi
   - Linha ~33: sua_senha_aqui → sua senha
5. Conecte ESP32 via USB
6. Ferramentas → Placa → ESP32
7. Ferramentas → Porta → Selecione a porta COM
8. Sketch → Upload (ou Ctrl+U)
9. Aguarde (pode demorar)
```

### Passo 5: Montar Circuito
```
Ver arquivo: CONEXAO_PINAGEM.md
Tempo: 1-2 horas

Ou resumido:
┌─────────────────┐
│     ESP32       │
├─────────────────┤
│ GPIO 21 ────────┬─→ SDA (OLED)
│ GPIO 22 ────────┼─→ SCL (OLED)
│ GPIO 4  ────────┼─→ DQ (DS18B20)
│ GPIO 34 ────────┼─→ A0 (MQ-4)
│ GPIO 35 ────────┼─→ PH (pH4502C)
│ GPIO 32 ────────┼─→ OUT (Pressão)
│ GND ────────────┼─→ GND comum
│ 3.3V ───────────┼─→ VCC comum
└─────────────────┘
```

### Passo 6: Testar
```
1. Serial Monitor: Tools → Serial Monitor
2. Baud Rate: 115200
3. Verifique se Serial mostra:
   - ✓ Inicializando SPIFFS... OK
   - ✓ Inicializando Display OLED... OK
   - ✓ Conectando WiFi
   - ✓ WiFi OK - IP: 192.168.1.X

4. OLED mostra: Biodigestor v3.0
5. Valores aparecem
```

### Passo 7: Acessar Dashboard
```
1. Copie o IP do Serial Monitor (192.168.1.100)
2. Abra navegador
3. Digite: http://192.168.1.100
4. 🎊 Dashboard mostra dados em tempo real!
```

### Resultado ✅
```
✓ ESP32 conectada ao WiFi
✓ OLED mostra dados
✓ Serial tem leituras
✓ Website acessível
✓ Dados salvando em arquivo
```

---

## ✅ OPÇÃO 3: Hospedar Online (Avançado - 1 hora)

**Para acessar dados de fora de casa pelo celular.**

### Passo 1: Preparar Pasta
```
Crie pasta com:
- server.js
- package.json
- dashboard_biodigestor.html
- vercel.json
- .gitignore
```

### Passo 2: Criar Conta Vercel
```
1. Acesse: https://vercel.com
2. Clique "Sign Up"
3. Use GitHub
4. Conecte GitHub
```

### Passo 3: Criar Repositório GitHub
```
1. Acesse: https://github.com/new
2. Nome: biodigestor-dashboard
3. Descrição: Dashboard biodigestor
4. Público
5. Create
```

### Passo 4: Upload no GitHub
```
Abra Terminal/CMD na pasta do projeto:

git init
git add .
git commit -m "Upload inicial"
git branch -M main
git remote add origin https://github.com/SEU_USUARIO/biodigestor-dashboard.git
git push -u origin main
```

### Passo 5: Deploy no Vercel
```
1. Acesse Vercel
2. Clique "Add New"
3. Selecione seu repositório
4. Deploy
5. Aguarde (~2 min)
6. Recebe URL: biodigestor-dashboard.vercel.app
```

### Passo 6: Configurar ESP32
```
No código biodigestor_esp32_v3_completo.ino:

Linha ~32:
  Mude: const char* API_URL = "http://192.168.1.100";
  Para: const char* API_URL = "https://biodigestor-dashboard.vercel.app";

Carregue novamente na ESP32.
```

### Resultado ✅
```
✓ Site online funciona
✓ Acessível via celular em qualquer lugar
✓ Dados salvando em servidor
✓ Pode compartilhar link com amigos
```

---

## 🎮 Simular Sensores no Wokwi

Depois que tiver montado:

### Mudar Temperatura
```
1. Clique no sensor DS18B20
2. Arraste o slider para cima
3. Vê temperatura aumentar em tempo real
4. Serial mostra: "Temp: 35.2 C"
5. OLED atualiza
```

### Aumentar Metano
```
1. Clique no MQ-4
2. Arraste slider para 600 ppm
3. Serial mostra: "🔴 ALERTA: Nível alto de metano!"
4. Diminua para 100 ppm
5. Alerta desaparece
```

### Testar pH
```
1. Clique no sensor de pH
2. Arraste para pH < 6.0
3. Serial mostra: "⚠️ AVISO pH"
4. Ajuste para 7.0-7.5
5. Aviso some
```

---

## 📊 Comandos no Serial Monitor

```
R → Reset mín/máx dos sensores
S → Mostrar estatísticas
C → Reconectar WiFi
T → Testar sensores
D → Mostrar todos os logs
E → Estatísticas do experimento
L → Limpar dados armazenados
```

**Como usar:**
```
1. Abra Serial Monitor
2. Digite "R" e pressione Enter
3. Resultado aparece
```

---

## 🚨 Problemas? Aqui está a Solução

| Problema | Solução |
|----------|---------|
| OLED não mostra | Verifique pinos 21/22 |
| Sem WiFi | SSID/Senha errada |
| DS18B20 = -127°C | Falta resistor 4.7k no GPIO 4 |
| Website não abre | ESP32 desconectou, reinicie |
| Serial vazio | Verifique USB e Baud Rate 115200 |

---

## ✅ Checklist de Progresso

### Simulação Wokwi
- [ ] Conta criada
- [ ] Projeto ESP32
- [ ] Componentes adicionados
- [ ] Pinos conectados
- [ ] Código carregado
- [ ] Play - Simulação rodando
- [ ] OLED mostra dados
- [ ] Serial funciona

### Hardware Real
- [ ] Componentes comprados
- [ ] Arduino IDE instalada
- [ ] ESP32 configurada
- [ ] Código carregado
- [ ] Circuito montado
- [ ] Serial mostra dados
- [ ] OLED funciona
- [ ] Website acessível

### Online
- [ ] GitHub criado
- [ ] Vercel conectado
- [ ] Deploy feito
- [ ] URL funciona
- [ ] ESP32 enviando dados
- [ ] Dados aparecem no site

---

## 🎓 Próximos Passos

1. ✅ Simule no Wokwi
2. ✅ Monte o hardware
3. ✅ Hospede online
4. ✅ Rode experimento 10 dias
5. ✅ Analise dados
6. ✅ Compartilhe resultados

---

## 💡 Dicas Importantes

```
💡 Sempre edite WiFi ANTES de carregar
💡 Verifique USB conectado
💡 Use Baud Rate 115200
💡 Reinicie ESP32 se tiver erro
💡 Teste cada sensor isolado
💡 Backup dos dados regularmente
💡 Não deixe ESP32 ao sol direto
```

---

## 🆘 Precisa de Ajuda?

```
1. Leia: DOCUMENTACAO_COMPLETA.md
2. Veja: TUTORIAL_WOKWI.md ou WEBSITE_GUIA_COMPLETO.md
3. Procure arquivo específico de erro
4. Pesquise no Google
5. Pergunte em comunidades Arduino
```

---

## 🎉 Você Consegue!

Siga os passos e em ~3 horas você terá:
- ✅ Sistema funcionando
- ✅ Dashboard com dados em tempo real
- ✅ 10 dias de armazenamento
- ✅ Website online (opcional)

**Divirta-se! 🚀**

---

**Versão:** 3.0.0  
**Atualizado:** 12 de Junho de 2026  
**Linguagem:** Português (Brasil)
