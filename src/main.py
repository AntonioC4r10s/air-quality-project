import serial
import csv
import re
from sensor_CO import calibration_MQ9_CO, CO_in_PPM
import pandas as pd

COST_MINUTE = 12

# Configurações da porta serial
porta_serial = 'COM3'  # Substitua 'COM3' pela porta serial correta
velocidade_serial = 9600

# Nome do arquivo CSV
nome_arquivo_csv = 'dados_sensor.csv'
nome_arquivo_calibragem_csv = 'calibration.csv'

# Função para extrair as informações da linha fornecida
def extrair_informacoes(linha):
    # Define os padrões de expressão regular para cada variável
    temperatura_padrao = r'temperature: (\d+\.\d+)'
    umidade_padrao = r'humidity: (\d+\.\d+)'
    co_analogico_padrao = r'co_analogic: (\d+)'
    co_digital_padrao = r'co_digital: (\d+)'
    co2_analogico_padrao = r'co2_analogic: (\d+)'
    co2_digital_padrao = r'co2_digital: (\d+)'

    # Busca os valores correspondentes na string
    temperatura_match = re.search(temperatura_padrao, linha)
    umidade_match = re.search(umidade_padrao, linha)
    co_analogico_match = re.search(co_analogico_padrao, linha)
    co_digital_match = re.search(co_digital_padrao, linha)
    co2_analogico_match = re.search(co2_analogico_padrao, linha)
    co2_digital_match = re.search(co2_digital_padrao, linha)

    # Extrai os valores se as correspondências forem encontradas
    temperatura = temperatura_match.group(1) if temperatura_match else None
    umidade = umidade_match.group(1) if umidade_match else None
    co_analogico = co_analogico_match.group(1) if co_analogico_match else None
    co_digital = co_digital_match.group(1) if co_digital_match else None
    co2_analogico = co2_analogico_match.group(1) if co2_analogico_match else None
    co2_digital = co2_digital_match.group(1) if co2_digital_match else None

    return temperatura, umidade, co_analogico, co_digital, co2_analogico, co2_digital

################ Calibragem dos sensores #################
# Abre o arquivo CSV para escrita
ser = serial.Serial(porta_serial, velocidade_serial, timeout=5)
with open('calibration.csv', mode='w', newline='') as arquivo_csv:
    escritor_csv = csv.writer(arquivo_csv, quoting=csv.QUOTE_NONE, escapechar='\\')
    escritor_csv.writerow(["temperature,humidity,co_analogic,co_digital,co2_analogic,co2_digital"])  # Escreve o cabeçalho no arquivo CSV

    # Loop para ler as linhas fornecidas pela porta serial e escrever no arquivo CSV
    try:
        cont = 0
        NUM_INTERATIONS = COST_MINUTE*3
        while cont < NUM_INTERATIONS:
            linha = ser.readline().decode().strip()  # Lê uma linha da porta serial
            temperature, humidity, co_analogic, co_digital, co2_analogic, co2_digital = extrair_informacoes(linha)  # Extrai as informações da linha
            escritor_csv.writerow([temperature, humidity, co_analogic, co_digital, co2_analogic, co2_digital])  # Escreve as informações no arquivo CSV
            print("Dados armazenados no arquivo CSV:", temperature, humidity, co_analogic, co_digital, co2_analogic, co2_digital)  # Imprime as informações
            cont += 1
    except KeyboardInterrupt:
        print("Interrompendo a leitura da porta serial.")
    arquivo_csv.close()

df = pd.read_csv(nome_arquivo_calibragem_csv).dropna()
co_data = df['co_analogic\\'].to_numpy()
print(co_data)
r0_co = calibration_MQ9_CO(values=co_data)
print(r0_co)

################ Coleta com Sensores Calibrados #################
# Abre o arquivo CSV para escrita
with open(nome_arquivo_csv, mode='w', newline='') as arquivo_csv:
    escritor_csv = csv.writer(arquivo_csv, quoting=csv.QUOTE_NONE, escapechar='\\')
    escritor_csv.writerow(["temperature,humidity,co_analogic,co_digital,co2_analogic,co2_digital"])  # Escreve o cabeçalho no arquivo CSV

    # Loop para ler as linhas fornecidas pela porta serial e escrever no arquivo CSV
    try:
        cont = 0
        NUM_INTERATIONS = COST_MINUTE*10
        while cont < NUM_INTERATIONS:
            co_in_ppm = CO_in_PPM(value=float(co_analogic), r0=r0_co)

            linha = ser.readline().decode().strip()  # Lê uma linha da porta serial
            temperature, humidity, co_analogic, co_digital, co2_analogic, co2_digital = extrair_informacoes(linha)  # Extrai as informações da linha
            escritor_csv.writerow([temperature, humidity, co_in_ppm, co_digital, co2_analogic, co2_digital])  # Escreve as informações no arquivo CSV
            print("Dados armazenados no arquivo CSV:", temperature, humidity, co_in_ppm, co_digital, co2_analogic, co2_digital)  # Imprime as informações
            cont += 1
    except KeyboardInterrupt:
        print("Interrompendo a leitura da porta serial.")
    arquivo_csv.close()
# Fecha a comunicação serial
ser.close()
