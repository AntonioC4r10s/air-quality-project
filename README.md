# Análise de Dados de Sensores e Controle Fuzzy

Este repositório contém códigos em Python e Arduino para realizar a coleta de dados de sensores, a análise desses dados utilizando lógica fuzzy e a visualização dos resultados por meio de gráficos.

## Arduino

O código Arduino, encontrado em `sensor_fuzzy.ino`, é responsável por:

- Ler os valores de temperatura, umidade, CO2 e CO de sensores conectados ao microcontrolador Arduino.
- Utilizar lógica fuzzy para determinar a saída desejada para um umidificador de ar, com base nos valores dos sensores.
- Enviar os valores lidos e a saída fuzzy pela porta serial para um computador.

## Python

O código Python, encontrado em `src/main.py` e `src/data.py`, realiza as seguintes respectivamente as tarefas:

- Recebe os dados enviados pelo Arduino pela porta serial.
- Armazena esses dados em um arquivo CSV para análise posterior.
- Utiliza a biblioteca pandas para carregar os dados do arquivo CSV e a biblioteca matplotlib para plotar gráficos dos dados.
- Gera gráficos separados para temperatura, umidade, CO2 e CO, bem como para a saída do umidificador de ar, com cada gráfico em uma cor distinta.
- Os gráficos são dispostos em uma matriz 2x2 para facilitar a visualização.

## Uso

Para utilizar este projeto, siga os seguintes passos:

1. Carregue o código Arduino `sensor_fuzzy.ino` em um microcontrolador Arduino.
2. Conecte os sensores de temperatura, umidade, CO2 e CO ao Arduino.
3. Execute o código Python `main.py` em um ambiente Python com as bibliotecas pandas e matplotlib instaladas.
4. Visualize os gráficos gerados para analisar os dados coletados pelos sensores e a saída do umidificador de ar determinada pela lógica fuzzy através de `src/data.py`.

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para abrir uma issue ou enviar um pull request.

