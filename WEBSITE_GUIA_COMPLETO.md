# Guia Completo - Website com Dados em Tempo Real

## 📱 O que Você Terá

Um **dashboard web completo** que mostra:
- ✅ Dados em tempo real dos sensores
- ✅ Histórico com filtros de data/hora
- ✅ Gráficos interativos
- ✅ Exportar dados como CSV
- ✅ Atualização automática a cada 10 segundos

---

## 🚀 Opção 1: Acessar Direto do ESP32 (Mais Simples)

### Requisitos
```
- ESP32 conectada à WiFi
- Código v3.0.0 carregado
- Computador e ESP32 na mesma rede
```

### Passo 1: Descobrir IP do ESP32

```
1. Serial Monitor mostra:
   ✓ WiFi OK - IP: 192.168.1.100
   
2. Anote este IP (seu pode ser diferente)
```

### Passo 2: Acessar Dashboard

```
1. Abra navegador (Chrome, Firefox, Edge)
2. Digite na barra de endereço:
   http://192.168.1.100

3. Pronto! Dashboard aparece
4. Dados atualizam a cada 10 segundos
```

### Vantagens ✅
- Sem servidor externo necessário
- Funcionamento imediato
- Gratuito

### Desvantagens ❌
- Só funciona na mesma rede WiFi
- Não acessa de fora de casa
- Se reiniciar ESP32, IP pode mudar

---

## 🌍 Opção 2: Hospedar em Servidor Online (Recomendado)

### Plataformas Recomendadas

#### A) Vercel (Gratuito - Melhor para Frontend)

**Vantagens:**
- Gratuito
- Hospeda HTML/CSS/JS
- Deploy automático
- Domínio personalizado

**Desvantagens:**
- Precisa de backend separado para dados
- Sem banco de dados integrado

#### B) Railway (Gratuito - Com Backend)

**Vantagens:**
- Gratuito (primeiros $5/mês)
- Suporta Node.js, Python
- Banco de dados PostgreSQL
- Deploy fácil

**Desvantagens:**
- Precisa GitHub
- Limite de tempo de execução

#### C) Render (Gratuito - Com Backend)

**Vantagens:**
- Gratuito
- Suporta múltiplas linguagens
- PostgreSQL gratuito
- Deploy contínuo

#### D) Heroku (Pago - Mais confiável)

**Vantagens:**
- Confiável
- Escalável
- Suporte profissional
- PostgreSQL integrado

**Desvantagens:**
- Começa em $7/mês
- Menos recursos na versão gratuita

---

## 📤 Setup Completo (Opção Recomendada: Vercel + Node.js)

### Passo 1: Criar Conta no Vercel

```
1. Acesse: https://vercel.com
2. Clique "Sign Up"
3. Use GitHub (recomendado)
4. Autorize Vercel
```

### Passo 2: Criar Repositório GitHub

```
1. Acesse: https://github.com/new
2. Nome: biodigestor-dashboard
3. Descrição: Dashboard para biodigestor
4. Público (para usar Vercel grátis)
5. Criar repositório
```

### Passo 3: Preparar Arquivos

```
Estrutura do projeto:
biodigestor-dashboard/
├── public/
│   └── index.html          (Dashboard)
├── server.js               (Backend Node.js)
├── package.json            (Dependências)
└── vercel.json             (Config Vercel)
```

### Passo 4: Criar Backend Node.js

**Arquivo: server.js**

```javascript
const express = require('express');
const cors = require('cors');
const app = express();

// Dados em memória (usar banco de dados em produção)
let dadosAtual = {
  metano: 0,
  temperatura: 28.5,
  pH: 7.2,
  pressao: 1013.25,
  timestamp: new Date().toLocaleTimeString(),
  wifi: true
};

let historico = [];

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// API: Dados atuais
app.get('/api/dados', (req, res) => {
  res.json(dadosAtual);
});

// API: Estatísticas
app.get('/api/stats', (req, res) => {
  if (historico.length === 0) {
    return res.json({
      metano: { min: 0, max: 0 },
      temperatura: { min: 0, max: 0 },
      pH: { min: 0, max: 0 },
      pressao: { min: 0, max: 0 }
    });
  }

  const metanos = historico.map(d => parseFloat(d[2]));
  const temps = historico.map(d => parseFloat(d[3]));
  const phs = historico.map(d => parseFloat(d[4]));
  const press = historico.map(d => parseFloat(d[5]));

  res.json({
    metano: { min: Math.min(...metanos), max: Math.max(...metanos) },
    temperatura: { min: Math.min(...temps), max: Math.max(...temps) },
    pH: { min: Math.min(...phs), max: Math.max(...phs) },
    pressao: { min: Math.min(...press), max: Math.max(...press) }
  });
});

// API: Histórico
app.get('/api/logs', (req, res) => {
  let csv = 'Nº,Data/Hora,Metano,Temperatura,pH,Pressao,WiFi\n';
  historico.forEach(d => {
    csv += d.join(',') + '\n';
  });
  res.header('Content-Type', 'text/csv');
  res.send(csv);
});

// API: Receber dados do ESP32
app.post('/api/dados', (req, res) => {
  const { metano, temperatura, pH, pressao, wifi } = req.body;

  dadosAtual = {
    metano,
    temperatura,
    pH,
    pressao,
    timestamp: new Date().toLocaleString('pt-BR'),
    wifi
  };

  // Adiciona ao histórico
  historico.push([
    historico.length + 1,
    dadosAtual.timestamp,
    metano,
    temperatura,
    pH,
    pressao,
    wifi ? 'OK' : 'OFFLINE'
  ]);

  // Mantém apenas últimas 10000 leituras
  if (historico.length > 10000) {
    historico.shift();
  }

  res.json({ status: 'ok' });
});

// Iniciar servidor
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Servidor rodando em http://localhost:${PORT}`);
});
```

### Passo 5: Criar package.json

```json
{
  "name": "biodigestor-dashboard",
  "version": "1.0.0",
  "description": "Dashboard para monitoramento de biodigestor",
  "main": "server.js",
  "scripts": {
    "start": "node server.js",
    "dev": "nodemon server.js"
  },
  "dependencies": {
    "express": "^4.18.2",
    "cors": "^2.8.5"
  },
  "engines": {
    "node": "18.x"
  }
}
```

### Passo 6: Criar vercel.json

```json
{
  "version": 2,
  "builds": [
    { "src": "server.js", "use": "@vercel/node" }
  ],
  "routes": [
    { "src": "/(.*)", "dest": "server.js" }
  ]
}
```

### Passo 7: Deploy no Vercel

```
1. No GitHub, faça commit e push
2. Acesse Vercel
3. Clique "Add New..."
4. Selecione "Project"
5. Selecione seu repositório
6. Vercel detecta Node.js
7. Clique "Deploy"
8. Aguarde ~2 minutos
9. Recebe URL: biodigestor-dashboard.vercel.app
```

---

## 🔌 Conectar ESP32 com Website

### Atualizar Código ESP32

No código v3.0.0, altere a URL:

```cpp
// Antes (local):
const char* API_URL = "http://192.168.1.100";

// Depois (online):
const char* API_URL = "https://biodigestor-dashboard.vercel.app";
```

### Adicionar Função para Enviar Dados

```cpp
void enviarDadosParaServidor() {
  if (!wifiConnected) return;

  WiFiClient client;
  String url = "/api/dados";
  
  String json = "{";
  json += "\"metano\":" + String(metano, 2) + ",";
  json += "\"temperatura\":" + String(temperatura, 2) + ",";
  json += "\"pH\":" + String(pH, 2) + ",";
  json += "\"pressao\":" + String(pressao, 2) + ",";
  json += "\"wifi\":true";
  json += "}";

  if (client.connect("biodigestor-dashboard.vercel.app", 443)) {
    client.print("POST " + url + " HTTP/1.1\r\n");
    client.print("Host: biodigestor-dashboard.vercel.app\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(json.length()) + "\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(json);

    delay(1000);
    client.stop();
  }
}

// Chamar no loop a cada hora:
if (timeNow - lastServerUpdate >= 3600000) {
  lastServerUpdate = timeNow;
  enviarDadosParaServidor();
}
```

---

## 📊 Usar Banco de Dados (PostgreSQL)

### Opção: Railway com PostgreSQL

**Passo 1: Criar Conta Railway**

```
1. Acesse: https://railway.app
2. Clique "Dashboard"
3. Conecte GitHub
```

**Passo 2: Criar PostgreSQL**

```
1. Clique "New Project"
2. Selecione "Provision PostgreSQL"
3. Railway cria automaticamente
```

**Passo 3: Conectar ao Node.js**

```javascript
const { Pool } = require('pg');

const pool = new Pool({
  connectionString: process.env.DATABASE_URL
});

// Criar tabela
pool.query(`
  CREATE TABLE IF NOT EXISTS leituras (
    id SERIAL PRIMARY KEY,
    data_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    metano FLOAT,
    temperatura FLOAT,
    pH FLOAT,
    pressao FLOAT,
    wifi BOOLEAN
  )
`);

// Salvar leitura
async function salvarLeitura(metano, temperatura, pH, pressao, wifi) {
  await pool.query(
    'INSERT INTO leituras (metano, temperatura, pH, pressao, wifi) VALUES ($1, $2, $3, $4, $5)',
    [metano, temperatura, pH, pressao, wifi]
  );
}
```

---

## 🌐 Domínio Personalizado

### Usar Domínio Próprio (Gratuito)

```
1. Registre domínio em: Namecheap, GoDaddy, etc.
2. No Vercel, acesse "Domains"
3. Clique "Add Domain"
4. Digite seu domínio
5. Vercel mostra nameservers
6. Configure no provedor do domínio
7. Aguarde ~24h propagação
```

### Domínios Gratuitos

```
- .tk (Freenom - gratuito 1 ano)
- .ml (Freenom)
- .ga (Freenom)
```

---

## 🔒 Segurança (Importante!)

### HTTPS Automático

```
✅ Vercel: Automático com SSL
✅ Railway: Automático com SSL
✅ Render: Automático com SSL
```

### Autenticação Básica

Adicione no Node.js:

```javascript
const basicAuth = require('express-basic-auth');

app.use(basicAuth({
  users: { 'admin': 'sua_senha_forte' }
}));
```

### Validação de Dados

```javascript
app.post('/api/dados', (req, res) => {
  const { metano, temperatura, pH, pressao } = req.body;

  if (metano < 0 || metano > 10000) {
    return res.status(400).json({ erro: 'Metano inválido' });
  }

  if (temperatura < -40 || temperatura > 125) {
    return res.status(400).json({ erro: 'Temperatura inválida' });
  }

  // ... mais validações ...

  res.json({ status: 'ok' });
});
```

---

## 📱 Acessar Pelo Celular

### WiFi Local

```
Mesmo que computador:
1. Celular conectado à mesma WiFi
2. Abra navegador
3. Digite: http://192.168.1.100
```

### De Fora de Casa

```
Se usar servidor online (Vercel):
1. Abra navegador
2. Digite: https://biodigestor-dashboard.vercel.app
3. Acessa de qualquer lugar
4. Qualquer dispositivo
```

---

## 🔧 Troubleshooting

### Problema: "Não consigo acessar o dashboard"

**Solução:**
1. Verifique se ESP32 está ligada
2. Verifique IP correto com serial monitor
3. ESP32 e computador mesma rede WiFi
4. Firewall pode estar bloqueando

### Problema: "Dados não atualizam"

**Solução:**
1. Verifique WiFi conectada
2. Serial monitor mostra envio de dados?
3. Se offline, desabilite CORS no navegador
4. Recarregue página (F5)

### Problema: Servidor offline

**Solução:**
1. Vercel: Verificar logs (vercel.com/dashboard)
2. Railway: Verificar status da aplicação
3. Reiniciar aplicação
4. Verificar se ultrapassou limite gratuito

---

## 📈 Próximos Passos

### Adicionar Mais Funcionalidades

```
1. Autenticação de usuário
2. Múltiplos biodigestores
3. Notificações por email
4. Gráficos mais avançados
5. Relatórios em PDF
6. API para integrar com outros sistemas
```

### Melhorar Performance

```
1. Cache de dados
2. Compressão GZIP
3. CDN para arquivos estáticos
4. Paginação de dados
```

### Mobile App

```
1. React Native
2. Flutter
3. PWA (Progressive Web App)
```

---

## 📚 Recursos Úteis

| Recurso | Link |
|---------|------|
| Vercel | https://vercel.com |
| Railway | https://railway.app |
| Render | https://render.com |
| Node.js | https://nodejs.org |
| Express | https://expressjs.com |
| Chart.js | https://www.chartjs.org |

---

## ✅ Checklist - Setup Completo

- [ ] Dashboard HTML criado
- [ ] Acessar local (192.168.1.x)
- [ ] Dados aparecem em tempo real
- [ ] Filtros funcionando
- [ ] Exportar CSV funciona
- [ ] GitHub preparado
- [ ] Vercel deploy feito
- [ ] API respondendo
- [ ] ESP32 enviando dados
- [ ] Domínio personalizado (opcional)
- [ ] HTTPS funcionando
- [ ] Acessar pelo celular

---

**Versão:** 3.0.0  
**Data:** 12 de Junho de 2026  
**Status:** Produção
