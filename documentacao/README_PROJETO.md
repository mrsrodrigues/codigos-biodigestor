# 🌱 Biodigestor - Dashboard IoT em Tempo Real

Sistema completo de monitoramento de biodigestor com ESP32, sensores IoT, armazenamento em nuvem e dashboard web.

## 🎯 Características

- ✅ **5 Sensores Integrados**: Metano (CH₄), Temperatura, pH, Pressão, Display OLED
- ✅ **Armazenamento**: Dados em arquivo CSV no ESP32 (10+ dias)
- ✅ **Dashboard Web**: Interface bonita e responsiva
- ✅ **API REST**: 4 endpoints para acesso remoto
- ✅ **Filtros Avançados**: Pesquisar por data, hora e sensor
- ✅ **Exportação**: Baixar dados em CSV
- ✅ **Gráficos**: Visualização de evolução
- ✅ **WiFi**: Conectado à internet
- ✅ **Simulador**: Teste no Wokwi antes de montar

---

## 🚀 Começar Rápido (5 minutos)

### 1. Simular no Wokwi (Grátis)

```bash
# Abra em seu navegador
https://wokwi.com

# Siga o tutorial
Veja: TUTORIAL_WOKWI.md
```

### 2. Acessar Dashboard Local

```bash
# Conecte ESP32 à WiFi
# Abra navegador com IP da ESP32
http://192.168.1.100

# Vê dados em tempo real
```

### 3. Hospedar Online (30 min)

```bash
# Veja guia completo
Veja: WEBSITE_GUIA_COMPLETO.md

# Deploy no Vercel
vercel deploy
```

---

## 📦 Arquivos do Projeto

```
biodigestor/
├── biodigestor_esp32_v3_completo.ino    # ⭐ Código ESP32 principal
├── dashboard_biodigestor.html           # 📊 Website
├── server.js                            # 🚀 Backend Node.js
├── package.json                         # 📋 Dependências npm
├── vercel.json                          # ☁️ Config Vercel
├── TUTORIAL_WOKWI.md                    # 🌐 Simulador
├── WEBSITE_GUIA_COMPLETO.md            # 📖 Deploy
├── DOCUMENTACAO_COMPLETA.md            # 📚 Tudo
└── README.md                            # 👋 Este arquivo
```

---

## 🛠️ Hardware Necessário

| Componente | GPIO | Protocolo | Link |
|------------|------|-----------|------|
| ESP32 DevKit | - | - | [AliExpress](https://www.aliexpress.com/) |
| OLED SSD1306 | 21/22 | I2C | [AliExpress](https://www.aliexpress.com/) |
| DS18B20 Temp | 4 | 1-Wire | [Amazon](https://www.amazon.com/) |
| MQ-4 Metano | 34 | ADC | [AliExpress](https://www.aliexpress.com/) |
| pH4502C | 35 | ADC | [AliExpress](https://www.aliexpress.com/) |
| Sensor Pressão | 32 | ADC | [AliExpress](https://www.aliexpress.com/) |

**Custo Total:** ~R$150-200

---

## 💻 Instalação (Hardware)

### Passo 1: Comprar Componentes

```
Tempo: 1 semana (envio)
Custo: ~R$150
```

### Passo 2: Montar Circuito

```bash
# Siga pinagem em CONEXAO_PINAGEM.md
# Conecte cada sensor ao GPIO correspondente
# Verifique com multímetro
Tempo: 1-2 horas
```

### Passo 3: Programar ESP32

```bash
# Arduino IDE
1. Instale ESP32 board
2. Abra biodigestor_esp32_v3_completo.ino
3. Configure WiFi (SSID/Senha)
4. Clique "Upload"
5. Aguarde compilação

Tempo: 5 minutos
```

### Passo 4: Testar

```bash
# Serial Monitor
1. Abra Tools → Serial Monitor (115200)
2. Veja dados dos sensores
3. OLED mostra valores
4. Teste cada sensor

Tempo: 10 minutos
```

---

## 🌐 Desenvolvimento Web

### Opção 1: Local (Sem Internet)

```bash
# Python 3
python -m http.server 8000

# Node.js
npx http-server

# Acesse: http://localhost:8000
```

### Opção 2: Vercel (Recomendado - Gratuito)

```bash
# 1. Crie GitHub
git clone https://github.com/seu-usuario/biodigestor.git
cd biodigestor

# 2. Instale Vercel
npm install -g vercel

# 3. Deploy
vercel

# 4. Acesse: biodigestor-dashboard.vercel.app
```

### Opção 3: Railway (Com Banco de Dados)

```bash
# Veja: WEBSITE_GUIA_COMPLETO.md
# Seção "Banco de Dados PostgreSQL"
```

---

## 📊 API Endpoints

### GET /api/dados
```json
{
  "metano": 45.5,
  "temperatura": 28.3,
  "pH": 7.2,
  "pressao": 1013.25,
  "timestamp": "12/06/2026 14:30:45",
  "wifi": true
}
```

### GET /api/stats
```json
{
  "metano": { "min": 10, "max": 150, "media": "45.2" },
  "temperatura": { "min": 26, "max": 32, "media": "28.5" },
  "pH": { "min": 6.8, "max": 7.5, "media": "7.2" },
  "pressao": { "min": 1010, "max": 1020, "media": "1015" }
}
```

### GET /api/logs
```csv
Nº,Data/Hora,Metano (ppm),Temperatura (°C),pH,Pressão (kPa),WiFi
1,12/06/2026 10:00:00,45.5,28.3,7.2,1013.25,OK
2,12/06/2026 11:00:00,48.2,28.5,7.1,1013.50,OK
```

---

## 🎓 Tutoriais

### Tutorial 1: Começar no Wokwi (30 min)

```bash
1. Acesse: https://wokwi.com
2. Crie novo projeto ESP32
3. Siga: TUTORIAL_WOKWI.md
4. Carregue código
5. Clique Play
```

### Tutorial 2: Montar Hardware (2-3 h)

```bash
1. Compre componentes
2. Siga: CONEXAO_PINAGEM.md
3. Teste cada sensor
4. Verifique OLED
5. Calibre sensores
```

### Tutorial 3: Deploy Online (1 h)

```bash
1. Crie GitHub
2. Faça git push
3. Conecte Vercel
4. Deploy automático
5. Acesse remotamente
```

---

## 🔧 Troubleshooting

### OLED não mostra dados

```
1. Verifique conexão I2C (pinos 21/22)
2. Teste com código de exemplo
3. Mude endereço de 0x3C para 0x3D
```

### DS18B20 retorna -127°C

```
1. Verifique GPIO 4
2. Adicione resistor pull-up 4.7kΩ
3. Teste com cabo mais curto
```

### WiFi não conecta

```
1. Verifique SSID/Senha no código
2. ESP32 na mesma rede
3. Signal WiFi forte
```

### Website mostra "Offline"

```
1. Verifique URL correta
2. Limpe cache (Ctrl+Shift+Del)
3. Teste em outro navegador
4. Verifi ESP32 conectada
```

---

## 📱 Acesso Remoto

### Via WiFi Local

```
http://192.168.1.X
```

### Via Internet (Vercel)

```
https://seu-dominio.vercel.app
```

### Via Celular

```
1. Mesmo WiFi: http://192.168.1.X
2. Fora de casa: https://seu-dominio.vercel.app
```

---

## 🔐 Segurança

- ✅ HTTPS automático (Vercel)
- ✅ Validação de dados
- ✅ Sem credenciais em código
- ✅ Autenticação (opcional)

---

## 📈 Futuro

- [ ] Notificações por email
- [ ] Múltiplos biodigestores
- [ ] Mobile app nativa
- [ ] Banco de dados real
- [ ] Relatórios em PDF
- [ ] Integração com IA

---

## 📚 Documentação Completa

```
DOCUMENTACAO_COMPLETA.md     ← Leia PRIMEIRO
├── Setup ESP32
├── Pinagem
├── Calibração
├── Website
└── Troubleshooting
```

---

## 🤝 Contribuir

```bash
# Fork o projeto
# Crie branch (git checkout -b feature/AmazingFeature)
# Commit (git commit -m 'Add AmazingFeature')
# Push (git push origin feature/AmazingFeature)
# Abra Pull Request
```

---

## 📝 Licença

Este projeto está sob licença MIT. Veja LICENSE.md para detalhes.

---

## 👨‍💻 Autor

**GitHub Copilot** - Assistente de IA
- 📧 Email: suporte@github.com
- 🌐 Website: https://github.com/features/copilot

---

## 🙏 Agradecimentos

- ESP32 Community
- Arduino
- Wokwi Simulador
- Vercel
- Express.js

---

## 📞 Suporte

```
1. Leia DOCUMENTACAO_COMPLETA.md
2. Veja troubleshooting
3. Abra issue no GitHub
4. Pesquise no Google/Stack Overflow
```

---

## 🎉 Começar Agora!

```bash
# 1. Clone projeto
git clone https://github.com/seu-usuario/biodigestor.git

# 2. Instale dependências
cd biodigestor
npm install

# 3. Configure WiFi
Edite: biodigestor_esp32_v3_completo.ino

# 4. Carregue na ESP32
Arduino IDE → Upload

# 5. Acesse dashboard
http://192.168.1.X

# 🎊 Pronto!
```

---

**Versão:** 3.0.0  
**Status:** ✅ Pronto para Produção  
**Último Update:** 12 de Junho de 2026

