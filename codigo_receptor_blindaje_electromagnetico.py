import serial
import csv
import time
import threading
from pathlib import Path
from datetime import datetime
# ===== CONFIGURACION =====
PORT = "COM6"      # Cambia al puerto correcto
BAUD = 9600
CARPETA = Path(r"C:\Users\ThinkPad\Desktop\codigoreceptor")
ARCHIVO = CARPETA / "recepcion_nrf.csv"
detener = False
def escuchar_teclado():
    global detener
    while True:
        comando = input().strip()
        if comando == "1":
            detener = True
            break
try:
    arduino = serial.Serial(PORT, BAUD, timeout=1)
except Exception as e:
    print(f"\n No se pudo abrir el puerto {PORT}")
    print(e)
    exit()
time.sleep(2)
print(f"\n Los datos se guardaran en:")
print(ARCHIVO)
print("\n Recibiendo datos...")
print(" Escribe 1 y presiona ENTER para detener.\n")
threading.Thread(
    target=escuchar_teclado,
    daemon=True
).start()
with open(ARCHIVO, "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    writer.writerow([
        "timestamp_epoch",
        "fecha_hora",
        "mensaje"])
    while not detener:
        try:
            linea = arduino.readline().decode(
                "utf-8",
                errors="ignore"
            ).strip()
            if linea:
                timestamp = time.time()
                fecha_hora = datetime.now().strftime(
                    "%Y-%m-%d %H:%M:%S.%f"
                )[:-3]
                print(f"{fecha_hora} | {linea}")
                writer.writerow([
                    timestamp,
                    fecha_hora,
                    linea])
                f.flush()
        except Exception as e:
            print(f"ADVERTENCIA: Error de lectura: {e}")
arduino.close()
print("\nDETENIDO: Recepcion detenida.")
print(f"OK: CSV guardado en:\n{ARCHIVO}")
