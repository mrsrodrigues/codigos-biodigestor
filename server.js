/**
 * Backend Node.js para Biodigestor
 * Use com Vercel ou Railway
 * 
 * Instalação de dependências:
 * npm install express cors
 * 
 * Rodar local:
 * npm start
 * 
 * Vercel deploy:
 * 1. npm install -g vercel
 * 2. vercel
 */

const express = require('express');
const cors = require('cors');
const path = require('path');

// Inicializar app
const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));

// ================================================================
// ARMAZENAMENTO DE DADOS (Em memória)
// Nota: Em produção, use banco de dados como PostgreSQL
// ================================================================

let dadosAtual = {
  metano: 45.5,
  temperatura: 28.3,
  pH: 7.2,
  pressao: 1013.25,
  timestamp: new Date().toLocaleString('pt-BR'),
  wifi: true
};

let historico = [];

// Inicializar com dados de exemplo
for (let i = 0; i < 24; i++) {
  const agora = new Date();
  agora.setHours(agora.getHours() - i);
  
  historico.unshift([
    i + 1,
    agora.toLocaleString('pt-BR'),
    45 + Math.random() * 20,
    28 + Math.sin(i / 5) * 2,
    7.2 + Math.random() * 0.3,
    1013 + Math.random() * 10,
    'OK'
  ]);
}

// ================================================================
// ROTAS DA API
// ================================================================

/**
 * GET /api/dados
 * Retorna dados atuais dos sensores
 */
app.get('/api/dados', (req, res) => {
  res.json(dadosAtual);
});

/**
 * POST /api/dados
 * Recebe dados do ESP32
 */
app.post('/api/dados', (req, res) => {
  try {
    const { metano, temperatura, pH, pressao, wifi } = req.body;

    // Validação básica
    if (metano === undefined || temperatura === undefined || pH === undefined || pressao === undefined) {
      return res.status(400).json({ erro: 'Dados incompletos' });
    }

    if (metano < 0 || metano > 10000) {
      return res.status(400).json({ erro: 'Metano fora do intervalo' });
    }

    if (temperatura < -40 || temperatura > 125) {
      return res.status(400).json({ erro: 'Temperatura fora do intervalo' });
    }

    if (pH < 0 || pH > 14) {
      return res.status(400).json({ erro: 'pH fora do intervalo' });
    }

    if (pressao < 0 || pressao > 1500) {
      return res.status(400).json({ erro: 'Pressão fora do intervalo' });
    }

    // Atualizar dados atuais
    dadosAtual = {
      metano: parseFloat(metano),
      temperatura: parseFloat(temperatura),
      pH: parseFloat(pH),
      pressao: parseFloat(pressao),
      timestamp: new Date().toLocaleString('pt-BR'),
      wifi: wifi !== false
    };

    // Adicionar ao histórico
    historico.push([
      historico.length + 1,
      dadosAtual.timestamp,
      dadosAtual.metano,
      dadosAtual.temperatura,
      dadosAtual.pH,
      dadosAtual.pressao,
      dadosAtual.wifi ? 'OK' : 'OFFLINE'
    ]);

    // Limitar histórico a 10000 entradas (~420 dias)
    if (historico.length > 10000) {
      historico.shift();
    }

    res.json({ status: 'ok', mensagem: 'Dados recebidos' });

  } catch (erro) {
    console.error('Erro ao receber dados:', erro);
    res.status(500).json({ erro: 'Erro interno' });
  }
});

/**
 * GET /api/stats
 * Retorna estatísticas (mín/máx)
 */
app.get('/api/stats', (req, res) => {
  try {
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
      metano: {
        min: Math.min(...metanos),
        max: Math.max(...metanos),
        media: (metanos.reduce((a, b) => a + b) / metanos.length).toFixed(2)
      },
      temperatura: {
        min: Math.min(...temps),
        max: Math.max(...temps),
        media: (temps.reduce((a, b) => a + b) / temps.length).toFixed(2)
      },
      pH: {
        min: Math.min(...phs),
        max: Math.max(...phs),
        media: (phs.reduce((a, b) => a + b) / phs.length).toFixed(2)
      },
      pressao: {
        min: Math.min(...press),
        max: Math.max(...press),
        media: (press.reduce((a, b) => a + b) / press.length).toFixed(2)
      }
    });

  } catch (erro) {
    console.error('Erro ao calcular estatísticas:', erro);
    res.status(500).json({ erro: 'Erro interno' });
  }
});

/**
 * GET /api/logs
 * Retorna histórico em formato CSV
 */
app.get('/api/logs', (req, res) => {
  try {
    let csv = 'Nº,Data/Hora,Metano (ppm),Temperatura (°C),pH,Pressão (kPa),WiFi\n';
    
    historico.forEach(d => {
      csv += `${d[0]},${d[1]},${d[2]},${d[3]},${d[4]},${d[5]},${d[6]}\n`;
    });

    res.header('Content-Type', 'text/csv');
    res.header('Content-Disposition', 'attachment; filename="biodigestor_dados.csv"');
    res.send(csv);

  } catch (erro) {
    console.error('Erro ao gerar CSV:', erro);
    res.status(500).json({ erro: 'Erro interno' });
  }
});

/**
 * GET /api/logs?filtro=data
 * Retorna histórico filtrado
 */
app.get('/api/logs-filtrado', (req, res) => {
  try {
    const { dataInicio, dataFim, horaInicio, horaFim, sensor } = req.query;

    let dados = historico;

    // Filtro de data
    if (dataInicio) {
      dados = dados.filter(d => {
        const data = d[1].split(' ')[0]; // DD/MM/YYYY
        return data >= dataInicio;
      });
    }

    if (dataFim) {
      dados = dados.filter(d => {
        const data = d[1].split(' ')[0];
        return data <= dataFim;
      });
    }

    // Filtro de hora
    if (horaInicio) {
      dados = dados.filter(d => {
        const hora = d[1].split(' ')[1]; // HH:MM:SS
        return hora >= horaInicio;
      });
    }

    if (horaFim) {
      dados = dados.filter(d => {
        const hora = d[1].split(' ')[1];
        return hora <= horaFim;
      });
    }

    // Filtro de sensor (retorna apenas aquela coluna)
    if (sensor && sensor !== '') {
      const csv = 'Data/Hora,Valor\n' + dados.map(d => {
        switch(sensor) {
          case 'metano': return `${d[1]},${d[2]}`;
          case 'temperatura': return `${d[1]},${d[3]}`;
          case 'pH': return `${d[1]},${d[4]}`;
          case 'pressao': return `${d[1]},${d[5]}`;
          default: return '';
        }
      }).join('\n');

      res.header('Content-Type', 'text/csv');
      res.header('Content-Disposition', 'attachment; filename="biodigestor_filtrado.csv"');
      res.send(csv);
    } else {
      // Retorna tudo em CSV
      let csv = 'Nº,Data/Hora,Metano (ppm),Temperatura (°C),pH,Pressão (kPa),WiFi\n';
      dados.forEach(d => {
        csv += `${d[0]},${d[1]},${d[2]},${d[3]},${d[4]},${d[5]},${d[6]}\n`;
      });

      res.header('Content-Type', 'text/csv');
      res.header('Content-Disposition', 'attachment; filename="biodigestor_filtrado.csv"');
      res.send(csv);
    }

  } catch (erro) {
    console.error('Erro ao filtrar dados:', erro);
    res.status(500).json({ erro: 'Erro interno' });
  }
});

/**
 * GET /api/info
 * Informações gerais do servidor
 */
app.get('/api/info', (req, res) => {
  res.json({
    versao: '3.0.0',
    servidor: 'Node.js Express',
    totalLeituras: historico.length,
    ultimaAtualizacao: dadosAtual.timestamp,
    uptime: process.uptime()
  });
});

/**
 * POST /api/reset
 * Limpa histórico (requer confirmação)
 */
app.post('/api/reset', (req, res) => {
  const { confirmacao } = req.body;

  if (confirmacao !== 'DELETAR_TODOS_DADOS') {
    return res.status(400).json({ erro: 'Confirmação incorreta' });
  }

  historico = [];
  res.json({ status: 'ok', mensagem: 'Histórico apagado' });
});

// ================================================================
// ROTAS ESTÁTICAS
// ================================================================

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Health check (para monitoramento)
app.get('/health', (req, res) => {
  res.status(200).json({ status: 'ok' });
});

// 404
app.use((req, res) => {
  res.status(404).json({ erro: 'Rota não encontrada' });
});

// ================================================================
// INICIAR SERVIDOR
// ================================================================

app.listen(PORT, () => {
  console.log(`
╔════════════════════════════════════════╗
║  Servidor Biodigestor v3.0             ║
║  Rodando em http://localhost:${PORT}   ║
║  Pressione Ctrl+C para parar           ║
╚════════════════════════════════════════╝
  `);

  console.log(`
Endpoints disponíveis:
  GET  /                    → Dashboard HTML
  GET  /api/dados          → Dados atuais (JSON)
  POST /api/dados          → Receber dados do ESP32
  GET  /api/stats          → Estatísticas (JSON)
  GET  /api/logs           → Histórico (CSV)
  GET  /api/logs-filtrado  → Histórico filtrado
  GET  /api/info           → Info do servidor
  GET  /health             → Health check
  `);
});

module.exports = app;
