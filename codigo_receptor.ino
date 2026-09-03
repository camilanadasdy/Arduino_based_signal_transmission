#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);
const byte address[6] = "00001";
char msg[32];
void setup() {
  Serial.begin(9600);
  if (!radio.begin()) {
    Serial.println("NRF NO DETECTADO");
    while (1);}
  radio.openReadingPipe(0, address);
  // Configuracion identica al transmisor
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.setRetries(15, 15);
  radio.startListening();
  Serial.println("Chat RX listo");}
void loop() {
  if (radio.available()) {
    memset(msg, 0, sizeof(msg));
    radio.read(msg, sizeof(msg));
    Serial.print("Recibido: ");
    Serial.println(msg);}}
\end{lstlisting}

\section{Código Python}
\subsection{Código Arduino Mega -- Transmisor}
\begin{lstlisting}
import serial
import time
import csv
from pathlib import Path
from datetime import datetime
# ===== CONFIG =====
PORT = "COM8"   # MEGA
BAUD = 9600
CARPETA = Path(r"C:\Users\USER\Documents\archivos CAMILA\ESPOCH\5to Semestre\Electrodynamics\Proyecto Final\codigo_transmisor")
ARCHIVO = CARPETA / "envio_nrf.csv"
arduino = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)
n = int(input(" Paquetes a enviar: "))
delay = float(input(" Delay (s): "))
print("\n Enviando...\n")
with open(ARCHIVO, "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    writer.writerow([
        "id",
        "timestamp",
        "fecha_hora",
        "mensaje"])
    for i in range(1, n + 1):
        msg = f"PKT{i:04d}"
        ts = time.time()
        fecha = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
        #  IMPORTANTE: SOLO newline al final
        arduino.write((msg + "\n").encode("utf-8"))
        print(f"{fecha} | Enviado: {msg}")
        writer.writerow([i, ts, fecha, msg])
        f.flush()
        time.sleep(delay)
arduino.close()
print("\nOK: Terminado")
print(f" CSV: {ARCHIVO}")
