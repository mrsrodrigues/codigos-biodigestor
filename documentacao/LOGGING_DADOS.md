# Salvando Dados

## Onde os dados ficam
- Os dados são salvos na própria placa.
- Também podem ser usados no app Blynk.
- Não precisa de servidor nem API.

## Formato simples
Os valores podem ser salvos em arquivo CSV ou exportados para planilha.

## Passo a passo para Excel
1. Pegue os dados gerados pelo sistema.
2. Copie para um arquivo CSV.
3. Abra o Excel.
4. Importe o arquivo ou cole os dados em uma planilha.
5. Organize as colunas: data, hora, metano, temperatura, pH e pressão.

## Fluxo do projeto
1. Sensor lê os dados.
2. O ESP32 processa as leituras.
3. Os valores aparecem no Blynk.
4. Os dados são guardados na placa e podem ir para Excel.
