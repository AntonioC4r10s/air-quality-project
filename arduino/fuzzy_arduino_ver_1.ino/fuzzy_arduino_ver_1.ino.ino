#include <Fuzzy.h>
#include <DHT.h>

// Definindo as portas do umidificador e dos sensores
#define HUMIDIFIER_PIN 9
#define CO2_SENSOR_PIN A0
#define CO_SENSOR_PIN A2
#define HUMIDITY_SENSOR_PIN A1

// Definindo o tipo de sensor DHT11
#define DHTTYPE DHT11

// Criando um objeto DHT
DHT dht(HUMIDITY_SENSOR_PIN, DHTTYPE);

// Instanciando um objeto Fuzzy
Fuzzy *fuzzy = new Fuzzy();

// Função para gerar valores aleatórios para CO e CO2
float gerarValorAleatorio() {
  // Gera um valor aleatório entre 0 e 1023
  return random(0, 1024);
}

void setup() {
  // Inicializando a comunicação serial
  Serial.begin(9600);

  // Definindo o pino do umidificador como saída
  pinMode(HUMIDIFIER_PIN, OUTPUT);

  // Definindo umidade como variável de entrada
  FuzzyInput *humidity = new FuzzyInput(1);
  FuzzySet *lowHumidity = new FuzzySet(0, 20, 20, 40);
  FuzzySet *mediumHumidity = new FuzzySet(30, 50, 50, 70);
  FuzzySet *highHumidity = new FuzzySet(60, 80, 80, 100);
  humidity->addFuzzySet(lowHumidity);
  humidity->addFuzzySet(mediumHumidity);
  humidity->addFuzzySet(highHumidity);
  fuzzy->addFuzzyInput(humidity);

  // Definindo CO2 como variável de entrada
  FuzzyInput *co2 = new FuzzyInput(2);
  FuzzySet *lowCO2 = new FuzzySet(0, 400, 400, 800);
  FuzzySet *mediumCO2 = new FuzzySet(600, 1000, 1000, 1400);
  FuzzySet *highCO2 = new FuzzySet(1200, 1600, 1600, 2000);
  co2->addFuzzySet(lowCO2);
  co2->addFuzzySet(mediumCO2);
  co2->addFuzzySet(highCO2);
  fuzzy->addFuzzyInput(co2);

  // Definindo CO como variável de entrada
  FuzzyInput *co = new FuzzyInput(3);
  FuzzySet *lowCO = new FuzzySet(0, 2, 2, 4);
  FuzzySet *mediumCO = new FuzzySet(3, 6, 6, 9);
  FuzzySet *highCO = new FuzzySet(8, 10, 10, 12);
  co->addFuzzySet(lowCO);
  co->addFuzzySet(mediumCO);
  co->addFuzzySet(highCO);
  fuzzy->addFuzzyInput(co);

  // Definindo potência do umidificador como variável de saída
  FuzzyOutput *humidifierPower = new FuzzyOutput(1);
  FuzzySet *offPower = new FuzzySet(0, 0, 0, 0);
  FuzzySet *normalPower = new FuzzySet(0, 50, 50, 100);
  FuzzySet *maxPower = new FuzzySet(50, 100, 100, 100);
  humidifierPower->addFuzzySet(offPower);
  humidifierPower->addFuzzySet(normalPower);
  humidifierPower->addFuzzySet(maxPower);
  fuzzy->addFuzzyOutput(humidifierPower);

  // Definindo regras fuzzy
  FuzzyRuleAntecedent *ifLowHumidity = new FuzzyRuleAntecedent();
  FuzzyRuleAntecedent *ifHighCO2 = new FuzzyRuleAntecedent();
  FuzzyRuleAntecedent *ifLowCO = new FuzzyRuleAntecedent();
  FuzzyRuleAntecedent *combinedAntecedents = new FuzzyRuleAntecedent();
  combinedAntecedents->joinSingle(lowHumidity);
  combinedAntecedents->joinSingle(highCO2);
  combinedAntecedents->joinSingle(lowCO);
  FuzzyRuleConsequent *thenMaxPower = new FuzzyRuleConsequent();
  thenMaxPower->addOutput(maxPower);
  FuzzyRule *rule1 = new FuzzyRule(1, combinedAntecedents, thenMaxPower);
  fuzzy->addFuzzyRule(rule1);

  dht.begin();
}

void loop() {
  // Lendo os valores de umidade, CO2 e CO usando os sensores
  float humidityValue = dht.readHumidity();
  float co2Value = gerarValorAleatorio(); // Usando a função para gerar um valor aleatório
  float coValue = gerarValorAleatorio(); // Usando a função para gerar um valor aleatório
  float temperatureValue = dht.readTemperature(); // Lendo a temperatura

  // Configurando as entradas do sistema fuzzy
  fuzzy->setInput(1, humidityValue);
  fuzzy->setInput(2, co2Value);
  fuzzy->setInput(3, coValue);

  // Executando o processo de fuzzificação e defuzzificação
  fuzzy->fuzzify();
  float powerLevel = fuzzy->defuzzify(1);

  // Controlando o nível de potência do umidificador
  if (powerLevel <= 0) {
    digitalWrite(HUMIDIFIER_PIN, LOW); // Desliga o umidificador
  } else if (powerLevel <= 50) {
    analogWrite(HUMIDIFIER_PIN, 128); // Liga o umidificador em potência normal
  } else {
    analogWrite(HUMIDIFIER_PIN, 255); // Liga o umidificador na máxima potência
  }

  // Criando uma string formatada com todas as informações
  String output = "Temperatura: " + String(temperatureValue) + ", Umidade: " + String(humidityValue) + ", CO2: " + String(co2Value) + ", CO: " + String(coValue) + ", Nível de Potência do Umidificador: " + String(powerLevel);

  // Enviando a string pela porta serial
  Serial.println(output);

  // Aguardando um intervalo antes da próxima leitura
  delay(1000); // Ajuste conforme necessário
}
