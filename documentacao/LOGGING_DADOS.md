# Sistema de Logging e Armazenamento de Dados - Biodigestor ESP32

## 📋 Visão Geral

Este documento descreve o sistema de armazenamento de dados para experimentos longos (até 10+ dias) com o biodigestor ESP32. Todos os dados são salvos em um arquivo CSV dentro da memória Flash (SPIFFS) da placa.

---

## 🎯 Características do Sistema de Logging

### ✅ O que o Sistema Faz

```
✓ Grava 1 leitura a cada hora (automaticamente)
✓ Armazena em arquivo CSV estruturado
✓ Permite até 300 registros (12.5 dias)
✓ Consulta dados gravados
✓ Calcula estatísticas do experimento
✓ Operação em tempo real (10 segundos)
✓ WiFi opcional
```

### 📊 Estrutura dos Dados

O arquivo `dados_biodigestor.csv` armazena:

```
LEITURA,DATA_HORA,TEMP_C,TEMP_MIN,TEMP_MAX,PRESSAO_HPA,PH,WIFI_STATUS
1,12/06/2026 10:00:00,28.5,28.5,28.5,1013.25,7.20,OK
2,12/06/2026 11:00:00,28.3,28.3,28.5,1013.30,7.19,OK
3,12/06/2026 12:00:00,28.7,28.3,28.7,1013.20,7.21,OK
```

---

## 📁 Arquivo de Dados

### Formato CSV

```
Coluna 1: LEITURA
  - Número sequencial (1, 2, 3, ...)
  - Máximo: 300 entradas

Coluna 2: DATA_HORA
  - Formato: DD/MM/YYYY HH:MM:SS
  - Requer sincronização NTP (via WiFi)

Coluna 3: TEMP_C
  - Temperatura em °C
  - Precisão: 0.01°C

Coluna 4: TEMP_MIN
  - Temperatura mínima registrada desde última leitura
  - Monitoramento de variação térmica

Coluna 5: TEMP_MAX
  - Temperatura máxima registrada
  - Monitoramento de picos

Coluna 6: PRESSAO_HPA
  - Pressão em hectopascal
  - Precisão: 0.01 hPa

Coluna 7: PH
  - Valor de pH (0-14)
  - Precisão: ±0.5 (após calibração)

Coluna 8: WIFI_STATUS
  - "OK" se WiFi conectado
  - "OFFLINE" se desconectado
```

### Localização do Arquivo

```
Armazenamento: SPIFFS (Flash da ESP32)
Caminho: /dados_biodigestor.csv
Arquivo de marcador: /header_escrito

Tamanho estimado:
  - 300 linhas ≈ 6 KB
  - Espaço disponível: ~1 MB (em geral)
```

---

## ⏰ Cronograma de Leitura

### Leituras Rápidas (10 segundos)

```
┌─────────────────────────────────────────┐
│ Loop Principal a cada 10 segundos      │
├─────────────────────────────────────────┤
│                                         │
│ 1. Lê DHT22 (Temperatura)              │
│ 2. Lê BMP280 (Pressão)                 │
│ 3. Lê Sensor pH                        │
│ 4. Verifica alertas                    │
│ 5. Exibe no Serial Monitor             │
│ 6. Atualiza WiFi se necessário         │
│                                         │
└─────────────────────────────────────────┘
    ↓ (Cada 1 hora)
    
┌─────────────────────────────────────────┐
│ Logging em Arquivo CSV                 │
├─────────────────────────────────────────┤
│                                         │
│ 1. Grava hora/data atual               │
│ 2. Grava valores de todos sensores     │
│ 3. Grava status WiFi                   │
│ 4. Incrementa contador                 │
│ 5. Salva em SPIFFS                     │
│                                         │
└─────────────────────────────────────────┘
```

### Timeline de Experimento de 10 Dias

```
Hora 0:00 → Entrada #1 gravada
Hora 1:00 → Entrada #2 gravada
Hora 2:00 → Entrada #3 gravada
...
Hora 240:00 (10 dias) → Entrada #240 gravada
```

---

## 🔧 Comandos Serial para Logging

### Novo: Consultar Dados

| Comando | Função | Saída |
|---------|--------|-------|
| **D** | Exibe todos os dados | Arquivo completo CSV |
| **E** | Estatísticas do experimento | Média, mín, máx, duração |
| **L** | Limpa dados (novo experimento) | Confirma operação |

### Exemplos de Uso

#### Comando D - Exibir Dados

```
Digite: D
Enter

Resultado:
=================================================
        DADOS ARMAZENADOS NO EXPERIMENTO         
=================================================
LEITURA,DATA_HORA,TEMP_C,TEMP_MIN,TEMP_MAX,PRESSAO_HPA,PH,WIFI_STATUS
1,12/06/2026 10:00:00,28.5,28.5,28.5,1013.25,7.20,OK
2,12/06/2026 11:00:00,28.3,28.3,28.5,1013.30,7.19,OK
3,12/06/2026 12:00:00,28.7,28.3,28.7,1013.20,7.21,OK
4,12/06/2026 13:00:00,28.4,28.3,28.7,1013.25,7.22,OK
=================================================
Total de linhas: 5
=================================================
```

#### Comando E - Estatísticas

```
Digite: E
Enter

Resultado:
=================================================
      ESTATÍSTICAS DO EXPERIMENTO               
=================================================

TEMPERATURA:
  Mínima: 28.30°C
  Máxima: 28.70°C
  Média: 28.48°C
  Amplitude: 0.40°C

PRESSÃO:
  Média: 1013.25 hPa

pH:
  Média: 7.21

Total de Leituras: 4
Duração Estimada: 3 horas
Que corresponde a: 0 dias e 3 horas
=================================================
```

#### Comando L - Limpar Dados

```
Digite: L
Enter

Resultado:
⚠️  Tem certeza que deseja limpar dados?
Confirme digitando 'Y' ou 'N':
Y

Limpando dados do experimento anterior...
✓ Arquivo de dados removido
✓ Marcador removido
✓ Sistema pronto para novo experimento!
```

---

## 💾 Importar Dados para Análise

### Método 1: Enviar para Serial (Copiar-Colar)

```
1. Digite comando 'D' no Serial Monitor
2. Copie os dados exibidos
3. Cole em arquivo de texto
4. Salve como "dados.csv"
5. Abra no Excel/LibreOffice
```

### Método 2: Extrair via SPIFFS Explorer

```
Se usar VS Code com PlatformIO:
1. Instale extensão SPIFFS
2. Conecte ESP32
3. Navegue para /dados_biodigestor.csv
4. Download do arquivo
5. Abra em programa de análise
```

### Método 3: Criar Endpoint HTTP (Futuro)

```cpp
// Adicionar em enviarDadosServidor():
GET /api/biodigestor/dados HTTP/1.1
Content-Type: application/csv

[Arquivo CSV aqui]
```

---

## 📈 Análise de Dados em Excel/LibreOffice

### Preparação

```
1. Abra arquivo dados.csv
2. Dados já estão formatados em colunas
3. Defina tipos de dados:
   - LEITURA: Número
   - DATA_HORA: Data/Hora
   - TEMP_C: Número decimal
   - Etc.
```

### Gráficos Recomendados

#### Gráfico 1: Temperatura vs Tempo

```
X: DATA_HORA
Y: TEMP_C

Mostra: Evolução térmica do experimento
Análise: Estabilidade, variações diárias
```

#### Gráfico 2: Pressão vs Tempo

```
X: DATA_HORA
Y: PRESSAO_HPA

Mostra: Acúmulo de biogás
Análise: Produção e consumo
```

#### Gráfico 3: pH vs Tempo

```
X: DATA_HORA
Y: PH

Mostra: Estabilidade do pH
Análise: Acidificação ou alcalinização
```

#### Gráfico 4: Amplitude Térmica

```
Y1: TEMP_MIN
Y2: TEMP_MAX
X: DATA_HORA

Mostra: Variação térmica em cada período
Análise: Estabilidade térmica do biodigestor
```

---

## 🔄 Fluxo de Dados Completo

```
┌──────────────────────────────────────────────────────┐
│           EXPERIMENTO DE 10 DIAS                     │
├──────────────────────────────────────────────────────┤
│                                                      │
│  Sensores (Em tempo real)                            │
│  ├─ DHT22 (Temperatura)                             │
│  ├─ BMP280 (Pressão)                                │
│  └─ pH (Analógico)                                  │
│      ↓ (A cada 10 segundos)                         │
│                                                      │
│  ┌──────────────────────────────────┐               │
│  │ Loop Principal (10s)             │               │
│  │ - Lê sensores                    │               │
│  │ - Verifica alertas               │               │
│  │ - Exibe no Serial                │               │
│  └──────────────────────────────────┘               │
│      ↓ (A cada 1 hora)                              │
│                                                      │
│  ┌──────────────────────────────────┐               │
│  │ Logging (1 hora)                 │               │
│  │ - Grava em arquivo CSV           │               │
│  │ - SPIFFS Flash                   │               │
│  │ - Até 300 entradas               │               │
│  └──────────────────────────────────┘               │
│      ↓ (Ao final do experimento)                    │
│                                                      │
│  ┌──────────────────────────────────┐               │
│  │ Exportação de Dados              │               │
│  │ - Comando 'D' (ver dados)        │               │
│  │ - Salvar como CSV                │               │
│  │ - Importar em Excel              │               │
│  └──────────────────────────────────┘               │
│      ↓                                               │
│                                                      │
│  ┌──────────────────────────────────┐               │
│  │ Análise                          │               │
│  │ - Gráficos                       │               │
│  │ - Estatísticas                   │               │
│  │ - Relatório                      │               │
│  └──────────────────────────────────┘               │
│                                                      │
└──────────────────────────────────────────────────────┘
```

---

## 📝 Estrutura do Código de Logging

### Funções Principais

```cpp
// Cria arquivo CSV com cabeçalho
void criarArquivoComCabecalho()

// Grava uma entrada a cada hora
void logarDados()

// Exibe informações de espaço SPIFFS
void exibirInfoSPIFFS()

// Exibe todos os dados armazenados
void exibirDadosArmazenados()

// Calcula estatísticas
void exibirStatisticasExperimento()

// Limpa arquivo para novo experimento
void limparDadosArmazenados()
```

### Fluxo de Logging Detalhado

```cpp
// Setup
criarArquivoComCabecalho()
  → Abre arquivo no modo 'w' (escrita)
  → Escreve cabeçalho CSV
  → Cria marcador /header_escrito

// Loop (a cada 1 hora)
logarDados()
  → Lê valores atuais dos sensores
  → Obtém timestamp NTP
  → Formata linha CSV
  → Abre arquivo no modo 'a' (append)
  → Escreve linha
  → Fecha arquivo
  → Incrementa contador de entradas
  → Exibe confirmação no Serial
```

---

## ⚙️ Configurações do Logging

### Ajustar Intervalo de Logging

```cpp
// Padrão: 3600000 ms (1 hora)
#define LOGGING_INTERVAL 3600000

// Para 30 minutos:
#define LOGGING_INTERVAL 1800000

// Para 2 horas:
#define LOGGING_INTERVAL 7200000

// Para 1 dia:
#define LOGGING_INTERVAL 86400000
```

### Ajustar Limite de Entradas

```cpp
// Padrão: 300 entradas (12.5 dias com leitura horária)
#define MAX_ENTRIES 300

// Para 7 dias:
#define MAX_ENTRIES 168

// Para 30 dias (máximo):
#define MAX_ENTRIES 720
```

---

## 🔔 Notificações de Logging

### No Serial Monitor

```
Quando uma leitura é gravada:

=================================================
✓ Dados gravados - Entrada #1 em 12/06/2026 10:00:00
=================================================

Mostra:
- Número da entrada
- Data e hora exata
- Status do arquivo
```

### Alertas

```
Se atingir limite de entradas:
⚠️  AVISO: Limite de registros atingido!
    Arquivo contém 300 entradas (12.5 dias)
    Comando 'D' para consultar ou 'L' para limpar

Se arquivo não puder ser criado:
ERRO: Não foi possível criar arquivo de dados!
```

---

## 🛠️ Troubleshooting de Logging

### Problema: Dados não são gravados

**Solução:**
1. Verifique com comando 'D' se arquivo existe
2. Verifique se SPIFFS foi inicializado com sucesso
3. Execute comando 'L' para criar novo arquivo
4. Reinicie ESP32

### Problema: Timestamp incorreto

**Solução:**
1. ESP32 deve estar conectado ao WiFi
2. Timezone está configurado para UTC-3 (Brasil)
3. Se offline, timestamp pode estar errado
4. Reconecte WiFi com comando 'C'

### Problema: Sem espaço em disco

**Solução:**
1. Execute comando 'E' para ver duração do experimento
2. Se >300 entradas, use comando 'L' para limpar
3. Exporte dados com 'D' ANTES de limpar
4. Salve CSV em computador antes de limpar

### Problema: Arquivo corrompido

**Solução:**
1. Use comando 'D' para verificar integridade
2. Se não conseguir ler, execute 'L' para limpar
3. Se perdeu dados, consulte backup em serial log
4. Inicie novo experimento

---

## 📊 Exemplo Completo de Experimento

### Dia 1 - Inicialização

```
10:00 - Ligo ESP32 → "Sistema pronto para experimento!"
10:00 - Entrada #1 gravada (primeira hora)
11:00 - Entrada #2 gravada
...
23:00 - Entrada #13 gravada (13 horas decorridas)
```

### Dia 5 - Meio do Experimento

```
Digito: E
Enter

Resultado:
TEMPERATURA:
  Mínima: 27.8°C
  Máxima: 30.2°C
  Média: 28.9°C
  Amplitude: 2.4°C

PRESSÃO:
  Média: 1013.8 hPa

pH:
  Média: 7.15

Total de Leituras: 96
Duração Estimada: 95 horas
Que corresponde a: 3 dias e 23 horas
```

### Dia 10 - Final do Experimento

```
Digito: D
Enter

[Exibe 240 linhas de dados]

Digito: E
Enter

TEMPERATURA:
  Mínima: 27.5°C
  Máxima: 30.8°C
  Média: 28.7°C
  Amplitude: 3.3°C

Total de Leituras: 240
Duração Estimada: 239 horas
Que corresponde a: 9 dias e 23 horas
```

---

## 📚 Referência Rápida de Comandos Logging

```
R → Reseta min/max
S → Estatísticas sistema
C → Reconecta WiFi
T → Testa sensores
───────────────────────
D → [NOVO] Exibe dados armazenados
E → [NOVO] Estatísticas experimento
L → [NOVO] Limpa dados
H → Ajuda
```

---

## ✅ Checklist para Experimento

- [ ] Código v2.0.0 carregado
- [ ] SPIFFS inicializado com sucesso
- [ ] Arquivo CSV criado com cabeçalho
- [ ] Primeira leitura gravada
- [ ] WiFi conectado (para timestamp correto)
- [ ] Timezone configurado (UTC-3 para Brasil)
- [ ] Sensores calibrados
- [ ] Experimento iniciado
- [ ] Monitore com comando 'E' a cada dia
- [ ] Ao final, execute 'D' e salve dados
- [ ] Importe em Excel para análise

---

## 🎉 Próximos Passos

1. **Usar código v2.0.0** - Substitua pelo antigo
2. **Calibrar sensores** - Especialmente pH
3. **Iniciar experimento** - Comando 'L' para limpar
4. **Monitorar diariamente** - Comando 'E' para ver progresso
5. **Ao final** - Comando 'D' para exportar dados
6. **Analisar em Excel** - Criar gráficos e relatório

---

**Versão:** 2.0.0  
**Data:** 12 de Junho de 2026  
**Tipo:** Sistema de Logging para Experimentos Longos
