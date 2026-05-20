#include <DHT.h>

// PINOS
#define PIN_N    15
#define PIN_P    2
#define PIN_K    4
#define PIN_LDR  34
#define PIN_DHT  23
#define PIN_RELE 5
#define DHTTYPE  DHT22

DHT dht(PIN_DHT, DHTTYPE);

// PARÂMETROS DA CULTURA DE SOJA
const float UMIDADE_MIN = 60.0;
const float PH_MIN      = 6.0;
const float PH_MAX      = 7.0;

// 0 = sem chuva prevista
// 1 = chuva prevista
int previsaoChuva = 0;

// Converte leitura do LDR para pH
float ldrParaPH(int ldrVal) {
  return (ldrVal / 4095.0) * 14.0;
}

void setup() {

  Serial.begin(115200);

  // INPUT_PULLUP porque os botões estão ligados ao GND
  pinMode(PIN_N, INPUT_PULLUP);
  pinMode(PIN_P, INPUT_PULLUP);
  pinMode(PIN_K, INPUT_PULLUP);

  pinMode(PIN_RELE, OUTPUT);

  digitalWrite(PIN_RELE, LOW);

  dht.begin();

  delay(1000);

  // Cabeçalho CSV
  Serial.println(
    "tempo_ms,nitrogenio,fosforo,potassio,ldr,ph,umidade,temperatura,previsao_chuva,bomba"
  );
}

void loop() {

  // invertido por causa do INPUT_PULLUP
  bool nitrogenio = !digitalRead(PIN_N);
  bool fosforo    = !digitalRead(PIN_P);
  bool potassio   = !digitalRead(PIN_K);

  int ldrVal = analogRead(PIN_LDR);

  float ph = ldrParaPH(ldrVal);

  float umidade = dht.readHumidity();

  float temperatura = dht.readTemperature();

  // validação DHT
  if (isnan(umidade) || isnan(temperatura)) {

    Serial.println("erro_dht");

    delay(2000);

    return;
  }

  // lógica da irrigação
  bool nutrienteBaixo =
      !nitrogenio ||
      !fosforo ||
      !potassio;

  bool phAdequado =
      ph >= PH_MIN &&
      ph <= PH_MAX;

  bool soloSeco =
      umidade < UMIDADE_MIN;

  bool semChuva =
      previsaoChuva == 0;

  bool ligarBomba =
      soloSeco &&
      phAdequado &&
      nutrienteBaixo &&
      semChuva;

  digitalWrite(
      PIN_RELE,
      ligarBomba ? HIGH : LOW
  );

  // CSV
  Serial.print(millis());
  Serial.print(",");

  Serial.print(nitrogenio);
  Serial.print(",");

  Serial.print(fosforo);
  Serial.print(",");

  Serial.print(potassio);
  Serial.print(",");

  Serial.print(ldrVal);
  Serial.print(",");

  Serial.print(ph, 2);
  Serial.print(",");

  Serial.print(umidade, 2);
  Serial.print(",");

  Serial.print(temperatura, 2);
  Serial.print(",");

  Serial.print(previsaoChuva);
  Serial.print(",");

  Serial.println(
      ligarBomba ? 1 : 0
  );

  delay(3000);
}