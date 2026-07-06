# 📚 Documentação Completa - Biodigestor v3.0

## 🎯 Objetivo Final

Criar um **sistema completo de monitoramento de biodigestor** com:
- ✅ Firmware ESP32 com 5 sensores
- ✅ Display OLED em tempo real
- ✅ Armazenamento em SPIFFS (10 dias de dados)
- ✅ Website com dashboard e filtros
- ✅ Tutorial do simulador Wokwi
- ✅ Documentação detalhada

---

## 📁 Arquivos do Projeto

### 1. **biodigestor_esp32_v3_completo.ino** ⭐ PRINCIPAL

**O que é:** Código principal ESP32 com TODOS os sensores

**Sensores Inclusos:**
- MQ-4: Sensor de metano (biogás)
- DS18B20: Temperatura à prova d'água
- PH4502C: Sensor de pH BNC
- Sensor de Pressão 1.2 MPa
- OLED SSD1306: Display 128×64

**Funcionalidades:**
- Leitura de sensores a cada 10 segundos
- Display OLED atualizado a cada segundo
- Logging em arquivo CSV a cada hora
- Servidor HTTP na porta 80
- 4 endpoints API (/api/dados, /api/stats, /api/logs)
- Alertas de temperatura/pH/metano
- WiFi com reconexão automática

**Uso:**
```
1. Arduino IDE: Ferramentas → Placa → ESP32
2. Colar código no editor
3. Conectar ESP32 via USB
4. Carregar (Ctrl+U)
5. Abrir Serial Monitor (115200 baud)
```

**Configurar ANTES de Carregar:**
```cpp
// Linha ~32-33:
const char* ssid = "seu_SSID_aqui";
const char* password = "sua_senha_aqui";
```

---

### 2. **TUTORIAL_WOKWI.md** 🌐 SIMULADOR

**O que é:** Guia passo a passo para testar no simulador Wokwi

**Conteúdo:**
- Criar conta no Wokwi
- Adicionar cada componente (OLED, DS18B20, MQ-4, etc.)
- Conectar pinos via "fios"
- Carregar código
- Executar simulação
- Simular sensores
- Testar endpoints HTTP
- Debugar erros

**Como Usar:**
```
1. Acesse https://wokwi.com
2. Crie novo projeto ESP32
3. Siga tutorial TUTORIAL_WOKWI.md
4. Carregue código v3.0.0
5. Clique "Play" para simular
```

**Tempo:** ~30 minutos do zero

---

### 3. **dashboard_biodigestor.html** 📊 WEBSITE

**O que é:** Página web completa com dashboard em tempo real

**Funcionalidades:**
- Cards mostrando valores atuais (Metano, Temp, pH, Pressão)
- Status WiFi conectado/desconectado
- Filtros de data/hora para buscar dados históricos
- Tabela com histórico completo
- Gráfico interativo de temperatura e pressão
- Exportar dados como CSV
- Design responsivo (funciona em celular)
- Atualização automática a cada 10 segundos

**Como Usar (Opção 1 - Local):**
```
1. Salve arquivo em: C:\seu_caminho\dashboard_biodigestor.html
2. Abra no navegador (duplo clique)
3. Navegador abre local (file:///)
4. Não conecta ao ESP32 (arquivo local não pode)
```

**Como Usar (Opção 2 - Conectado ao ESP32):**
```
1. ESP32 conectada à WiFi
2. Abra terminal/cmd
3. Navegue para pasta do arquivo
4. Python: python -m http.server 8000
5. Abra navegador: http://localhost:8000
6. Clique em dashboard_biodigestor.html
7. Atualiza com dados do ESP32 em tempo real
```

**Como Usar (Opção 3 - Vercel Online):**
```
Ver arquivo WEBSITE_GUIA_COMPLETO.md
```

---

### 4. **WEBSITE_GUIA_COMPLETO.md** 🚀 DEPLOY

**O que é:** Guia completo para hospedar website online

**Cobre:**
- Opção 1: Acessar direto do ESP32 (192.168.1.100)
- Opção 2: Hospedar em Vercel (gratuito)
- Opção 3: Usar Railway com PostgreSQL
- Criar backend Node.js
- Deploy passo a passo
- Segurança e autenticação
- Acesso remoto pelo celular
- Troubleshooting

**Plataformas Suportadas:**
- Vercel (frontend + Node.js)
- Railway (backend + PostgreSQL)
- Render (alternativa)
- Heroku (pago - mais confiável)

**Tempo:** ~1 hora para setup completo

---

### 5. **README.md** 📖 OVERVIEW

Se você tiver, este arquivo contém:
- Visão geral do projeto
- Características principais
- Requisitos de hardware
- Instruções básicas
- Protocolos de comunicação
- Troubleshooting inicial

---

### 6. **CHANGELOG.md** 📝 HISTÓRICO

Se você tiver, contém:
- Versão 1.0: Sistema básico com DHT22
- Versão 2.0: Logging com armazenamento SPIFFS
- Versão 3.0: Novos sensores + OLED + Web

---

## 🔧 Componentes Necessários (Hardware)

| Componente | GPIO | Protocolo | Quantidade |
|------------|------|-----------|-----------|
| ESP32 DevKit | - | - | 1 |
| OLED SSD1306 | 21/22 | I2C | 1 |
| DS18B20 | 4 | 1-Wire | 1 |
| MQ-4 | 34 | ADC | 1 |
| PH4502C | 35 | ADC | 1 |
| Sensor Pressão | 32 | ADC | 1 |
| Resistor 4.7k | - | Pull-up | 1 |
| Cabo Micro USB | - | Programação | 1 |

---

## 🚀 Guia Rápido - Começar do Zero

### Fase 1: Simular (30 min)

```
1. Abra https://wokwi.com
2. Siga TUTORIAL_WOKWI.md
3. Carregue código v3.0.0
4. Simule por 5 minutos
5. Verifique OLED e Serial
```

**Resultado:** Confirmar que código funciona

### Fase 2: Montar Hardware (2-3 h)

```
1. Compre componentes
2. Monte conforme CONEXAO_PINAGEM.md
3. Programe ESP32 com código v3.0.0
4. Verifique Serial Monitor
5. Acesse http://192.168.1.X no navegador
```

**Resultado:** Dashboard funciona no navegador

### Fase 3: Dados Online (1 h)

```
1. Siga WEBSITE_GUIA_COMPLETO.md
2. Crie conta Vercel
3. Deploy do website
4. Conecte ESP32 ao servidor
5. Acesse remotamente
```

**Resultado:** Acessa dados do celular em qualquer lugar

### Fase 4: Filtros e Histórico (30 min)

```
1. Use filtros de data/hora no website
2. Selecione períodos específicos
3. Exporte dados como CSV
4. Importe em Excel para análise
```

**Resultado:** Análise completa de 10 dias de experimento

---

## 📊 Pinagem Completa

```
ESP32 → Sensores

GPIO 21 (SDA) → OLED SSD1306 SDA
GPIO 22 (SCL) → OLED SSD1306 SCL
GPIO 4 (Digital) → DS18B20 DQ
GPIO 34 (ADC) → MQ-4 A0
GPIO 35 (ADC) → pH4502C PH
GPIO 32 (ADC) → Pressão VOUT
GPIO GND → GND comum
GPIO 3.3V → VCC (OLED, DS18B20, Pressão)
GPIO 5V → VCC (MQ-4, opcional)

Resistores:
- 4.7kΩ entre GPIO 4 e 3.3V (Pull-up DS18B20)
```

---

## 💾 Armazenamento de Dados

**Arquivo:** /dados_biodigestor.csv (em SPIFFS)

**Formato CSV:**
```
LEITURA,DATA_HORA,TEMP_C,TEMP_MIN,TEMP_MAX,PRESSAO_HPA,PH,WIFI_STATUS
1,12/06/2026 10:00:00,28.5,28.5,28.5,1013.25,7.2,OK
2,12/06/2026 11:00:00,29.1,28.5,29.1,1014.50,7.1,OK
...
```

**Capacidade:**
- 300 entradas máximo (≈ 12.5 dias a cada hora)
- Se chegar ao limite, sobrescreve os dados antigos

**Acessar:**
- Via API: GET /api/logs (baixa CSV)
- Serial: Comando "D" mostra todos
- Website: Clique "Exportar CSV"

---

## 🔌 Comandos Serial Úteis

```
R - Reset mín/máx
S - Mostrar estatísticas
C - Reconectar WiFi
T - Testar sensores
D - Mostrar todos os logs
E - Estatísticas do experimento
L - Limpar dados
```

**Usar em Serial Monitor:**
```
1. Abra Tools → Serial Monitor (115200 baud)
2. Digite "R" e pressione Enter
3. Aparece confirmação
```

---

## 🌐 Endpoints API

Disponíveis no ESP32 em `http://192.168.1.X:`

### GET /
```
Retorna: HTML do dashboard
Tipo: text/html
```

### GET /api/dados
```
Retorna: JSON com leitura atual
Tipo: application/json

{
  "metano": 45.5,
  "temperatura": 28.3,
  "pH": 7.2,
  "pressao": 1013.25,
  "timestamp": "12:34:56",
  "wifi": true
}
```

### GET /api/stats
```
Retorna: JSON com mín/máx
Tipo: application/json

{
  "metano": {"min": 10, "max": 150},
  "temperatura": {"min": 26, "max": 32},
  "pH": {"min": 6.8, "max": 7.5},
  "pressao": {"min": 1010, "max": 1020}
}
```

### GET /api/logs
```
Retorna: CSV com histórico completo
Tipo: text/csv
Tamanho: ~50-100 KB
```

---

## ⚙️ Calibração dos Sensores

### MQ-4 (Metano)
```
Fórmula: ppm = (voltagem - 0.9) × 200

Teste:
- Ar limpo: ~0-10 ppm
- Perto de gás: ~50-100 ppm
- Gás concentrado: >500 ppm (ALERTA)
```

### DS18B20
```
Lê em Celsius diretamente
Range: -55°C a +125°C
Precisão: ±0.5°C
```

### pH4502C
```
Fórmula: pH = 7 + (2.0 - voltagem) / 0.6

Teste:
- pH 7.0 (neutro): ~2.0V
- pH 6.0 (ácido): ~2.6V
- pH 8.0 (básico): ~1.4V
```

### Pressão
```
Fórmula: kPa = (ADC / 4095) × 1200

Teste:
- Pressão atmosférica: ~101.3 kPa
- Com pressão: 100-1200 kPa
```

---

## 🎓 Exemplos de Uso

### Exemplo 1: Acompanhar Experimento de 10 Dias

```
1. Inicialize ESP32
2. Deixe rodando 10 dias
3. Verifique Serial à noite (dados salvos)
4. Acesse website: http://192.168.1.X
5. Veja evolução dos sensores
6. Exporte CSV ao final
7. Analise em Excel
```

### Exemplo 2: Comparar Dois Períodos

```
1. Acesse website
2. Filtro 1: Data 01 a 05 de junho
3. Exporta: dados_periodo1.csv
4. Filtro 2: Data 06 a 10 de junho
5. Exporta: dados_periodo2.csv
6. Compara em Excel (gráfico)
```

### Exemplo 3: Detectar Anomalia

```
1. Website mostra ALERTA em Serial
2. Clique no gráfico no período
3. Identifica horário exato
4. Verifica causa (temperatura alta?)
5. Toma ação corretiva
```

---

## ❌ Problemas Comuns e Soluções

| Problema | Causa | Solução |
|----------|-------|---------|
| OLED não mostra dados | I2C desconectado | Verifique pinos 21/22 |
| DS18B20 retorna -127 | Sensor desconectado | Verifique GPIO 4 e pull-up |
| WiFi não conecta | SSID/senha errada | Verifique credenciais no código |
| Dados não salvam | SPIFFS cheio | Comando "L" para limpar |
| Website offline | ESP32 reiniciou | Reinicie também ESP32 |
| Gráfico vazio | Sem dados histórico | Espere 1 hora de logging |

---

## 📱 Acessar pelo Celular

### WiFi Local (Mesma rede)
```
1. Conecte celular à mesma WiFi
2. Navegador: http://192.168.1.X
3. Vê dados em tempo real
4. Funciona no mesmo cômodo
```

### WiFi Remoto (Fora de casa)
```
1. Use Vercel conforme guia
2. Navegador: https://seu-dominio.com
3. Acessa de qualquer lugar
4. Funciona com dados móveis
```

### App Mobile (Futuro)
```
Pode criar com:
- React Native
- Flutter
- PWA (Progressive Web App)
```

---

## 🔐 Dicas de Segurança

```
✅ Use HTTPS (Vercel = automático)
✅ Autentique acesso (basicAuth)
✅ Valide dados recebidos
✅ Não exponha credenciais
✅ Backup dos dados regularmente
✅ Monitor de anomalias
```

---

## 📈 Próximas Melhorias

```
[ ] Notificações por email
[ ] Múltiplos biodigestores
[ ] Gráficos mais avançados
[ ] Integração com IA
[ ] Mobile app nativa
[ ] Banco de dados real
[ ] Cache de dados
[ ] Relatórios em PDF
```

---

## 🎓 Recursos Educacionais

| Tema | Link |
|------|------|
| ESP32 | https://randomnerdtutorials.com/ |
| Wokwi | https://docs.wokwi.com/ |
| Arduino | https://www.arduino.cc/ |
| Node.js | https://nodejs.org/ |
| HTML/CSS/JS | https://www.w3schools.com/ |

---

## 📞 Suporte

**Problemas com Código?**
- [ ] Verifique Serial Monitor (115200 baud)
- [ ] Teste cada sensor individualmente
- [ ] Verifique pinagem conforme CONEXAO_PINAGEM.md
- [ ] Releia este documento

**Problemas com Website?**
- [ ] Verifique conexão WiFi
- [ ] Verifique URL correta (http://IP ou https://dominio)
- [ ] Limpe cache do navegador (Ctrl+Shift+Del)
- [ ] Teste em outro navegador

**Problemas com Wokwi?**
- [ ] Verifique bibliotecas instaladas
- [ ] Releia TUTORIAL_WOKWI.md
- [ ] Teste código no Arduino IDE antes

---

## ✅ Checklist Final

- [ ] Código compilado sem erros
- [ ] Simulação Wokwi funcionando
- [ ] Hardware montado corretamente
- [ ] WiFi conectada
- [ ] OLED mostra dados
- [ ] Serial mostra leituras
- [ ] Website acessível (local)
- [ ] API respondendo
- [ ] Filtros funcionam
- [ ] Exportação CSV funciona
- [ ] Dados salvando em arquivo
- [ ] Website online (Vercel)
- [ ] Acesso remoto funcionando
- [ ] Documentação revisada

---

## 📊 Dados do Projeto

| Métrica | Valor |
|---------|-------|
| Versão | 3.0.0 |
| Data | 12 de Junho de 2026 |
| Sensores | 5 (MQ-4, DS18B20, pH, Pressão, OLED) |
| Linhas de Código | ~1000 |
| Documentação | 4 arquivos |
| Endpoints API | 4 |
| Capacidade Storage | 10+ dias |
| Tempo Setup | 2-3 horas |

---

## 🎉 Conclusão

Você agora tem um **sistema completo e profissional** de monitoramento de biodigestor!

**O que foi entregue:**
✅ Firmware ESP32 completo e documentado
✅ Tutorial do simulador Wokwi
✅ Website com dashboard e filtros
✅ Guia de deploy online
✅ Calibração de sensores
✅ Troubleshooting

**Próximos passos:**
1. Simule no Wokwi
2. Compre componentes
3. Monte hardware
4. Programe ESP32
5. Execute experimento
6. Analise dados
7. Compartilhe resultados

---

**Criado por:** GitHub Copilot  
**Versão:** 3.0.0  
**Status:** Produção ✅

