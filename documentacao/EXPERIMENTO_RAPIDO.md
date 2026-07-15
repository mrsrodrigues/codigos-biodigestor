# Guia Rápido - Sistema de Logging para Experimento de 10 Dias

## ⚡ Setup em 3 Minutos

### 1️⃣ Carregue o Novo Código

```
Substitua: biodigestor_esp32.ino
Por: biodigestor_esp32_logging.ino
```

**Mudanças principais:**
- Novo arquivo v2.0.0
- Sistema de logging automático (1 hora)
- 6 novos comandos
- Armazenamento em SPIFFS

### 2️⃣ Configure e Carregue

```
1. Abra biodigestor_esp32_logging.ino
2. Altere WiFi (linhas 28-29)
3. Verify → Upload
4. Abra Serial Monitor (115200 bps)
```

### 3️⃣ Inicie o Experimento

```
Serial Monitor mostra:
✓ SPIFFS inicializado
✓ Sensores conectados
✓ Arquivo criado com cabeçalho
```

**Pronto! Sistema está gravando automaticamente a cada hora.**

---

## 🎯 Comandos para o Experimento

### Monitorar Progresso (Diariamente)

```
Comando: E
Enter

Mostra:
- Temperatura (mín, máx, média)
- Pressão média
- pH médio
- Total de horas decorridas
```

### Ver Todos os Dados Coletados

```
Comando: D
Enter

Mostra:
- Todas as 240+ linhas
- Formato CSV
- Pronto para copiar-colar
```

### Limpar para Novo Experimento

```
Comando: L
Enter

⚠️  Confirme com Y
Antes disso, use D para salvar os dados!
```

---

## 📊 Cronograma Automático

```
Inicialização:
  ↓
  [Arquivo criado com cabeçalho]
  
Loop contínuo:
  ├─ A cada 10 segundos → Lê sensores e Serial
  └─ A cada 1 HORA → Grava no arquivo CSV
  
Resultado:
  ├─ Leitura #1 às 10:00
  ├─ Leitura #2 às 11:00
  ├─ Leitura #3 às 12:00
  └─ ... até 240+ leituras
```

---

## 🔍 Verificar se Está Funcionando

### Teste 1: Primeiro Log (Imediato)

```
Depois de 10 segundos no Serial:
✓ Dados gravados - Entrada #1 em 12/06/2026 10:00:00
```

### Teste 2: Segundo Log (Aguarde 1 hora ou teste)

```
1. Digite: L (limpa dados)
2. Confirma com: Y
3. Modifique: #define LOGGING_INTERVAL 10000
   (Isso faz gravar a cada 10 segundos para teste)
4. Recarregue código
5. Aguarde 30 segundos
6. Digite: D (vê dados)
7. Deve mostrar 3 entradas
```

---

## 💾 Exportar Dados para Excel

### Passo 1: Coletar Dados

```
No Serial Monitor:
Comando: D
Enter

Copia todo o conteúdo exibido
```

### Passo 2: Colar em Arquivo

```
1. Abra Bloco de Notas
2. Cole os dados
3. Salve como: dados_biodigestor.csv
```

### Passo 3: Abrir no Excel

```
1. Abra Excel
2. Arquivo → Abrir
3. Selecione dados_biodigestor.csv
4. Excel reconhece automaticamente as colunas
5. Crie gráficos
```

---

## 📈 Gráficos Automáticos no Excel

### Gráfico 1: Temperatura

```
Selecione:
  - Coluna: DATA_HORA (X)
  - Coluna: TEMP_C (Y)

Tipo: Gráfico de Linha
Resultado: Visualiza evolução térmica
```

### Gráfico 2: Pressão

```
Selecione:
  - Coluna: DATA_HORA (X)
  - Coluna: PRESSAO_HPA (Y)

Tipo: Gráfico de Linha
Resultado: Produção de biogás
```

### Gráfico 3: Todos Juntos

```
Selecione:
  - DATA_HORA
  - TEMP_C
  - PRESSAO_HPA (conversão de escala)
  - PH

Tipo: Gráfico com múltiplos eixos
Resultado: Comparação visual
```

---

## ⚙️ Personalizações

### Mudar Intervalo de Gravação

Se quer gravar a cada **30 minutos** em vez de 1 hora:

```cpp
// Linha ~53
#define LOGGING_INTERVAL 1800000  // 30 minutos

// Para 2 horas:
#define LOGGING_INTERVAL 7200000
```

### Aumentar Limite de Entradas

Se quer até **20 dias** em vez de 12.5:

```cpp
// Linha ~54
#define MAX_ENTRIES 480  // 480 × 1 hora = 20 dias

// Para 30 dias:
#define MAX_ENTRIES 720
```

---

## 🔧 Troubleshooting Rápido

| Problema | Solução |
|----------|---------|
| Dados não gravam | Verifique SPIFFS com 'D' |
| Timestamp errado | Conecte WiFi com 'C' |
| Sem espaço | Use 'D' para salvar, depois 'L' para limpar |
| Arquivo corrompido | Use 'L' e inicie novo experimento |

---

## 📝 Checklist do Experimento

```
ANTES DE INICIAR:
  ☐ Código v2.0.0 carregado
  ☐ WiFi configurado
  ☐ Sensores calibrados
  ☐ ESP32 conectada

INÍCIAR EXPERIMENTO:
  ☐ Conecte alimentação
  ☐ Abra Serial Monitor
  ☐ Veja "Sistema pronto"
  ☐ Primeira entrada gravada em 1 hora

DURANTE OS 10 DIAS:
  ☐ Digita 'E' todo dia para ver progresso
  ☐ Monitora alertas de temperatura/pH
  ☐ Verifica WiFi ocasionalmente

AO FINAL:
  ☐ Digita 'D' para exportar dados
  ☐ Salva como CSV
  ☐ Importa no Excel
  ☐ Cria gráficos e análise
```

---

## 🎯 Resultado Final Esperado

### Arquivo de Dados

```
dados_biodigestor.csv com ~240 linhas:
- Temperatura cada hora
- Pressão cada hora
- pH cada hora
- Tudo pronto para análise
```

### Gráficos no Excel

```
Visualizações:
✓ Evolução térmica (10 dias)
✓ Produção de biogás (pressão)
✓ Estabilidade de pH
✓ Variação diária
✓ Tendências
```

### Relatório

```
Possível gerar automaticamente:
- Média de temperatura
- Amplitude térmica
- Estabilidade do processo
- Eficiência de produção
- Conclusões do experimento
```

---

## 🚀 Comece Agora!

1. ✅ Carregue o código v2.0.0
2. ✅ Aguarde 1 hora para primeiro log
3. ✅ Monitore com comando 'E' diariamente
4. ✅ Ao final, use 'D' para exportar
5. ✅ Analise em Excel

**Boa sorte com seu experimento! 🎉**

---

**Versão:** 2.0.0  
**Última Atualização:** 12 de Junho de 2026
