#include <Fuzzy.h>
#include <DHT.h>

// Definindo as portas do ventilador e dos sensores
#define FAN_PIN 9
#define CO2_SENSOR_PIN A0
#define CO_SENSOR_PIN A2
#define HUMIDITY_SENSOR_PIN A1

// Definindo o tipo de sensor DHT11
#define DHTTYPE DHT11

// Criando um objeto DHT
DHT dht(HUMIDITY_SENSOR_PIN, DHTTYPE);

// Instanciando um objeto Fuzzy
Fuzzy *fuzzy = new Fuzzy();

// Função para combinar antecedentes de regras difusas usando AND
FuzzyRuleAntecedent* andAntecedents(FuzzyRuleAntecedent* antecedent1, FuzzyRuleAntecedent* antecedent2, FuzzyRuleAntecedent* antecedent3) {
  FuzzyRuleAntecedent* result = new FuzzyRuleAntecedent();
  result->joinWithAND(antecedent1, antecedent2);
  result->joinWithAND(result, antecedent3);
  return result;
}

void setup() {
  // Inicializando a comunicação serial
  Serial.begin(9600);

  // Definindo o pino do ventilador como saída
  pinMode(FAN_PIN, OUTPUT);

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

  // Definindo velocidade do ventilador como variável de saída
  FuzzyOutput *fanSpeed = new FuzzyOutput(1);
  FuzzySet *lowSpeed = new FuzzySet(0, 30, 30, 60);
  FuzzySet *mediumSpeed = new FuzzySet(40, 70, 70, 100);
  FuzzySet *highSpeed = new FuzzySet(60, 100, 100, 100);
  fanSpeed->addFuzzySet(lowSpeed);
  fanSpeed->addFuzzySet(mediumSpeed);
  fanSpeed->addFuzzySet(highSpeed);
  fuzzy->addFuzzyOutput(fanSpeed);

  // Definindo regras fuzzy
  FuzzyRuleAntecedent *ifLowHumidity = new FuzzyRuleAntecedent();
  ifLowHumidity->joinSingle(lowHumidity);
  FuzzyRuleAntecedent *ifHighCO2 = new FuzzyRuleAntecedent();
  ifHighCO2->joinSingle(highCO2);
  FuzzyRuleAntecedent *ifLowCO = new FuzzyRuleAntecedent();
  ifLowCO->joinSingle(lowCO);
  FuzzyRuleAntecedent* combinedAntecedents = andAntecedents(ifLowHumidity, ifHighCO2, ifLowCO);
  FuzzyRuleConsequent *thenHighFanSpeed = new FuzzyRuleConsequent();
  thenHighFanSpeed->addOutput(highSpeed);
  FuzzyRule *rule1 = new FuzzyRule(1, combinedAntecedents, thenHighFanSpeed);
  fuzzy->addFuzzyRule(rule1);

  // Adicione mais regras conforme necessário para outros casos

  dht.begin();
}

void loop() {
  // Lendo os valores de umidade, CO2 e CO usando os sensores
  float humidityValue = dht.readHumidity();
  float co2Value = analogRead(CO2_SENSOR_PIN);
  float coValue = analogRead(CO_SENSOR_PIN);

  // Imprimindo os valores lidos
  Serial.print("Umidade: ");
  Serial.println(humidityValue);
  Serial.print("CO2: ");
  Serial.println(co2Value);
  Serial.print("CO: ");
  Serial.println(coValue);

  // Configurando as entradas do sistema fuzzy
  fuzzy->setInput(1, humidityValue);
  fuzzy->setInput(2, co2Value);
  fuzzy->setInput(3, coValue);

  // Executando o processo de fuzzificação e defuzzificação
  fuzzy->fuzzify();
  float fanSpeed = fuzzy->defuzzify(1);

  // Imprimindo a velocidade do ventilador
  Serial.print("Velocidade do Ventilador: ");
  Serial.println(fanSpeed);

  // Controlando a velocidade do ventilador
  analogWrite(FAN_PIN, fanSpeed);

  // Aguardando um intervalo antes da próxima leitura
  delay(1000); // Ajuste conforme necessário
}
