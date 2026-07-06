# 🚀 Como Usar server.js com Vercel

## O Que É server.js?

`server.js` é um **servidor Node.js** que:
- Recebe dados do ESP32
- Armazena histórico
- Fornece API para o website
- Hospeda o dashboard HTML

---

## ⚡ Quick Start (15 minutos)

### 1. Instalar Node.js

```bash
# Acesse: https://nodejs.org
# Baixe LTS (versão estável)
# Instale normalmente

# Verifique instalação:
node --version    # Deve mostrar v18.x.x ou superior
npm --version     # Deve mostrar 9.x.x ou superior
```

### 2. Preparar Arquivos

```bash
# Crie pasta:
C:\biodigestor-dashboard

# Copie para dentro:
- server.js
- package.json
- dashboard_biodigestor.html
- vercel.json
- .gitignore
```

### 3. Instalar Dependências

```bash
# Abra Terminal/CMD na pasta
cd C:\biodigestor-dashboard

# Digite:
npm install

# Aguarde 1-2 minutos
# Deve criar pasta "node_modules"
```

### 4. Testar Local

```bash
# Na mesma pasta, digite:
npm start

# Deve aparecer:
╔════════════════════════════════════════╗
║  Servidor Biodigestor v3.0             ║
║  Rodando em http://localhost:3000      ║
╚════════════════════════════════════════╝

# Abra navegador:
http://localhost:3000

# Vê dashboard com dados de teste!
```

### 5. Deploy no Vercel

```bash
# Instale Vercel CLI:
npm install -g vercel

# Na pasta do projeto:
vercel

# Siga instruções:
1. Acesse link do navegador
2. Conecte GitHub (ou crie conta)
3. Autorize Vercel
4. Volta pro terminal
5. Escolha configurações
6. Deploy!

# Resultado:
✅ https://biodigestor-dashboard.vercel.app
```

---

## 🔧 Estrutura de Pastas

```
biodigestor-dashboard/
│
├── server.js                    ← Servidor principal
├── package.json                 ← Dependências
├── vercel.json                  ← Config Vercel
├── .gitignore                   ← Arquivos ignorados
├── dashboard_biodigestor.html   ← Website
│
├── node_modules/                ← Instalado automaticamente
└── .vercel/                     ← Criado pelo Vercel
```

---

## 📡 Conectar ESP32 com server.js

### Opção 1: Local (Mesma WiFi)

**No código ESP32, mude:**

```cpp
// Antes:
const char* API_URL = "http://192.168.1.100";

// Depois (IP do seu computador):
const char* API_URL = "http://192.168.1.50";  // Seu IP local
```

**Para descobrir seu IP:**

```bash
# Windows (CMD):
ipconfig
# Procure: IPv4 Address: 192.168.1.X

# Mac/Linux:
ifconfig
# Procure: inet 192.168.1.X
```

### Opção 2: Online (Vercel)

**No código ESP32, mude:**

```cpp
// Antes:
const char* API_URL = "http://192.168.1.100";

// Depois:
const char* API_URL = "https://biodigestor-dashboard.vercel.app";
```

---

## 🧪 Testar Endpoints

### Teste 1: Dados Atuais

```bash
# No navegador, acesse:
http://localhost:3000/api/dados

# Deve retornar JSON como:
{
  "metano": 45.5,
  "temperatura": 28.3,
  "pH": 7.2,
  "pressao": 1013.25,
  "timestamp": "12/06/2026 14:30:45",
  "wifi": true
}
```

### Teste 2: Estatísticas

```bash
# No navegador:
http://localhost:3000/api/stats

# Retorna min/max/média
```

### Teste 3: Histórico CSV

```bash
# No navegador:
http://localhost:3000/api/logs

# Baixa arquivo CSV
```

### Teste 4: Enviar Dados (Avançado)

```bash
# Usando curl (Terminal):
curl -X POST http://localhost:3000/api/dados \
  -H "Content-Type: application/json" \
  -d '{
    "metano": 50.5,
    "temperatura": 29.1,
    "pH": 7.3,
    "pressao": 1014.5,
    "wifi": true
  }'

# Deve retornar: {"status":"ok","mensagem":"Dados recebidos"}
```

---

## 🐛 Troubleshooting

### Erro: "Cannot find module express"

**Solução:**
```bash
npm install express cors
```

### Erro: "Port 3000 already in use"

**Solução:**
```bash
# Windows:
netstat -ano | findstr :3000
taskkill /PID XXXX /F

# Mac/Linux:
lsof -ti:3000 | xargs kill -9
```

### Erro: "GitHub não autorizado"

**Solução:**
```bash
vercel logout
vercel login
vercel deploy
```

### Dashboard não atualiza

**Verificar:**
1. Browser console (F12)
2. ESP32 está conectada?
3. URL correta no código?
4. Server rodando? (npm start)

---

## 📊 Arquivos Gerados

Depois que server.js roda, você verá:

### Em Memória (Enquanto rodando)
```
- dadosAtual: Última leitura
- historico[]: Array com histórico
```

### Em Vercel (Produção)
```
- Dados persistem durante sessão
- Reinicia se redesployer
- Para produção: use banco de dados
```

---

## 🔄 Desenvolvimento Contínuo

### Modo Desenvolvimento (Auto-reload)

```bash
# Instale nodemon:
npm install -D nodemon

# No package.json, mude script start:
"start": "nodemon server.js"

# Quando alterar código, reinicia automaticamente
```

### Fazer Alterações no Code

**Exemplo: Alterar limite de dados**

```javascript
// Em server.js, linha ~80:

// Antes:
if (historico.length > 10000) {
  historico.shift();
}

// Depois:
if (historico.length > 50000) {  // Maior limite
  historico.shift();
}

// Salve e server reinicia automaticamente (se nodemon)
```

---

## 🌍 Domínio Personalizado

### Adicionar Domínio ao Vercel

```bash
1. Acesse: vercel.com/dashboard
2. Selecione projeto
3. Settings → Domains
4. Digite seu domínio
5. Siga instruções de DNS
6. Aguarde propagação (24h)

# Resultado:
✅ https://seudominio.com
```

### Usar Domínio Gratuito

```bash
# Freenom: freenom.com
# Registre: seudominio.tk

# No Vercel, adicione esse domínio
# Pronto!
```

---

## 💾 Backup de Dados

### Exportar Histórico

```bash
# Acesse:
http://localhost:3000/api/logs

# Salva arquivo CSV com todos os dados
# Pode importar em Excel
```

### Restaurar Histórico

Para restaurar dados em nova instância, seria necessário:

1. Modificar `/api/dados` POST para aceitar histórico
2. Adicionar função de restauração
3. Reconectar dados no array

(Recomendado: use banco de dados para isto)

---

## 🔐 Segurança em Produção

### Adicionar Autenticação

```javascript
// server.js - linha 10:

const basicAuth = require('express-basic-auth');

app.use(basicAuth({
  users: { 'admin': 'sua_senha_forte_aqui' }
}));
```

### Usar Variáveis de Ambiente

```javascript
// Crie arquivo: .env
ADMIN_PASSWORD=sua_senha_secreta

// Em server.js:
const password = process.env.ADMIN_PASSWORD;
```

### HTTPS Automático

```
✅ Vercel: Automático (sem configurar)
✅ Railway: Automático
✅ Local: Use certificado autoassinado
```

---

## 📈 Escalar para Produção

### Adicionar Banco de Dados Real

```bash
# Railway oferece PostgreSQL grátis
# 1. Crie conta: railway.app
# 2. Conecte GitHub
# 3. Adicione PostgreSQL
# 4. Obtenha connection string

# Em server.js:
const { Pool } = require('pg');
const pool = new Pool({
  connectionString: process.env.DATABASE_URL
});

// Agora dados persistem indefinidamente
```

---

## 🎯 Próximas Etapas

### Fase 1: Local (Hoje)
```
✓ npm install
✓ npm start
✓ Teste em localhost:3000
```

### Fase 2: Deploy (Esta semana)
```
✓ Criar GitHub
✓ Git push
✓ Vercel deploy
✓ Acesso remoto
```

### Fase 3: Produção (Este mês)
```
✓ Banco de dados
✓ Domínio personalizado
✓ Autenticação
✓ Backup automático
```

---

## 📚 Recursos Úteis

| Recurso | Link |
|---------|------|
| Node.js | https://nodejs.org |
| Express | https://expressjs.com |
| Vercel | https://vercel.com |
| NPM | https://www.npmjs.com |
| Railway | https://railway.app |

---

## ✅ Checklist - server.js

- [ ] Node.js instalado
- [ ] npm install executado
- [ ] npm start funciona
- [ ] localhost:3000 mostra dashboard
- [ ] /api/dados retorna JSON
- [ ] /api/logs baixa CSV
- [ ] Vercel CLI instalada
- [ ] GitHub conectado
- [ ] Vercel deploy bem-sucedido
- [ ] URL online funciona
- [ ] ESP32 enviando dados
- [ ] Website mostra dados em tempo real
- [ ] Filtros funcionam
- [ ] Exportação CSV funciona

---

## 🆘 Precisa de Ajuda?

```
1. Verifique console Node.js
2. Procure erro no Google
3. Leia comentários no código
4. Teste cada endpoint isolado
5. Pergunte em comunidades Node.js
```

---

## 🎉 Pronto!

Você agora sabe como:
✅ Instalar server.js
✅ Testar local
✅ Deploy no Vercel
✅ Conectar ESP32
✅ Escalar para produção

---

**Versão:** 3.0.0  
**Data:** 12 de Junho de 2026  
**Status:** Completo ✅
