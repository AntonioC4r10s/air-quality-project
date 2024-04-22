import pandas as pd
import matplotlib.pyplot as plt

# Nome do arquivo CSV
nome_arquivo_csv = 'dados_sensor.csv'

# Carrega os dados do arquivo CSV usando o pandas
dados = pd.read_csv(nome_arquivo_csv)

# Define o número de subplots com base no número de variáveis
num_variaveis = len(dados.columns) - 2  # Exclui a coluna de índices e de temperatura
fig, axs = plt.subplots(2, 2, figsize=(12, 8))

# Lista de cores para os gráficos
cores = ['blue', 'green', 'red', 'purple']

# Cria um gráfico de linha para cada variável
for i, ax in enumerate(axs.flat):
    if i == 0:
        ax.plot(dados.index, dados['Umidade'], color=cores[i])
        ax.set_ylabel('Umidade')
        ax.set_xlabel('Amostra')
        ax.set_title('Gráfico de Umidade')
    elif i == 1:
        ax.plot(dados.index, dados['Temperatura'], color='orange')
        ax.set_ylabel('Temperatura')
        ax.set_xlabel('Amostra')
        ax.set_title('Gráfico de Temperatura')
    elif i == 2:
        ax.plot(dados.index, dados['CO2'], color=cores[i-1])
        ax.set_ylabel('CO2')
        ax.set_xlabel('Amostra')
        ax.set_title('Gráfico de CO2')
    elif i == 3:
        ax.plot(dados.index, dados['CO'], color=cores[i-1])
        ax.set_ylabel('CO')
        ax.set_xlabel('Amostra')
        ax.set_title('Gráfico de CO')
    else:
        ax.axis('off')  # Desativa os eixos para os subplots vazios

plt.savefig('src\image/data.png')
plt.tight_layout()
plt.show()
