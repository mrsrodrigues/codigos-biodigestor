# Conexão da Placa

## Pinos principais
- OLED: SDA no pino 17 e SCL no pino 18
- Metano: pino 101
- Temperatura: pino 102
- Pressão: pino 107
- pH: pino 106

## Ligação básica
- GND → GND comum
- 3.3V → alimentação dos módulos
- OLED → pinos I2C da ESP32
- Sensores → entradas analógicas ou digitais conforme o modelo

## Observação
Se alguma ligação não funcionar, confira:
- alimentação
- terra comum
- pinos corretos
- cabo USB
