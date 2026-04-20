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

// Dado obtido manualmente da API OpenWeather (openWeatherAPICall.R)
// 0 = sem chuva prevista → irrigação permitida
// 1 = chuva prevista     → irrigação suspensa
int previsaoChuva = 0;

// Mapeia leitura analógica do LDR para pH
float ldrParaPH(int ldrVal) {
  return (ldrVal / 4095.0) * 14.0;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_N,    INPUT_PULLDOWN);
  pinMode(PIN_P,    INPUT_PULLDOWN);
  pinMode(PIN_K,    INPUT_PULLDOWN);
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW);
  dht.begin();

  Serial.println("FarmTech Solutions - Fase 2");
  Serial.println("Cultura: Soja");
  Serial.println("Iniciando sistema...");

  if (previsaoChuva == 1) {
    Serial.println("Aviso: chuva prevista (fonte: OpenWeather)");
    Serial.println("Irrigacao suspensa por precaucao");
  }

  delay(1000);
}

void loop() {
  // Leitura dos sensores
  bool nitrogenio  = digitalRead(PIN_N);
  bool fosforo     = digitalRead(PIN_P);
  bool potassio    = digitalRead(PIN_K);

  int   ldrVal      = analogRead(PIN_LDR);
  float ph          = ldrParaPH(ldrVal);
  float umidade     = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Validação DHT22
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro ao ler o DHT22.");
    delay(2000);
    return;
  }

  // Monitor Serial
  Serial.println("\nLeitura dos sensores:");
  Serial.print("Nitrogenio: ");
  Serial.println(nitrogenio ? "presente" : "baixo");
  Serial.print("Fosforo: ");
  Serial.println(fosforo ? "presente" : "baixo");
  Serial.print("Potassio: ");
  Serial.println(potassio ? "presente" : "baixo");
  Serial.print("pH do solo: ");
  Serial.print(ph, 1);
  Serial.print("  (ldr: ");
  Serial.print(ldrVal);
  Serial.println(")");
  Serial.print("Umidade: ");
  Serial.print(umidade, 1);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println("C");
  Serial.print("Previsao de chuva: ");
  Serial.println(previsaoChuva == 1 ? "sim (OpenWeather)" : "nao");

  // Lógica de Irrigação
  bool nutrienteBaixo = !nitrogenio || !fosforo || !potassio;
  bool phAdequado     = (ph >= PH_MIN && ph <= PH_MAX);
  bool soloSeco       = (umidade < UMIDADE_MIN);
  bool semChuva       = (previsaoChuva == 0);

  bool ligarBomba = soloSeco && phAdequado && nutrienteBaixo && semChuva;

  Serial.println("\nDecisao:");
  if (ligarBomba) {
    digitalWrite(PIN_RELE, HIGH);
    Serial.println("bomba ligada");
    Serial.println("motivo: solo seco + pH ok + nutriente baixo + sem chuva prevista");
  } else {
    digitalWrite(PIN_RELE, LOW);
    Serial.println("bomba desligada");
    if (!soloSeco)       Serial.println("motivo: umidade ok");
    if (!phAdequado)     Serial.println("motivo: pH fora da faixa");
    if (!nutrienteBaixo) Serial.println("motivo: nutrientes ok");
    if (!semChuva)       Serial.println("motivo: chuva prevista (OpenWeather)");
  }

  delay(3000);
}
