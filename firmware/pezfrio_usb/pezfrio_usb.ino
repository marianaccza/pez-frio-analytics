// PEZ-FRIO - Lectura de sensores por USB (sin WiFi)
// NodeMCU (ESP8266)
//   DS18B20 (sonda, EXTERIOR)        -> D2  (con 4.7 kOhm entre DATA y 3V3)
//   DHT22 (INTERIOR: temp + humedad) -> D5
//
// Cada ~2 s imprime por USB (115200 baudios) una linea con este formato:
//   PEZ,<temperatura_interna>,<temperatura_ambiente>,<humedad_relativa>
// Ejemplo:  PEZ,4.2,27.31,81.5
// Si una lectura falla, su campo queda VACIO (nunca se inventa un valor):
//   PEZ,,27.31,          <- el DHT22 fallo
//
// Las lineas que empiezan por # son informativas y se pueden ignorar.
// La fecha y hora las pone la pagina web con el reloj del computador.

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define PIN_DS18B20  D2
#define PIN_DHT      D5
#define TIPO_DHT     DHT22

const unsigned long INTERVALO = 2000;   // ms entre lecturas (el DHT22 pide >= 2 s)

OneWire oneWire(PIN_DS18B20);
DallasTemperature ds18b20(&oneWire);
DHT dht(PIN_DHT, TIPO_DHT);

unsigned long ultimaLectura = 0;

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("# PEZ-FRIO firmware USB v1");
  Serial.println("# formato: PEZ,temperatura_interna,temperatura_ambiente,humedad_relativa");

  ds18b20.begin();
  dht.begin();
}

void loop() {
  if (millis() - ultimaLectura < INTERVALO) return;
  ultimaLectura = millis();

  // Exterior: DS18B20 (-127 = desconectado, 85 = valor de arranque sin medir)
  ds18b20.requestTemperatures();
  float tExt = ds18b20.getTempCByIndex(0);
  bool okExt = (tExt != DEVICE_DISCONNECTED_C) && (tExt != 85.0) &&
               (tExt >= -55.0) && (tExt <= 125.0);

  // Interior: DHT22
  float hum = dht.readHumidity();
  float tInt = dht.readTemperature();
  bool okTemp = !isnan(tInt) && (tInt >= -40.0) && (tInt <= 80.0);
  bool okHum = !isnan(hum) && (hum >= 0.0) && (hum <= 100.0);

  Serial.print("PEZ,");
  if (okTemp) Serial.print(tInt, 1);
  Serial.print(",");
  if (okExt) Serial.print(tExt, 2);
  Serial.print(",");
  if (okHum) Serial.print(hum, 1);
  Serial.println();
}
