#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);

// Direccion (DEBE SER IGUAL EN UNO)
const byte address[6] = "00001";

char input[32];

void setup() {
  Serial.begin(9600);
  if (!radio.begin()) {
    Serial.println("NRF NO DETECTADO");
    while (1);}
  radio.openWritingPipe(address);
  radio.stopListening();
  //  Configuracion estable
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  // IMPORTANTE: evitar corrupcion
  radio.setRetries(10, 15);
  radio.enableDynamicPayloads();
  Serial.println("MEGA listo");}
void loop() {
  if (Serial.available()) {
    int len = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
    if (len <= 0) return;
    // eliminar basura CR si existe
    if (input[len - 1] == '\r') len--;
    input[len] = '\0';
    //  enviar SOLO datos reales
    bool ok = radio.write(input, strlen(input) + 1);
    if (ok) {
      Serial.print("OK TX: ");
      Serial.println(input);
    } else {
      Serial.println("ERROR TX NRF");}}}
