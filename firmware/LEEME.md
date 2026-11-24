# PEZ-FRÍO · Modo en vivo por USB (rama `en-vivo-usb`)

Esta rama agrega a **Prototipo inicial 2 sensores.html** un panel «En vivo (USB)» que lee la placa
conectada al computador, muestra los datos en tiempo real y guarda un CSV automáticamente.
La rama `main` no se modificó. Para volver a como estaba basta con usar `main`.

## Qué contiene

| Archivo | Para qué sirve |
|---|---|
| `Prototipo inicial 2 sensores.html` | Misma página, con el panel «En vivo (USB)» agregado al final del archivo |
| `firmware/pezfrio_usb/pezfrio_usb.ino` | Programa de la NodeMCU (ESP8266). Imprime una línea por USB cada ~2 s |
| `firmware/pezfrio_usb.bin` | El mismo programa ya compilado, para cargarlo sin Arduino IDE |

## Conexiones (NodeMCU ESP8266)

| Sensor | Pin de la placa |
|---|---|
| DS18B20 (sonda, **exterior**): DATA | D2, con 4.7 kΩ entre DATA y 3V3 |
| DHT22 (**interior**, temperatura y humedad): DATA | D5 |
| Alimentación de ambos | 3V3 y GND |

## Cómo se usa la página

1. Conecta la placa al computador por USB.
2. Abre la página en **Chrome o Edge** (Firefox y Safari no permiten conectar la placa).
3. En el panel «En vivo (USB)» pulsa **Conectar placa** y elige el puerto (aparece como CP210x o CH340).
4. Pulsa **Guardar CSV automáticamente…** y elige dónde guardar el archivo. Después de eso se actualiza solo en cada registro.
5. Elige cada cuánto se registra un dato (por defecto 5 minutos).

Detalles:

- La **fecha y hora** salen del reloj del computador. La placa no necesita reloj ni internet.
- Si una lectura falla, su celda queda **vacía** en el CSV. Nunca se inventa un valor.
- Cuando hay una alerta (límites de la página: alerta 8 °C y mínimo 0 °C por defecto), suena un aviso y se guarda un registro extra de inmediato.
- Los registros también se guardan dentro del navegador. Si se cierra la página por accidente, se recuperan al volver a abrirla.
- Cierra el Monitor Serie de Arduino IDE antes de conectar: solo un programa puede usar el puerto a la vez.

## Formato del CSV

```
fecha_hora,temperatura_interna,temperatura_ambiente,humedad_relativa
2026-11-23 23:16:58,4.2,27.31,81.5
```

## Cargar el programa en la placa

- **Sin Arduino IDE (recomendado):** abrir `instalar.html` desde GitHub Pages en Chrome o Edge
  (https://marianaccza.github.io/pez-frio-analytics/instalar.html) y pulsar «Instalar programa en la placa».
  Usa ESP Web Tools con `firmware/manifest.json`, que carga `pezfrio_usb.bin` en la dirección `0x0`.
- **Con Arduino IDE:** abrir `pezfrio_usb.ino`, elegir la placa «NodeMCU 1.0 (ESP-12E Module)» y subir.
  Necesita las bibliotecas OneWire, DallasTemperature y DHT sensor library (Adafruit).

## Qué no incluye todavía

- Conexión por WiFi o por celular.
- GPS.
- La página principal `index.html` no se tocó: pide una tercera temperatura (`T3_carga`) y no maneja humedad.
