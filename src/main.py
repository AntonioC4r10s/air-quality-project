import serial
import csv

# Configurações da porta serial
porta_serial = 'COM3'  # Substitua 'COM3' pela porta serial correta
velocidade_serial = 9600

# Nome do arquivo CSV
nome_arquivo_csv = 'dados_sensor.csv'

# Função para extrair as informações da linha fornecida
def extrair_informacoes(linha):
    informacoes = linha.strip().split(', ')
    temperatura = float(informacoes[0].split(': ')[1])
    umidade = float(informacoes[1].split(': ')[1])
    co2 = float(informacoes[2].split(': ')[1])
    co = float(informacoes[3].split(': ')[1])
    fuzzy_info = informacoes[4].split(': ')[1]
    return temperatura, umidade, co2, co, fuzzy_info

# Inicializa a comunicação serial
ser = serial.Serial(porta_serial, velocidade_serial)

# Abre o arquivo CSV para escrita
with open(nome_arquivo_csv, mode='w', newline='') as arquivo_csv:
    escritor_csv = csv.writer(arquivo_csv)
    escritor_csv.writerow(['Temperatura', 'Umidade', 'CO2', 'CO', 'FuzzyInfo'])  # Escreve o cabeçalho no arquivo CSV

    # Loop para ler as linhas fornecidas pela porta serial e escrever no arquivo CSV
    try:
        while True:
            linha = ser.readline().decode().strip()  # Lê uma linha da porta serial
            temperatura, umidade, co2, co, fuzzy_info = extrair_informacoes(linha)  # Extrai as informações da linha
            escritor_csv.writerow([temperatura, umidade, co2, co, fuzzy_info])  # Escreve as informações no arquivo CSV
            print("Dados armazenados no arquivo CSV:", temperatura, umidade, co2, co, fuzzy_info)  # Imprime as informações
    except KeyboardInterrupt:
        print("Interrompendo a leitura da porta serial.")

# Fecha a comunicação serial
ser.close()
